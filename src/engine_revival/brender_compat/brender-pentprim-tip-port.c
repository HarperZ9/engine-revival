/*
 * SPDX-License-Identifier: MIT
 * Portions copyright (c) 1998 Argonaut Software Limited (BRender v1.3.2).
 * Copyright (c) 2026 Zain Dana Harper.
 * See compat/LICENSE-NOTE.md, LICENSE and LICENSES/MIT-BRender.txt.
 */
/*
 * The 15 and 16-bit perspective-correct lit texture scanlines, a
 * line-by-line C port of BRender v1.3.2 drivers/pentprim (MIT, Argonaut
 * Software 1998):
 *
 *   ScanLinePIZ2TIP256_RGB_555, ScanLinePIZ2TIPB256_RGB_555 / _565
 *       t15_pizp.asm's ScanLinePIZ2TIP macro (z-buffered)
 *   ScanLinePITIP256_RGB_555, ScanLinePITIPB256_RGB_555 / _565
 *       t15_pip.asm's ScanLinePITIP macro (z-sorted)
 *
 * Argonaut's C triangle setup (perspz.c / perspzi.h and persp.c / perspi.h)
 * fills work.pu, pv, pq, pi, pz and work.tsl and calls one of these per
 * scanline. The map is 8-bit, 256 x 256: work.tsl.source holds v in its high
 * byte and u in its low byte, stepped with inc/dec al and ah so each wraps
 * at 256. A texel of 0 is transparent. The lit texel is the 16-bit shade
 * table entry (intensity << 8 | texel), read at [table + 2 * index]. The
 * perspective step keeps u and v as remainders against the running
 * denominator and moves them a texel at a time, exactly as the assembly's
 * loops do, with their signed tests and their carry tests (jnc).
 *
 * The blended forms (TIPB, BLEND 15 or 16) halve the lit texel and the
 * screen pixel and add them. The z-buffered blended form tests depth but
 * does not store it, as in the assembly.
 *
 * Additions, none of which changes a pixel the assembly would draw inside
 * its buffers: a texel, shade entry, pixel or depth value outside its
 * buffer is not read or written (the pixel is skipped).
 */
#define __BR_V1DB__ 1
#include "brender.h"

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "drv.h"

typedef struct tip_k {
    int zbuffered, blend; /* blend: 0, 15 or 16 */
    int64_t cbytes, zbytes, tbytes;
} tip_k;

static int tip_in(const void *p, const void *base, int64_t bytes, int64_t need)
{
    return base != NULL && (const char *)p >= (const char *)base
           && (const char *)p - (const char *)base + need <= bytes;
}

/* The perspective state the loops carry in registers and in their statics. */
typedef struct tip_regs {
    uint32_t eax;            /* source: v in ah, u in al */
    int32_t esi, ebx;        /* u and v remainders */
    int32_t du, dv;          /* du_numerator, dv_numerator */
    int32_t den;             /* denominator */
    int32_t dq;              /* work.pq.grad_x */
} tip_regs;

static void tip_incu(tip_regs *r) { r->eax = (r->eax & ~0xffu) | ((r->eax + 1u) & 0xffu); }
static void tip_decu(tip_regs *r) { r->eax = (r->eax & ~0xffu) | ((r->eax - 1u) & 0xffu); }
static void tip_incv(tip_regs *r) { r->eax = (r->eax & ~0xff00u) | ((r->eax + 0x100u) & 0xff00u); }
static void tip_decv(tip_regs *r) { r->eax = (r->eax & ~0xff00u) | ((r->eax - 0x100u) & 0xff00u); }

/* add a,b and report the carry out */
static int tip_add_carry(int32_t *a, int32_t b)
{
    uint32_t s = (uint32_t)*a + (uint32_t)b;
    int c = s < (uint32_t)*a;
    *a = (int32_t)s;
    return c;
}

/* sub a,b then jge: taken when a >= b as true signed values */
static int tip_sub_ge(int32_t *a, int32_t b)
{
    int ge = (int64_t)*a >= (int64_t)b;
    *a = (int32_t)((uint32_t)*a - (uint32_t)b);
    return ge;
}

/* The u and v step after one pixel, forwards (left to right) or backwards.
 * Every jl / jge follows a cmp, sub or add, so it compares the true signed
 * operands; every jnc follows an add and tests its unsigned carry. */
static void tip_step(tip_regs *r, int backwards)
{
    int32_t edx = r->den;
    int vneg;
    if (!backwards) {
        r->esi = (int32_t)((uint32_t)r->esi - (uint32_t)r->du);
        edx = (int32_t)((uint32_t)edx + (uint32_t)r->dq);
    } else {
        r->esi = (int32_t)((uint32_t)r->esi + (uint32_t)r->du);
        edx = (int32_t)((uint32_t)edx - (uint32_t)r->dq);
    }
    if (r->esi >= edx) {              /* cmp esi,edx ; jl not taken: incu */
        r->esi = (int32_t)((uint32_t)r->esi - (uint32_t)edx);
        do {
            tip_incu(r);
            r->du = (int32_t)((uint32_t)r->du + (uint32_t)r->dq);
        } while (tip_sub_ge(&r->esi, edx));
        r->esi = (int32_t)((uint32_t)r->esi + (uint32_t)edx);
    } else if (r->esi < 0) {          /* test esi,esi ; jge not taken: decu */
        do {
            tip_decu(r);
            r->du = (int32_t)((uint32_t)r->du - (uint32_t)r->dq);
        } while (!tip_add_carry(&r->esi, edx));
    }
    if (!backwards) {                 /* sub ebx,edi ; jl decv */
        vneg = (int64_t)r->ebx < (int64_t)r->dv;
        r->ebx = (int32_t)((uint32_t)r->ebx - (uint32_t)r->dv);
    } else {                          /* add ebx,edi ; jl decv */
        vneg = (int64_t)r->ebx + (int64_t)r->dv < 0;
        r->ebx = (int32_t)((uint32_t)r->ebx + (uint32_t)r->dv);
    }
    if (vneg) {
        do {
            tip_decv(r);
            r->dv = (int32_t)((uint32_t)r->dv - (uint32_t)r->dq);
        } while (!tip_add_carry(&r->ebx, edx));
    } else if (r->ebx >= edx) {       /* cmp ebx,edx ; jge incv */
        r->ebx = (int32_t)((uint32_t)r->ebx - (uint32_t)edx);
        do {
            tip_incv(r);
            r->dv = (int32_t)((uint32_t)r->dv + (uint32_t)r->dq);
        } while (tip_sub_ge(&r->ebx, edx));
        r->ebx = (int32_t)((uint32_t)r->ebx + (uint32_t)edx);
    }
    r->den = edx;
}

/* One pixel: the depth test (z-buffered), the texel, the lit (and blended)
 * colour, the stores. z is the 16.16 depth before ror. */
static void tip_pixel(const tip_k *k, uint8_t *edi, uint8_t *zp, uint32_t z, uint32_t source, int32_t i)
{
    uint16_t zbuf, z16 = (uint16_t)(z >> 16), lit;
    uint8_t texel;
    uint32_t index;
    const uint8_t *entry;
    if (k->zbuffered) {
        if (!tip_in(zp, work.depth.base, k->zbytes, 2)) return;
        memcpy(&zbuf, zp, 2);
        if (z16 > zbuf) return;                               /* cmp bx,dx ; ja: behind */
    }
    if (!tip_in((const uint8_t *)work.texture.base + source, work.texture.base, k->tbytes, 1)) return;
    texel = ((const uint8_t *)work.texture.base)[source];
    if (texel == 0) return;                                   /* transparent */
    index = ((uint32_t)i >> 16 & 0xffu) << 8 | texel;         /* mov ch,byte ptr i+2 */
    entry = work.shade_table + 2u * index;
    if (!tip_in(edi, work.colour.base, k->cbytes, 2)) return;
    memcpy(&lit, entry, 2);
    if (k->blend) {
        uint16_t dst;
        uint32_t c = lit, d;
        memcpy(&dst, edi, 2);
        d = dst;
        if (k->blend == 15) { c = (c >> 1) & 0x3defu; d = (d & 0x7bdeu) >> 1; }
        else { c = (c >> 1) & 0x7befu; d = (d & 0xf7deu) >> 1; }
        lit = (uint16_t)(c + d);
    } else if (k->zbuffered) {
        memcpy(zp, &z16, 2);                                  /* mov [ebp],bx */
    }
    memcpy(edi, &lit, 2);
}

static void tip_scanline(int zbuffered, int blend)
{
    tip_k k;
    tip_regs r;
    uint8_t *edi = (uint8_t *)work.tsl.start, *end = (uint8_t *)work.tsl.end, *zp = (uint8_t *)work.tsl.zstart;
    uint32_t z = (uint32_t)work.pz.current;
    int32_t i = work.pi.current;
    int backwards = edi > end;
    k.zbuffered = zbuffered;
    k.blend = blend;
    k.cbytes = (int64_t)work.colour.stride_b * work.colour.height;
    k.zbytes = (int64_t)work.depth.stride_b * work.depth.height;
    k.tbytes = (int64_t)work.texture.size;
    r.esi = work.pu.current;
    r.ebx = work.pv.current;
    r.du = work.pu.grad_x;
    r.dv = work.pv.grad_x;
    r.den = work.pq.current;
    r.dq = work.pq.grad_x;
    r.eax = (uint32_t)work.tsl.source;
    for (;;) {
        if (!backwards ? edi >= end : edi <= end) return;     /* jae / jbe L50 */
        tip_pixel(&k, edi, zp, z, r.eax, i);
        if (!backwards) {
            z += (uint32_t)work.pz.grad_x;
            i = (int32_t)((uint32_t)i + (uint32_t)work.pi.grad_x);
            zp += 2;
            edi += 2;
        } else {
            z -= (uint32_t)work.pz.grad_x;
            i = (int32_t)((uint32_t)i - (uint32_t)work.pi.grad_x);
            zp -= 2;
            edi -= 2;
        }
        if (!backwards ? edi >= end : edi <= end) return;
        tip_step(&r, backwards);
    }
}

void BR_ASM_CALL ScanLinePIZ2TIP256_RGB_555(void) { tip_scanline(1, 0); }
void BR_ASM_CALL ScanLinePIZ2TIPB256_RGB_555(void) { tip_scanline(1, 15); }
void BR_ASM_CALL ScanLinePIZ2TIPB256_RGB_565(void) { tip_scanline(1, 16); }
void BR_ASM_CALL ScanLinePITIP256_RGB_555(void) { tip_scanline(0, 0); }
void BR_ASM_CALL ScanLinePITIPB256_RGB_555(void) { tip_scanline(0, 15); }
void BR_ASM_CALL ScanLinePITIPB256_RGB_565(void) { tip_scanline(0, 16); }
