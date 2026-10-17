/*
 * SPDX-License-Identifier: MIT
 * Portions copyright (c) 1998 Argonaut Software Limited (BRender v1.3.2).
 * Copyright (c) 2026 Zain Dana Harper.
 * See compat/LICENSE-NOTE.md, LICENSE and LICENSES/MIT-BRender.txt.
 */
/*
 * Softrend's triangle subdivision for the FLOAT build, a line-by-line C port
 * of BRender v1.3.2 drivers/softrend/subdiv.asm (MIT, Argonaut Software
 * 1998): subdivideCheck, averageVerticesOnScreen and averageVertices. In the
 * FLOAT build faceops.c has no C versions of these; subdiv.asm is one of the
 * seven assembly overlays the harness does not assemble.
 *
 * Softrend subdivides a triangle (OpTriangleSubdivide, faceops.c) when the
 * matched primitive block carries BR_PRIMF_SUBDIVIDE: pentprim sets it for
 * BR_MATF_SUBDIVIDE materials and for blocks whose range flags include
 * RF_NEED_SUBDIVIDE, such as the perspective rows of the MMX table.
 *
 * The x87 work is the assembly's: each operation rounds as one double
 * operation does (53-bit precision, round to nearest), and each store to a
 * vertex component rounds to single. Mid points are (a + b) * 0.5; the
 * projection takes one reciprocal of the three w's product and recovers each
 * 1/w from it, then sc * P * (1/w) + offset. When a mid point is off screen,
 * each on-screen one is projected on its own, as x * (1/w) * scale + offset.
 *
 * Kept as found: averageVerticesOnScreen leaves the mid points' outcodes
 * unset (softrend does not read them on that path), and averageVertices'
 * user clip plane loop compares a vertex pointer with BRT_PLANE, so it never
 * outcodes a user clip plane.
 */
#define __BR_V1DB__ 1
#include "brender.h"

#include <stdint.h>
#include <string.h>
#include "drv.h"

#if defined(_MSC_VER)
#pragma fp_contract(off)
#pragma float_control(precise, on)
#endif

static uint32_t sd_bits(float f) { uint32_t b; memcpy(&b, &f, 4); return b; }

/* subdiv.asm's sort tables (the same values as setupdat.asm's) */
static const int sd_sort_0[8] = {0, 0, 0, 2, 1, 0, 1, 2}; /* sort_table_0 and on: min |z| */
static const int sd_sort_2[8] = {2, 1, 0, 1, 2, 0, 0, 0}; /* sort_table_2 and on: max |z| */

br_boolean BR_ASM_CALL subdivideCheck(brp_vertex *v0, brp_vertex *v1, brp_vertex *v2)
{
    brp_vertex *v[3];
    uint32_t a = sd_bits(v0->comp_f[C_Z]) & 0x7fffffffu, b = sd_bits(v1->comp_f[C_Z]) & 0x7fffffffu;
    uint32_t c = sd_bits(v2->comp_f[C_Z]) & 0x7fffffffu, k, t, zmin;
    v[0] = v0; v[1] = v1; v[2] = v2;
    k = (uint32_t)(b < a) << 2 | (uint32_t)(c < a) << 1 | (uint32_t)(c < b);
    /* fld threshold ; fmul [max].z ; fstp temp ; and both 7fffffff ; cmp ; jge subdivide */
    t = sd_bits((float)((double)rend.subdivide_threshold * (double)v[sd_sort_2[k]]->comp_f[C_Z])) & 0x7fffffffu;
    zmin = sd_bits(v[sd_sort_0[k]]->comp_f[C_Z]) & 0x7fffffffu;
    return (int32_t)t >= (int32_t)zmin ? BR_TRUE : BR_FALSE;
}

/* COMPUTE_COMPONENT_MID_POINT_VALUES: m0 = (v1 + v2) / 2, m1 = (v0 + v2) / 2,
 * m2 = (v0 + v1) / 2 */
static void sd_mid(int c, brp_vertex *m0, brp_vertex *m1, brp_vertex *m2, brp_vertex *v0, brp_vertex *v1,
                   brp_vertex *v2)
{
    double p0 = v0->comp_f[c], p1 = v1->comp_f[c], p2 = v2->comp_f[c];
    m2->comp_f[c] = (float)((p0 + p1) * 0.5);
    m0->comp_f[c] = (float)((p1 + p2) * 0.5);
    m1->comp_f[c] = (float)((p0 + p2) * 0.5);
}

/* The shared projection: one divide for 1/(w0 w1 w2), then PROJECT_COMPONENT
 * for SX, SY, SZ and the three q's. */
static void sd_project_all(struct br_renderer *r, brp_vertex *m0, brp_vertex *m1, brp_vertex *m2)
{
    double w1w0 = (double)m0->comp_f[C_W] * (double)m1->comp_f[C_W];
    double w1w2 = (double)m1->comp_f[C_W] * (double)m2->comp_f[C_W];
    double w2w1w0 = w1w0 * (double)m2->comp_f[C_W];
    double w2w0 = (double)m0->comp_f[C_W] * (double)m2->comp_f[C_W];
    double inv = 1.0 / w2w1w0, q0 = w1w2 * inv, q1 = w2w0 * inv, q2 = w1w0 * inv;
    static const int comps[3][2] = {{C_X, C_SX}, {C_Y, C_SY}, {C_Z, C_SZ}};
    int k;
    for (k = 0; k < 3; k++) {
        int p = comps[k][0], sp = comps[k][1];
        double s = r->state.cache.comp_scales[sp], o = r->state.cache.comp_offsets[sp];
        m0->comp_f[sp] = (float)(s * (double)m0->comp_f[p] * q0 + o);
        m1->comp_f[sp] = (float)(s * (double)m1->comp_f[p] * q1 + o);
        m2->comp_f[sp] = (float)(s * (double)m2->comp_f[p] * q2 + o);
    }
    m1->comp_f[C_Q] = (float)q1;
    m0->comp_f[C_Q] = (float)q0;
    m2->comp_f[C_Q] = (float)q2;
}

void BR_ASM_CALL averageVerticesOnScreen(struct br_renderer *renderer, brp_vertex *m0, brp_vertex *m1, brp_vertex *m2,
                                         brp_vertex *v0, brp_vertex *v1, brp_vertex *v2)
{
    static const int always[] = {C_X, C_Y, C_Z, C_W}, later[] = {C_I, C_U, C_V}, rgb[] = {C_R, C_G, C_B};
    int k;
    for (k = 0; k < 4; k++) sd_mid(always[k], m0, m1, m2, v0, v1, v2);
    if (rend.block->convert_mask_f & (CM_R | CM_G | CM_B))
        for (k = 0; k < 3; k++) sd_mid(rgb[k], m0, m1, m2, v0, v1, v2);
    for (k = 0; k < 3; k++) sd_mid(later[k], m0, m1, m2, v0, v1, v2);
    sd_project_all(renderer, m0, m1, m2);
}

/* OUTCODE_ORDINATE's 16-entry tables, indexed by (l sign, w sign,
 * |l| < |w|, |w| < |l|) from the float bits; g is the >= plane, l the < -w one */
static uint32_t sd_outcode_toggle(float lv, float wv, uint32_t g, uint32_t l)
{
    const uint32_t G = g, L = l, GL = g | l;
    const uint32_t table[16] = {G, G, 0, 0, G, G, GL, 0, 0, L, 0, 0, GL, L, GL, 0};
    uint32_t w = sd_bits(wv), x = sd_bits(lv);
    uint32_t aw = w & 0x7fffffffu, ax = x & 0x7fffffffu;
    uint32_t idx = (x >> 31) << 3 | (w >> 31) << 2 | ((ax - aw) >> 31) << 1 | ((aw - ax) >> 31);
    return table[idx];
}

static void sd_outcode(brp_vertex *m)
{
    uint32_t edx = OUTCODES_NOT;
    edx ^= sd_outcode_toggle(m->comp_f[C_X], m->comp_f[C_W], OUTCODE_RIGHT | OUTCODE_N_RIGHT, OUTCODE_LEFT | OUTCODE_N_LEFT);
    edx ^= sd_outcode_toggle(m->comp_f[C_Y], m->comp_f[C_W], OUTCODE_TOP | OUTCODE_N_TOP, OUTCODE_BOTTOM | OUTCODE_N_BOTTOM);
    edx ^= sd_outcode_toggle(m->comp_f[C_Z], m->comp_f[C_W], OUTCODE_HITHER | OUTCODE_N_HITHER, OUTCODE_YON | OUTCODE_N_YON);
    m->flags = edx;
}

/* TEST_AND_PROJECT_VERTEX */
static void sd_project_one(struct br_renderer *r, brp_vertex *m)
{
    double q, x, y, z;
    if (m->flags & OUTCODES_ALL) return;
    q = 1.0 / (double)m->comp_f[C_W];
    m->comp_f[C_Q] = (float)q;
    x = (double)m->comp_f[C_X] * q;
    y = (double)m->comp_f[C_Y] * q;
    z = q * (double)m->comp_f[C_Z];
    m->comp_f[C_SX] = (float)(x * (double)r->state.cache.comp_scales[C_SX] + (double)r->state.cache.comp_offsets[C_SX]);
    m->comp_f[C_SY] = (float)(y * (double)r->state.cache.comp_scales[C_SY] + (double)r->state.cache.comp_offsets[C_SY]);
    m->comp_f[C_SZ] = (float)(z * (double)r->state.cache.comp_scales[C_SZ] + (double)r->state.cache.comp_offsets[C_SZ]);
}

void BR_ASM_CALL averageVertices(struct br_renderer *renderer, brp_vertex *m0, brp_vertex *m1, brp_vertex *m2,
                                 brp_vertex *v0, brp_vertex *v1, brp_vertex *v2)
{
    static const int comps[] = {C_X, C_Y, C_Z, C_W, C_R, C_G, C_B, C_I, C_U, C_V};
    int k;
    for (k = 0; k < 10; k++) sd_mid(comps[k], m0, m1, m2, v0, v1, v2);
    sd_outcode(m0);
    sd_outcode(m1);
    sd_outcode(m2);
    if (!(m0->flags & OUTCODES_ALL) && !(m1->flags & OUTCODES_ALL) && !(m2->flags & OUTCODES_ALL)) {
        sd_project_all(renderer, m0, m1, m2);
        return;
    }
    sd_project_one(renderer, m0); /* selectiveProjection */
    sd_project_one(renderer, m1);
    sd_project_one(renderer, m2);
}
