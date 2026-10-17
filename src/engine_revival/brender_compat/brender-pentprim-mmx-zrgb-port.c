/*
 * SPDX-License-Identifier: MIT
 * Portions copyright (c) 1998 Argonaut Software Limited (BRender v1.3.2).
 * Copyright (c) 2026 Zain Dana Harper.
 * See compat/LICENSE-NOTE.md, LICENSE and LICENSES/MIT-BRender.txt.
 */
/*
 * The MMX Z-buffered RGB rasterisers, RasteriseMMX555_Z_RGB and
 * RasteriseMMX565_Z_RGB with their _D (dithered), _S (screendoor) and _SD
 * forms, left to right and right to left: a line-by-line C port of
 * BRender v1.3.2 drivers/pentprim/xzrgb.inc as xzrgb.asm and xzrgb555.asm
 * instance it (MIT, Argonaut Software 1998), with unpack.inc's
 * UNPACK_PARAM_RGB.
 *
 * The rasteriser draws 4-pixel words. Each colour channel is four 16-bit
 * lanes (8.8, one per pixel of the word), stepped a word at a time; the
 * depth test is the shared Z pass (brender-pentprim-mmx-common-port.c). Flat
 * triangles use the same kernel with zero steps (the table's "duplicate"
 * rows). Pixels outside the span or failing the depth test keep their
 * colour: each word is read, merged under the mask and written back.
 */
#define __BR_V1DB__ 1
#include "brender.h"

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "drv.h"
#include "brender-pentprim-mmx-port.h"

#define WT struct work_mmx_zrgb
#define PT struct param_zrgb
#define WO(f) offsetof(WT, f)
#define PO(f) (offsetof(WT, h) + offsetof(PT, f))

static mmx_t ldq(const uint8_t *w, size_t o) { mmx_t v; memcpy(&v, w + o, 8); return v; }
static void stq(uint8_t *w, size_t o, mmx_t v) { memcpy(w + o, &v, 8); }

typedef struct zrgb_k {
    int rl, c15, dither, sd;
    uint8_t *w;               /* the work structure: the block less PARAM_OFFSET */
    struct tsb_header *h;
    mmx_t mask_g;             /* mask_5 or mask_6 for green */
    int g_shift;              /* 6 or 5 */
} zrgb_k;

/* reds, greens and blues of a word to pixels (565, or 555 with the reds
 * shifted down one) */
static mmx_t zrgb_pack(const zrgb_k *k, mmx_t r, mmx_t g, mmx_t b)
{
    r = mmx_pand(r, mmx_mask_5);
    g = mmx_psrlw(mmx_pand(g, k->mask_g), (uint64_t)k->g_shift);
    if (k->c15) r = mmx_psrlw(r, 1);
    return mmx_por(mmx_por(r, g), mmx_psrlw(b, 11));
}

static mmx_t zrgb_step(const zrgb_k *k, mmx_t v, mmx_t d) { return k->rl ? mmx_psubw(v, d) : mmx_paddw(v, d); }

/* the colour pass for two or more words */
static void zrgb_colour_words(const zrgb_k *k, const mmx_span *s, uint8_t *edi)
{
    const mmx_bounds *b = mmx_block_bounds;
    const int32_t step = s->rl ? -8 : 8;
    int32_t ebx = s->eax, ebp = s->mask_base;
    mmx_t mm0 = ldq(k->w, WO(r01)), mm1 = ldq(k->w, WO(g01)), mm2 = ldq(k->w, WO(b01));
    mmx_t mm3, mm4, mm5, mm6;
    if (k->dither) {
        int32_t row = k->h->start_scanline & 3;
        mm0 = mmx_paddw(mm0, mmx_dither_table_11[row]);
        mm1 = mmx_paddw(mm1, mmx_dither_table_10[row]);
        mm2 = mmx_paddw(mm2, mmx_dither_table_11[row]);
    }
    mm4 = mm1;
    mm5 = mm2;
    mm1 = zrgb_step(k, mm1, ldq(k->w, WO(d_g_x)));
    mm3 = mm0;
    mm0 = zrgb_step(k, mm0, ldq(k->w, WO(d_r_x)));
    mm3 = zrgb_pack(k, mm3, mm4, mm5);
    mm6 = mmx_mask_read(ebp + ebx);
    mm4 = mmx_read8(b->cbase, b->cbytes, edi + ebx);
    mm6 = mmx_por(mmx_pandn(mm6, mm4), mmx_pand(mm3, mm6));
    ebx += step;
    while (ebx != 0) {
        mm2 = zrgb_step(k, mm2, ldq(k->w, WO(d_b_x)));
        mm3 = mm0;
        mmx_write8(b->cbase, b->cbytes, edi + ebx - step, mm6);
        mm4 = mm1;
        mm0 = zrgb_step(k, mm0, ldq(k->w, WO(d_r_x)));
        mm5 = mm2;
        mm1 = zrgb_step(k, mm1, ldq(k->w, WO(d_g_x)));
        mm3 = zrgb_pack(k, mm3, mm4, mm5);
        mm6 = mmx_mask_read(ebp + ebx);
        mm4 = mmx_read8(b->cbase, b->cbytes, edi + ebx);
        mm6 = mmx_por(mmx_pandn(mm6, mm4), mmx_pand(mm3, mm6));
        ebx += step;
    }
    /* c_loop_tail */
    mm2 = zrgb_step(k, mm2, ldq(k->w, WO(d_b_x)));
    mmx_write8(b->cbase, b->cbytes, edi - step, mm6);
    mm0 = zrgb_pack(k, mm0, mm1, mm2);
    mm6 = mmx_mask_read(ebp);
    mm4 = mmx_read8(b->cbase, b->cbytes, edi);
    mmx_write8(b->cbase, b->cbytes, edi, mmx_por(mmx_pandn(mm6, mm4), mmx_pand(mm0, mm6)));
}

/* one word: the colours are this word's lanes, unstepped */
static void zrgb_colour_one(const zrgb_k *k, uint8_t *at, mmx_t mask, mmx_t r, mmx_t g, mmx_t bl)
{
    const mmx_bounds *b = mmx_block_bounds;
    mmx_t px = zrgb_pack(k, r, g, bl), old = mmx_read8(b->cbase, b->cbytes, at);
    mmx_write8(b->cbase, b->cbytes, at, mmx_por(mmx_pand(px, mask), mmx_pandn(mask, old)));
}

/* one span: start: to the end of the colour pass */
static void zrgb_span(const zrgb_k *k, const mmx_walk *wk)
{
    struct tsb_header *h = k->h;
    mmx_span s;
    mmx_t z01 = ldq(k->w, WO(z0)), z23 = ldq(k->w, WO(z2));
    uint8_t *edi = (uint8_t *)h->screen_address;
    s.rl = k->rl;
    s.screendoor = k->sd;
    s.sd_mask = 0;
    s.depth_row = (uint8_t *)h->depth_address;
    s.x0 = wk->x0;
    s.x1 = wk->x1;
    if (k->sd) s.sd_mask = mmx_screendoor_masks[(h->start_scanline & 3) | (int32_t)h->c];
    if (mmx_span_split(&s)) {
        if (mmx_zpass_words(&s, z01, z23, ldq(k->w, WO(d_z_x))) == 0) return;
        zrgb_colour_words(k, &s, edi + 2 * s.ebx);
    } else {
        mmx_t mask, r = ldq(k->w, WO(r01)), g = ldq(k->w, WO(g01)), bl = ldq(k->w, WO(b01));
        if (k->dither) {
            int32_t row = h->start_scanline & 3;
            r = mmx_paddw(r, mmx_dither_table_11[row]);
            g = mmx_paddw(g, mmx_dither_table_10[row]);
            bl = mmx_paddw(bl, mmx_dither_table_11[row]);
        }
        if (mmx_zpass_one(&s, z01, z23, &mask) == 0) return;
        zrgb_colour_one(k, edi + 2 * s.ebx, mask, r, g, bl);
    }
}

/* sl_bottom_cont: the parameter steps, d_*_y1 after a carry */
static void zrgb_increment(const zrgb_k *k, int carry)
{
    uint8_t *w = k->w;
    size_t zy = carry ? WO(d_z_y1) : WO(d_z_y0);
    stq(w, WO(z0), mmx_paddd(ldq(w, WO(z0)), ldq(w, zy)));
    stq(w, WO(z2), mmx_paddd(ldq(w, WO(z2)), ldq(w, zy)));
    stq(w, WO(r01), mmx_paddw(ldq(w, WO(r01)), ldq(w, carry ? WO(d_r_y1) : WO(d_r_y0))));
    stq(w, WO(g01), mmx_paddw(ldq(w, WO(g01)), ldq(w, carry ? WO(d_g_y1) : WO(d_g_y0))));
    stq(w, WO(b01), mmx_paddw(ldq(w, WO(b01)), ldq(w, carry ? WO(d_b_y1) : WO(d_b_y0))));
}

static void zrgb_unpack(zrgb_k *k)
{
    uint8_t *w = k->w;
    uint32_t p[4], o[8];
    static const size_t from[3] = {PO(s_r), PO(s_g), PO(s_b)};
    static const size_t to[3][4] = {
        {WO(r01), WO(d_r_y1), WO(d_r_y0), WO(d_r_x)},
        {WO(g01), WO(d_g_y1), WO(d_g_y0), WO(d_g_x)},
        {WO(b01), WO(d_b_y1), WO(d_b_y0), WO(d_b_x)},
    };
    int c;
    for (c = 0; c < 3; c++) { /* UNPACK_PARAM_RGB */
        memcpy(p, w + from[c], 16);
        mmx_unpack_param_8(p, o, o + 2, o + 4, o + 6);
        memcpy(w + to[c][0], o, 8);
        memcpy(w + to[c][1], o + 2, 8);
        memcpy(w + to[c][2], o + 4, 8);
        memcpy(w + to[c][3], o + 6, 8);
    }
    if (k->sd) k->h->c = (k->h->c >> 28) << 2; /* UNPACK_SCREENDOOR_ALPHA */
    {
        uint32_t z[16];
        memcpy(p, w + PO(s_z), 16);
        mmx_unpack_param_16(p, z, z + 4, z + 6, z + 8);
        memcpy(w + WO(z0), z, 16);
        memcpy(w + WO(d_z_y1), z + 4, 8);
        memcpy(w + WO(d_z_y0), z + 6, 8);
        memcpy(w + WO(d_z_x), z + 8, 8);
    }
}

static void zrgb(int rl, int c15, int dither, int sd)
{
    zrgb_k k;
    mmx_walk wk;
    k.rl = rl;
    k.c15 = c15;
    k.dither = dither;
    k.sd = sd;
    k.w = mmx_block - offsetof(WT, h);
    k.h = (struct tsb_header *)mmx_block;
    k.mask_g = c15 ? mmx_mask_5 : mmx_mask_6;
    k.g_shift = c15 ? 6 : 5;
    zrgb_unpack(&k);
    mmx_walk_addresses(k.h);
    if (!mmx_walk_first(k.h, &wk)) return;
    for (;;) {
        /* start: J_EMPTY skips a line with no pixels */
        if (rl ? wk.x0 >= wk.x1 : wk.x0 <= wk.x1) zrgb_span(&k, &wk);
        if (!mmx_walk_next(k.h, &wk)) return;
        zrgb_increment(&k, wk.carry);
        if (dither || sd) k.h->start_scanline++;
        mmx_walk_step_addresses(k.h);
    }
}

#define ZRGB_PAIR(name, c15, d, s) \
    void BR_ASM_CALL name##_LR(void) { zrgb(0, c15, d, s); } \
    void BR_ASM_CALL name##_RL(void) { zrgb(1, c15, d, s); }

ZRGB_PAIR(RasteriseMMX565_Z_RGB, 0, 0, 0)
ZRGB_PAIR(RasteriseMMX565_Z_RGB_D, 0, 1, 0)
ZRGB_PAIR(RasteriseMMX565_Z_RGB_S, 0, 0, 1)
ZRGB_PAIR(RasteriseMMX565_Z_RGB_SD, 0, 1, 1)
ZRGB_PAIR(RasteriseMMX555_Z_RGB, 1, 0, 0)
ZRGB_PAIR(RasteriseMMX555_Z_RGB_D, 1, 1, 0)
ZRGB_PAIR(RasteriseMMX555_Z_RGB_S, 1, 0, 1)
ZRGB_PAIR(RasteriseMMX555_Z_RGB_SD, 1, 1, 1)
