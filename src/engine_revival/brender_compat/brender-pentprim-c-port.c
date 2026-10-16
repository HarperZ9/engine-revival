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

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
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

/* SafeFixedMac2Div(a,b,c,d,e) = (a*b + c*d) / e, from safediv.asm: a 64-bit
 * sum, an unsigned divide of the magnitudes, the sign restored afterwards,
 * and 0 when e is 0 or the quotient would not fit in 32 bits. */
br_int_32 SafeFixedMac2Div(br_int_32 a, br_int_32 b, br_int_32 c, br_int_32 d, br_int_32 e)
{
    long long num = (long long)a * b + (long long)c * d;
    unsigned long long m = num < 0 ? 0ull - (unsigned long long)num : (unsigned long long)num;
    unsigned int sign = (unsigned int)e ^ (unsigned int)((unsigned long long)num >> 32);
    unsigned int divisor = e < 0 ? 0u - (unsigned int)e : (unsigned int)e;
    unsigned int q;
    if (divisor == 0 || (unsigned int)(m >> 32) >= divisor) return 0;
    q = (unsigned int)(m / divisor);
    return (br_int_32)((sign & 0x80000000u) ? 0u - q : q);
}

void RasteriseBufferDisable(void)
{
}

/* ---- diagnostic stubs: print name when called ---- */

void BR_ASM_CALL TrapezoidRenderPIZ2TA(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2, br_uint_16 (*fp_vertices)[3]) { (void)block; (void)v0; (void)v1; (void)v2; (void)fp_vertices; }
void BR_ASM_CALL TrapezoidRenderPIZ2TIA(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2, br_uint_16 (*fp_vertices)[3]) { (void)block; (void)v0; (void)v1; (void)v2; (void)fp_vertices; }
void BR_ASM_CALL TrapezoidRenderPIZ2TAN(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2, br_uint_16 (*fp_vertices)[3]) { (void)block; (void)v0; (void)v1; (void)v2; (void)fp_vertices; }
void BR_ASM_CALL TrapezoidRenderPIZ2TA15(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2, br_uint_16 (*fp_vertices)[3]) { (void)block; (void)v0; (void)v1; (void)v2; (void)fp_vertices; }
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
/*
 * TrapezoidRenderPIZ2TA24 (textured, RGB_888 texture into RGB_888) and
 * TrapezoidRenderPIZ2TIA_RGB_888 (8-bit texture lit through the shade table
 * into RGB_888): a line-by-line C port of the TriangleRenderZ2 macro in
 * BRender v1.3.2 drivers/pentprim/t_piza.asm (MIT, Argonaut 1998), built
 * with WRAP, WRAPROW and TRANSP as that file sets them. Argonaut's own C
 * setup in awtmz.c / awtmi.h fills work.awsl and calls these once per half
 * of the triangle.
 *
 * Registers are modelled as 32-bit unsigned values so adds wrap and carries
 * come out where the assembly takes them: eax holds the u fraction in its
 * high half and the biased integer u in its low half, ebx the v fraction.
 *
 * Additions, none of which changes a pixel the assembly would draw inside
 * its buffers: a texel outside the texture is not read (the pixel is
 * skipped), and a pixel or depth value outside its buffer is not written.
 */
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

static void awsl_trapezoid(int light, int sbpp, int dbpp)
{
    const size_t colour_bytes = (size_t)work.colour.stride_b * work.colour.height;
    const size_t depth_bytes = (size_t)work.depth.stride_b * work.depth.height;
    const size_t texture_bytes = (size_t)work.texture.size;

    for (;;) {
        struct scan_edge *edge = work.awsl.edge;
        char *edi, *esi, *ebp;
        uint32_t eax, ebx, carry;

        if (edge->count == 0) return;
        edge->count--;

        edi = work.awsl.start;
        eax = ((uint32_t)work.awsl.u_current & 0xffff0000u) | (awsl_u_int() & 0xffffu);
        ebx = (uint32_t)work.awsl.v_current;
        work.pz.currentpix = work.pz.current;
        if (light) work.pi.currentpix = work.pi.current;
        esi = work.awsl.source_current;
        ebp = work.awsl.zstart;

        if (edi < work.awsl.end) {
            /* forward scan */
            while (edi < work.awsl.end) {
                int pass = awsl_in(esi, work.texture.base, texture_bytes, (size_t)sbpp);
                if (pass) { /* TRANSP: a zero texel is transparent */
                    if (sbpp == 1) pass = esi[0] != 0;
                    else pass = (esi[0] | esi[1] | esi[2]) != 0;
                }
                if (pass && awsl_in(ebp, work.depth.base, depth_bytes, 2)) {
                    uint16_t z16 = (uint16_t)((uint32_t)work.pz.currentpix >> 16), zbuf;
                    memcpy(&zbuf, ebp, 2);
                    pass = zbuf >= z16; /* cmp dl,bl ; sbb dh,bh ; jb failf */
                } else {
                    pass = 0;
                }
                if (pass && awsl_in(edi, work.colour.base, colour_bytes, (size_t)dbpp)) {
                    uint16_t z16 = (uint16_t)((uint32_t)work.pz.currentpix >> 16);
                    memcpy(ebp, &z16, 2);
                    if (light) {
                        uint32_t index = (((uint32_t)work.pi.currentpix >> 16) & 0xffu) << 8 | (uint8_t)esi[0];
                        const uint8_t *lit = work.shade_table + index * 4;
                        edi[0] = (char)lit[0];
                        edi[1] = (char)lit[1];
                        edi[2] = (char)lit[2];
                    } else {
                        edi[0] = esi[0];
                        edi[1] = esi[1];
                        edi[2] = esi[2];
                    }
                }
                /* both paths: zptr += 2, ptr += DBPP, z += dz, i += di */
                ebp += 2;
                edi += dbpp;
                work.pz.currentpix = (br_fixed_ls)((uint32_t)work.pz.currentpix + (uint32_t)work.pz.grad_x);
                if (light) work.pi.currentpix = (br_fixed_ls)((uint32_t)work.pi.currentpix + (uint32_t)work.pi.grad_x);

                /* contf: step u, the source pointer and v */
                carry = (uint32_t)(((uint64_t)eax + (uint32_t)work.awsl.du) >> 32);
                eax += (uint32_t)work.awsl.du;
                esi += work.awsl.dsource;
                eax += carry;
                esi += (ptrdiff_t)carry * sbpp;
                eax += (uint32_t)work.awsl.du_int;
                carry = (uint32_t)(((uint64_t)ebx + (uint32_t)work.awsl.dv) >> 32);
                ebx += (uint32_t)work.awsl.dv;
                if (carry) esi += work.texture.stride_b;
                if (!(eax & 0x8000u)) { /* test ah,ah ; jl nowidthf */
                    eax += work.texture.width_p;
                    esi += (ptrdiff_t)work.texture.width_p * sbpp;
                }
                if (esi < (char *)work.texture.base) esi += work.texture.size;
            }
        } else {
            /* backward scan */
            while (edi > work.awsl.end) {
                int pass;
                work.pz.currentpix = (br_fixed_ls)((uint32_t)work.pz.currentpix - (uint32_t)work.pz.grad_x);
                if (light) work.pi.currentpix = (br_fixed_ls)((uint32_t)work.pi.currentpix - (uint32_t)work.pi.grad_x);

                carry = (uint32_t)(((uint64_t)eax + (uint32_t)work.awsl.du) >> 32);
                eax += (uint32_t)work.awsl.du;
                esi += work.awsl.dsource;
                eax += carry;
                eax += (uint32_t)work.awsl.du_int;
                esi += (ptrdiff_t)carry * sbpp;
                carry = (uint32_t)(((uint64_t)ebx + (uint32_t)work.awsl.dv) >> 32);
                ebx += (uint32_t)work.awsl.dv;
                if (carry) esi += work.texture.stride_b;
                if (!(eax & 0x8000u)) {
                    eax += work.texture.width_p;
                    esi += (ptrdiff_t)work.texture.width_p * sbpp;
                }
                if (esi < (char *)work.texture.base) esi += work.texture.size;
                ebp -= 2;
                edi -= dbpp;

                pass = awsl_in(esi, work.texture.base, texture_bytes, (size_t)sbpp);
                if (pass) {
                    if (sbpp == 1) pass = esi[0] != 0;
                    else pass = (esi[0] | esi[1] | esi[2]) != 0;
                }
                if (pass && awsl_in(ebp, work.depth.base, depth_bytes, 2)) {
                    uint16_t z16 = (uint16_t)((uint32_t)work.pz.currentpix >> 16), zbuf;
                    memcpy(&zbuf, ebp, 2);
                    pass = zbuf >= z16; /* sub bl,dl ; sbb bh,dh ; jb loopb */
                } else {
                    pass = 0;
                }
                if (!pass || !awsl_in(edi, work.colour.base, colour_bytes, (size_t)dbpp)) continue;
                {
                    uint16_t z16 = (uint16_t)((uint32_t)work.pz.currentpix >> 16);
                    memcpy(ebp, &z16, 2);
                }
                if (light) {
                    uint32_t index = (((uint32_t)work.pi.currentpix >> 16) & 0xffu) << 8 | (uint8_t)esi[0];
                    const uint8_t *lit = work.shade_table + index * 4;
                    edi[0] = (char)lit[0];
                    edi[1] = (char)lit[1];
                    edi[2] = (char)lit[2];
                } else {
                    edi[0] = esi[0];
                    edi[1] = esi[1];
                    edi[2] = esi[2];
                }
            }
        }

        /* next: per scan line updates */
        {
            uint32_t f = (uint32_t)edge->f, d;
            carry = (f + (uint32_t)edge->d_f) < f;
            edge->f = (br_int_32)(f + (uint32_t)edge->d_f);
            work.awsl.end += ((ptrdiff_t)edge->d_i + carry) * dbpp;

            f = (uint32_t)work.main.f;
            carry = (f + (uint32_t)work.main.d_f) < f;
            work.main.f = (br_int_32)(f + (uint32_t)work.main.d_f);
            d = (uint32_t)work.main.d_i + carry;
            work.awsl.zstart += (ptrdiff_t)(br_int_32)d * 2;
            work.awsl.start += (ptrdiff_t)(br_int_32)d * dbpp;

            /* scan_inc carry|nocarry */
            if (light)
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
                work.awsl.source_current += (ptrdiff_t)ucarry * sbpp;
                vcarry = (work.awsl.v_current + dv) < work.awsl.v_current;
                work.awsl.v_current += dv;
                if (vcarry) work.awsl.source_current += work.texture.stride_b;
            }
            /* cont: wrap u and the source pointer */
            if (!(awsl_u_int() & 0x8000u)) {
                awsl_set_u_int(awsl_u_int() + work.texture.width_p);
                work.awsl.source_current += (ptrdiff_t)work.texture.width_p * sbpp;
            }
            if (work.awsl.source_current < (char *)work.texture.base)
                work.awsl.source_current += work.texture.size;
        }
    }
}

void BR_ASM_CALL TrapezoidRenderPIZ2TA24(void) { awsl_trapezoid(0, 3, 3); }
void BR_ASM_CALL TrapezoidRenderPIZ2TIA_RGB_888(void) { awsl_trapezoid(1, 1, 3); }
