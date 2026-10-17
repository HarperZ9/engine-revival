/*
 * SPDX-License-Identifier: MIT
 * Portions copyright (c) 1998 Argonaut Software Limited (BRender v1.3.2).
 * Copyright (c) 2026 Zain Dana Harper.
 * See compat/LICENSE-NOTE.md, LICENSE and LICENSES/MIT-BRender.txt.
 */
/*
 * The MMX Z-buffered textured rasterisers modulated by a constant colour,
 * RasteriseMMX555_Z_UV_C and RasteriseMMX565_Z_UV_C with their _D, _S and
 * _SD forms, left to right and right to left: a line-by-line C port of
 * BRender v1.3.2 drivers/pentprim/xzuvc.inc as xzuvc.asm and xzuvc555.asm
 * instance it (BLEND 0), with unpack.inc's UNPACK_UV_32 and
 * UNPACK_CONSTANT_COLOUR (MIT, Argonaut Software 1998).
 *
 * U and V are single packed 20.12 accumulators stepped a pixel at a time;
 * each texel's palette colour is halved to 7 bits, multiplied by the 7-bit
 * constant colour with pmulhw and shifted back up by 2. The dither here is
 * paddw (it wraps), where the unmodulated kernel uses paddusw.
 */
#define __BR_V1DB__ 1
#include "brender.h"

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "drv.h"
#include "brender-pentprim-mmx-port.h"

#define WT struct work_mmx_zuvc
#define PT struct param_zuv
#define WO(f) offsetof(WT, f)
#define PO(f) (offsetof(WT, h) + offsetof(PT, f))

static mmx_t ldq(const uint8_t *w, size_t o) { mmx_t v; memcpy(&v, w + o, 8); return v; }
static void stq(uint8_t *w, size_t o, mmx_t v) { memcpy(w + o, &v, 8); }
static uint32_t ldd(const uint8_t *w, size_t o) { uint32_t v; memcpy(&v, w + o, 4); return v; }
static void std_(uint8_t *w, size_t o, uint32_t v) { memcpy(w + o, &v, 4); }
static void *ldp(const uint8_t *w, size_t o) { void *v; memcpy(&v, w + o, sizeof(v)); return v; }

typedef struct zuvc_k {
    int rl, c15, dither, sd;
    uint8_t *w;
    struct tsb_header *h;
} zuvc_k;

static void zuvc_unpack(const zuvc_k *k)
{
    uint8_t *w = k->w;
    struct param_zuv *p = (struct param_zuv *)k->h;
    uint32_t uv[8], masks[2], z[16], in[4];
    mmx_t mm0, mm1, mm2;
    if (k->rl) mmx_uv_flip(&p->s_u, &p->d_u_x, &p->s_v, &p->d_v_x);
    memcpy(uv, w + PO(s_u), 16);
    memcpy(uv + 4, w + PO(s_v), 16);
    mmx_unpack_uv_32(&p->tinfo, uv, masks);
    memcpy(w + WO(u), uv, 16);
    memcpy(w + WO(v), uv + 4, 16);
    memcpy(w + WO(u_mask), masks, 8);
    /* UNPACK_CONSTANT_COLOUR SCREENDOOR */
    mm0 = mmx_dd(k->h->c, 0);
    mm0 = mmx_psrlw(mmx_punpcklbw(mm0, mm0), 1); /* reduce to 7 bits (the multiply is signed) */
    mm2 = mm0;
    mm0 = mmx_punpcklwd(mm0, mm0);
    mm1 = mm0;
    mm2 = mmx_punpckhwd(mm2, mm2);
    stq(w, WO(cb01), mmx_punpckldq(mm0, mm0));
    stq(w, WO(cg01), mmx_punpckhdq(mm1, mm1));
    stq(w, WO(cr01), mmx_punpckldq(mm2, mm2));
    if (k->sd) k->h->c = (k->h->c >> 26) & 0x3cu;
    memcpy(in, w + PO(s_z), 16);
    mmx_unpack_param_16(in, z, z + 4, z + 6, z + 8);
    memcpy(w + WO(z0), z, 16);
    memcpy(w + WO(d_z_y1), z + 4, 8);
    memcpy(w + WO(d_z_y0), z + 6, 8);
    memcpy(w + WO(d_z_x), z + 8, 8);
}

/* four texels from the U and V accumulators, a pixel at a time: texel i is
 * pixel i left to right, pixel 3 - i right to left */
static void zuvc_quad(const zuvc_k *k, uint32_t *ecx, uint32_t *ebp, mmx_quad *q)
{
    uint8_t *w = k->w;
    uint32_t off[4], du = ldd(w, WO(d_u_x)), dv = ldd(w, WO(d_v_x)), um = ldd(w, WO(u_mask)), vm = ldd(w, WO(v_mask));
    int i;
    for (i = 0; i < 4; i++) {
        off[k->rl ? 3 - i : i] = (*ebp + *ecx) >> (MMX_FRACTION_BITS + 1);
        *ecx = (*ecx + du) & um;
        *ebp = (*ebp + dv) & vm;
    }
    mmx_quad_fetch(q, (const uint8_t *)ldp(w, WO(texture_address)), (const uint8_t *)ldp(w, WO(map_address)), off);
}

static mmx_t zuvc_pixels(const zuvc_k *k, const mmx_quad *q)
{
    uint8_t *w = k->w;
    mmx_t b, g, r;
    mmx_quad_channels(q, &b, &g, &r);
    b = mmx_psllw(mmx_pmulhw(mmx_psrlw(b, 1), ldq(w, WO(cb01))), 2);
    g = mmx_psllw(mmx_pmulhw(mmx_psrlw(g, 1), ldq(w, WO(cg01))), 2);
    r = mmx_psllw(mmx_pmulhw(mmx_psrlw(r, 1), ldq(w, WO(cr01))), 2);
    if (k->dither) {
        int32_t row = k->h->start_scanline & 3;
        b = mmx_paddw(b, mmx_dither_table_11[row]);
        g = mmx_paddw(g, mmx_dither_table_10[row]);
        r = mmx_paddw(r, mmx_dither_table_11[row]);
    }
    r = mmx_pand(r, mmx_mask_5);
    g = mmx_pand(g, k->c15 ? mmx_mask_5 : mmx_mask_6);
    if (k->c15) r = mmx_psrlw(r, 1);
    b = mmx_psrlw(b, 11);
    return mmx_por(mmx_por(b, r), mmx_psrlw(g, k->c15 ? 6 : 5));
}

static void zuvc_span(const zuvc_k *k, const mmx_walk *wk)
{
    struct tsb_header *h = k->h;
    uint8_t *w = k->w;
    uint32_t ecx = ldd(w, WO(u)), ebp = ldd(w, WO(v));
    mmx_quad q;
    mmx_span s;
    s.rl = k->rl;
    s.screendoor = k->sd;
    s.sd_mask = k->sd ? mmx_screendoor_masks[h->start_scanline | (int32_t)h->c] : 0;
    s.depth_row = (uint8_t *)h->depth_address;
    s.x0 = wk->x0;
    s.x1 = wk->x1;
    if (mmx_span_split(&s)) {
        const int32_t step = s.rl ? -8 : 8;
        uint8_t *edi = (uint8_t *)h->screen_address + 2 * s.ebx;
        int32_t eax = s.eax;
        if (mmx_zpass_words(&s, ldq(w, WO(z0)), ldq(w, WO(z2)), ldq(w, WO(d_z_x))) == 0) return;
        for (;;) {
            zuvc_quad(k, &ecx, &ebp, &q);
            mmx_quad_store(edi + eax, zuvc_pixels(k, &q), mmx_quad_transparent(&q), mmx_mask_read(s.mask_base + eax));
            if (eax == 0) break;
            eax += step;
        }
    } else {
        mmx_t mask;
        if (mmx_zpass_one(&s, ldq(w, WO(z0)), ldq(w, WO(z2)), &mask) == 0) return;
        zuvc_quad(k, &ecx, &ebp, &q);
        mmx_quad_store((uint8_t *)h->screen_address + 2 * s.ebx, zuvc_pixels(k, &q), mmx_quad_transparent(&q), mask);
    }
}

static void zuvc_increment(const zuvc_k *k, int carry)
{
    uint8_t *w = k->w;
    mmx_t dz = ldq(w, carry ? WO(d_z_y1) : WO(d_z_y0));
    stq(w, WO(z0), mmx_paddd(ldq(w, WO(z0)), dz));
    stq(w, WO(z2), mmx_paddd(ldq(w, WO(z2)), dz));
    std_(w, WO(u), (ldd(w, WO(u)) + ldd(w, carry ? WO(d_u_y1) : WO(d_u_y0))) & ldd(w, WO(u_mask)));
    std_(w, WO(v), (ldd(w, WO(v)) + ldd(w, carry ? WO(d_v_y1) : WO(d_v_y0))) & ldd(w, WO(v_mask)));
}

static void zuvc(int rl, int c15, int dither, int sd)
{
    zuvc_k k;
    mmx_walk wk;
    k.rl = rl;
    k.c15 = c15;
    k.dither = dither;
    k.sd = sd;
    k.w = mmx_block - offsetof(WT, h);
    k.h = (struct tsb_header *)mmx_block;
    zuvc_unpack(&k);
    mmx_walk_addresses(k.h);
    if (sd || dither) k.h->start_scanline &= 3;
    if (!mmx_walk_first(k.h, &wk)) return;
    for (;;) {
        if (rl ? wk.x0 >= wk.x1 : wk.x0 <= wk.x1) zuvc_span(&k, &wk);
        if (!mmx_walk_next(k.h, &wk)) return;
        zuvc_increment(&k, wk.carry);
        if (dither || sd) k.h->start_scanline = (k.h->start_scanline + 1) & 3;
        mmx_walk_step_addresses(k.h);
    }
}

#define ZUVC_PAIR(name, c15, d, s) \
    void BR_ASM_CALL name##_LR(void) { zuvc(0, c15, d, s); } \
    void BR_ASM_CALL name##_RL(void) { zuvc(1, c15, d, s); }

ZUVC_PAIR(RasteriseMMX565_Z_UV_C, 0, 0, 0)
ZUVC_PAIR(RasteriseMMX565_Z_UV_C_D, 0, 1, 0)
ZUVC_PAIR(RasteriseMMX565_Z_UV_C_S, 0, 0, 1)
ZUVC_PAIR(RasteriseMMX565_Z_UV_C_SD, 0, 1, 1)
ZUVC_PAIR(RasteriseMMX555_Z_UV_C, 1, 0, 0)
ZUVC_PAIR(RasteriseMMX555_Z_UV_C_D, 1, 1, 0)
ZUVC_PAIR(RasteriseMMX555_Z_UV_C_S, 1, 0, 1)
ZUVC_PAIR(RasteriseMMX555_Z_UV_C_SD, 1, 1, 1)
