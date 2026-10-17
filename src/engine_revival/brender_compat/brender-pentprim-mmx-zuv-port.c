/*
 * SPDX-License-Identifier: MIT
 * Portions copyright (c) 1998 Argonaut Software Limited (BRender v1.3.2).
 * Copyright (c) 2026 Zain Dana Harper.
 * See compat/LICENSE-NOTE.md, LICENSE and LICENSES/MIT-BRender.txt.
 */
/*
 * The MMX Z-buffered textured rasterisers, RasteriseMMX555_Z_UV and
 * RasteriseMMX565_Z_UV with their _D, _S and _SD forms, left to right and
 * right to left: a line-by-line C port of BRender v1.3.2
 * drivers/pentprim/xzuv1.inc as xzuv.asm and xzuv555.asm instance it (MIT,
 * Argonaut Software 1998), with DITHER_BILINEAR 0 as the file sets it.
 *
 * The map is 8-bit with a palette, a power of two on each side. U and V are
 * packed 20.12 values (11 fraction bits, a carry bridge bit, then the
 * integer part shifted into place), two pixels to a register: the texel
 * offset of a pixel is (u + v) >> 12. Index 0 is transparent.
 *
 * As in the assembly, the dithered 16-bit kernel does not mask green in the
 * span of two or more words (xzuv1.inc tests COLOUR_TYPE_15 twice where the
 * second test should be COLOUR_TYPE_16), so green's low bits reach blue
 * there; the one-word span masks it.
 */
#define __BR_V1DB__ 1
#include "brender.h"

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "drv.h"
#include "brender-pentprim-mmx-port.h"

#define WT struct work_mmx_zuv4c
#define PT struct param_zuv
#define WO(f) offsetof(WT, f)
#define PO(f) (offsetof(WT, h) + offsetof(PT, f))

static mmx_t ldq(const uint8_t *w, size_t o) { mmx_t v; memcpy(&v, w + o, 8); return v; }
static void stq(uint8_t *w, size_t o, mmx_t v) { memcpy(w + o, &v, 8); }
static uint32_t ldd(const uint8_t *w, size_t o) { uint32_t v; memcpy(&v, w + o, 4); return v; }
static void std_(uint8_t *w, size_t o, uint32_t v) { memcpy(w + o, &v, 4); }

typedef struct zuv_k {
    int rl, c15, dither, sd;
    uint8_t *w;
    struct tsb_header *h;
} zuv_k;

/* One of U or V: the per-scanline steps, the word step and the four starts */
static void zuv_unpack_one(const zuv_k *k, const mmx_uvpack *pk, int is_v)
{
    uint8_t *w = k->w;
    size_t s = is_v ? PO(s_v) : PO(s_u), dy1 = is_v ? PO(d_v_y1) : PO(d_u_y1);
    size_t dx = is_v ? PO(d_v_x) : PO(d_u_x), dy0 = is_v ? PO(d_v_y0) : PO(d_u_y0);
    mmx_t mm0, mm1, mm3;
    mm0 = mmx_dd(ldd(w, dy1), ldd(w, dy0));
    mm0 = is_v ? mmx_pack_v(pk, mm0, 1) : mmx_pack_u(pk, mm0, 1);
    stq(w, is_v ? WO(d_v_y1) : WO(d_u_y1), mmx_punpckldq(mm0, mm0));
    stq(w, is_v ? WO(d_v_y0) : WO(d_u_y0), mmx_punpckhdq(mm0, mm0));
    mm0 = mmx_pslld(mmx_dd(ldd(w, dx), ldd(w, dx)), 2);
    mm0 = is_v ? mmx_pack_v(pk, mm0, 1) : mmx_pack_u(pk, mm0, 1);
    std_(w, is_v ? WO(d_v_x) : WO(d_u_x), mmx_d(mm0, 0));
    mm0 = mmx_dd(ldd(w, s), ldd(w, s));
    mm1 = mmx_dd(0, ldd(w, dx));
    mm3 = mmx_dd(ldd(w, dx), ldd(w, dx));
    mm1 = mmx_paddd(mm1, mm1);
    mm0 = mmx_paddd(mm0, mm1);
    mm3 = mmx_paddd(mm3, mm0);
    stq(w, is_v ? WO(v0) : WO(u0), is_v ? mmx_pack_v(pk, mm0, 0) : mmx_pack_u(pk, mm0, 0));
    stq(w, is_v ? WO(v1) : WO(u1), is_v ? mmx_pack_v(pk, mm3, 0) : mmx_pack_u(pk, mm3, 0));
}

static void zuv_unpack(const zuv_k *k)
{
    uint8_t *w = k->w;
    struct param_zuv *p = (struct param_zuv *)k->h;
    mmx_uvpack pk;
    uint32_t z[16], in[4];
    if (k->rl) mmx_uv_flip(&p->s_u, &p->d_u_x, &p->s_v, &p->d_v_x);
    mmx_uvpack_init(&pk, &p->tinfo);
    zuv_unpack_one(k, &pk, 0);
    zuv_unpack_one(k, &pk, 1);
    std_(w, WO(u_mask), mmx_d(mmx_por(pk.u_int, mmx_fraction_mask), 0));
    std_(w, WO(v_mask), mmx_d(mmx_por(mmx_por(pk.v_high, pk.tile), mmx_fraction_mask), 0));
    if (k->sd) k->h->c = (k->h->c >> 28) << 2;
    memcpy(in, w + PO(s_z), 16);
    mmx_unpack_param_16(in, z, z + 4, z + 6, z + 8);
    memcpy(w + WO(z0), z, 16);
    memcpy(w + WO(d_z_y1), z + 4, 8);
    memcpy(w + WO(d_z_y0), z + 6, 8);
    memcpy(w + WO(d_z_x), z + 8, 8);
}

static void *ldp(const uint8_t *w, size_t o) { void *v; memcpy(&v, w + o, sizeof(v)); return v; }

/* The texel offsets of pixels 0 to 3: (u + v) >> 12 of lane 0 and lane 1
 * of the U0.U2 + V0.V2 and U1.U3 + V1.V3 sums (texels 0 to 3 in that order);
 * right to left, texel 0 is pixel 3 (M0 = 3). */
static void zuv_offsets(int rl, mmx_t uv02, mmx_t uv13, uint32_t off[4])
{
    uint32_t t[4];
    uv02 = mmx_psrld(uv02, MMX_FRACTION_BITS + 1);
    uv13 = mmx_psrld(uv13, MMX_FRACTION_BITS + 1);
    t[0] = mmx_d(uv02, 0);
    t[1] = mmx_d(uv13, 0);
    t[2] = mmx_d(uv02, 1);
    t[3] = mmx_d(uv13, 1);
    off[0] = t[rl ? 3 : 0];
    off[1] = t[rl ? 2 : 1];
    off[2] = t[rl ? 1 : 2];
    off[3] = t[rl ? 0 : 3];
}

/* The texels of a word as pixels. many_words selects the span-of-two-or-
 * more-words code, whose dithered 16-bit form leaves green unmasked. */
static mmx_t zuv_pixels(const zuv_k *k, const mmx_quad *q, int many_words)
{
    mmx_t b, g, r;
    mmx_quad_channels(q, &b, &g, &r);
    if (k->dither) {
        int32_t row = k->h->start_scanline;
        r = mmx_paddusw(r, mmx_dither_table_11[row & 3]);
        b = mmx_paddusw(b, mmx_dither_table_11[row & 3]);
        g = mmx_paddusw(g, mmx_dither_table_10[row & 3]);
        if (many_words) {
            r = mmx_pand(r, mmx_mask_5);
            if (k->c15) g = mmx_pand(mmx_pand(g, mmx_mask_5), mmx_mask_6);
            b = mmx_psrlw(b, 11);
            if (k->c15) r = mmx_psrlw(r, 1);
            return mmx_por(mmx_por(b, r), mmx_psrlw(g, k->c15 ? 6 : 5));
        }
    }
    r = mmx_pand(r, mmx_mask_5);
    if (k->c15) r = mmx_psrlw(r, 1);
    g = mmx_pand(g, k->c15 ? mmx_mask_5 : mmx_mask_6);
    b = mmx_psrlw(b, 11);
    return mmx_por(mmx_por(b, r), mmx_psrlw(g, k->c15 ? 6 : 5));
}

static void zuv_word(const zuv_k *k, uint8_t *at, mmx_t uv02, mmx_t uv13, mmx_t depth_mask, int many_words)
{
    mmx_quad q;
    uint32_t off[4];
    zuv_offsets(k->rl, uv02, uv13, off);
    mmx_quad_fetch(&q, (const uint8_t *)ldp(k->w, WO(texture_address)), (const uint8_t *)ldp(k->w, WO(map_address)), off);
    mmx_quad_store(at, zuv_pixels(k, &q, many_words), mmx_quad_transparent(&q), depth_mask);
}

static void zuv_span(const zuv_k *k, const mmx_walk *wk)
{
    struct tsb_header *h = k->h;
    uint8_t *w = k->w;
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
        mmx_t mm4 = ldq(w, WO(u0)), mm5 = ldq(w, WO(v0)), mm6 = ldq(w, WO(u1)), mm7 = ldq(w, WO(v1));
        mmx_t duv = ldq(w, WO(d_u_x)), uvm = ldq(w, WO(u_mask));
        mmx_t du = mmx_punpckldq(duv, duv), dv = mmx_punpckhdq(duv, duv);
        mmx_t um = mmx_punpckldq(uvm, uvm), vm = mmx_punpckhdq(uvm, uvm);
        if (mmx_zpass_words(&s, ldq(w, WO(z0)), ldq(w, WO(z2)), ldq(w, WO(d_z_x))) == 0) return;
        for (;;) {
            mmx_t uv02 = mmx_paddd(mm4, mm5), uv13 = mmx_paddd(mm6, mm7);
            mm4 = mmx_pand(mmx_paddd(mm4, du), um);
            mm6 = mmx_pand(mmx_paddd(mm6, du), um);
            mm5 = mmx_pand(mmx_paddd(mm5, dv), vm);
            mm7 = mmx_pand(mmx_paddd(mm7, dv), vm);
            zuv_word(k, edi + eax, uv02, uv13, mmx_mask_read(s.mask_base + eax), 1);
            if (eax == 0) break; /* test eax,eax ; lea eax,[eax+WORD_STEP] ; jne */
            eax += step;
        }
    } else {
        mmx_t mask;
        if (mmx_zpass_one(&s, ldq(w, WO(z0)), ldq(w, WO(z2)), &mask) == 0) return;
        zuv_word(k, (uint8_t *)h->screen_address + 2 * s.ebx, mmx_paddd(ldq(w, WO(u0)), ldq(w, WO(v0))),
                 mmx_paddd(ldq(w, WO(u1)), ldq(w, WO(v1))), mask, 0);
    }
}

static void zuv_increment(const zuv_k *k, int carry)
{
    uint8_t *w = k->w;
    mmx_t du = ldq(w, carry ? WO(d_u_y1) : WO(d_u_y0)), dv = ldq(w, carry ? WO(d_v_y1) : WO(d_v_y0));
    mmx_t um = mmx_dd(ldd(w, WO(u_mask)), ldd(w, WO(u_mask))), vm = mmx_dd(ldd(w, WO(v_mask)), ldd(w, WO(v_mask)));
    mmx_t dz = ldq(w, carry ? WO(d_z_y1) : WO(d_z_y0));
    stq(w, WO(u0), mmx_pand(mmx_paddd(ldq(w, WO(u0)), du), um));
    stq(w, WO(v0), mmx_pand(mmx_paddd(ldq(w, WO(v0)), dv), vm));
    stq(w, WO(u1), mmx_pand(mmx_paddd(ldq(w, WO(u1)), du), um));
    stq(w, WO(v1), mmx_pand(mmx_paddd(ldq(w, WO(v1)), dv), vm));
    stq(w, WO(z0), mmx_paddd(ldq(w, WO(z0)), dz));
    stq(w, WO(z2), mmx_paddd(ldq(w, WO(z2)), dz));
}

static void zuv(int rl, int c15, int dither, int sd)
{
    zuv_k k;
    mmx_walk wk;
    k.rl = rl;
    k.c15 = c15;
    k.dither = dither;
    k.sd = sd;
    k.w = mmx_block - offsetof(WT, h);
    k.h = (struct tsb_header *)mmx_block;
    zuv_unpack(&k);
    mmx_walk_addresses(k.h);
    if (sd || dither) k.h->start_scanline &= 3;
    if (!mmx_walk_first(k.h, &wk)) return;
    for (;;) {
        if (rl ? wk.x0 >= wk.x1 : wk.x0 <= wk.x1) zuv_span(&k, &wk);
        if (!mmx_walk_next(k.h, &wk)) return;
        zuv_increment(&k, wk.carry);
        if (dither || sd) k.h->start_scanline = (k.h->start_scanline + 1) & 3;
        mmx_walk_step_addresses(k.h);
    }
}

#define ZUV_PAIR(name, c15, d, s) \
    void BR_ASM_CALL name##_LR(void) { zuv(0, c15, d, s); } \
    void BR_ASM_CALL name##_RL(void) { zuv(1, c15, d, s); }

ZUV_PAIR(RasteriseMMX565_Z_UV, 0, 0, 0)
ZUV_PAIR(RasteriseMMX565_Z_UV_D, 0, 1, 0)
ZUV_PAIR(RasteriseMMX565_Z_UV_S, 0, 0, 1)
ZUV_PAIR(RasteriseMMX565_Z_UV_SD, 0, 1, 1)
ZUV_PAIR(RasteriseMMX555_Z_UV, 1, 0, 0)
ZUV_PAIR(RasteriseMMX555_Z_UV_D, 1, 1, 0)
ZUV_PAIR(RasteriseMMX555_Z_UV_S, 1, 0, 1)
ZUV_PAIR(RasteriseMMX555_Z_UV_SD, 1, 1, 1)
