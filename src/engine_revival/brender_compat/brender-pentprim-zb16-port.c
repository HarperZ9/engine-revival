/*
 * SPDX-License-Identifier: MIT
 * Portions copyright (c) 1998 Argonaut Software Limited (BRender v1.3.2).
 * Copyright (c) 2026 Zain Dana Harper.
 * See compat/LICENSE-NOTE.md, LICENSE and LICENSES/MIT-BRender.txt.
 */
/*
 * The unlit power-of-two 16-bit texture kernels, TriangleRender_ZT_RGB565_D16
 * _32, _64, _128, _256 and _1024: a line-by-line C port of
 * DRAW_ZT_RGB565_D16_POW2 and TriangleRender_ZT_RGB565_D16_POW2 in BRender
 * v1.3.2 drivers/pentprim/zb16p2ul.asm (MIT, Argonaut Software 1998), with
 * fpsetup.asm's TriangleSetup_ZT (SETUP_FLOAT and three SETUP_FLOAT_PARAMs,
 * in brender-pentprim-fpsetup-port.c).
 *
 * Both 16-bit tables use these kernels for a 2^n square map whose rows have
 * no padding (prm_t15f.c lists them for RGB_555 under the 565 name): the
 * kernel copies 16-bit texels and never looks inside them. A texel of 0 is
 * transparent. The depth test draws where z <= *zptr.
 *
 * Registers are 32-bit unsigned values, so adds wrap and the carries come
 * out where the assembly takes them. z is kept rotated (integer in the low
 * half, fraction in the high half) and stepped with add and adc 0, as the
 * assembly does, after its "cmp edx,80000000 ; adc edx,-1": the constant is
 * decimal, so a step of 80000000 (0x04C4B400) or more loses one.
 *
 * Additions, none of which changes a pixel the assembly would draw inside
 * its buffers: a pixel or depth value outside its buffer is not written,
 * and a texel outside the map is not read (the pixel is skipped). The
 * assembly relies on softrend having clipped the triangle.
 */
#define __BR_V1DB__ 1
#include "brender.h"

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "drv.h"
#include "brender-pentprim-fpsetup-port.h"

static uint32_t zb16_ror16(uint32_t v) { return (v >> 16) | (v << 16); }

typedef struct zb16_target {
    uint8_t *cbase, *zbase;
    const uint8_t *tex;
    int64_t cbytes, zbytes, tbytes;
    int32_t cstride, zstride;
} zb16_target;

static int zb16_open(zb16_target *t)
{
    t->cbase = (uint8_t *)work.colour.base;
    t->zbase = (uint8_t *)work.depth.base;
    t->tex = (const uint8_t *)work.texture.base;
    t->cstride = work.colour.stride_b;
    t->zstride = work.depth.stride_b;
    t->cbytes = (int64_t)work.colour.stride_b * work.colour.height;
    t->zbytes = (int64_t)work.depth.stride_b * work.depth.height;
    t->tbytes = (int64_t)work.texture.size;
    return t->cbase != NULL && t->zbase != NULL && t->tex != NULL;
}

/* drawPixel: the texel address, the depth test, the texel and the stores.
 * coff and zoff are byte offsets of this pixel from the buffer bases. */
static void zb16_pixel(const zb16_target *t, int32_t coff, int32_t zoff, uint32_t edx, uint32_t c_u, uint32_t c_v,
                       int pow2)
{
    uint32_t mask = (1u << pow2) - 1u, at;
    uint16_t zbuf, texel, z16 = (uint16_t)edx;
    /* shr esi,16-(pow2+1) ; and esi,mask shl (pow2+1) ; shr eax,15 ; and eax,mask shl 1 ; or eax,esi */
    at = ((c_v >> (15 - pow2)) & (mask << (pow2 + 1))) | ((c_u >> 15) & (mask << 1));
    if (zoff < 0 || (int64_t)zoff + 2 > t->zbytes) return;
    memcpy(&zbuf, t->zbase + zoff, 2);
    if (z16 > zbuf) return;                       /* cmp dx,bx ; ja noPlot */
    if ((int64_t)at + 2 > t->tbytes) return;
    memcpy(&texel, t->tex + at, 2);               /* mov ax,[esi+eax] */
    if (texel == 0) return;                       /* test ax,ax ; jz noPlot */
    if (coff < 0 || (int64_t)coff + 2 > t->cbytes) return;
    memcpy(t->cbase + coff, &texel, 2);
    memcpy(t->zbase + zoff, &z16, 2);
}

/* DRAW_ZT_RGB565_D16_POW2 minorX,direction,half,pow2: one half of the
 * triangle. scan and depth are scanAddress and depthAddress as offsets. */
static void zb16_half(const zb16_target *t, uint32_t *minor, uint32_t d_minor, int32_t *count, int rl, int pow2,
                      int32_t *scan, int32_t *depth)
{
    uint32_t edx, ebx, ecx, c_u, c_v;
    if (*count < 0) return;
    edx = fps.s_z;
    ebx = *minor;
    ecx = fps.xm;
    c_u = fps.s_u;
    c_v = fps.s_v;
    for (;;) {
        int32_t cnt, pix, zrow;
        ebx >>= 16;
        ecx >>= 16;
        pix = *scan + 2 * (int32_t)ebx;           /* lea edi,[edi+2*ebx] */
        zrow = *depth + 2 * (int32_t)ebx;         /* lea ebp,[ebp+2*ebx] */
        edx = zb16_ror16(edx);
        cnt = (int32_t)(ecx - ebx);
        if (rl ? cnt >= 0 : cnt <= 0) {           /* jg_d lineDrawn */
            do {
                zb16_pixel(t, pix + 2 * cnt, zrow + 2 * cnt, edx, c_u, c_v, pow2);
                if (!rl) {                        /* add_d edx,ebx ; adc_d edx,0 */
                    uint64_t s = (uint64_t)edx + fps.d_z_x;
                    edx = (uint32_t)s + (uint32_t)(s >> 32);
                    c_v += fps.d_v_x;
                    c_u += fps.d_u_x;
                } else {
                    uint32_t cf = edx < fps.d_z_x;
                    edx -= fps.d_z_x;
                    edx -= cf;
                    c_v -= fps.d_v_x;
                    c_u -= fps.d_u_x;
                }
                cnt += rl ? -1 : 1;               /* inc_d ecx ; jle_d drawPixel */
            } while (rl ? cnt >= 0 : cnt <= 0);
        }
        /* lineDrawn: the per line updates, d_*_y_1 after a carry */
        {
            uint32_t carry = (uint32_t)(((uint64_t)fps.xm_f + fps.d_xm_f) >> 32);
            fps.xm_f += fps.d_xm_f;
            *depth += t->zstride;
            c_u = fps.s_u += carry ? fps.d_u_y_1 : fps.d_u_y_0;
            c_v = fps.s_v += carry ? fps.d_v_y_1 : fps.d_v_y_0;
            edx = fps.s_z += carry ? fps.d_z_y_1 : fps.d_z_y_0;
            *scan += t->cstride;
            ebx = *minor += d_minor;
            ecx = fps.xm += fps.d_xm;
        }
        if (--*count < 0) break;
    }
}

/* TriangleRender_ZT_RGB565_D16_POW2 pow2: setup, then the rasteriser. */
static void zb16_triangle(union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2, int pow2)
{
    zb16_target t;
    int32_t scan, depth;
    if (fps_setup(v0, v1, v2) != 0) return;
    fps_param(C_SZ, v0, v1, v2, &fps.s_z, &fps.d_z_y_1, &fps.d_z_x, &fps.d_z_y_0, 1);
    fps_param(C_U, v0, v1, v2, &fps.s_u, &fps.d_u_y_1, &fps.d_u_x, &fps.d_u_y_0, 0);
    fps_param(C_V, v0, v1, v2, &fps.s_v, &fps.d_v_y_1, &fps.d_v_x, &fps.d_v_y_0, 0);
    if (!zb16_open(&t)) return;
    /* colour.base - 2 + colour.stride_b * (t_y - 1), and the same for depth,
     * on the FPU in the assembly; these integers in both. */
    scan = t.cstride * (fps.t_y - 1) - 2;
    depth = t.zstride * (fps.t_y - 1) - 2;
    fps.xm_f = fps.xm << 16;
    fps.d_xm_f = fps.d_xm << 16;
    if (fps.d_z_x >= 80000000u) fps.d_z_x -= 1u;  /* cmp edx,80000000 ; adc edx,-1 */
    fps.d_z_x = zb16_ror16(fps.d_z_x);
    zb16_half(&t, &fps.x1, fps.d_x1, &fps.topCount, fps.flip != 0, pow2, &scan, &depth);
    zb16_half(&t, &fps.x2, fps.d_x2, &fps.bottomCount, fps.flip != 0, pow2, &scan, &depth);
}

#define ZB16_KERNEL(size, pow2) \
    void BR_ASM_CALL TriangleRender_ZT_RGB565_D16_##size(struct brp_block *block, union brp_vertex *v0, \
                                                         union brp_vertex *v1, union brp_vertex *v2) \
    { (void)block; zb16_triangle(v0, v1, v2, pow2); }

ZB16_KERNEL(32, 5)
ZB16_KERNEL(64, 6)
ZB16_KERNEL(128, 7)
ZB16_KERNEL(256, 8)
ZB16_KERNEL(1024, 10)
