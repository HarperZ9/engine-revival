/*
 * SPDX-License-Identifier: MIT
 * Portions copyright (c) 1998 Argonaut Software Limited (BRender v1.3.2).
 * Copyright (c) 2026 Zain Dana Harper.
 * See compat/LICENSE-NOTE.md, LICENSE and LICENSES/MIT-BRender.txt.
 */
/*
 * The texture side the three textured MMX kernels share, ported from
 * BRender v1.3.2 drivers/pentprim (MIT, Argonaut Software 1998):
 * unpack.inc's UNPACK_UV_32 and the PACK_U and PACK_V macros of xzuv1.inc
 * (U and V as packed 20.12 values with carry bridge bits), and the fetch of a
 * word's four texels through the palette, the transparency mask (index 0)
 * and the masked write the colour passes end with.
 */
#define __BR_V1DB__ 1
#include "brender.h"

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "drv.h"
#include "brender-pentprim-mmx-port.h"

void mmx_quad_fetch(mmx_quad *q, const uint8_t *tex, const uint8_t *pal, const uint32_t off[4])
{
    const mmx_bounds *b = mmx_block_bounds;
    mmx_t p[4];
    int k;
    q->idx = 0;
    for (k = 0; k < 4; k++) {
        uint8_t i = mmx_read1(b->tex, b->tbytes, tex + off[k]); /* mov dl,[esi+ebx] */
        q->idx |= (uint32_t)i << (8 * k);
        p[k] = mmx_read4(b->pal, b->pbytes, pal + 4u * i);     /* movd mm,[edi+edx*4] */
    }
    q->lo = mmx_punpcklbw(p[0], p[1]);
    q->hi = mmx_punpcklbw(p[2], p[3]);
}

void mmx_quad_channels(const mmx_quad *q, mmx_t *b, mmx_t *g, mmx_t *r)
{
    mmx_t mm0 = mmx_punpcklwd(q->lo, q->hi), mm2 = mmx_punpckhwd(q->lo, q->hi), mm1 = mm0;
    *r = mmx_punpcklbw(mm2, mm2);
    *b = mmx_punpcklbw(mm0, mm0);
    *g = mmx_punpckhbw(mm1, mm1);
}

mmx_t mmx_quad_transparent(const mmx_quad *q)
{
    mmx_t m = mmx_pcmpeqb((mmx_t)q->idx, 0);
    return mmx_punpcklbw(m, m);
}

void mmx_quad_store(uint8_t *at, mmx_t px, mmx_t transparent, mmx_t depth_mask)
{
    const mmx_bounds *b = mmx_block_bounds;
    mmx_t m = mmx_pandn(transparent, depth_mask), old = mmx_read8(b->cbase, b->cbytes, at);
    mmx_write8(b->cbase, b->cbytes, at, mmx_por(mmx_pand(px, m), mmx_pandn(m, old)));
}

void mmx_uvpack_init(mmx_uvpack *k, const struct texture_info *t)
{
    uint32_t eax = (uint8_t)t->tile_s, ebx = (uint8_t)t->width_s, ecx = (uint8_t)t->height_s + ebx;
    mmx_t mm3, mm4;
    ebx += eax;
    /* a power-of-two map is at most 1024 on a side, so the index stays in uv_masks */
    k->tile = mmx_uv_masks[eax % 21u];
    mm3 = mmx_uv_masks[ebx % 21u];
    mm4 = mmx_uv_masks[ecx % 21u];
    k->v_high = mmx_pandn(mm3, mm4);
    k->u_int = mmx_pandn(k->tile, mm3);
    k->tile_s = eax;
    k->v_shift = ebx - eax;
}

/* PACK_U: fraction bits down one, integer part shifted up by the tile shift */
mmx_t mmx_pack_u(const mmx_uvpack *k, mmx_t mm0, int bridge)
{
    mmx_t mm1 = mmx_pand(mmx_pslld(mm0, k->tile_s), k->u_int);
    mm0 = mmx_por(mmx_pand(mmx_psrad(mm0, 1), mmx_fraction_mask), mm1);
    if (bridge) mm0 = mmx_por(mmx_por(mm0, mmx_fraction_bit), k->tile);
    return mm0;
}

/* PACK_V: as U, with the high integer bits shifted by the width and the
 * low (tile) bits left where they are */
mmx_t mmx_pack_v(const mmx_uvpack *k, mmx_t mm0, int bridge)
{
    mmx_t mm1 = mmx_pand(mmx_pslld(mm0, k->v_shift), k->v_high), mm2 = mmx_pand(mm0, k->tile);
    mm0 = mmx_por(mmx_por(mmx_pand(mmx_psrad(mm0, 1), mmx_fraction_mask), mm1), mm2);
    if (bridge) mm0 = mmx_por(mmx_por(mm0, mmx_fraction_bit), k->u_int);
    return mm0;
}

void mmx_uv_flip(uint32_t *s_u, uint32_t *d_u_x, uint32_t *s_v, uint32_t *d_v_x)
{
    *s_u += 3u * *d_u_x;
    *s_v += 3u * *d_v_x;
    *d_u_x = 0u - *d_u_x;
    *d_v_x = 0u - *d_v_x;
}

void mmx_unpack_uv_32(const struct texture_info *t, uint32_t uv[8], uint32_t masks[2])
{
    mmx_uvpack k;
    mmx_t u, du, v, dv, m;
    mmx_uvpack_init(&k, t);
    u = mmx_pack_u(&k, mmx_dd(uv[0], uv[1]), 1);
    du = mmx_pack_u(&k, mmx_dd(uv[2], uv[3]), 1);
    v = mmx_pack_v(&k, mmx_dd(uv[4], uv[5]), 1);
    dv = mmx_pack_v(&k, mmx_dd(uv[6], uv[7]), 1);
    m = mmx_por(mmx_punpckldq(k.u_int, mmx_por(k.v_high, k.tile)), mmx_fraction_mask);
    masks[0] = mmx_d(m, 0);
    masks[1] = mmx_d(m, 1);
    uv[0] = mmx_d(u, 0) & masks[0]; /* mask start values */
    uv[1] = mmx_d(u, 1);
    uv[2] = mmx_d(du, 0);
    uv[3] = mmx_d(du, 1);
    uv[4] = mmx_d(v, 0) & masks[1];
    uv[5] = mmx_d(v, 1);
    uv[6] = mmx_d(dv, 0);
    uv[7] = mmx_d(dv, 1);
}
