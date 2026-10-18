/*
 * SPDX-License-Identifier: MIT
 * Portions copyright (c) 1998 Argonaut Software Limited (BRender v1.3.2).
 * Copyright (c) 2026 Zain Dana Harper.
 * See compat/LICENSE-NOTE.md, LICENSE and LICENSES/MIT-BRender.txt.
 */
/*
 * pfpsetup.asm's perspective correct triangle setup (BRender v1.3.2
 * drivers/pentprim, MIT, Argonaut Software 1998), the float path, ported
 * line by line: TriangleSetup_ZPT and TriangleSetup_ZPTI with the
 * SETUP_FLOAT_CHECK_PERSPECTIVE_CHEAT and SETUP_FLOAT_UV_PERSPECTIVE macros.
 *
 * pfpsetup.asm's SETUP_FLOAT and SETUP_FLOAT_PARAM do the same double
 * operations in the same order as fpsetup.asm's, so the port calls
 * fps_setup and fps_param (brender-pentprim-fpsetup-port.c). The two differ
 * in what they leave behind: pfpsetup.asm keeps flip_table's parity in
 * workspace.flip and puts the LR/RL bit in work.tsl.direction, and it keeps
 * the sorted vertex pointers. fps_setup records both.
 *
 * As in fpsetup-port.c: one double operation per x87 operation (53-bit
 * precision, round to nearest, the control word the process has), a single
 * where the assembly stores a dword, no contraction into fused
 * multiply-adds. The perspective values are converted with fp_conv_d,
 * 1.5 * 2^52: the low 32 bits of the sum are the value rounded to an
 * integer.
 */
#define __BR_V1DB__ 1
#include "brender.h"

#include <math.h>
#include <stdint.h>
#include <string.h>
#include "drv.h"
#include "brender-pentprim-fpsetup-port.h"
#include "brender-pentprim-pfpsetup-port.h"

#if defined(_MSC_VER)
#pragma fp_contract(off)
#pragma float_control(precise, on)
#endif

pfps_workspace pfps;

#define FP_CONV_D 6755399441055744.0 /* fp_conv_d: 1.5 * 2^52, units of 1 */

static uint32_t pfps_bits(float f)
{
    uint32_t b;
    memcpy(&b, &f, sizeof(b));
    return b;
}

static float pfps_float(uint32_t b)
{
    float f;
    memcpy(&f, &b, sizeof(f));
    return f;
}

/* fadd fp_conv_d ; fstp real8 ; mov reg,low dword */
static uint32_t pfps_conv(double v)
{
    volatile double sum = v + FP_CONV_D;
    double copy = sum;
    uint64_t bits;
    memcpy(&bits, &copy, sizeof(bits));
    return (uint32_t)bits;
}

/* fistp dword: round to nearest even; out of range stores the integer
 * indefinite, 80000000h */
static int32_t pfps_fistp(double v)
{
    if (!(v >= -2147483648.5 && v < 2147483647.5)) return INT32_MIN;
    return (int32_t)nearbyint(v);
}

/* SETUP_FLOAT_CHECK_PERSPECTIVE_CHEAT: nonzero when w_range * s_range is
 * below 4 * w_min, compared as the bits of two singles (jb). The sorts
 * compare float bits as integers, which relies on positive values. */
static int pfps_cheat(void)
{
    static const int sort_flat[14] = {1, 2, 0, 0, 0, 0, 2, 1, 0, 1, 2, 0, 0, 0};
    union brp_vertex *vv[3], *lo, *hi;
    double yrange, xrange, srange, wrange;
    float xr_yr, wr_sr, wmin_4;
    uint32_t k, c, a, b, d;

    vv[0] = fps.top; vv[1] = fps.mid; vv[2] = fps.bot;
    a = pfps_bits(vv[0]->comp_f[C_SX]); b = pfps_bits(vv[1]->comp_f[C_SX]); d = pfps_bits(vv[2]->comp_f[C_SX]);
    k = ((uint32_t)(b < a) << 2) | ((uint32_t)(d < a) << 1) | (uint32_t)(d < b);
    yrange = (double)vv[2]->comp_f[C_SY] - (double)vv[0]->comp_f[C_SY];
    lo = vv[sort_flat[3 + k]];
    hi = vv[sort_flat[6 + k]];
    xrange = (double)hi->comp_f[C_SX] - (double)lo->comp_f[C_SX];
    a = pfps_bits(vv[0]->comp_f[C_W]); b = pfps_bits(vv[1]->comp_f[C_W]); d = pfps_bits(vv[2]->comp_f[C_W]);
    c = ((uint32_t)(b < a) << 2) | ((uint32_t)(d < a) << 1) | (uint32_t)(d < b);
    xr_yr = (float)(xrange - yrange);
    srange = (pfps_bits(xr_yr) & 0x80000000u) ? yrange : xrange; /* test ebx,80000000h */
    lo = vv[sort_flat[3 + c]];
    hi = vv[sort_flat[6 + c]];
    wrange = (double)hi->comp_f[C_W] - (double)lo->comp_f[C_W];
    wr_sr = (float)(srange * wrange);
    wmin_4 = (float)((double)lo->comp_f[C_W] * 4.0); /* fmul fp_four */
    return pfps_bits(wr_sr) < pfps_bits(wmin_4);
}

/* SETUP_FLOAT_UV_PERSPECTIVE, first block: the gradient multipliers again,
 * from the sorted vertices, negated when the sort was an odd permutation. */
static void pfps_uv_gradients(void)
{
    union brp_vertex *t = fps.top, *m = fps.mid, *b = fps.bot;
    double a = (double)fps.iarea, dx1, dx2, dy1, dy2;
    if (fps.sort_flip != 0) a = -a; /* fchs */
    dx1 = (double)m->comp_f[C_SX] - (double)t->comp_f[C_SX];
    dx2 = (double)b->comp_f[C_SX] - (double)t->comp_f[C_SX];
    dx1 = dx1 * a;
    dy1 = (double)m->comp_f[C_SY] - (double)t->comp_f[C_SY];
    dx2 = dx2 * a;
    dy2 = (double)b->comp_f[C_SY] - (double)t->comp_f[C_SY];
    dy1 = dy1 * a;
    fps.dx1_a = (float)dx1;
    dy2 = dy2 * a;
    fps.dy1_a = (float)dy1;
    fps.dx2_a = (float)dx2;
    fps.dy2_a = (float)dy2;
}

/* u'q', v'q' at the top vertex, their deltas to the other two, and the
 * three q' values, which the assembly stores as singles (q0, q1, q2). */
typedef struct pfps_uvq {
    double uq0, vq0, du1, du2, dv1, dv2;
    float q0, q1, q2;
} pfps_uvq;

/* SETUP_FLOAT_UV_PERSPECTIVE, second block: integral base texel by fistp,
 * u' = u - ub, q' = product of the other two w, u'q', and maxuv, the sum of
 * |q'0| + |u'0| + |v'0| (stored as a single) and the six |u'q'|, |v'q'|. */
static float pfps_uv_deltas(pfps_uvq *d)
{
    union brp_vertex *t = fps.top, *m = fps.mid, *b = fps.bot;
    int32_t ui = pfps_fistp((double)t->comp_f[C_U]), vi = pfps_fistp((double)t->comp_f[C_V]);
    double ub = (double)ui, vb = (double)vi, mx, q, u0, v0, u0q, v0q, u1q, v1q, u2q, v2q;
    float maxuv;

    pfps.u_current = (uint32_t)ui;
    pfps.v_current = (uint32_t)vi;
    q = (double)m->comp_f[C_W] * (double)b->comp_f[C_W]; /* q'0 */
    u0 = (double)t->comp_f[C_U] - ub;
    v0 = (double)t->comp_f[C_V] - vb;
    d->q0 = (float)q;                                    /* fst q0 */
    mx = fabs(q) + fabs(u0);
    mx = mx + fabs(v0);
    u0q = u0 * q;
    v0q = v0 * q;
    maxuv = (float)mx;                                   /* fstp maxuv */
    q = (double)b->comp_f[C_W] * (double)t->comp_f[C_W]; /* q'1 */
    u1q = ((double)m->comp_f[C_U] - ub) * q;
    v1q = ((double)m->comp_f[C_V] - vb) * q;
    d->q1 = (float)q;
    q = (double)t->comp_f[C_W] * (double)m->comp_f[C_W]; /* q'2 */
    u2q = ((double)b->comp_f[C_U] - ub) * q;
    d->q2 = (float)q;
    v2q = ((double)b->comp_f[C_V] - vb) * q;
    mx = (double)maxuv;                                  /* fld maxuv */
    mx = mx + fabs(u0q);
    mx = mx + fabs(v0q);
    mx = mx + fabs(u1q);
    mx = mx + fabs(v1q);
    mx = mx + fabs(u2q);
    mx = mx + fabs(v2q);
    d->dv1 = v1q - v0q;
    d->dv2 = v2q - v0q;
    d->du1 = u1q - u0q;
    d->du2 = u2q - u0q;
    d->uq0 = u0q;
    d->vq0 = v0q;
    return (float)mx;                                    /* fstp maxuv */
}

/* Round maxuv up to a power of two with integer instructions and form
 * 2^28 / maxuv from the bits: neg ; add (127*2+28) shl 23. */
static float pfps_normalise(float maxuv)
{
    uint32_t e = pfps_bits(maxuv);
    if (e & 0x007fffffu) {
        e &= ~0x007fffffu;
        e += 1u << 23;
    }
    e = 0u - e;
    e += (uint32_t)(127 * 2 + 28) << 23;
    return pfps_float(e);
}

/* The u (or v) parameter: pdx, pdy, pdy_0 and the start, scaled by s and
 * converted. d_carry is not formed: the trapezium adds grad_x on a carry. */
static void pfps_uv_param(double d1, double d2, double p0, float s, pfps_param *p)
{
    double b1 = d1 * (double)fps.dy2_a, a2 = d2 * (double)fps.dy1_a, d1d = d1 * (double)fps.dx2_a;
    double pdx = b1 - a2, c2 = d2 * (double)fps.dx1_a, fx = (double)fps.t_dx * pdx;
    double pdy = c2 - d1d, start = p0 + fx, pdy_0;
    start = start + (double)fps.t_dy * pdy;
    pdy_0 = pdx * (double)fps.xstep_0;
    pdy_0 = pdy + pdy_0;
    p->current = pfps_conv(start * (double)s);
    p->grad_x = pfps_conv(pdx * (double)s);
    p->d_carry = pfps_conv(pdy_0 * (double)s);
    p->d_nocarry = p->d_carry;
}

/* The q' parameter, from the single precision q0, q1, q2, with both
 * per-scanline steps. */
static void pfps_q_param(const pfps_uvq *d, float s)
{
    double dq2 = (double)d->q2 - (double)d->q0, dq1 = (double)d->q1 - (double)d->q0;
    double a2 = dq2 * (double)fps.dy1_a, b1 = dq1 * (double)fps.dy2_a;
    double c2 = dq2 * (double)fps.dx1_a, d1 = dq1 * (double)fps.dx2_a;
    double qdx = b1 - a2, qdy = c2 - d1, qdy_0, qdy_1, start;
    qdy_0 = qdx * (double)fps.xstep_0;
    qdy_1 = qdx * (double)fps.xstep_1;
    qdy_0 = qdy_0 + qdy;
    start = (double)fps.t_dx * qdx;
    qdy_1 = qdy_1 + qdy;
    start = start + (double)d->q0;
    start = start + (double)fps.t_dy * qdy;
    pfps.pq.d_nocarry = pfps_conv(qdy_0 * (double)s);
    pfps.pq.d_carry = pfps_conv(qdy_1 * (double)s);
    pfps.pq.grad_x = pfps_conv(qdx * (double)s);
    pfps.pq.current = pfps_conv(start * (double)s);
}

static void pfps_uv_perspective(void)
{
    pfps_uvq d;
    float s;
    pfps_uv_gradients();
    s = pfps_normalise(pfps_uv_deltas(&d));
    pfps_uv_param(d.du1, d.du2, d.uq0, s, &pfps.pu);
    pfps_uv_param(d.dv1, d.dv2, d.vq0, s, &pfps.pv);
    pfps_q_param(&d, s);
}

int pfps_setup(union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2, int lit)
{
    if (fps_setup(v0, v1, v2) != 0) return -1; /* SETUP_FLOAT <ret>, carry clear */
    pfps.direction = (uint32_t)fps.flip;
    fps_param(C_SZ, v0, v1, v2, &fps.s_z, &fps.d_z_y_1, &fps.d_z_x, &fps.d_z_y_0, 1);
    /* lit == 1: SETUP_FLOAT_PARAM C_I (interpolated). lit == 2: TriangleSetup_
     * ZPTI_FLAT's SETUP_FLOAT_CONST C_I, the intensity of the mid vertex with
     * no gradient, so the shade row is fixed for the face. */
    if (lit == 1) fps_param(C_I, v0, v1, v2, &fps.s_i, &fps.d_i_y_1, &fps.d_i_x, &fps.d_i_y_0, 0);
    else if (lit == 2) { fps_const_i(v0); } /* SETUP_FLOAT leaves ebx = v0 */
    if (pfps_cheat()) {
        /* mov esi,work.tsl.direction ; mov workspace.flip,esi: fps.flip
         * already holds the LR/RL bit, which the linear rasterisers read */
        fps_param(C_U, v0, v1, v2, &fps.s_u, &fps.d_u_y_1, &fps.d_u_x, &fps.d_u_y_0, 0);
        fps_param(C_V, v0, v1, v2, &fps.s_v, &fps.d_v_y_1, &fps.d_v_x, &fps.d_v_y_0, 0);
        return 1;
    }
    pfps_uv_perspective();
    return 0;
}
