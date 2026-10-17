/*
 * SPDX-License-Identifier: MIT
 * Portions copyright (c) 1998 Argonaut Software Limited (BRender v1.3.2).
 * Copyright (c) 2026 Zain Dana Harper.
 * See compat/LICENSE-NOTE.md, LICENSE and LICENSES/MIT-BRender.txt.
 */
/*
 * The arbitrary-width texture trapezoids, a line-by-line C port of the
 * TriangleRenderZ2 macro in BRender v1.3.2 drivers/pentprim/t_piza.asm (MIT,
 * Argonaut 1998), built with WRAP, WRAPROW and TRANSP as that file sets
 * them, for the four instances the true colour tables use:
 *
 *   TrapezoidRenderPIZ2TA24          LIGHT 0, SBPP 3, DBPP 3: RGB_888 map
 *   TrapezoidRenderPIZ2TIA_RGB_888   LIGHT 1, SBPP 1, DBPP 3: 8-bit map, shade table
 *   TrapezoidRenderPIZ2TA15          LIGHT 0, SBPP 2, DBPP 2: RGB_555 / 565 map
 *   TrapezoidRenderPIZ2TIA_RGB_555   LIGHT 1, SBPP 1, DBPP 2: 8-bit map, shade table
 *
 * (prm_t16f.c uses the TA15 and TIA_RGB_555 instances for RGB_565 too: both
 * copy 16-bit words and never look inside them.) Argonaut's own C setup in
 * awtmz.c / awtmi.h fills work.awsl and calls these once per half of the
 * triangle.
 *
 * Registers are modelled as 32-bit unsigned values so adds wrap and carries
 * come out where the assembly takes them: eax holds the u fraction in its
 * high half and the biased integer u in its low half, ebx the v fraction.
 * The assembly reuses bl and bh for the depth test and the texel test; that
 * cannot change a carry, because awtmi.h builds v_current and every dv with
 * a zero low half, so the port keeps ebx whole.
 *
 * The lit kernels index the shade table with (intensity << 8 | texel): the
 * DBPP 2 branch reads [edx+ecx*2], a 2-byte entry, which matches its
 * RGB_555 / RGB_565 table; the DBPP 3 branch reads [edx+ecx*4] from a table
 * of 3-byte entries, a 1998 defect the parity README records.
 *
 * Additions, none of which changes a pixel the assembly would draw inside
 * its buffers: a texel outside the texture is not read (the pixel is
 * skipped), and a pixel or depth value outside its buffer is not written.
 */
#define __BR_V1DB__ 1
#include "brender.h"

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "drv.h"

static uint32_t awsl_u_int(void)
{
    uint32_t v;
    memcpy(&v, &work.awsl.u_int_current, sizeof(v)); /* the assembly reads it as a dword */
    return v;
}

static void awsl_set_u_int(uint32_t v) { memcpy(&work.awsl.u_int_current, &v, sizeof(v)); }

static int awsl_in(const char *p, const void *base, size_t bytes, size_t need)
{
    return p >= (const char *)base && p + need <= (const char *)base + bytes;
}

typedef struct awsl_kernel {
    int light, sbpp, dbpp;
    size_t colour_bytes, depth_bytes, texture_bytes;
} awsl_kernel;

/* TRANSP: a zero texel (all SBPP bytes) is transparent. */
static int awsl_opaque(const awsl_kernel *k, const char *esi)
{
    if (!awsl_in(esi, work.texture.base, k->texture_bytes, (size_t)k->sbpp)) return 0;
    if (k->sbpp == 1) return esi[0] != 0;
    if (k->sbpp == 2) return (esi[0] | esi[1]) != 0;
    return (esi[0] | esi[1] | esi[2]) != 0;
}

/* The 16-bit depth test: draw where *zptr >= z (cmp dl,bl ; sbb dh,bh ; jb). */
static int awsl_depth_passes(const awsl_kernel *k, const char *ebp)
{
    uint16_t z16 = (uint16_t)((uint32_t)work.pz.currentpix >> 16), zbuf;
    if (!awsl_in(ebp, work.depth.base, k->depth_bytes, 2)) return 0;
    memcpy(&zbuf, ebp, 2);
    return zbuf >= z16;
}

/* Store z, then the texel: copied (LIGHT 0), or looked up in the shade table
 * at the stride the kernel reads it (LIGHT 1). */
static void awsl_store(const awsl_kernel *k, char *edi, char *ebp, const char *esi)
{
    uint16_t z16 = (uint16_t)((uint32_t)work.pz.currentpix >> 16);
    memcpy(ebp, &z16, 2);
    if (k->light) {
        uint32_t index = (((uint32_t)work.pi.currentpix >> 16) & 0xffu) << 8 | (uint8_t)esi[0];
        const uint8_t *lit = work.shade_table + index * (k->dbpp == 2 ? 2u : 4u);
        memcpy(edi, lit, (size_t)k->dbpp);
    } else {
        memcpy(edi, esi, (size_t)k->sbpp);
    }
}

/* contf / the backward scan's step: u, the source pointer and v. */
static void awsl_step_uv(const awsl_kernel *k, uint32_t *eax, uint32_t *ebx, char **esi)
{
    uint32_t carry = (uint32_t)(((uint64_t)*eax + (uint32_t)work.awsl.du) >> 32);
    *eax += (uint32_t)work.awsl.du;
    *esi += work.awsl.dsource;
    *eax += carry;
    *esi += (ptrdiff_t)carry * k->sbpp;
    *eax += (uint32_t)work.awsl.du_int;
    carry = (uint32_t)(((uint64_t)*ebx + (uint32_t)work.awsl.dv) >> 32);
    *ebx += (uint32_t)work.awsl.dv;
    if (carry) *esi += work.texture.stride_b;
    if (!(*eax & 0x8000u)) { /* test ah,ah ; jl nowidth */
        *eax += work.texture.width_p;
        *esi += (ptrdiff_t)work.texture.width_p * k->sbpp;
    }
    if (*esi < (char *)work.texture.base) *esi += work.texture.size;
}

static void awsl_scan(const awsl_kernel *k)
{
    char *edi = work.awsl.start, *esi = work.awsl.source_current, *ebp = work.awsl.zstart;
    uint32_t eax = ((uint32_t)work.awsl.u_current & 0xffff0000u) | (awsl_u_int() & 0xffffu);
    uint32_t ebx = (uint32_t)work.awsl.v_current;

    work.pz.currentpix = work.pz.current;
    if (k->light) work.pi.currentpix = work.pi.current;
    if (edi < work.awsl.end) {
        /* forward scan */
        while (edi < work.awsl.end) {
            if (awsl_opaque(k, esi) && awsl_depth_passes(k, ebp)
                && awsl_in(edi, work.colour.base, k->colour_bytes, (size_t)k->dbpp))
                awsl_store(k, edi, ebp, esi);
            /* both paths: zptr += 2, ptr += DBPP, z += dz, i += di */
            ebp += 2;
            edi += k->dbpp;
            work.pz.currentpix = (br_fixed_ls)((uint32_t)work.pz.currentpix + (uint32_t)work.pz.grad_x);
            if (k->light) work.pi.currentpix = (br_fixed_ls)((uint32_t)work.pi.currentpix + (uint32_t)work.pi.grad_x);
            awsl_step_uv(k, &eax, &ebx, &esi);
        }
    } else {
        /* backward scan */
        while (edi > work.awsl.end) {
            work.pz.currentpix = (br_fixed_ls)((uint32_t)work.pz.currentpix - (uint32_t)work.pz.grad_x);
            if (k->light) work.pi.currentpix = (br_fixed_ls)((uint32_t)work.pi.currentpix - (uint32_t)work.pi.grad_x);
            awsl_step_uv(k, &eax, &ebx, &esi);
            ebp -= 2;
            edi -= k->dbpp;
            if (awsl_opaque(k, esi) && awsl_depth_passes(k, ebp)
                && awsl_in(edi, work.colour.base, k->colour_bytes, (size_t)k->dbpp))
                awsl_store(k, edi, ebp, esi);
        }
    }
}

/* next: the per scan line updates, then scan_inc carry|nocarry and the wrap. */
static void awsl_next_line(const awsl_kernel *k, struct scan_edge *edge)
{
    uint32_t f = (uint32_t)edge->f, d, carry;
    carry = (f + (uint32_t)edge->d_f) < f;
    edge->f = (br_int_32)(f + (uint32_t)edge->d_f);
    work.awsl.end += ((ptrdiff_t)edge->d_i + carry) * k->dbpp;

    f = (uint32_t)work.main.f;
    carry = (f + (uint32_t)work.main.d_f) < f;
    work.main.f = (br_int_32)(f + (uint32_t)work.main.d_f);
    d = (uint32_t)work.main.d_i + carry;
    work.awsl.zstart += (ptrdiff_t)(br_int_32)d * 2;
    work.awsl.start += (ptrdiff_t)(br_int_32)d * k->dbpp;

    if (k->light)
        work.pi.current = (br_fixed_ls)((uint32_t)work.pi.current + (uint32_t)(carry ? work.pi.d_carry : work.pi.d_nocarry));
    work.pz.current = (br_fixed_ls)((uint32_t)work.pz.current + (uint32_t)(carry ? work.pz.d_carry : work.pz.d_nocarry));
    {
        uint32_t du = carry ? work.awsl.du_carry : work.awsl.du_nocarry;
        uint32_t dv = carry ? work.awsl.dv_carry : work.awsl.dv_nocarry;
        int dsrc = carry ? work.awsl.dsource_carry : work.awsl.dsource_nocarry;
        int du_int = carry ? work.awsl.du_int_carry : work.awsl.du_int_nocarry;
        uint32_t ucarry = (work.awsl.u_current + du) < work.awsl.u_current;
        uint32_t vcarry;
        work.awsl.u_current += du;
        work.awsl.source_current += dsrc;
        awsl_set_u_int(awsl_u_int() + (uint32_t)du_int + ucarry);
        work.awsl.source_current += (ptrdiff_t)ucarry * k->sbpp;
        vcarry = (work.awsl.v_current + dv) < work.awsl.v_current;
        work.awsl.v_current += dv;
        if (vcarry) work.awsl.source_current += work.texture.stride_b;
    }
    /* cont: wrap u and the source pointer */
    if (!(awsl_u_int() & 0x8000u)) {
        awsl_set_u_int(awsl_u_int() + work.texture.width_p);
        work.awsl.source_current += (ptrdiff_t)work.texture.width_p * k->sbpp;
    }
    if (work.awsl.source_current < (char *)work.texture.base)
        work.awsl.source_current += work.texture.size;
}

static void awsl_trapezoid(int light, int sbpp, int dbpp)
{
    awsl_kernel k;
    k.light = light;
    k.sbpp = sbpp;
    k.dbpp = dbpp;
    k.colour_bytes = (size_t)work.colour.stride_b * work.colour.height;
    k.depth_bytes = (size_t)work.depth.stride_b * work.depth.height;
    k.texture_bytes = (size_t)work.texture.size;
    for (;;) {
        struct scan_edge *edge = work.awsl.edge;
        if (edge->count == 0) return;
        edge->count--;
        awsl_scan(&k);
        awsl_next_line(&k, edge);
    }
}

void BR_ASM_CALL TrapezoidRenderPIZ2TA24(void) { awsl_trapezoid(0, 3, 3); }
void BR_ASM_CALL TrapezoidRenderPIZ2TIA_RGB_888(void) { awsl_trapezoid(1, 1, 3); }
void BR_ASM_CALL TrapezoidRenderPIZ2TA15(void) { awsl_trapezoid(0, 2, 2); }
void BR_ASM_CALL TrapezoidRenderPIZ2TIA_RGB_555(void) { awsl_trapezoid(1, 1, 2); }
