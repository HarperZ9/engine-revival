/*
 * SPDX-License-Identifier: MIT
 * Portions copyright (c) 1998 Argonaut Software Limited (BRender v1.3.2).
 * Copyright (c) 2026 Zain Dana Harper.
 *
 * The perspective correct triangle setup of BRender v1.3.2
 * drivers/pentprim/pfpsetup.asm, ported to C, and the entry points the
 * perspective kernels share with the linear 8-bit kernels. See
 * brender-pentprim-pfpsetup-port.c and brender-pentprim-i8p-port.c.
 */
#ifndef BRENDER_PENTPRIM_PFPSETUP_PORT_H
#define BRENDER_PENTPRIM_PFPSETUP_PORT_H

#include <stdint.h>

/* work.h's struct scan_parameter, the fields the perspective kernels use:
 * plain 32-bit integers (u'q', v'q' and q' scaled by the normalisation
 * factor), not 16.16 fixed point. */
typedef struct pfps_param {
    uint32_t current, d_carry, d_nocarry, grad_x;
} pfps_param;

/* work.pq, work.pu, work.pv, the integer base texel work.awsl.u_current and
 * v_current, and work.tsl.direction (the LR/RL bit, 1 scans right to left). */
typedef struct pfps_workspace {
    pfps_param pq, pu, pv;
    uint32_t u_current, v_current, direction;
} pfps_workspace;

extern pfps_workspace pfps;

/* TriangleSetup_ZPT (lit = 0) and TriangleSetup_ZPTI (lit = 1): SETUP_FLOAT,
 * SETUP_FLOAT_PARAM for z (and i), SETUP_FLOAT_CHECK_PERSPECTIVE_CHEAT, then
 * SETUP_FLOAT_UV_PERSPECTIVE or, in the cheat case, SETUP_FLOAT_PARAM for u
 * and v. Returns -1 for an empty triangle, 1 for the cheat case (the
 * assembly returns with carry set and the caller jumps into the linear
 * rasteriser) and 0 for the perspective case. */
int pfps_setup(union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2, int lit);

/* The linear rasterisers the cheat case jumps into, after their setup:
 * fti8_piz.asm TriangleRasterise_ZT_I8_D16_256 and zb8.asm
 * TriangleRasterise_ZTI_I8_D16_256, in brender-pentprim-i8-port.c. */
void i8_rasterise_zt_256(void);
void i8_rasterise_zti_256(void);

/* The same for the other power of two maps, 1 << pow2 wide: zb8p2unl.asm
 * TriangleRasterise_ZT_I8_D16_<n> and zb8p2lit.asm
 * TriangleRasterise_ZTI_I8_D16_<n>, in brender-pentprim-i8pow2-port.c. */
void i8_rasterise_zt_pow2(uint32_t pow2);
void i8_rasterise_zti_pow2(uint32_t pow2);

#endif
