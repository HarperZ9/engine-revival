/*
 * SPDX-License-Identifier: MIT
 * Portions copyright (c) 1998 Argonaut Software Limited (BRender v1.3.2).
 * Copyright (c) 2026 Zain Dana Harper.
 *
 * The workspace of BRender v1.3.2 drivers/pentprim/fpwork.inc and the
 * floating point triangle setup of fpsetup.asm, ported to C. See
 * brender-pentprim-fpsetup-port.c.
 */
#ifndef BRENDER_PENTPRIM_FPSETUP_PORT_H
#define BRENDER_PENTPRIM_FPSETUP_PORT_H

#include <stdint.h>

/* fpwork.inc's _workspace, the fields the ported kernels use. Fixed point
 * values are the 32-bit words the assembly stores; the *_a, xstep, t_dx and
 * t_dy fields are the single precision floats it stores. */
typedef struct fps_workspace {
    uint32_t xm, d_xm, x1, d_x1, x2, d_x2;
    uint32_t s_z, d_z_y_1, d_z_x, d_z_y_0;
    uint32_t s_i, d_i_y_1, d_i_x, d_i_y_0;
    uint32_t s_u, d_u_y_1, d_u_x, d_u_y_0;
    uint32_t s_v, d_v_y_1, d_v_x, d_v_y_0;
    float iarea, dx1_a, dx2_a, dy1_a, dy2_a, xstep_0, xstep_1, t_dx, t_dy;
    int32_t t_y, flip, topCount, bottomCount;
    uint32_t xm_f, d_xm_f;
    uint8_t colour;
} fps_workspace;

extern fps_workspace fps;

/* SETUP_FLOAT: sort, edges, counts and the per-parameter multipliers.
 * Returns nonzero for an empty triangle (both counts -1). */
int fps_setup(union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2);

/* SETUP_FLOAT_PARAM comp, conv = fp_conv_d16, unsigned: start, the two
 * per-scanline steps and the x step, as 16.16 fixed point. */
void fps_param(int comp, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2,
               uint32_t *s, uint32_t *d_y_1, uint32_t *d_x, uint32_t *d_y_0, int is_unsigned);

/* SETUP_FLOAT_COLOUR: the flat colour index from the first vertex's C_I. */
void fps_colour(union brp_vertex *v0);

#endif
