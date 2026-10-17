/*
 * SPDX-License-Identifier: MIT
 * Portions copyright (c) 1998 Argonaut Software Limited (BRender v1.3.2).
 * Copyright (c) 2026 Zain Dana Harper.
 * See compat/LICENSE-NOTE.md, LICENSE and LICENSES/MIT-BRender.txt.
 */
/*
 * The MMX Z-buffered textured rasterisers modulated by an interpolated
 * colour, RasteriseMMX555_Z_UV_RGB and RasteriseMMX565_Z_UV_RGB with their
 * _D, _S and _SD forms, left to right and right to left: a line-by-line C
 * port of BRender v1.3.2 drivers/pentprim/xzuvrgb.inc as xzuvrgb.asm and
 * xzuvrgb5.asm instance it (MIT, Argonaut Software 1998), with unpack.inc's
 * UNPACK_UV_32 and UNPACK_PARAM_RGB.
 *
 * The palette colours are halved to 7 bits before they are unpacked, then
 * multiplied with pmulhw by the interpolated colour (four 8.8 lanes a
 * channel, stepped a word at a time, from the table's 7-bit colour range).
 * The products are packed from bits 9 to 13 (mask_5d, mask_6d).
 */
#define __BR_V1DB__ 1
#include "brender.h"

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "drv.h"
#include "brender-pentprim-mmx-port.h"

#define WT struct work_mmx_zrgbuv
#define PT struct param_zrgbuv
#define WO(f) offsetof(WT, f)
#define PO(f) (offsetof(WT, h) + offsetof(PT, f))

static mmx_t ldq(const uint8_t *w, size_t o) { mmx_t v; memcpy(&v, w + o, 8); return v; }
static void stq(uint8_t *w, size_t o, mmx_t v) { memcpy(w + o, &v, 8); }
static uint32_t ldd(const uint8_t *w, size_t o) { uint32_t v; memcpy(&v, w + o, 4); return v; }
static void std_(uint8_t *w, size_t o, uint32_t v) { memcpy(w + o, &v, 4); }
static void *ldp(const uint8_t *w, size_t o) { void *v; memcpy(&v, w + o, sizeof(v)); return v; }

#define MASK_7F 0x7f7f7f7f7f7f7f7full /* pcmpeqw ; psrlw 9 ; packuswb */

typedef struct zuvrgb_k {
    int rl, c15, dither, sd;
    uint8_t *w;
    struct tsb_header *h;
} zuvrgb_k;

static void zuvrgb_unpack(const zuvrgb_k *k)
{
    uint8_t *w = k->w;
    struct param_zrgbuv *p = (struct param_zrgbuv *)k->h;
    static const size_t from[3] = {PO(s_r), PO(s_g), PO(s_b)};
    static const size_t to[3][4] = {
        {WO(r01), WO(d_r_y1), WO(d_r_y0), WO(d_r_x)},
        {WO(g01), WO(d_g_y1), WO(d_g_y0), WO(d_g_x)},
        {WO(b01), WO(d_b_y1), WO(d_b_y0), WO(d_b_x)},
    };
    uint32_t uv[8], masks[2], z[16], in[4], o[8];
    int c;
    if (k->rl) mmx_uv_flip(&p->s_u, &p->d_u_x, &p->s_v, &p->d_v_x);
    memcpy(uv, w + PO(s_u), 16);
    memcpy(uv + 4, w + PO(s_v), 16);
    mmx_unpack_uv_32(&p->tinfo, uv, masks);
    memcpy(w + WO(u), uv, 16);
    memcpy(w + WO(v), uv + 4, 16);
    memcpy(w + WO(u_mask), masks, 8);
    for (c = 0; c < 3; c++) { /* UNPACK_PARAM_RGB */
        memcpy(in, w + from[c], 16);
        mmx_unpack_param_8(in, o, o + 2, o + 4, o + 6);
        memcpy(w + to[c][0], o, 8);
        memcpy(w + to[c][1], o + 2, 8);
        memcpy(w + to[c][2], o + 4, 8);
        memcpy(w + to[c][3], o + 6, 8);
    }
    if (k->sd) k->h->c = (k->h->c >> 28) << 2;
    memcpy(in, w + PO(s_z), 16);
    mmx_unpack_param_16(in, z, z + 4, z + 6, z + 8);
    memcpy(w + WO(z0), z, 16);
    memcpy(w + WO(d_z_y1), z + 4, 8);
    memcpy(w + WO(d_z_y0), z + 6, 8);
    memcpy(w + WO(d_z_x), z + 8, 8);
}

/* four texels, a pixel at a time (texel i is pixel i, or 3 - i right to
 * left), each palette byte halved (psrld 1 ; pand 7f7f7f7f) */
static void zuvrgb_quad(const zuvrgb_k *k, uint32_t *ecx, uint32_t *ebp, mmx_quad *q)
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
    q->lo = mmx_pand(mmx_psrld(q->lo, 1), MASK_7F);
    q->hi = mmx_pand(mmx_psrld(q->hi, 1), MASK_7F);
}

/* modulate by this word's colour lanes and pack */
static mmx_t zuvrgb_pixels(const zuvrgb_k *k, const mmx_quad *q, mmx_t cr, mmx_t cg, mmx_t cb)
{
    mmx_t b, g, r;
    mmx_quad_channels(q, &b, &g, &r);
    r = mmx_pmulhw(r, cr);
    b = mmx_pmulhw(b, cb);
    g = mmx_pmulhw(g, cg);
    if (k->dither) {
        int32_t row = k->h->start_scanline;
        r = mmx_paddw(r, mmx_dither_table_9[row & 3]);
        b = mmx_paddw(b, mmx_dither_table_9[row & 3]);
        g = mmx_paddw(g, mmx_dither_table_8[row & 3]);
    }
    r = mmx_psllw(mmx_pand(r, mmx_mask_5d), k->c15 ? 1 : 2);
    b = mmx_psrlw(mmx_pand(b, mmx_mask_5d), 9);
    g = mmx_psrlw(mmx_pand(g, k->c15 ? mmx_mask_5d : mmx_mask_6d), k->c15 ? 4 : 3);
    return mmx_por(mmx_por(b, r), g);
}

static mmx_t zuvrgb_step(const zuvrgb_k *k, mmx_t v, mmx_t d) { return k->rl ? mmx_psubw(v, d) : mmx_paddw(v, d); }

static void zuvrgb_span(const zuvrgb_k *k, const mmx_walk *wk)
{
    struct tsb_header *h = k->h;
    uint8_t *w = k->w;
    uint32_t ecx = ldd(w, WO(u)), ebp = ldd(w, WO(v));
    mmx_t mm7 = ldq(w, WO(r01)), mm6 = ldq(w, WO(g01)), mm5 = ldq(w, WO(b01));
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
            mmx_t px;
            zuvrgb_quad(k, &ecx, &ebp, &q);
            px = zuvrgb_pixels(k, &q, mm7, mm6, mm5);
            mm7 = zuvrgb_step(k, mm7, ldq(w, WO(d_r_x)));
            mm6 = zuvrgb_step(k, mm6, ldq(w, WO(d_g_x)));
            mm5 = zuvrgb_step(k, mm5, ldq(w, WO(d_b_x)));
            mmx_quad_store(edi + eax, px, mmx_quad_transparent(&q), mmx_mask_read(s.mask_base + eax));
            if (eax == 0) break;
            eax += step;
        }
    } else {
        mmx_t mask;
        if (mmx_zpass_one(&s, ldq(w, WO(z0)), ldq(w, WO(z2)), &mask) == 0) return;
        zuvrgb_quad(k, &ecx, &ebp, &q);
        mmx_quad_store((uint8_t *)h->screen_address + 2 * s.ebx, zuvrgb_pixels(k, &q, mm7, mm6, mm5),
                       mmx_quad_transparent(&q), mask);
    }
}

static void zuvrgb_increment(const zuvrgb_k *k, int carry)
{
    uint8_t *w = k->w;
    mmx_t dz = ldq(w, carry ? WO(d_z_y1) : WO(d_z_y0));
    stq(w, WO(z0), mmx_paddd(ldq(w, WO(z0)), dz));
    stq(w, WO(z2), mmx_paddd(ldq(w, WO(z2)), dz));
    std_(w, WO(u), (ldd(w, WO(u)) + ldd(w, carry ? WO(d_u_y1) : WO(d_u_y0))) & ldd(w, WO(u_mask)));
    std_(w, WO(v), (ldd(w, WO(v)) + ldd(w, carry ? WO(d_v_y1) : WO(d_v_y0))) & ldd(w, WO(v_mask)));
    stq(w, WO(r01), mmx_paddw(ldq(w, WO(r01)), ldq(w, carry ? WO(d_r_y1) : WO(d_r_y0))));
    stq(w, WO(g01), mmx_paddw(ldq(w, WO(g01)), ldq(w, carry ? WO(d_g_y1) : WO(d_g_y0))));
    stq(w, WO(b01), mmx_paddw(ldq(w, WO(b01)), ldq(w, carry ? WO(d_b_y1) : WO(d_b_y0))));
}

static void zuvrgb(int rl, int c15, int dither, int sd)
{
    zuvrgb_k k;
    mmx_walk wk;
    k.rl = rl;
    k.c15 = c15;
    k.dither = dither;
    k.sd = sd;
    k.w = mmx_block - offsetof(WT, h);
    k.h = (struct tsb_header *)mmx_block;
    zuvrgb_unpack(&k);
    mmx_walk_addresses(k.h);
    if (sd || dither) k.h->start_scanline &= 3;
    if (!mmx_walk_first(k.h, &wk)) return;
    for (;;) {
        if (rl ? wk.x0 >= wk.x1 : wk.x0 <= wk.x1) zuvrgb_span(&k, &wk);
        if (!mmx_walk_next(k.h, &wk)) return;
        zuvrgb_increment(&k, wk.carry);
        if (dither || sd) k.h->start_scanline = (k.h->start_scanline + 1) & 3;
        mmx_walk_step_addresses(k.h);
    }
}

#define ZUVRGB_PAIR(name, c15, d, s) \
    void BR_ASM_CALL name##_LR(void) { zuvrgb(0, c15, d, s); } \
    void BR_ASM_CALL name##_RL(void) { zuvrgb(1, c15, d, s); }

ZUVRGB_PAIR(RasteriseMMX565_Z_UV_RGB, 0, 0, 0)
ZUVRGB_PAIR(RasteriseMMX565_Z_UV_RGB_D, 0, 1, 0)
ZUVRGB_PAIR(RasteriseMMX565_Z_UV_RGB_S, 0, 0, 1)
ZUVRGB_PAIR(RasteriseMMX565_Z_UV_RGB_SD, 0, 1, 1)
ZUVRGB_PAIR(RasteriseMMX555_Z_UV_RGB, 1, 0, 0)
ZUVRGB_PAIR(RasteriseMMX555_Z_UV_RGB_D, 1, 1, 0)
ZUVRGB_PAIR(RasteriseMMX555_Z_UV_RGB_S, 1, 0, 1)
ZUVRGB_PAIR(RasteriseMMX555_Z_UV_RGB_SD, 1, 1, 1)
