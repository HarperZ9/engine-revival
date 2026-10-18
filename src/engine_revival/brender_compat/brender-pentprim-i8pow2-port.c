/*
 * SPDX-License-Identifier: MIT
 * Portions copyright (c) 1998 Argonaut Software Limited (BRender v1.3.2).
 * Copyright (c) 2026 Zain Dana Harper.
 * See compat/LICENSE-NOTE.md, LICENSE and LICENSES/MIT-BRender.txt.
 */
/*
 * The 8-bit linear texture kernels for the power of two maps that are not
 * 256 wide (8, 16, 32, 64, 128 and 1024), ported line by line from BRender
 * v1.3.2 drivers/pentprim (MIT, Argonaut Software 1998):
 *
 *   TriangleRender_ZT_I8_D16_<n>        zb8p2unl.asm  DRAW_ZT_I8_D16_POW2, unlit
 *   TriangleRender_ZTI_I8_D16_<n>       zb8p2lit.asm  DRAW_ZTI_I8_D16_POW2, lit
 *                                                     through the shade table
 *   TriangleRender_ZT_I8_D16_<n>_FLAT   zb8p2fla.asm  DRAW_ZT_I8_D16_POW2_FLAT,
 *                                                     flat lit: the shade row
 *                                                     is fixed for the face
 *
 * with fpsetup.asm's TriangleSetup_ZT, TriangleSetup_ZTI and
 * TriangleSetup_ZT_FLAT_LIT (brender-pentprim-fpsetup-port.c). The flat lit
 * kernel masks the texel to a byte before the shade lookup (`and eax,0ffh`),
 * so it reads shade_row[texel], always in the row the setup chose; it does not
 * read past the table the way the smooth 1024 kernel does. The rasterisers after the setup (the
 * TriangleRasterise_ labels) are separate functions, because the
 * perspective kernels of the same sizes jump into them in their cheat case
 * (brender-pentprim-i8p-port.c).
 *
 * The texel is ((v >> 16) mod n) * n + (u >> 16) mod n: shr esi,16-pow2
 * and and esi,mask shl pow2 for v, shr eax,16 and and eax,mask for u.
 *
 * Kept as found:
 * - Both kernels adjust the z gradient with "cmp edx,80000000", a decimal
 *   constant where the 256 kernels compare with 80000000h. So a gradient of
 *   80,000,000 (04c4b400h) or more loses one in its last place, and a
 *   negative one under that does not.
 * - The smooth lit kernel looks the shade table up with the whole of eax,
 *   whose low two bytes it has replaced with the intensity row and the texel.
 *   For maps up to 128 wide the texel address fits in those two bytes. For a
 *   1024 wide map a texel at v of 64 or more leaves bits above them, and the
 *   assembly reads past the table. Measured with a 1024 wide lit scene and a
 *   guard page (brender-archival parity README): the 1998 kernel reads up to
 *   about 975 KB past a 16 KB table, and faults on a normal heap table. The
 *   flat lit kernel (zb8p2fla.asm, below) masks the texel to a byte first, so
 *   it does not. This port skips the smooth read when it is past the table
 *   bound (see Additions); no shipped scene reaches that kernel.
 *
 * Additions, none of which changes a pixel the assembly would draw inside
 * its buffers and tables: a pixel or depth value outside its buffer is not
 * written, and a texel or shade entry outside its map or table is not read
 * (the pixel is skipped). The assembly relies on softrend having clipped the
 * triangle, and in the 1024 lit case reads whatever follows the table.
 */
#define __BR_V1DB__ 1
#include "brender.h"

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "drv.h"
#include "brender-pentprim-fpsetup-port.h"
#include "brender-pentprim-pfpsetup-port.h"

#define P2_SHADE_BYTES 0x10000u /* 256 rows of 256 entries */

static uint32_t p2_ror16(uint32_t v) { return (v >> 16) | (v << 16); }

enum { P2_UNLIT, P2_SMOOTH, P2_FLAT };

typedef struct p2_target {
    uint8_t *cbase, *zbase;
    const uint8_t *tex, *shade, *shade_row;
    int32_t climit, zlimit, cstride, zstride;
    uint32_t tex_bytes, mask, pow2;
    int32_t scan, depth;
} p2_target;

static int p2_open(p2_target *t, uint32_t pow2)
{
    t->cbase = (uint8_t *)work.colour.base;
    t->zbase = (uint8_t *)work.depth.base;
    t->tex = (const uint8_t *)work.texture.base;
    t->shade = work.shade_table;
    t->shade_row = fps.shade_row;
    t->cstride = work.colour.stride_b;
    t->zstride = work.depth.stride_b;
    t->climit = work.colour.stride_b * (int32_t)work.colour.height;
    t->zlimit = work.depth.stride_b * (int32_t)work.depth.height;
    t->tex_bytes = work.texture.size;
    t->pow2 = pow2;
    t->mask = (1u << pow2) - 1u;                     /* MAKE_N_LOW_BIT_MASK mask,pow2 */
    return t->cbase != NULL && t->zbase != NULL;
}

static int p2_zread(const p2_target *t, int32_t off, uint16_t *z)
{
    if (off < 0 || off + 2 > t->zlimit) return 0;
    memcpy(z, t->zbase + off, 2);
    return 1;
}

static void p2_write(const p2_target *t, int32_t coff, int32_t zoff, uint16_t z, uint8_t c)
{
    if (coff < 0 || coff >= t->climit || zoff < 0 || zoff + 2 > t->zlimit) return;
    memcpy(t->zbase + zoff, &z, 2);
    t->cbase[coff] = c;
}

/* shr v,16-pow2 ; and v,mask shl pow2 ; shr u,16 ; and u,mask ; or */
static uint32_t p2_texel(const p2_target *t, uint32_t u, uint32_t v)
{
    return ((v >> (16 - t->pow2)) & (t->mask << t->pow2)) | ((u >> 16) & t->mask);
}

/* add_d / adc_d edx,0 on the rotated z: the carry out of the fraction goes
 * into the integer in the low word. */
static uint32_t p2_step_z(uint32_t z, uint32_t dz, int rl)
{
    if (!rl) { uint64_t s = (uint64_t)z + dz; return (uint32_t)s + (uint32_t)(s >> 32); }
    { uint32_t cf = z < dz; return z - dz - cf; }
}

/* The per-line steps after lineDrawn: the major edge's fraction, then each
 * parameter by its carry (d_*_y_1) or no-carry (d_*_y_0) step. */
static void p2_next_line(p2_target *t, uint32_t *minor, uint32_t d_minor, int mode)
{
    uint32_t carry = (uint32_t)(((uint64_t)fps.xm_f + fps.d_xm_f) >> 32);
    fps.xm_f += fps.d_xm_f;
    t->depth += t->zstride;
    if (mode == P2_SMOOTH) fps.s_i += carry ? fps.d_i_y_1 : fps.d_i_y_0;
    fps.s_z += carry ? fps.d_z_y_1 : fps.d_z_y_0;
    fps.s_u += carry ? fps.d_u_y_1 : fps.d_u_y_0;
    fps.s_v += carry ? fps.d_v_y_1 : fps.d_v_y_0;
    t->scan += t->cstride;
    *minor += d_minor;
    fps.xm += fps.d_xm;
}

/* DRAW_ZT_I8_D16_POW2 (unlit), DRAW_ZTI_I8_D16_POW2 (smooth lit) and
 * DRAW_ZT_I8_D16_POW2_FLAT (flat lit): one half of the triangle. */
static void p2_half(p2_target *t, uint32_t *minor, uint32_t d_minor, int32_t *count, int rl, int mode)
{
    if (*count < 0) return;
    for (;;) {
        int32_t ebx = (int32_t)(*minor >> 16), cnt = (int32_t)(fps.xm >> 16);
        int32_t dest = t->scan + ebx, zrow = t->depth + 2 * ebx;
        int64_t diff = (int64_t)cnt - ebx;           /* sub ecx,ebx ; jg_d lineDrawn */
        cnt = (int32_t)((uint32_t)cnt - (uint32_t)ebx);
        if (!(rl ? diff < 0 : diff > 0)) {
            uint32_t z = p2_ror16(fps.s_z), u = fps.s_u, v = fps.s_v, i = fps.s_i;
            int64_t next;
            do {
                uint16_t zb;
                uint32_t at = p2_texel(t, u, v);
                if (p2_zread(t, zrow + 2 * cnt, &zb) && !((z & 0xffffu) > zb)  /* cmp dx,bx ; ja noPlot */
                    && t->tex != NULL && at < t->tex_bytes) {
                    uint8_t texel = t->tex[at];
                    if (texel != 0) {
                        uint8_t c = texel;
                        int draw = 1;
                        if (mode == P2_SMOOTH) {     /* mov ah,byte ptr c_i+2 ; mov al,[edi+eax] */
                            uint32_t row = (at & 0xffff0000u) | ((i >> 16) & 0xffu) << 8 | texel;
                            if (t->shade == NULL || row >= P2_SHADE_BYTES) draw = 0;
                            else c = t->shade[row];
                        } else if (mode == P2_FLAT) { /* and eax,0ffh ; mov al,[shade_row+eax] */
                            if (t->shade_row == NULL) draw = 0;
                            else c = t->shade_row[texel];
                        }
                        if (draw) p2_write(t, dest + cnt, zrow + 2 * cnt, (uint16_t)z, c);
                    }
                }
                z = p2_step_z(z, fps.d_z_x, rl);
                if (mode == P2_SMOOTH) i = rl ? i - fps.d_i_x : i + fps.d_i_x;
                if (rl) { v -= fps.d_v_x; u -= fps.d_u_x; }
                else { v += fps.d_v_x; u += fps.d_u_x; }
                next = (int64_t)cnt + (rl ? -1 : 1);  /* inc_d ecx ; jle_d drawPixel */
                cnt = (int32_t)((uint32_t)cnt + (rl ? 0xffffffffu : 1u));
            } while (rl ? next >= 0 : next <= 0);
        }
        p2_next_line(t, minor, d_minor, mode);
        if (--*count < 0) break;
    }
}

/* TriangleRasterise_ZT(I)_I8_D16_<n>: everything after the setup call. */
static void p2_rasterise(uint32_t pow2, int mode)
{
    p2_target t;
    uint32_t edx;
    int rl;
    if (!p2_open(&t, pow2)) return;
    t.scan = t.cstride * (fps.t_y - 1) - 1;          /* colour.base - 1 + stride * (t_y - 1) */
    t.depth = t.zstride * (fps.t_y - 1) - 2;
    fps.xm_f = fps.xm << 16;
    fps.d_xm_f = fps.d_xm << 16;
    edx = fps.d_z_x;                                  /* cmp edx,80000000 ; adc edx,-1 ; ror edx,16 */
    edx = edx < 80000000u ? edx : edx - 1u;
    fps.d_z_x = p2_ror16(edx);
    rl = fps.flip != 0;
    p2_half(&t, &fps.x1, fps.d_x1, &fps.topCount, rl, mode);
    p2_half(&t, &fps.x2, fps.d_x2, &fps.bottomCount, rl, mode);
}

void i8_rasterise_zt_pow2(uint32_t pow2) { p2_rasterise(pow2, P2_UNLIT); }
void i8_rasterise_zti_pow2(uint32_t pow2) { p2_rasterise(pow2, P2_SMOOTH); }

static void p2_render(union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2, uint32_t pow2, int mode)
{
    if (mode == P2_FLAT) fps_flat_shade(v0); /* TriangleSetup_ZT_FLAT_LIT: before SETUP_FLOAT */
    if (fps_setup(v0, v1, v2) != 0) return;
    fps_param(C_SZ, v0, v1, v2, &fps.s_z, &fps.d_z_y_1, &fps.d_z_x, &fps.d_z_y_0, 1);
    fps_param(C_U, v0, v1, v2, &fps.s_u, &fps.d_u_y_1, &fps.d_u_x, &fps.d_u_y_0, 0);
    fps_param(C_V, v0, v1, v2, &fps.s_v, &fps.d_v_y_1, &fps.d_v_x, &fps.d_v_y_0, 0);
    if (mode == P2_SMOOTH) fps_param(C_I, v0, v1, v2, &fps.s_i, &fps.d_i_y_1, &fps.d_i_x, &fps.d_i_y_0, 0);
    p2_rasterise(pow2, mode);
}

#define P2_KERNEL(name, mode, pow2)                                                                 \
    void BR_ASM_CALL name(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1,      \
                          union brp_vertex *v2) { (void)block; p2_render(v0, v1, v2, pow2, mode); }
#define P2_KERNELS(n, pow2)                                                                         \
    P2_KERNEL(TriangleRender_ZT_I8_D16_##n, P2_UNLIT, pow2)                                         \
    P2_KERNEL(TriangleRender_ZTI_I8_D16_##n, P2_SMOOTH, pow2)                                       \
    P2_KERNEL(TriangleRender_ZT_I8_D16_##n##_FLAT, P2_FLAT, pow2)

P2_KERNELS(8, 3)
P2_KERNELS(16, 4)
P2_KERNELS(32, 5)
P2_KERNELS(64, 6)
P2_KERNELS(128, 7)
P2_KERNEL(TriangleRender_ZT_I8_D16_256_FLAT, P2_FLAT, 8) /* 256: the smooth ZT/ZTI live in the i8 port */
P2_KERNELS(1024, 10)
