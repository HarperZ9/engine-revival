/*
 * SPDX-License-Identifier: MIT
 * Portions copyright (c) 1998 Argonaut Software Limited (BRender v1.3.2).
 * Copyright (c) 2026 Zain Dana Harper.
 * See compat/LICENSE-NOTE.md, LICENSE and LICENSES/MIT-BRender.txt.
 */
/*
 * Pentprim C port: DIAGNOSTIC build - every stub prints when called.
 * This traces which rasterizer functions fire during BrZbSceneRender.
 */
#define __BR_V1DB__ 1
#include "brender.h"

#include <stdio.h>
#include "drv.h"

#if defined(_DEBUG)
#include <crtdbg.h>
#endif

/* ---- real-semantics helpers ---- */

int _sar16(int v)
{
    return v >> 16;
}

#undef BrFixedDiv
br_int_32 BrFixedDiv(br_int_32 a, br_int_32 b)
{
    if (b == 0) return (a >= 0) ? 0x7fffffff : -0x7fffffff;
    return (br_int_32)(((long long)a << 16) / b);
}

br_int_32 SafeFixedMac2Div(br_int_32 a, br_int_32 b, br_int_32 c, br_int_32 d, br_int_32 e)
{
    br_int_32 num = a * b;
    br_int_32 den = c * d;
    if (den == 0) return (num >= 0) ? 0x7fffffff : -0x7fffffff;
    return (br_int_32)(((long long)num << 16) / den);
}

void RasteriseBufferDisable(void)
{
}

/* ---- diagnostic stubs: print name when called ---- */

void BR_ASM_CALL TrapezoidRenderPIZ2TA(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2, br_uint_16 (*fp_vertices)[3]) { (void)block; (void)v0; (void)v1; (void)v2; (void)fp_vertices; }
void BR_ASM_CALL TrapezoidRenderPIZ2TIA(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2, br_uint_16 (*fp_vertices)[3]) { (void)block; (void)v0; (void)v1; (void)v2; (void)fp_vertices; }
void BR_ASM_CALL TrapezoidRenderPIZ2TAN(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2, br_uint_16 (*fp_vertices)[3]) { (void)block; (void)v0; (void)v1; (void)v2; (void)fp_vertices; }
void BR_ASM_CALL TrapezoidRenderPIZ2TA15(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2, br_uint_16 (*fp_vertices)[3]) { (void)block; (void)v0; (void)v1; (void)v2; (void)fp_vertices; }
void BR_ASM_CALL TrapezoidRenderPIZ2TA24(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2, br_uint_16 (*fp_vertices)[3]) { (void)block; (void)v0; (void)v1; (void)v2; (void)fp_vertices; }
void BR_ASM_CALL TrapezoidRenderPIZ2TIA_RGB_555(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2, br_uint_16 (*fp_vertices)[3]) { (void)block; (void)v0; (void)v1; (void)v2; (void)fp_vertices; }

void BR_ASM_CALL ScanLinePITIP256_RGB_555(void)
{
    fprintf(stderr, "CALL ScanLinePITIP256_RGB_555\n"); fflush(stderr);
    
}

void BR_ASM_CALL ScanLinePITIPB256_RGB_555(void)
{
    fprintf(stderr, "CALL ScanLinePITIPB256_RGB_555\n"); fflush(stderr);
    
}

void BR_ASM_CALL ScanLinePITIPB256_RGB_565(void)
{
    fprintf(stderr, "CALL ScanLinePITIPB256_RGB_565\n"); fflush(stderr);
    
}

void BR_ASM_CALL ScanLinePIZ2TIP256_RGB_555(void)
{
    fprintf(stderr, "CALL ScanLinePIZ2TIP256_RGB_555\n"); fflush(stderr);
    
}

void BR_ASM_CALL ScanLinePIZ2TIPB256_RGB_555(void)
{
    fprintf(stderr, "CALL ScanLinePIZ2TIPB256_RGB_555\n"); fflush(stderr);
    
}

void BR_ASM_CALL ScanLinePIZ2TIPB256_RGB_565(void)
{
    fprintf(stderr, "CALL ScanLinePIZ2TIPB256_RGB_565\n"); fflush(stderr);
    
}

void BR_ASM_CALL TrapezoidRenderPITA(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2, br_uint_16 (*fp_vertices)[3])
{
    fprintf(stderr, "CALL TrapezoidRenderPITA\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2; (void)fp_vertices;
}

void BR_ASM_CALL TrapezoidRenderPITA15(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2, br_uint_16 (*fp_vertices)[3])
{
    fprintf(stderr, "CALL TrapezoidRenderPITA15\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2; (void)fp_vertices;
}

void BR_ASM_CALL TrapezoidRenderPITA24(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2, br_uint_16 (*fp_vertices)[3])
{
    fprintf(stderr, "CALL TrapezoidRenderPITA24\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2; (void)fp_vertices;
}

void BR_ASM_CALL TrapezoidRenderPITAN(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2, br_uint_16 (*fp_vertices)[3])
{
    fprintf(stderr, "CALL TrapezoidRenderPITAN\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2; (void)fp_vertices;
}

void BR_ASM_CALL TrapezoidRenderPITIA(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2, br_uint_16 (*fp_vertices)[3])
{
    fprintf(stderr, "CALL TrapezoidRenderPITIA\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2; (void)fp_vertices;
}

void BR_ASM_CALL TrapezoidRenderPITIA_RGB_555(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2, br_uint_16 (*fp_vertices)[3])
{
    fprintf(stderr, "CALL TrapezoidRenderPITIA_RGB_555\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2; (void)fp_vertices;
}

void BR_ASM_CALL TrapezoidRenderPITIA_RGB_888(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2, br_uint_16 (*fp_vertices)[3])
{
    fprintf(stderr, "CALL TrapezoidRenderPITIA_RGB_888\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2; (void)fp_vertices;
}















void BR_ASM_CALL TriangleRenderPII_RGB_555(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRenderPII_RGB_555\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRenderPII_RGB_565(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRenderPII_RGB_565\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRenderPII_RGB_888(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRenderPII_RGB_888\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRenderPIZ2I_RGB_555(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRenderPIZ2I_RGB_555\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRenderPIZ2I_RGB_565(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRenderPIZ2I_RGB_565\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRenderPIZ2TPD1024(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRenderPIZ2TPD1024\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRenderPIZ2TPD128(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRenderPIZ2TPD128\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRenderPIZ2TPD256(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRenderPIZ2TPD256\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRenderPIZ2TPD64(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRenderPIZ2TPD64\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRenderPIZ2_RGB_555(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRenderPIZ2_RGB_555\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRenderPIZ2_RGB_565(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRenderPIZ2_RGB_565\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}


/*
 * TriangleRenderPIZ2_RGB_888 (constant colour) and TriangleRenderPIZ2I_RGB_888
 * (interpolated colour): a line-by-line C port of RawTriangle_PIZ2_RGB_888 and
 * RawTriangle_PIZ2I_RGB_888 in BRender v1.3.2 drivers/pentprim/tt24_piz.asm
 * (MIT, Argonaut Software 1998), with the SETUP_PI, PARAM_PI_DIRN, GDIVIDE
 * and FIX_DIV macros it uses.
 *
 * The arithmetic is the assembly's: 32-bit wrapping adds, 64-bit products,
 * truncating divides, a 16-bit fraction stepping with carry, and the depth
 * test "draw if z < *zbuf" on the unsigned top half of the biased 16.16 z.
 * The prim blocks in prim_t24.c ask for SX, SY, SZ, R, G and B as fixed
 * point, so every component is read through comp_x.
 *
 * One addition: a pixel whose offset falls outside the colour buffer is not
 * written. The assembly relies on softrend having clipped the triangle; this
 * guard only matters when it has not, where the assembly would have written
 * outside the buffer.
 */
#include <stdint.h>

typedef struct tt24_edge {
    int32_t x, y, grad, count;
    uint32_t f, d_f;
    int32_t i, d_i;
} tt24_edge;

typedef struct tt24_param {
    int32_t grad_x, grad_y, current, d_nocarry, d_carry;
} tt24_param;

/* FIX_DIV then idiv: (dx << 16) / dy, truncating toward zero. */
static int32_t tt24_fix_div(int32_t dx, int32_t dy)
{
    return (int32_t)(((int64_t)dx * 65536) / dy);
}

/* GDIVIDE: divide a signed 64-bit numerator by the positive g_divisor,
 * forwards (f) or backwards (b), with the macro's overflow behaviour (the
 * quotient register keeps the low dword of the possibly negated numerator). */
static int32_t tt24_gdivide(int64_t num, uint32_t g, int backwards)
{
    if (num >= 0) {
        uint64_t m = (uint64_t)num;
        if (g <= (uint32_t)(m >> 32)) return (int32_t)(uint32_t)m;
        {
            uint32_t q = (uint32_t)(m / g);
            return backwards ? (int32_t)(0u - q) : (int32_t)q;
        }
    } else {
        uint64_t m = 0u - (uint64_t)num;
        if (g <= (uint32_t)(m >> 32)) return (int32_t)(uint32_t)m;
        {
            uint32_t q = (uint32_t)(m / g);
            return backwards ? (int32_t)q : (int32_t)(0u - q);
        }
    }
}

/* PARAM_PI_DIRN for one fixed-point component slot. */
static void tt24_param_setup(tt24_param *p, int slot, union brp_vertex *a, union brp_vertex *b, union brp_vertex *c,
                             const tt24_edge *top, const tt24_edge *main_e, uint32_t g, int backwards, int is_unsigned)
{
    int32_t d2 = (int32_t)((uint32_t)c->comp_x[slot] - (uint32_t)a->comp_x[slot]);
    int32_t d1 = (int32_t)((uint32_t)b->comp_x[slot] - (uint32_t)a->comp_x[slot]);
    int32_t half;
    uint32_t cur;
    p->grad_x = tt24_gdivide((int64_t)d1 * main_e->y - (int64_t)d2 * top->y, g, backwards);
    p->grad_y = tt24_gdivide((int64_t)d2 * top->x - (int64_t)d1 * main_e->x, g, backwards);
    /* sar ebx,1 ; adc eax,ebx */
    half = p->grad_x >> 1;
    cur = (uint32_t)a->comp_x[slot] + (uint32_t)half + ((uint32_t)p->grad_x & 1u);
    if (is_unsigned) cur ^= 0x80000000u;
    p->current = (int32_t)cur;
    /* movsx eax, word ptr main.grad+2 ; imul grad_x ; add grad_y */
    p->d_nocarry = (int32_t)((uint32_t)((int32_t)(int16_t)((uint32_t)main_e->grad >> 16) * p->grad_x) + (uint32_t)p->grad_y);
    p->d_carry = (int32_t)((uint32_t)p->d_nocarry + (uint32_t)p->grad_x);
}

static void tt24_edge_grad(tt24_edge *e, int32_t stride)
{
    e->grad = tt24_fix_div(e->x, e->y);
    e->d_f = ((uint32_t)e->grad & 0xffffu) << 16;  /* mov word ptr d_f+2, ax */
    e->d_i = (e->grad >> 16) + stride;              /* sar eax,16 ; add stride */
}

static void tt24_triangle(union brp_vertex *a, union brp_vertex *b, union brp_vertex *c, int interpolated)
{
    union brp_vertex *t;
    tt24_edge top, bot, main_e;
    tt24_param pz, pr, pg, pb;
    int32_t vx[3], vy[3], g, stride = (int32_t)work.colour.stride_p;
    int32_t limit = stride * (int32_t)work.colour.height;
    uint32_t gd, temp_r = 0, temp_g = 0, temp_b = 0;
    int backwards, pass;
    uint8_t *cbase = (uint8_t *)work.colour.base;
    uint8_t *zbase = (uint8_t *)work.depth.base;
    int32_t esi, edx, ebp;

    if (cbase == NULL || zbase == NULL) return;
    if (!interpolated) { /* colour comes from the first vertex, before sorting */
        temp_r = (uint32_t)a->comp_x[C_R];
        temp_g = (uint32_t)a->comp_x[C_G];
        temp_b = (uint32_t)a->comp_x[C_B];
    }

    /* SETUP_PI: sort by y, signed compares of the fixed values */
    {
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
    }
    vx[0] = a->comp_x[C_SX] >> 16; vx[1] = b->comp_x[C_SX] >> 16; vx[2] = c->comp_x[C_SX] >> 16;
    vy[0] = a->comp_x[C_SY] >> 16; vy[1] = b->comp_x[C_SY] >> 16; vy[2] = c->comp_x[C_SY] >> 16;
    if (vy[0] == vy[2]) return;

    top.x = vx[1] - vx[0]; top.y = vy[1] - vy[0]; top.count = top.y;
    if (top.y != 0) tt24_edge_grad(&top, stride);
    bot.x = vx[2] - vx[1]; bot.y = vy[2] - vy[1]; bot.count = bot.y;
    if (bot.y != 0) tt24_edge_grad(&bot, stride);
    main_e.x = vx[2] - vx[0]; main_e.y = vy[2] - vy[0];
    tt24_edge_grad(&main_e, stride);

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

    tt24_param_setup(&pz, C_SZ, a, b, c, &top, &main_e, gd, backwards, 1);
    if (interpolated) {
        tt24_param_setup(&pr, C_R, a, b, c, &top, &main_e, gd, backwards, 0);
        tt24_param_setup(&pg, C_G, a, b, c, &top, &main_e, gd, backwards, 0);
        tt24_param_setup(&pb, C_B, a, b, c, &top, &main_e, gd, backwards, 0);
    }

    esi = main_e.i;
    edx = pz.current;
    ebp = pz.grad_x;
    for (pass = 0; pass < 2; pass++) {
        tt24_edge *half = pass == 0 ? &top : &bot;
        int32_t ecx;
        if (half->count == 0) continue;
        ecx = half->i;
        do {
            int32_t p = esi, zz = edx;
            uint32_t r = interpolated ? (uint32_t)pr.current : temp_r;
            uint32_t gg = interpolated ? (uint32_t)pg.current : temp_g;
            uint32_t bb = interpolated ? (uint32_t)pb.current : temp_b;
            uint32_t carry;
            if (!backwards ? p < ecx : ecx < p) {
                do {
                    if (backwards) {
                        zz = (int32_t)((uint32_t)zz - (uint32_t)ebp);
                        if (interpolated) { r -= (uint32_t)pr.grad_x; gg -= (uint32_t)pg.grad_x; bb -= (uint32_t)pb.grad_x; }
                        p--;
                    }
                    if (p >= 0 && p < limit) {
                        uint16_t z16 = (uint16_t)((uint32_t)zz >> 16);
                        uint16_t *zp = (uint16_t *)(zbase + (size_t)p * 2);
                        if (z16 < *zp) {
                            uint8_t *cp = cbase + (size_t)p * 3;
                            *zp = z16;
                            cp[0] = (uint8_t)(bb >> 16);
                            cp[1] = (uint8_t)(gg >> 16);
                            cp[2] = (uint8_t)(r >> 16);
                        }
                    }
                    if (!backwards) {
                        zz = (int32_t)((uint32_t)zz + (uint32_t)ebp);
                        if (interpolated) { r += (uint32_t)pr.grad_x; gg += (uint32_t)pg.grad_x; bb += (uint32_t)pb.grad_x; }
                        p++;
                    }
                } while (!backwards ? p < ecx : ecx < p);
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

void BR_ASM_CALL TriangleRenderPIZ2I_RGB_888(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    (void)block;
    tt24_triangle(v0, v1, v2, 1);
}

void BR_ASM_CALL TriangleRenderPIZ2_RGB_888(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    (void)block;
    tt24_triangle(v0, v1, v2, 0);
}

void BR_ASM_CALL TriangleRenderPI_RGB_555(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRenderPI_RGB_555\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRenderPI_RGB_565(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRenderPI_RGB_565\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRenderPI_RGB_888(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRenderPI_RGB_888\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_I8(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_I8\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_I_I8(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_I_I8\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_PTI_I8_1024(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_PTI_I8_1024\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_PTI_I8_1024_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_PTI_I8_1024_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_PTI_I8_128(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_PTI_I8_128\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_PTI_I8_128_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_PTI_I8_128_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_PTI_I8_256(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_PTI_I8_256\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_PTI_I8_256_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_PTI_I8_256_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_PTI_I8_64(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_PTI_I8_64\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_PTI_I8_64_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_PTI_I8_64_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_PT_I8_1024(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_PT_I8_1024\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_PT_I8_128(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_PT_I8_128\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_PT_I8_256(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_PT_I8_256\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_PT_I8_32(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_PT_I8_32\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_PT_I8_64(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_PT_I8_64\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_TID_I8(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_TID_I8\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_TID_I8_256(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_TID_I8_256\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_TID_I8_256_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_TID_I8_256_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_TID_I8_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_TID_I8_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_TI_I8(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_TI_I8\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_TI_I8_256(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_TI_I8_256\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_TI_I8_256_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_TI_I8_256_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_TI_I8_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_TI_I8_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_T_I8(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_T_I8\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_T_I8_1024(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_T_I8_1024\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_T_I8_128(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_T_I8_128\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_T_I8_256(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_T_I8_256\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_T_I8_32(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_T_I8_32\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_T_I8_64(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_T_I8_64\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZIF_I8_D16(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZIF_I8_D16\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZIF_I8_D16_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZIF_I8_D16_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZI_I8_D16(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZI_I8_D16\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZI_I8_D16_ShadeTable(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZI_I8_D16_ShadeTable\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTB_I8_D16_1024(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTB_I8_D16_1024\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTB_I8_D16_128(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTB_I8_D16_128\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTB_I8_D16_256(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTB_I8_D16_256\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTB_I8_D16_32(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTB_I8_D16_32\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTB_I8_D16_64(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTB_I8_D16_64\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTFB_I8_D16_1024(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTFB_I8_D16_1024\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTFB_I8_D16_128(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTFB_I8_D16_128\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTFB_I8_D16_256(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTFB_I8_D16_256\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTFB_I8_D16_32(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTFB_I8_D16_32\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTFB_I8_D16_64(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTFB_I8_D16_64\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTF_I8_D16_1024(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTF_I8_D16_1024\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTF_I8_D16_128(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTF_I8_D16_128\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTF_I8_D16_256(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTF_I8_D16_256\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTF_I8_D16_32(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTF_I8_D16_32\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTF_I8_D16_64(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTF_I8_D16_64\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTIB_I8_D16_1024(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTIB_I8_D16_1024\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTIB_I8_D16_1024_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTIB_I8_D16_1024_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTIB_I8_D16_128(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTIB_I8_D16_128\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTIB_I8_D16_128_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTIB_I8_D16_128_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTIB_I8_D16_256(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTIB_I8_D16_256\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTIB_I8_D16_256_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTIB_I8_D16_256_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTIB_I8_D16_32(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTIB_I8_D16_32\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTIB_I8_D16_32_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTIB_I8_D16_32_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTIB_I8_D16_64(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTIB_I8_D16_64\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTIB_I8_D16_64_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTIB_I8_D16_64_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTIFB_I8_D16_1024(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTIFB_I8_D16_1024\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTIFB_I8_D16_1024_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTIFB_I8_D16_1024_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTIFB_I8_D16_128(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTIFB_I8_D16_128\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTIFB_I8_D16_128_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTIFB_I8_D16_128_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTIFB_I8_D16_256(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTIFB_I8_D16_256\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTIFB_I8_D16_256_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTIFB_I8_D16_256_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTIFB_I8_D16_32(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTIFB_I8_D16_32\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTIFB_I8_D16_32_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTIFB_I8_D16_32_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTIFB_I8_D16_64(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTIFB_I8_D16_64\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTIFB_I8_D16_64_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTIFB_I8_D16_64_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTIF_I8_D16_1024(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTIF_I8_D16_1024\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTIF_I8_D16_1024_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTIF_I8_D16_1024_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTIF_I8_D16_128(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTIF_I8_D16_128\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTIF_I8_D16_128_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTIF_I8_D16_128_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTIF_I8_D16_256(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTIF_I8_D16_256\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTIF_I8_D16_256_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTIF_I8_D16_256_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTIF_I8_D16_32(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTIF_I8_D16_32\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTIF_I8_D16_32_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTIF_I8_D16_32_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTIF_I8_D16_64(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTIF_I8_D16_64\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTIF_I8_D16_64_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTIF_I8_D16_64_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTI_I8_D16_1024(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTI_I8_D16_1024\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTI_I8_D16_1024_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTI_I8_D16_1024_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTI_I8_D16_128(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTI_I8_D16_128\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTI_I8_D16_128_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTI_I8_D16_128_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTI_I8_D16_256(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTI_I8_D16_256\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTI_I8_D16_256_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTI_I8_D16_256_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTI_I8_D16_32(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTI_I8_D16_32\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTI_I8_D16_32_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTI_I8_D16_32_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTI_I8_D16_64(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTI_I8_D16_64\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTI_I8_D16_64_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTI_I8_D16_64_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPT_I8_D16_1024(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPT_I8_D16_1024\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPT_I8_D16_128(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPT_I8_D16_128\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPT_I8_D16_256(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPT_I8_D16_256\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPT_I8_D16_32(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPT_I8_D16_32\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPT_I8_D16_64(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPT_I8_D16_64\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTB_I8_D16(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTB_I8_D16\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTB_I8_D16_1024(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTB_I8_D16_1024\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTB_I8_D16_1024_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTB_I8_D16_1024_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTB_I8_D16_128(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTB_I8_D16_128\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTB_I8_D16_128_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTB_I8_D16_128_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTB_I8_D16_16(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTB_I8_D16_16\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTB_I8_D16_16_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTB_I8_D16_16_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTB_I8_D16_256(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTB_I8_D16_256\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTB_I8_D16_256_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTB_I8_D16_256_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTB_I8_D16_32(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTB_I8_D16_32\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTB_I8_D16_32_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTB_I8_D16_32_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTB_I8_D16_64(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTB_I8_D16_64\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTB_I8_D16_64_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTB_I8_D16_64_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTB_I8_D16_8(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTB_I8_D16_8\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTB_I8_D16_8_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTB_I8_D16_8_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTFB_I8_D16(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTFB_I8_D16\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTF_I8_D16(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTF_I8_D16\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTF_I8_D16_256(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTF_I8_D16_256\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTIB_I8_D16(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTIB_I8_D16\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTIB_I8_D16_1024(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTIB_I8_D16_1024\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTIB_I8_D16_128(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTIB_I8_D16_128\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTIB_I8_D16_16(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTIB_I8_D16_16\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTIB_I8_D16_256(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTIB_I8_D16_256\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTIB_I8_D16_32(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTIB_I8_D16_32\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTIB_I8_D16_64(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTIB_I8_D16_64\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTIB_I8_D16_8(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTIB_I8_D16_8\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTIB_I8_D16_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTIB_I8_D16_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTID_I8_D16(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTID_I8_D16\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTID_I8_D16_1024(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTID_I8_D16_1024\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTID_I8_D16_1024_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTID_I8_D16_1024_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTID_I8_D16_128(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTID_I8_D16_128\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTID_I8_D16_128_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTID_I8_D16_128_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTID_I8_D16_256(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTID_I8_D16_256\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTID_I8_D16_256_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTID_I8_D16_256_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTID_I8_D16_32(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTID_I8_D16_32\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTID_I8_D16_32_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTID_I8_D16_32_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTID_I8_D16_64(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTID_I8_D16_64\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTID_I8_D16_64_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTID_I8_D16_64_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTID_I8_D16_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTID_I8_D16_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTIFB_I8_D16(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTIFB_I8_D16\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTIFB_I8_D16_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTIFB_I8_D16_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTIF_I8_D16(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTIF_I8_D16\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTIF_I8_D16_256(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTIF_I8_D16_256\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTIF_I8_D16_256_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTIF_I8_D16_256_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTIF_I8_D16_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTIF_I8_D16_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTI_I8_D16(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTI_I8_D16\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTI_I8_D16_1024(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTI_I8_D16_1024\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTI_I8_D16_128(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTI_I8_D16_128\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTI_I8_D16_16(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTI_I8_D16_16\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTI_I8_D16_256(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTI_I8_D16_256\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTI_I8_D16_32(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTI_I8_D16_32\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTI_I8_D16_64(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTI_I8_D16_64\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTI_I8_D16_8(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTI_I8_D16_8\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTI_I8_D16_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTI_I8_D16_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZT_I8_D16(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZT_I8_D16\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZT_I8_D16_1024(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZT_I8_D16_1024\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZT_I8_D16_1024_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZT_I8_D16_1024_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZT_I8_D16_128(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZT_I8_D16_128\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZT_I8_D16_128_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZT_I8_D16_128_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZT_I8_D16_16(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZT_I8_D16_16\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZT_I8_D16_16_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZT_I8_D16_16_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZT_I8_D16_256(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZT_I8_D16_256\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZT_I8_D16_256_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZT_I8_D16_256_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZT_I8_D16_32(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZT_I8_D16_32\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZT_I8_D16_32_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZT_I8_D16_32_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZT_I8_D16_64(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZT_I8_D16_64\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZT_I8_D16_64_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZT_I8_D16_64_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZT_I8_D16_8(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZT_I8_D16_8\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZT_I8_D16_8_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZT_I8_D16_8_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZT_RGB565_D16_1024(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZT_RGB565_D16_1024\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZT_RGB565_D16_128(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZT_RGB565_D16_128\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZT_RGB565_D16_256(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZT_RGB565_D16_256\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZT_RGB565_D16_32(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZT_RGB565_D16_32\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZT_RGB565_D16_64(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZT_RGB565_D16_64\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_Z_I8_D16(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_Z_I8_D16\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_Z_I8_D16_ShadeTable(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_Z_I8_D16_ShadeTable\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void sar16(void)
{
    fprintf(stderr, "CALL sar16\n"); fflush(stderr);
    
}

/* ------------------------------------------------------------------ */
/* Real TriangleRenderPIZ2TIA_RGB_888: Z-buffered, iterated-intensity, */
/* affine-UV textured triangle into an RGB_888 target. Faithful to the */
/* period PIZ2TIA family (affine texture stepping, LIGHT=1), with the  */
/* measured DEPTH_16 convention: buffer clears to 0 and NEARER wins.   */
void BR_ASM_CALL TriangleRenderPIZ2TIA_RGB_888(struct brp_block *block, union brp_vertex *a, union brp_vertex *b, union brp_vertex *c)
{
    float x0 = (float)a->comp_x[C_SX] * (1.0f/65536.0f), y0 = (float)a->comp_x[C_SY] * (1.0f/65536.0f);
    float x1 = (float)b->comp_x[C_SX] * (1.0f/65536.0f), y1 = (float)b->comp_x[C_SY] * (1.0f/65536.0f);
    float x2 = (float)c->comp_x[C_SX] * (1.0f/65536.0f), y2 = (float)c->comp_x[C_SY] * (1.0f/65536.0f);
    long  z0 = a->comp_x[C_SZ], z1 = b->comp_x[C_SZ], z2 = c->comp_x[C_SZ];
    long  i0 = a->comp_x[C_I],  i1 = b->comp_x[C_I],  i2 = c->comp_x[C_I];
    long  u0 = a->comp_x[C_U],  u1 = b->comp_x[C_U],  u2 = c->comp_x[C_U];
    long  v0 = a->comp_x[C_V],  v1 = b->comp_x[C_V],  v2 = c->comp_x[C_V];

    int minx = (int)(x0 < x1 ? (x0 < x2 ? x0 : x2) : (x1 < x2 ? x1 : x2));
    int maxx = (int)(x0 > x1 ? (x0 > x2 ? x0 : x2) : (x1 > x2 ? x1 : x2)) + 1;
    int miny = (int)(y0 < y1 ? (y0 < y2 ? y0 : y2) : (y1 < y2 ? y1 : y2));
    int maxy = (int)(y0 > y1 ? (y0 > y2 ? y0 : y2) : (y1 > y2 ? y1 : y2)) + 1;

    if (minx < 0) minx = 0;
    if (miny < 0) miny = 0;
    if (maxx > (int)work.colour.width_p) maxx = (int)work.colour.width_p;
    if (maxy > (int)work.colour.height)  maxy = (int)work.colour.height;
    if (minx >= maxx || miny >= maxy) return;

    float d = (y1 - y2) * (x0 - x2) + (x2 - x1) * (y0 - y2);
    if (d == 0.0f) return;

    int twm = (int)work.texture.width_p - 1;
    int thm = (int)work.texture.height - 1;
    char *tbase = (char *)work.texture.base;
    int tstride = work.texture.stride_b;
    br_uint_32 *pal = (br_uint_32 *)work.texture.palette;

    char *cbase = (char *)work.colour.base;
    char *zbase = (char *)work.depth.base;
    int cstride = work.colour.stride_b;
    int zstride = work.depth.stride_b;

    for (int yy = miny; yy < maxy; yy++) {
        for (int xx = minx; xx < maxx; xx++) {
            float px = (float)xx + 0.5f, py = (float)yy + 0.5f;
            float w0 = ((y1 - y2) * (px - x2) + (x2 - x1) * (py - y2)) / d;
            float w1 = ((y2 - y0) * (px - x2) + (x0 - x2) * (py - y2)) / d;
            float w2 = 1.0f - w0 - w1;
            if (w0 < 0.0f || w1 < 0.0f || w2 < 0.0f) continue;

            long zz = (w0 * z0 + w1 * z1 + w2 * z2) > 0.0f
                    ? (long)((w0 * z0 + w1 * z1 + w2 * z2) / 65536.0f)
                    : 0;
            if (zz > 65535) zz = 65535;

            unsigned short *zptr = (unsigned short *)(zbase + yy * zstride + xx * 2);
            if (*zptr != 0 && zz < (long)*zptr) continue;

            int uu = (int)((w0 * u0 + w1 * u1 + w2 * u2) / 65536.0f) & twm;
            int vv = (int)((w0 * v0 + w1 * v1 + w2 * v2) / 65536.0f) & thm;

            unsigned char texel = *(unsigned char *)(tbase + vv * tstride + uu);
            br_uint_32 rgb = pal[texel];

            long ii = (long)(w0 * i0 + w1 * i1 + w2 * i2);
            int shade = (int)(ii >> 8);
            if (shade < 0) shade = 0;
            if (shade > 255) shade = 255;

            char *cp = cbase + yy * cstride + xx * 3;
            cp[0] = (char)((((rgb >> 16) & 0xFF) * shade) >> 8);
            cp[1] = (char)((((rgb >> 8) & 0xFF) * shade) >> 8);
            cp[2] = (char)(((rgb & 0xFF) * shade) >> 8);
            *zptr = (unsigned short)zz;
        }
    }
}

/* awtmz.c triangle wrappers excluded from this lane; unreachable for the
 * RGB_888 ZB configuration and superseded by template kernels later. */
void BR_ASM_CALL TriangleRenderPIZ2TA24(struct brp_block *b, union brp_vertex *a, union brp_vertex *c, union brp_vertex *d) { (void)b;(void)a;(void)c;(void)d; }
void BR_ASM_CALL TriangleRenderPIZ2TA15(struct brp_block *b, union brp_vertex *a, union brp_vertex *c, union brp_vertex *d) { (void)b;(void)a;(void)c;(void)d; }
void BR_ASM_CALL TriangleRenderPIZ2TIA_RGB_555(struct brp_block *b, union brp_vertex *a, union brp_vertex *c, union brp_vertex *d) { (void)b;(void)a;(void)c;(void)d; }
