/*
 * SPDX-License-Identifier: MIT
 * Portions copyright (c) 1998 Argonaut Software Limited (BRender v1.3.2).
 * Copyright (c) 2026 Zain Dana Harper.
 * See compat/LICENSE-NOTE.md, LICENSE and LICENSES/MIT-BRender.txt.
 */
/*
 * The untextured z-buffered true colour kernels, a line-by-line C port of
 * BRender v1.3.2 drivers/pentprim (MIT, Argonaut Software 1998):
 *
 *   TriangleRenderPIZ2_RGB_888 / PIZ2I_RGB_888   tt24_piz.asm
 *   TriangleRenderPIZ2_RGB_555 / PIZ2I_RGB_555   tt15_piz.asm
 *   TriangleRenderPIZ2_RGB_565 / PIZ2I_RGB_565   tt16_piz.asm
 *
 * PIZ2 is constant colour, PIZ2I interpolated colour. The three files share
 * SETUP_PI, PARAM_PI_DIRN, FIX_DIV and the trapezoid loops; they differ in
 * the pixel size, in how a colour becomes a pixel, and in GDIVIDE: on
 * overflow tt15/tt16 give 0 where tt24 keeps the low dword of the numerator.
 *
 * The arithmetic is the assembly's: 32-bit wrapping adds, 64-bit products,
 * truncating divides, a 16-bit fraction stepping with carry, and the depth
 * test "draw if z < *zbuf" on the unsigned top half of the biased 16.16 z.
 * The scanline ends are compared as the assembly compares them, unsigned,
 * after scaling the pixel offsets to bytes. The prim blocks ask for SX, SY,
 * SZ, R, G and B as fixed point, so every component is read through comp_x.
 *
 * One addition: a pixel or depth value outside its buffer is not written.
 * The assembly relies on softrend having clipped the triangle; this guard
 * only matters when it has not, where the assembly would have written
 * outside the buffer.
 */
#define __BR_V1DB__ 1
#include "brender.h"

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "drv.h"

enum tt_format { TT_888, TT_555, TT_565 };

typedef struct tt_edge {
    int32_t x, y, grad, count;
    uint32_t f, d_f;
    int32_t i, d_i;
} tt_edge;

typedef struct tt_param {
    int32_t grad_x, grad_y, current, d_nocarry, d_carry;
} tt_param;

/* FIX_DIV then idiv: (dx << 16) / dy, truncating toward zero. */
static int32_t tt_fix_div(int32_t dx, int32_t dy)
{
    return (int32_t)(((int64_t)dx * 65536) / dy);
}

/* GDIVIDE: divide a signed 64-bit numerator by the positive g_divisor,
 * forwards (f) or backwards (b). On overflow tt24_piz.asm falls through
 * with the low dword of the (possibly negated) numerator in eax;
 * tt15_piz.asm and tt16_piz.asm jump to "xor eax,eax". */
static int32_t tt_gdivide(int64_t num, uint32_t g, int backwards, int zero_on_overflow)
{
    uint64_t m = num >= 0 ? (uint64_t)num : 0u - (uint64_t)num;
    uint32_t q;
    if (g <= (uint32_t)(m >> 32)) return zero_on_overflow ? 0 : (int32_t)(uint32_t)m;
    q = (uint32_t)(m / g);
    if (num >= 0) return backwards ? (int32_t)(0u - q) : (int32_t)q;
    return backwards ? (int32_t)q : (int32_t)(0u - q);
}

/* PARAM_PI_DIRN for one fixed-point component slot. */
static void tt_param_setup(tt_param *p, int slot, union brp_vertex *a, union brp_vertex *b, union brp_vertex *c,
                           const tt_edge *top, const tt_edge *main_e, uint32_t g, int backwards, int is_unsigned,
                           int zero_on_overflow)
{
    int32_t d2 = (int32_t)((uint32_t)c->comp_x[slot] - (uint32_t)a->comp_x[slot]);
    int32_t d1 = (int32_t)((uint32_t)b->comp_x[slot] - (uint32_t)a->comp_x[slot]);
    int32_t half;
    uint32_t cur;
    p->grad_x = tt_gdivide((int64_t)d1 * main_e->y - (int64_t)d2 * top->y, g, backwards, zero_on_overflow);
    p->grad_y = tt_gdivide((int64_t)d2 * top->x - (int64_t)d1 * main_e->x, g, backwards, zero_on_overflow);
    /* sar ebx,1 ; adc eax,ebx */
    half = p->grad_x >> 1;
    cur = (uint32_t)a->comp_x[slot] + (uint32_t)half + ((uint32_t)p->grad_x & 1u);
    if (is_unsigned) cur ^= 0x80000000u;
    p->current = (int32_t)cur;
    /* movsx eax, word ptr main.grad+2 ; imul grad_x ; add grad_y */
    p->d_nocarry = (int32_t)((uint32_t)((int32_t)(int16_t)((uint32_t)main_e->grad >> 16) * p->grad_x) + (uint32_t)p->grad_y);
    p->d_carry = (int32_t)((uint32_t)p->d_nocarry + (uint32_t)p->grad_x);
}

static void tt_edge_grad(tt_edge *e, int32_t stride)
{
    e->grad = tt_fix_div(e->x, e->y);
    e->d_f = ((uint32_t)e->grad & 0xffffu) << 16;  /* mov word ptr d_f+2, ax */
    e->d_i = (e->grad >> 16) + stride;              /* sar eax,16 ; add stride */
}

/* The constant colour word of RawTriangle_PIZ2_RGB_555 / _565, built from
 * the first vertex before the sort: each 16.16 channel masked to the top
 * bits of its integer byte and shifted into place. */
static uint32_t tt_flat_word(enum tt_format fmt, uint32_t r, uint32_t g, uint32_t b)
{
    if (fmt == TT_565)
        return ((r & 0xf80000u) >> 8) | ((g & 0xfc0000u) >> 13) | ((b & 0xf80000u) >> 19);
    return ((r & 0xf80000u) >> 9) | ((g & 0xf80000u) >> 14) | ((b & 0xf80000u) >> 19);
}

/* TRAPEZOID_PIZ2I_RGB_555 / _565's pixel: the integer byte of each channel
 * (byte ptr temp_x+2), shr 3 (green shr 2 for 565), packed with shl and or. */
static uint32_t tt_interp_word(enum tt_format fmt, uint32_t r, uint32_t g, uint32_t b)
{
    uint32_t r8 = (r >> 16) & 0xffu, g8 = (g >> 16) & 0xffu, b8 = (b >> 16) & 0xffu;
    if (fmt == TT_565) return (((r8 >> 3) << 6 | (g8 >> 2)) << 5) | (b8 >> 3);
    return (((r8 >> 3) << 5 | (g8 >> 3)) << 5) | (b8 >> 3);
}

typedef struct tt_target {
    uint8_t *cbase, *zbase;
    int64_t cbytes, zbytes;
    uint32_t bpp;
    enum tt_format fmt;
} tt_target;

/* One pixel: the depth test, then the depth and the colour. coff is the
 * colour byte offset (esi after the lea), zoff the depth byte offset. */
static void tt_plot(const tt_target *t, uint32_t coff, int64_t zoff, uint32_t zz, int interpolated,
                    uint32_t r, uint32_t g, uint32_t b, uint32_t flat_word)
{
    uint16_t z16 = (uint16_t)(zz >> 16), zbuf;
    if (zoff < 0 || zoff + 2 > t->zbytes || (int64_t)coff + t->bpp > t->cbytes) return;
    memcpy(&zbuf, t->zbase + zoff, 2);
    if (!(z16 < zbuf)) return;                  /* cmp ax,[edi] ; jae pixel_behind */
    memcpy(t->zbase + zoff, &z16, 2);
    if (t->fmt == TT_888) {
        uint8_t *cp = t->cbase + coff;
        cp[0] = (uint8_t)(b >> 16);
        cp[1] = (uint8_t)(g >> 16);
        cp[2] = (uint8_t)(r >> 16);
    } else {
        uint16_t w = (uint16_t)(interpolated ? tt_interp_word(t->fmt, r, g, b) : flat_word);
        memcpy(t->cbase + coff, &w, 2);
    }
}

/* SETUP_PI: sort by y, signed compares of the fixed values. */
static void tt_sort(union brp_vertex **pa, union brp_vertex **pb, union brp_vertex **pc)
{
    union brp_vertex *a = *pa, *b = *pb, *c = *pc, *t;
    int32_t ya = a->comp_x[C_SY], yb = b->comp_x[C_SY], yc = c->comp_x[C_SY];
    if (ya > yb) {
        if (yc > yb) {
            if (ya > yc) { t = a; a = b; b = t; t = b; b = c; c = t; } /* bca */
            else { t = a; a = b; b = t; }                               /* bac */
        } else { t = a; a = c; c = t; }                                 /* cba */
    } else if (yb > yc) {
        if (ya > yc) { t = b; b = c; c = t; t = a; a = b; b = t; }     /* cab */
        else { t = b; b = c; c = t; }                                   /* acb */
    }
    *pa = a; *pb = b; *pc = c;
}

static void tt_triangle(union brp_vertex *a, union brp_vertex *b, union brp_vertex *c, int interpolated,
                        enum tt_format fmt)
{
    tt_target tg;
    tt_edge top, bot, main_e;
    tt_param pz, pr, pg, pb;
    int32_t vx[3], vy[3], g, stride = (int32_t)work.colour.stride_p;
    uint32_t gd, temp_r = 0, temp_g = 0, temp_b = 0, flat_word = 0;
    int backwards, pass, zo = fmt != TT_888;
    int32_t esi, edx, ebp;

    tg.cbase = (uint8_t *)work.colour.base;
    tg.zbase = (uint8_t *)work.depth.base;
    tg.cbytes = (int64_t)work.colour.stride_b * work.colour.height;
    tg.zbytes = (int64_t)work.depth.stride_b * work.depth.height;
    tg.bpp = fmt == TT_888 ? 3u : 2u;
    tg.fmt = fmt;
    if (tg.cbase == NULL || tg.zbase == NULL) return;
    if (!interpolated) { /* colour comes from the first vertex, before sorting */
        temp_r = (uint32_t)a->comp_x[C_R];
        temp_g = (uint32_t)a->comp_x[C_G];
        temp_b = (uint32_t)a->comp_x[C_B];
        flat_word = tt_flat_word(fmt, temp_r, temp_g, temp_b);
    }

    tt_sort(&a, &b, &c);
    vx[0] = a->comp_x[C_SX] >> 16; vx[1] = b->comp_x[C_SX] >> 16; vx[2] = c->comp_x[C_SX] >> 16;
    vy[0] = a->comp_x[C_SY] >> 16; vy[1] = b->comp_x[C_SY] >> 16; vy[2] = c->comp_x[C_SY] >> 16;
    if (vy[0] == vy[2]) return;

    top.x = vx[1] - vx[0]; top.y = vy[1] - vy[0]; top.count = top.y;
    if (top.y != 0) tt_edge_grad(&top, stride);
    bot.x = vx[2] - vx[1]; bot.y = vy[2] - vy[1]; bot.count = bot.y;
    if (bot.y != 0) tt_edge_grad(&bot, stride);
    main_e.x = vx[2] - vx[0]; main_e.y = vy[2] - vy[0];
    tt_edge_grad(&main_e, stride);

    main_e.i = vy[0] * stride + vx[0];
    main_e.f = 0x80000000u;
    top.i = main_e.i;
    top.f = 0x80000000u;
    bot.i = vy[1] * stride + vx[1];
    bot.f = 0x80000000u;

    g = (int32_t)((uint32_t)top.x * (uint32_t)main_e.y - (uint32_t)main_e.x * (uint32_t)top.y);
    if (g == 0) return;
    backwards = g < 0;
    gd = backwards ? 0u - (uint32_t)g : (uint32_t)g;

    tt_param_setup(&pz, C_SZ, a, b, c, &top, &main_e, gd, backwards, 1, zo);
    if (interpolated) {
        tt_param_setup(&pr, C_R, a, b, c, &top, &main_e, gd, backwards, 0, zo);
        tt_param_setup(&pg, C_G, a, b, c, &top, &main_e, gd, backwards, 0, zo);
        tt_param_setup(&pb, C_B, a, b, c, &top, &main_e, gd, backwards, 0, zo);
    }

    esi = main_e.i;
    edx = pz.current;
    ebp = pz.grad_x;
    for (pass = 0; pass < 2; pass++) {
        tt_edge *half = pass == 0 ? &top : &bot;
        int32_t ecx;
        if (half->count == 0) continue;
        ecx = half->i;
        do {
            /* lea edi,[edi+esi*2] ; lea esi,[esi*bpp] ; lea ecx,[ecx*bpp] */
            int64_t zoff = (int64_t)esi * 2;
            uint32_t s = (uint32_t)esi * tg.bpp, e = (uint32_t)ecx * tg.bpp, zz = (uint32_t)edx, carry;
            uint32_t r = interpolated ? (uint32_t)pr.current : temp_r;
            uint32_t gg = interpolated ? (uint32_t)pg.current : temp_g;
            uint32_t bb = interpolated ? (uint32_t)pb.current : temp_b;
            if (!backwards ? s < e : e < s) {
                do {
                    if (backwards) {
                        zz -= (uint32_t)ebp;
                        if (interpolated) { r -= (uint32_t)pr.grad_x; gg -= (uint32_t)pg.grad_x; bb -= (uint32_t)pb.grad_x; }
                        zoff -= 2;
                        s -= tg.bpp;
                    }
                    tt_plot(&tg, s, zoff, zz, interpolated, r, gg, bb, flat_word);
                    if (!backwards) {
                        zz += (uint32_t)ebp;
                        if (interpolated) { r += (uint32_t)pr.grad_x; gg += (uint32_t)pg.grad_x; bb += (uint32_t)pb.grad_x; }
                        zoff += 2;
                        s += tg.bpp;
                    }
                } while (!backwards ? s < e : e < s);
            }
            /* next scanline */
            carry = (half->f + half->d_f) < half->f;
            half->f += half->d_f;
            ecx = (int32_t)((uint32_t)ecx + (uint32_t)half->d_i + carry);
            carry = (main_e.f + main_e.d_f) < main_e.f;
            main_e.f += main_e.d_f;
            esi = (int32_t)((uint32_t)esi + (uint32_t)main_e.d_i + carry);
            edx = (int32_t)((uint32_t)edx + (uint32_t)(carry ? pz.d_carry : pz.d_nocarry));
            if (interpolated) {
                pr.current = (int32_t)((uint32_t)pr.current + (uint32_t)(carry ? pr.d_carry : pr.d_nocarry));
                pg.current = (int32_t)((uint32_t)pg.current + (uint32_t)(carry ? pg.d_carry : pg.d_nocarry));
                pb.current = (int32_t)((uint32_t)pb.current + (uint32_t)(carry ? pb.d_carry : pb.d_nocarry));
            }
        } while (--half->count != 0);
    }
}

#define TT_KERNEL(name, interpolated, fmt) \
    void BR_ASM_CALL name(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2) \
    { (void)block; tt_triangle(v0, v1, v2, interpolated, fmt); }

TT_KERNEL(TriangleRenderPIZ2_RGB_888, 0, TT_888)
TT_KERNEL(TriangleRenderPIZ2I_RGB_888, 1, TT_888)
TT_KERNEL(TriangleRenderPIZ2_RGB_555, 0, TT_555)
TT_KERNEL(TriangleRenderPIZ2I_RGB_555, 1, TT_555)
TT_KERNEL(TriangleRenderPIZ2_RGB_565, 0, TT_565)
TT_KERNEL(TriangleRenderPIZ2I_RGB_565, 1, TT_565)
