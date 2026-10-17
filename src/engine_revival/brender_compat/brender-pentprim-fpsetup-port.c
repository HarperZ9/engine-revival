/*
 * SPDX-License-Identifier: MIT
 * Portions copyright (c) 1998 Argonaut Software Limited (BRender v1.3.2).
 * Copyright (c) 2026 Zain Dana Harper.
 * See compat/LICENSE-NOTE.md, LICENSE and LICENSES/MIT-BRender.txt.
 */
/*
 * fpsetup.asm's floating point triangle setup (BRender v1.3.2
 * drivers/pentprim, MIT, Argonaut Software 1998), ported line by line:
 * the SETUP_FLOAT, SETUP_FLOAT_PARAM and SETUP_FLOAT_COLOUR macros.
 *
 * The assembly runs on the x87 with the control word the process has; the
 * setup does not load its own (only ARBITRARY_SETUP does). On Windows that
 * is 53-bit precision, round to nearest, so each x87 operation rounds as one
 * IEEE double operation does, and each store to a dword rounds to single.
 * The port does the same operations, in the same order, in doubles, one
 * rounding per operation (no contraction into fused multiply-adds).
 *
 * The fixed point conversions are the assembly's: add 1.5 * 2^36 and keep
 * the low 32 bits of the double, which is the value rounded to 1/65536.
 */
#define __BR_V1DB__ 1
#include "brender.h"

#include <stdint.h>
#include <string.h>
#include "drv.h"
#include "brender-pentprim-fpsetup-port.h"

#if defined(_MSC_VER)
#pragma fp_contract(off)
#pragma float_control(precise, on)
#endif

fps_workspace fps;
static union brp_vertex *fps_top; /* eax through SETUP_FLOAT_PARAM: the top vertex */

#define FP_CONV_D16 103079215104.0 /* fp_conv_d16: 1.5 * 2^36, units of 1/65536 */

static uint32_t fps_low32(double d)
{
    volatile double v = d; /* stored, so the sum is rounded once and in memory */
    uint64_t bits;
    double copy = v;
    memcpy(&bits, &copy, sizeof(bits));
    return (uint32_t)bits;
}

static uint32_t fps_fix16(double v) { return fps_low32(v + FP_CONV_D16); }

static uint32_t fps_bits(float f)
{
    uint32_t b;
    memcpy(&b, &f, sizeof(b));
    return b;
}

/* The integer y of a vertex, from the float's bits with integer
 * instructions: (mantissa | 1 << 23) >> ((150 << 23 | 0x7fffff) - bits) >> 23,
 * the shift count taken mod 32 as shr does. Right for 0 < y < 2^24, as the
 * source says; the port keeps what it does outside that range too. */
static int32_t fps_y_int(float y)
{
    uint32_t bits = fps_bits(y);
    uint32_t shift = ((((uint32_t)(127 + 23) << 23) | 0x7fffffu) - bits) >> 23;
    return (int32_t)(((bits & 0x7fffffu) | 0x800000u) >> (shift & 31u));
}

void fps_colour(union brp_vertex *v0)
{
    fps.colour = (uint8_t)(fps_fix16((double)v0->comp_f[C_I]) >> 16); /* byte ptr temporary_intensity+2 */
}

int fps_setup(union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    /* sort_table_1, _0 and _2 overlap in fpsetup.asm's data: one array */
    static const int sort_flat[14] = {1, 2, 0, 0, 0, 0, 2, 1, 0, 1, 2, 0, 0, 0};
    static const uint32_t flip_table[8] = {0, 0x80000000u, 0x80000000u, 0, 0x80000000u, 0, 0, 0x80000000u};
    union brp_vertex *vv[3], *top, *mid, *bot;
    double dx2, dx1, dy2, dy1, area2, ra, a, b, c, cb, dX1, dy123, dX2, R;
    double XYY, g1, dX3, aR, cdX2, bdX3, mdy, gm, tdy, g2, x_1, x_2, x_m;
    int32_t y_t, y_m, y_b, m_y, t_dx;
    uint32_t k, flip, d_xm, d_x1, d_x2, flipbit;

    vv[0] = v0; vv[1] = v1; vv[2] = v2;
    dx2 = (double)v2->comp_f[C_SX] - (double)v0->comp_f[C_SX];
    dx1 = (double)v1->comp_f[C_SX] - (double)v0->comp_f[C_SX];
    dy2 = (double)v2->comp_f[C_SY] - (double)v0->comp_f[C_SY];
    dy1 = (double)v1->comp_f[C_SY] - (double)v0->comp_f[C_SY];
    area2 = dx1 * dy2;
    area2 = area2 - dx2 * dy1;
    ra = 1.0 / area2;

    /* cmp ecx,eax ; rcl ; cmp edx,eax ; rcl ; cmp edx,ecx ; rcl on the float bits */
    {
        uint32_t y0 = fps_bits(v0->comp_f[C_SY]), y1 = fps_bits(v1->comp_f[C_SY]), y2 = fps_bits(v2->comp_f[C_SY]);
        k = ((uint32_t)(y1 < y0) << 2) | ((uint32_t)(y2 < y0) << 1) | (uint32_t)(y2 < y1);
    }
    top = vv[sort_flat[3 + k]];
    bot = vv[sort_flat[6 + k]];
    mid = vv[sort_flat[k]];
    flip = flip_table[k];
    fps.top = top; fps.mid = mid; fps.bot = bot;
    fps.sort_flip = flip;

    y_t = fps_y_int(top->comp_f[C_SY]);
    y_m = fps_y_int(mid->comp_f[C_SY]);
    y_b = fps_y_int(bot->comp_f[C_SY]);

    if (y_t == y_m) { /* top_zero */
        if (y_t == y_b) {
            fps.topCount = fps.bottomCount = -1;
            return 1;
        }
        dy1 = dy1 * ra;
        a = 1.0;
        dy2 = dy2 * ra;
        b = (double)bot->comp_f[C_SY] - (double)mid->comp_f[C_SY];
    } else if (y_m == y_b) { /* bottom_zero */
        dy1 = dy1 * ra;
        a = (double)mid->comp_f[C_SY] - (double)top->comp_f[C_SY];
        dy2 = dy2 * ra;
        b = 1.0;
    } else {
        dy1 = dy1 * ra;
        a = (double)mid->comp_f[C_SY] - (double)top->comp_f[C_SY];
        dy2 = dy2 * ra;
        b = (double)bot->comp_f[C_SY] - (double)mid->comp_f[C_SY];
    }
    /* count_cont */
    dx1 = dx1 * ra;
    c = (double)bot->comp_f[C_SY] - (double)top->comp_f[C_SY];
    dx2 = dx2 * ra;
    fps.iarea = (float)ra;
    fps.dx1_a = (float)dx1;
    fps.dy2_a = (float)dy2;
    fps.dx2_a = (float)dx2;
    fps.dy1_a = (float)dy1;

    /* edge gradients: R = 1/(dy1.dy2.dy3) */
    cb = c * b;
    dX1 = (double)mid->comp_f[C_SX] - (double)top->comp_f[C_SX];
    dy123 = cb * a;
    dX2 = (double)bot->comp_f[C_SX] - (double)mid->comp_f[C_SX];
    R = 1.0 / dy123;

    /* counts */
    fps.t_y = y_t + 1;
    fps.topCount = y_m - fps.t_y;
    m_y = y_m + 1;
    fps.bottomCount = y_b - m_y;
    flipbit = (flip ^ fps_bits(fps.iarea)) >> 31;
    fps.flip = (int32_t)flipbit;

    XYY = cb * dX1;
    g1 = XYY * R;
    dX3 = (double)bot->comp_f[C_SX] - (double)top->comp_f[C_SX];
    aR = a * R;
    cdX2 = c * dX2;
    bdX3 = b * dX3;
    mdy = (double)m_y - (double)mid->comp_f[C_SY];
    gm = bdX3 * aR;
    tdy = (double)fps.t_y - (double)top->comp_f[C_SY];
    g2 = cdX2 * aR;

    /* x intercepts with the top and middle scanlines */
    fps.t_dy = (float)tdy;
    x_2 = mdy * g2;
    x_1 = tdy * g1;
    x_2 = x_2 + (double)mid->comp_f[C_SX];
    x_m = tdy * gm;
    x_1 = x_1 + (double)top->comp_f[C_SX];
    d_x1 = fps_fix16(g1);
    x_m = x_m + (double)top->comp_f[C_SX];
    d_xm = fps_fix16(gm);
    /* fconv_d16_12[esi*8]: conv + 0.0 (LR) or conv + 1.0 (RL);
     * fconv_d16_m[esi*8]: conv + 1.0 (LR) or conv + 0.0 (RL) */
    fps.x2 = fps_low32(x_2 + (FP_CONV_D16 + (double)flipbit));
    d_x2 = fps_fix16(g2);
    fps.x1 = fps_low32(x_1 + (FP_CONV_D16 + (double)flipbit));
    fps.xm = fps_low32(x_m + (FP_CONV_D16 + (double)(1u - flipbit)));
    fps.d_xm = d_xm;
    fps.d_x1 = d_x1;
    fps.d_x2 = d_x2;

    t_dx = (int32_t)fps.xm >> 16;
    fps.t_dx = (float)((double)t_dx - (double)top->comp_f[C_SX]);
    fps.xstep_0 = (float)((int32_t)d_xm >> 16);
    fps.xstep_1 = (float)(((int32_t)d_xm >> 16) + 1);
    fps_top = top;
    return 0;
}

void fps_param(int comp, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2,
               uint32_t *s, uint32_t *d_y_1, uint32_t *d_x, uint32_t *d_y_0, int is_unsigned)
{
    double dp2 = (double)v2->comp_f[comp] - (double)v0->comp_f[comp];
    double dp1 = (double)v1->comp_f[comp] - (double)v0->comp_f[comp];
    double dp2a = dp2 * (double)fps.dy1_a;
    double dp1b = dp1 * (double)fps.dy2_a;
    double dp2c = dp2 * (double)fps.dx1_a;
    double dp1d = dp1 * (double)fps.dx2_a;
    double pdx = dp1b - dp2a;
    double pdy = dp2c - dp1d;
    double pdy_0 = pdx * (double)fps.xstep_0;
    double pdy_1 = pdx * (double)fps.xstep_1;
    double start;
    pdy_0 = pdy_0 + pdy;
    start = (double)fps.t_dx * pdx;
    pdy_1 = pdy_1 + pdy;
    start = start + (double)fps_top->comp_f[comp];
    start = start + (double)fps.t_dy * pdy;
    *d_y_0 = fps_fix16(pdy_0);
    *d_y_1 = fps_fix16(pdy_1);
    *d_x = fps_fix16(pdx);
    *s = fps_fix16(start);
    if (is_unsigned) *s ^= 0x80000000u; /* remap -1..1 signed to 0..1 unsigned */
}
