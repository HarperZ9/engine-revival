/*
 * SPDX-License-Identifier: MIT
 * Portions copyright (c) 1998 Argonaut Software Limited (BRender v1.3.2).
 * Copyright (c) 2026 Zain Dana Harper.
 * See compat/LICENSE-NOTE.md, LICENSE and LICENSES/MIT-BRender.txt.
 */
/*
 * The 8-bit (INDEX_8, 16-bit depth) pentprim kernels, ported line by line
 * from BRender v1.3.2 drivers/pentprim (MIT, Argonaut Software 1998):
 *
 *   TriangleRender_Z_I8_D16        zb8.asm      DRAW_Z_I8_D16, flat colour index
 *   TriangleRender_ZTI_I8_D16_256  zb8.asm      DRAW_ZTI_I8_D16_256, 256x256 map
 *                                               lit through the shade table
 *   TriangleRender_ZI_I8_D16       fti8_piz.asm TRAPEZIUM_ZI_I8_D16, Gouraud index
 *   TriangleRender_ZT_I8_D16_256   fti8_piz.asm TRAPEZIUM_ZT_I8_D16_256, 256x256 map
 *
 * with the floating point triangle setup they call, fpsetup.asm's
 * SETUP_FLOAT, SETUP_FLOAT_PARAM and SETUP_FLOAT_COLOUR, in
 * brender-pentprim-fpsetup-port.c. The ZT and ZTI rasterisers after their
 * setup (the TriangleRasterise_ labels) are separate functions, because the
 * perspective kernels jump into them (brender-pentprim-i8p-port.c).
 *
 * Registers are 32-bit unsigned values, so adds wrap and the carries come
 * out where the assembly takes them; a carry the assembly keeps in the flags
 * between instructions is a local variable here.
 *
 * Additions, none of which changes a pixel the assembly would draw inside
 * its buffers: a pixel or depth value outside its buffer is not written,
 * and a texel or shade entry outside its map is not read (the pixel is
 * skipped). The assembly relies on softrend having clipped the triangle.
 */
#define __BR_V1DB__ 1
#include "brender.h"

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "drv.h"
#include "brender-pentprim-fpsetup-port.h"
#include "brender-pentprim-pfpsetup-port.h"

static uint32_t i8_ror16(uint32_t v) { return (v >> 16) | (v << 16); }

/* The pixel and depth buffers, addressed as the assembly does: byte offsets
 * from the buffer base, checked before each write. */
typedef struct i8_target {
    uint8_t *cbase, *zbase;
    int32_t climit, zlimit, cstride, zstride;
} i8_target;

static int i8_target_open(i8_target *t)
{
    t->cbase = (uint8_t *)work.colour.base;
    t->zbase = (uint8_t *)work.depth.base;
    t->cstride = work.colour.stride_b;
    t->zstride = work.depth.stride_b;
    t->climit = work.colour.stride_b * (int32_t)work.colour.height;
    t->zlimit = work.depth.stride_b * (int32_t)work.depth.height;
    return t->cbase != NULL && t->zbase != NULL;
}

static int i8_zread(const i8_target *t, int32_t off, uint16_t *z)
{
    if (off < 0 || off + 2 > t->zlimit) return 0;
    memcpy(z, t->zbase + off, 2);
    return 1;
}

static void i8_write(const i8_target *t, int32_t coff, int32_t zoff, uint16_t z, uint8_t c)
{
    if (coff < 0 || coff >= t->climit || zoff < 0 || zoff + 2 > t->zlimit) return;
    memcpy(t->zbase + zoff, &z, 2);
    t->cbase[coff] = c;
}

/* The first scanline: colour.base - 1 + stride * (t_y - 1), and the depth
 * row two bytes left of it. zb8.asm computes these on the FPU, fti8_piz.asm
 * with imul; both give these integers. */
static void i8_first_line(const i8_target *t, int32_t *scan, int32_t *depth)
{
    *scan = t->cstride * (fps.t_y - 1) - 1;
    *depth = t->zstride * (fps.t_y - 1) - 2;
}

/* ------------------------------------------------------------------ */
/* zb8.asm DRAW_Z_I8_D16: one half of a flat triangle. */
static void z8_half(const i8_target *t, uint32_t *minor, uint32_t d_minor, int32_t *count, int rl,
                    uint32_t dzx, int32_t *scan, int32_t *depth)
{
    uint32_t eax, ebx, ecx;
    if (*count < 0) return;
    eax = fps.s_z;
    ebx = *minor;
    ecx = fps.xm;
    for (;;) {
        int32_t base, zrow, cnt;
        eax = i8_ror16(eax);
        ebx >>= 16;
        ecx >>= 16;
        base = *scan + (int32_t)ebx;
        cnt = (int32_t)(ecx - ebx);
        zrow = *depth + 2 * (int32_t)ebx;
        if (rl ? cnt >= 0 : cnt <= 0) {
            uint32_t cf = 0, edx = eax;
            do {
                uint16_t zb;
                eax = rl ? eax - cf : eax + cf;          /* adc_d eax,0 */
                if (i8_zread(t, zrow + 2 * cnt, &zb)) {
                    edx = (edx & 0xffff0000u) | zb;      /* mov dl / mov dh */
                    if (!(eax > edx))                    /* cmp eax,edx ; ja noPlot */
                        i8_write(t, base + cnt, zrow + 2 * cnt, (uint16_t)eax, fps.colour);
                }
                if (rl) { cf = eax < dzx; eax -= dzx; }  /* add_d eax,esi */
                else { cf = (uint32_t)(((uint64_t)eax + dzx) >> 32); eax += dzx; }
                cnt += rl ? -1 : 1;                      /* inc_d ecx */
                edx = eax;
            } while (rl ? cnt >= 0 : cnt <= 0);          /* jle_d drawPixel */
        }
        /* lineDrawn */
        {
            uint32_t carry = (uint32_t)(((uint64_t)fps.xm_f + fps.d_xm_f) >> 32);
            uint64_t s;
            fps.xm_f += fps.d_xm_f;
            s = (uint64_t)fps.s_z + (carry ? fps.d_z_y_1 : fps.d_z_y_0);
            fps.s_z = (uint32_t)s + (uint32_t)(s >> 32); /* add eax,ebx ; adc eax,0 */
            fps.xm += fps.d_xm;
            *scan += t->cstride;
            *depth += t->zstride;
            *minor += d_minor;
            eax = fps.s_z;
            ebx = *minor;
            ecx = fps.xm;
        }
        if (--*count < 0) break;
    }
}

void BR_ASM_CALL TriangleRender_Z_I8_D16(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    i8_target t;
    int32_t scan, depth;
    uint32_t dzx;
    (void)block;
    if (!i8_target_open(&t)) return;
    fps_colour(v0);
    if (fps_setup(v0, v1, v2) != 0) return;
    fps_param(C_SZ, v0, v1, v2, &fps.s_z, &fps.d_z_y_1, &fps.d_z_x, &fps.d_z_y_0, 1);
    i8_first_line(&t, &scan, &depth);
    fps.xm_f = fps.xm << 16;
    fps.d_xm_f = fps.d_xm << 16;
    /* cmp edx,80000000 ; adc edx,-1 -- the constant is decimal in zb8.asm, so
     * a step of 80000000 (0x04C4B400) or more loses one, not only a negative one */
    dzx = fps.d_z_x;
    if (dzx >= 80000000u) dzx -= 1u;
    dzx = i8_ror16(dzx);
    z8_half(&t, &fps.x1, fps.d_x1, &fps.topCount, fps.flip != 0, dzx, &scan, &depth);
    z8_half(&t, &fps.x2, fps.d_x2, &fps.bottomCount, fps.flip != 0, dzx, &scan, &depth);
}

/* ------------------------------------------------------------------ */
/* fti8_piz.asm: the major-edge fraction and the grad_x swap shared by the
 * Gouraud and textured trapeziums. cmp g,80000000h ; adc g,-1 ; ror g,16 */
static uint32_t fti8_swap_grad(uint32_t g)
{
    if (g >= 0x80000000u) g -= 1u;
    return i8_ror16(g);
}

typedef struct fti8_line {
    int32_t scan, depth;
    uint32_t main_f, main_d_f;
} fti8_line;

/* Per scanline: step the buffers, both edges and the major-edge fraction;
 * returns the carry that picks the d_carry (d_y_1) or d_nocarry (d_y_0) step. */
static uint32_t fti8_next_line(const i8_target *t, fti8_line *l, uint32_t *half_i, uint32_t half_d_i)
{
    uint32_t carry;
    l->scan += t->cstride;
    l->depth += t->zstride;
    fps.xm += fps.d_xm;
    *half_i += half_d_i;
    carry = (uint32_t)(((uint64_t)l->main_f + l->main_d_f) >> 32);
    l->main_f += l->main_d_f;
    return carry;
}

/* TRAPEZIUM_ZI_I8_D16 half,dirn */
static void zi8_half(const i8_target *t, fti8_line *l, uint32_t *half_i, uint32_t half_d_i, int32_t *count, int b)
{
    if (*count < 0) return;
    for (;;) {
        int32_t ebp = (int32_t)(fps.xm >> 16), edi = (int32_t)(*half_i >> 16), cnt = ebp - edi;
        if (b ? cnt >= 0 : cnt <= 0) {
            int32_t cbase = l->scan + edi, zrow = l->depth + 2 * edi;
            uint32_t eax = i8_ror16(fps.s_i), ecx = fps.d_i_x, edx = i8_ror16(fps.s_z), ebx, cf;
            uint64_t s;
            if (!b) { cf = eax < ecx; eax -= ecx; s = (uint64_t)eax - cf; cf = eax < cf; eax = (uint32_t)s; }
            else { s = (uint64_t)eax + ecx; eax = (uint32_t)s; cf = (uint32_t)(s >> 32);
                   s = (uint64_t)eax + cf; eax = (uint32_t)s; cf = (uint32_t)(s >> 32); }
            ebx = edx;
            do {
                uint16_t zb;
                int plot = 0;
                if (!b) edx += cf; else edx -= cf;      /* adc_/sbb_ edx,0 */
                if (i8_zread(t, zrow + 2 * cnt, &zb)) { ebx = (ebx & 0xffff0000u) | zb; plot = 1; }
                if (!b) { s = (uint64_t)eax + ecx; eax = (uint32_t)s; cf = (uint32_t)(s >> 32); eax += cf; }
                else { cf = eax < ecx; eax -= ecx; eax -= cf; }
                if (plot && !(edx > ebx))
                    i8_write(t, cbase + cnt, zrow + 2 * cnt, (uint16_t)edx, (uint8_t)eax);
                if (!b) { s = (uint64_t)edx + fps.d_z_x; edx = (uint32_t)s; cf = (uint32_t)(s >> 32); }
                else { cf = edx < fps.d_z_x; edx -= fps.d_z_x; }
                cnt += b ? -1 : 1;
                ebx = edx;
            } while (b ? cnt >= 0 : cnt <= 0);
        }
        {
            uint32_t carry = fti8_next_line(t, l, half_i, half_d_i);
            fps.s_i += carry ? fps.d_i_y_1 : fps.d_i_y_0;
            fps.s_z += carry ? fps.d_z_y_1 : fps.d_z_y_0;
        }
        if (--*count < 0) break;
    }
}

void BR_ASM_CALL TriangleRender_ZI_I8_D16(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    i8_target t;
    fti8_line l;
    (void)block;
    if (!i8_target_open(&t)) return;
    if (fps_setup(v0, v1, v2) != 0) return;
    fps_param(C_SZ, v0, v1, v2, &fps.s_z, &fps.d_z_y_1, &fps.d_z_x, &fps.d_z_y_0, 1);
    fps_param(C_I, v0, v1, v2, &fps.s_i, &fps.d_i_y_1, &fps.d_i_x, &fps.d_i_y_0, 0);
    i8_first_line(&t, &l.scan, &l.depth);
    l.main_f = fps.xm << 16;
    l.main_d_f = fps.d_xm << 16;
    fps.d_z_x = fti8_swap_grad(fps.d_z_x);
    fps.d_i_x = fti8_swap_grad(fps.d_i_x);
    zi8_half(&t, &l, &fps.x1, fps.d_x1, &fps.topCount, fps.flip != 0);
    zi8_half(&t, &l, &fps.x2, fps.d_x2, &fps.bottomCount, fps.flip != 0);
}

/* TRAPEZIUM_ZT_I8_D16_256 half,dirn */
static void zt8_half(const i8_target *t, fti8_line *l, uint32_t *half_i, uint32_t half_d_i, int32_t *count, int b)
{
    const uint8_t *tex = (const uint8_t *)work.texture.base;
    const uint32_t tex_bytes = work.texture.size;
    if (*count < 0) return;
    for (;;) {
        int32_t ebp = (int32_t)(fps.xm >> 16), edi = (int32_t)(*half_i >> 16), cnt = ebp - edi;
        if (b ? cnt >= 0 : cnt <= 0) {
            int32_t dest = l->scan + edi, zdest = l->depth + 2 * edi;
            uint32_t edx = i8_ror16(fps.s_z), esi = fps.s_u, ecx = fps.s_v, eax = esi >> 16, ebx = edx;
            do {
                uint16_t zb;
                ecx = i8_ror16(ecx);
                eax = (eax & 0xffff00ffu) | ((ecx & 0xffu) << 8); /* mov ah,cl */
                if (i8_zread(t, zdest + 2 * cnt, &zb)) {
                    ebx = (ebx & 0xffff0000u) | zb;
                    if (!(edx > ebx) && tex != NULL && eax < tex_bytes) {
                        uint8_t texel = tex[eax];
                        eax = (eax & 0xffffff00u) | texel;
                        if (texel != 0)
                            i8_write(t, dest + cnt, zdest + 2 * cnt, (uint16_t)edx, texel);
                    }
                }
                ecx = i8_ror16(ecx);
                if (!b) { esi += fps.d_u_x; ecx += fps.d_v_x; }
                else { esi -= fps.d_u_x; ecx -= fps.d_v_x; }
                eax = esi >> 16;
                if (!b) { uint64_t s = (uint64_t)edx + fps.d_z_x; edx = (uint32_t)s; edx += (uint32_t)(s >> 32); }
                else { uint32_t cf = edx < fps.d_z_x; edx -= fps.d_z_x; edx -= cf; }
                cnt += b ? -1 : 1;
                ebx = edx;
            } while (b ? cnt >= 0 : cnt <= 0);
        }
        {
            uint32_t carry = fti8_next_line(t, l, half_i, half_d_i);
            fps.s_z += carry ? fps.d_z_y_1 : fps.d_z_y_0;
            fps.s_u += carry ? fps.d_u_y_1 : fps.d_u_y_0;
            fps.s_v += carry ? fps.d_v_y_1 : fps.d_v_y_0;
        }
        if (--*count < 0) break;
    }
}

/* TriangleRasterise_ZT_I8_D16_256: everything after the setup call. The
 * perspective kernels jump here in their cheat case. */
void i8_rasterise_zt_256(void)
{
    i8_target t;
    fti8_line l;
    if (!i8_target_open(&t)) return;
    i8_first_line(&t, &l.scan, &l.depth);
    l.main_f = fps.xm << 16;
    l.main_d_f = fps.d_xm << 16;
    fps.d_z_x = fti8_swap_grad(fps.d_z_x);
    zt8_half(&t, &l, &fps.x1, fps.d_x1, &fps.topCount, fps.flip != 0);
    zt8_half(&t, &l, &fps.x2, fps.d_x2, &fps.bottomCount, fps.flip != 0);
}

void BR_ASM_CALL TriangleRender_ZT_I8_D16_256(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    (void)block;
    if (fps_setup(v0, v1, v2) != 0) return;
    fps_param(C_SZ, v0, v1, v2, &fps.s_z, &fps.d_z_y_1, &fps.d_z_x, &fps.d_z_y_0, 1);
    fps_param(C_U, v0, v1, v2, &fps.s_u, &fps.d_u_y_1, &fps.d_u_x, &fps.d_u_y_0, 0);
    fps_param(C_V, v0, v1, v2, &fps.s_v, &fps.d_v_y_1, &fps.d_v_x, &fps.d_v_y_0, 0);
    i8_rasterise_zt_256();
}

/* ------------------------------------------------------------------ */
/* zb8.asm DRAW_ZTI_I8_D16_256 (the "old but faster" version it builds). */
static void zti8_half(const i8_target *t, uint32_t *minor, uint32_t d_minor, int32_t *count, int rl,
                      int32_t *scan, int32_t *depth)
{
    const uint8_t *tex = (const uint8_t *)work.texture.base, *shade = work.shade_table;
    const uint32_t tex_bytes = work.texture.size;
    uint32_t c_z, c_u, c_v, c_i;
    if (*count < 0) return;
    c_z = fps.s_z; c_u = fps.s_u; c_i = fps.s_i; c_v = fps.s_v;
    for (;;) {
        int32_t edx = (int32_t)(*minor >> 16), cnt = (int32_t)(fps.xm >> 16) - edx;
        int32_t major = *scan + edx, zrow = *depth + 2 * edx;
        if (rl ? cnt >= 0 : cnt <= 0) {
            do {
                uint16_t zb;
                uint32_t z = c_z >> 16;
                if (i8_zread(t, zrow + 2 * cnt, &zb) && !(z > zb)) {
                    uint32_t texel_at = ((c_v >> 16) & 0xffu) << 8 | ((c_u >> 16) & 0xffu);
                    if (tex != NULL && shade != NULL && texel_at < tex_bytes) {
                        uint8_t texel = tex[texel_at];
                        if (texel != 0)
                            i8_write(t, major + cnt, zrow + 2 * cnt, (uint16_t)z, shade[((c_i >> 16) & 0xffu) << 8 | texel]);
                    }
                }
                if (rl) { c_z -= fps.d_z_x; c_i -= fps.d_i_x; c_u -= fps.d_u_x; c_v -= fps.d_v_x; }
                else { c_z += fps.d_z_x; c_i += fps.d_i_x; c_u += fps.d_u_x; c_v += fps.d_v_x; }
                cnt += rl ? -1 : 1;
            } while (rl ? cnt >= 0 : cnt <= 0);
        }
        /* lineDrawn */
        {
            uint32_t carry = (uint32_t)(((uint64_t)fps.xm_f + fps.d_xm_f) >> 32);
            fps.xm_f += fps.d_xm_f;
            fps.xm += fps.d_xm;
            *minor += d_minor;
            c_u = fps.s_u += carry ? fps.d_u_y_1 : fps.d_u_y_0;
            c_v = fps.s_v += carry ? fps.d_v_y_1 : fps.d_v_y_0;
            c_z = fps.s_z += carry ? fps.d_z_y_1 : fps.d_z_y_0;
            c_i = fps.s_i += carry ? fps.d_i_y_1 : fps.d_i_y_0;
            *depth += t->zstride;
            *scan += t->cstride;
        }
        if (--*count < 0) break;
    }
}

/* TriangleRasterise_ZTI_I8_D16_256: everything after the setup call. The
 * lit perspective kernel jumps here in its cheat case. */
void i8_rasterise_zti_256(void)
{
    i8_target t;
    int32_t scan, depth;
    if (!i8_target_open(&t)) return;
    i8_first_line(&t, &scan, &depth);
    fps.xm_f = fps.xm << 16;
    fps.d_xm_f = fps.d_xm << 16;
    zti8_half(&t, &fps.x1, fps.d_x1, &fps.topCount, fps.flip != 0, &scan, &depth);
    zti8_half(&t, &fps.x2, fps.d_x2, &fps.bottomCount, fps.flip != 0, &scan, &depth);
}

void BR_ASM_CALL TriangleRender_ZTI_I8_D16_256(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    (void)block;
    if (fps_setup(v0, v1, v2) != 0) return;
    fps_param(C_SZ, v0, v1, v2, &fps.s_z, &fps.d_z_y_1, &fps.d_z_x, &fps.d_z_y_0, 1);
    fps_param(C_U, v0, v1, v2, &fps.s_u, &fps.d_u_y_1, &fps.d_u_x, &fps.d_u_y_0, 0);
    fps_param(C_V, v0, v1, v2, &fps.s_v, &fps.d_v_y_1, &fps.d_v_x, &fps.d_v_y_0, 0);
    fps_param(C_I, v0, v1, v2, &fps.s_i, &fps.d_i_y_1, &fps.d_i_x, &fps.d_i_y_0, 0);
    i8_rasterise_zti_256();
}
