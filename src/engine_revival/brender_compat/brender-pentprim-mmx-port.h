/*
 * SPDX-License-Identifier: MIT
 * Portions copyright (c) 1998 Argonaut Software Limited (BRender v1.3.2).
 * Copyright (c) 2026 Zain Dana Harper.
 * See compat/LICENSE-NOTE.md, LICENSE and LICENSES/MIT-BRender.txt.
 */
/*
 * Shared declarations for the C port of BRender v1.3.2's MMX 15 and 16-bit
 * triangle family (drivers/pentprim gsetuptf.asm, rastbuff.asm,
 * rastrise.asm, unpack.inc, xzrgb.inc, xzuv1.inc, xzuvc.inc, xzuvrgb.inc).
 *
 * An MMX register is a uint64_t. Each mmx_* function below does what the
 * MMX instruction of the same name does to its operands, lane by lane,
 * wrapping or saturating exactly as the instruction does. Shift counts at or
 * above the lane width give 0 (or the sign, for the arithmetic shifts), as
 * the instructions do.
 */
#ifndef BRENDER_PENTPRIM_MMX_PORT_H
#define BRENDER_PENTPRIM_MMX_PORT_H

#include <stdint.h>

typedef uint64_t mmx_t;

static inline uint16_t mmx_w(mmx_t a, int i) { return (uint16_t)(a >> (16 * i)); }
static inline uint32_t mmx_d(mmx_t a, int i) { return (uint32_t)(a >> (32 * i)); }
static inline uint8_t mmx_b(mmx_t a, int i) { return (uint8_t)(a >> (8 * i)); }
static inline mmx_t mmx_dd(uint32_t lo, uint32_t hi) { return (mmx_t)lo | (mmx_t)hi << 32; }
static inline mmx_t mmx_ww(uint16_t w0, uint16_t w1, uint16_t w2, uint16_t w3)
{
    return (mmx_t)w0 | (mmx_t)w1 << 16 | (mmx_t)w2 << 32 | (mmx_t)w3 << 48;
}

static inline mmx_t mmx_pand(mmx_t a, mmx_t b) { return a & b; }
static inline mmx_t mmx_pandn(mmx_t a, mmx_t b) { return ~a & b; }
static inline mmx_t mmx_por(mmx_t a, mmx_t b) { return a | b; }
static inline mmx_t mmx_pxor(mmx_t a, mmx_t b) { return a ^ b; }

static inline mmx_t mmx_paddw(mmx_t a, mmx_t b)
{
    return mmx_ww((uint16_t)(mmx_w(a, 0) + mmx_w(b, 0)), (uint16_t)(mmx_w(a, 1) + mmx_w(b, 1)),
                  (uint16_t)(mmx_w(a, 2) + mmx_w(b, 2)), (uint16_t)(mmx_w(a, 3) + mmx_w(b, 3)));
}
static inline mmx_t mmx_psubw(mmx_t a, mmx_t b)
{
    return mmx_ww((uint16_t)(mmx_w(a, 0) - mmx_w(b, 0)), (uint16_t)(mmx_w(a, 1) - mmx_w(b, 1)),
                  (uint16_t)(mmx_w(a, 2) - mmx_w(b, 2)), (uint16_t)(mmx_w(a, 3) - mmx_w(b, 3)));
}
static inline mmx_t mmx_paddd(mmx_t a, mmx_t b) { return mmx_dd(mmx_d(a, 0) + mmx_d(b, 0), mmx_d(a, 1) + mmx_d(b, 1)); }
static inline mmx_t mmx_psubd(mmx_t a, mmx_t b) { return mmx_dd(mmx_d(a, 0) - mmx_d(b, 0), mmx_d(a, 1) - mmx_d(b, 1)); }

static inline uint16_t mmx_usat16(int32_t v) { return (uint16_t)(v < 0 ? 0 : v > 0xffff ? 0xffff : v); }
static inline mmx_t mmx_paddusw(mmx_t a, mmx_t b)
{
    mmx_t r = 0;
    int i;
    for (i = 0; i < 4; i++) r |= (mmx_t)mmx_usat16((int32_t)mmx_w(a, i) + mmx_w(b, i)) << (16 * i);
    return r;
}
static inline mmx_t mmx_psubusw(mmx_t a, mmx_t b)
{
    mmx_t r = 0;
    int i;
    for (i = 0; i < 4; i++) r |= (mmx_t)mmx_usat16((int32_t)mmx_w(a, i) - mmx_w(b, i)) << (16 * i);
    return r;
}
static inline mmx_t mmx_pcmpeqw(mmx_t a, mmx_t b)
{
    mmx_t r = 0;
    int i;
    for (i = 0; i < 4; i++) r |= (mmx_t)(mmx_w(a, i) == mmx_w(b, i) ? 0xffffu : 0u) << (16 * i);
    return r;
}
static inline mmx_t mmx_pcmpeqb(mmx_t a, mmx_t b)
{
    mmx_t r = 0;
    int i;
    for (i = 0; i < 8; i++) r |= (mmx_t)(mmx_b(a, i) == mmx_b(b, i) ? 0xffu : 0u) << (8 * i);
    return r;
}

static inline mmx_t mmx_psrlw(mmx_t a, uint64_t n)
{
    mmx_t r = 0;
    int i;
    if (n > 15) return 0;
    for (i = 0; i < 4; i++) r |= (mmx_t)(uint16_t)(mmx_w(a, i) >> n) << (16 * i);
    return r;
}
static inline mmx_t mmx_psllw(mmx_t a, uint64_t n)
{
    mmx_t r = 0;
    int i;
    if (n > 15) return 0;
    for (i = 0; i < 4; i++) r |= (mmx_t)(uint16_t)(mmx_w(a, i) << n) << (16 * i);
    return r;
}
static inline mmx_t mmx_psrld(mmx_t a, uint64_t n)
{
    return n > 31 ? 0 : mmx_dd(mmx_d(a, 0) >> n, mmx_d(a, 1) >> n);
}
static inline mmx_t mmx_pslld(mmx_t a, uint64_t n)
{
    return n > 31 ? 0 : mmx_dd(mmx_d(a, 0) << n, mmx_d(a, 1) << n);
}
static inline uint32_t mmx_sar32(uint32_t v, uint64_t n)
{
    if (n > 31) n = 31;
    return (v & 0x80000000u) ? ~(~v >> n) : v >> n;
}
static inline mmx_t mmx_psrad(mmx_t a, uint64_t n) { return mmx_dd(mmx_sar32(mmx_d(a, 0), n), mmx_sar32(mmx_d(a, 1), n)); }
static inline mmx_t mmx_psrlq(mmx_t a, uint64_t n) { return n > 63 ? 0 : a >> n; }
static inline mmx_t mmx_psllq(mmx_t a, uint64_t n) { return n > 63 ? 0 : a << n; }

static inline uint16_t mmx_ssat16(int32_t v) { return (uint16_t)(int16_t)(v < -32768 ? -32768 : v > 32767 ? 32767 : v); }
static inline uint8_t mmx_ssat8(int32_t v) { return (uint8_t)(int8_t)(v < -128 ? -128 : v > 127 ? 127 : v); }
static inline mmx_t mmx_packssdw(mmx_t a, mmx_t b)
{
    return mmx_ww(mmx_ssat16((int32_t)mmx_d(a, 0)), mmx_ssat16((int32_t)mmx_d(a, 1)),
                  mmx_ssat16((int32_t)mmx_d(b, 0)), mmx_ssat16((int32_t)mmx_d(b, 1)));
}
static inline mmx_t mmx_packsswb(mmx_t a, mmx_t b)
{
    mmx_t r = 0;
    int i;
    for (i = 0; i < 4; i++) {
        r |= (mmx_t)mmx_ssat8((int16_t)mmx_w(a, i)) << (8 * i);
        r |= (mmx_t)mmx_ssat8((int16_t)mmx_w(b, i)) << (8 * (i + 4));
    }
    return r;
}

static inline mmx_t mmx_punpcklbw(mmx_t a, mmx_t b)
{
    mmx_t r = 0;
    int i;
    for (i = 0; i < 4; i++) r |= (mmx_t)mmx_b(a, i) << (16 * i) | (mmx_t)mmx_b(b, i) << (16 * i + 8);
    return r;
}
static inline mmx_t mmx_punpckhbw(mmx_t a, mmx_t b) { return mmx_punpcklbw(a >> 32, b >> 32); }
static inline mmx_t mmx_punpcklwd(mmx_t a, mmx_t b) { return mmx_ww(mmx_w(a, 0), mmx_w(b, 0), mmx_w(a, 1), mmx_w(b, 1)); }
static inline mmx_t mmx_punpckhwd(mmx_t a, mmx_t b) { return mmx_ww(mmx_w(a, 2), mmx_w(b, 2), mmx_w(a, 3), mmx_w(b, 3)); }
static inline mmx_t mmx_punpckldq(mmx_t a, mmx_t b) { return mmx_dd(mmx_d(a, 0), mmx_d(b, 0)); }
static inline mmx_t mmx_punpckhdq(mmx_t a, mmx_t b) { return mmx_dd(mmx_d(a, 1), mmx_d(b, 1)); }

static inline mmx_t mmx_pmulhw(mmx_t a, mmx_t b)
{
    mmx_t r = 0;
    int i;
    for (i = 0; i < 4; i++)
        r |= (mmx_t)(uint16_t)((uint32_t)((int32_t)(int16_t)mmx_w(a, i) * (int16_t)mmx_w(b, i)) >> 16) << (16 * i);
    return r;
}
static inline mmx_t mmx_pmullw(mmx_t a, mmx_t b)
{
    mmx_t r = 0;
    int i;
    for (i = 0; i < 4; i++) r |= (mmx_t)(uint16_t)((int32_t)(int16_t)mmx_w(a, i) * (int16_t)mmx_w(b, i)) << (16 * i);
    return r;
}
static inline mmx_t mmx_pmaddwd(mmx_t a, mmx_t b)
{
    uint32_t lo = (uint32_t)((int32_t)(int16_t)mmx_w(a, 0) * (int16_t)mmx_w(b, 0))
                + (uint32_t)((int32_t)(int16_t)mmx_w(a, 1) * (int16_t)mmx_w(b, 1));
    uint32_t hi = (uint32_t)((int32_t)(int16_t)mmx_w(a, 2) * (int16_t)mmx_w(b, 2))
                + (uint32_t)((int32_t)(int16_t)mmx_w(a, 3) * (int16_t)mmx_w(b, 3));
    return mmx_dd(lo, hi);
}

/* ---- shared state and helpers (brender-pentprim-mmx-common-port.c) ---- */

/* rastrise.asm's tables. */
extern const mmx_t mmx_mask_5, mmx_mask_6, mmx_left_masks[4], mmx_right_masks[4];
extern const mmx_t mmx_mask_5d, mmx_mask_6d;
extern const mmx_t mmx_dither_table_11[4], mmx_dither_table_10[4], mmx_dither_table_9[4], mmx_dither_table_8[4];
extern const mmx_t mmx_fraction_mask, mmx_fraction_bit, mmx_uv_masks[21];
extern const mmx_t mmx_screendoor_masks[16 * 4];
#define MMX_FRACTION_BITS 11

/* The buffers one block may touch, recorded when it is set up, so that the
 * reads and writes of a deferred block are checked against the buffers it
 * was set up for. */
typedef struct mmx_bounds {
    uint8_t *cbase, *zbase;
    const uint8_t *tex, *pal;
    int64_t cbytes, zbytes, tbytes, pbytes;
} mmx_bounds;

/* The block being rasterised by RasteriseBufferFlush, and its bounds. */
extern uint8_t *mmx_block;
extern const mmx_bounds *mmx_block_bounds;

/* Checked memory access: a byte outside the block's buffer reads as 0 and is
 * not written (the assembly relies on softrend having clipped). */
mmx_t mmx_read8(const uint8_t *base, int64_t bytes, const uint8_t *p);
void mmx_write8(uint8_t *base, int64_t bytes, uint8_t *p, mmx_t v);
uint32_t mmx_read4(const uint8_t *base, int64_t bytes, const uint8_t *p);
uint8_t mmx_read1(const uint8_t *base, int64_t bytes, const uint8_t *p);

/* rastrise.asm's scanline_mask: 8192 words. Offsets are in bytes. */
mmx_t mmx_mask_read(int32_t off);
void mmx_mask_write(int32_t off, mmx_t v);
#define MMX_MASK_BYTES (8192 * 2)

/* unpack.inc. p points at a start value with its dy1 above it and dx at
 * p[2] with dy0 above that (the param_* layout). */
void mmx_unpack_param_16(const uint32_t *p, uint32_t *p0, uint32_t *d_p_y1, uint32_t *d_p_y0, uint32_t *d_p_x);
void mmx_unpack_param_8(const uint32_t *p, uint32_t *p0, uint32_t *d_p_y1, uint32_t *d_p_y0, uint32_t *d_p_x);

/* The scanline walk shared by every kernel: the edge stepping of the
 * sl_loop, next_line and next_trapezoid code. */
typedef struct mmx_walk {
    int32_t x0, x1;   /* eax and ebx: the integer start and end of the span */
    int carry;        /* edx after sbb: -1 when xm crossed a 4-pixel boundary */
} mmx_walk;
int mmx_walk_first(struct tsb_header *h, mmx_walk *w);
int mmx_walk_next(struct tsb_header *h, mmx_walk *w);
/* screen_address and depth_address += start_scanline * stride, by pmaddwd */
void mmx_walk_addresses(struct tsb_header *h);
/* the per scanline address step */
void mmx_walk_step_addresses(struct tsb_header *h);

/* The Z pass for a span of two or more 4-pixel words, and for one word.
 * Returns the low dword of packsswb of the accumulated mask (nonzero when
 * any pixel passed). Writes the depth buffer and the scanline masks. */
typedef struct mmx_span {
    int rl;                  /* right to left */
    int screendoor;
    mmx_t sd_mask;           /* the screendoor mask for this scanline */
    uint8_t *depth_row;      /* esi at the span: the scanline's depth address */
    int32_t x0, x1;          /* the integer start and end */
    int32_t eax, ebx, ecx, edx; /* after the split: -count bytes, end word, start and end pixel in word */
    int32_t mask_base;       /* ebp as an offset into scanline_mask */
} mmx_span;
int mmx_span_split(mmx_span *s);
uint32_t mmx_zpass_words(mmx_span *s, mmx_t z01, mmx_t z23, mmx_t d_z_x);
uint32_t mmx_zpass_one(mmx_span *s, mmx_t z01, mmx_t z23, mmx_t *mask);

/* Four texels of a word. off[k] is the texel offset of pixel k (left to
 * right in the buffer, whichever way the kernel walks). The palette
 * entries (B, G, R, X bytes) come back interleaved as the kernels' punpcklbw
 * leaves them: lo = pixels 0 and 1, hi = pixels 2 and 3; idx holds the four
 * texel indices, pixel k in byte k (the kernels' temp_mask). */
typedef struct mmx_quad {
    mmx_t lo, hi;
    uint32_t idx;
} mmx_quad;
void mmx_quad_fetch(mmx_quad *q, const uint8_t *tex, const uint8_t *pal, const uint32_t off[4]);
/* punpcklwd/punpckhwd/punpck*bw: each channel as four words, the byte in
 * both halves (b = blue, g = green, r = red) */
void mmx_quad_channels(const mmx_quad *q, mmx_t *b, mmx_t *g, mmx_t *r);
/* the transparency mask: 0xffff words where the texel index is 0 */
mmx_t mmx_quad_transparent(const mmx_quad *q);
/* pandn mm3,mask ; read ; merge ; write: draw px where the depth mask is
 * set and the texel is not transparent */
void mmx_quad_store(uint8_t *at, mmx_t px, mmx_t transparent, mmx_t depth_mask);

/* The U and V packing of unpack.inc and xzuv1.inc: from the map's width,
 * height and tile shifts, the tile mask (mm5), the U integer mask (mm6), the
 * V high integer mask (mm7) and the two shifts. */
typedef struct mmx_uvpack {
    mmx_t tile, u_int, v_high;
    uint32_t tile_s, v_shift;
} mmx_uvpack;
void mmx_uvpack_init(mmx_uvpack *k, const struct texture_info *t);
mmx_t mmx_pack_u(const mmx_uvpack *k, mmx_t v, int bridge);
mmx_t mmx_pack_v(const mmx_uvpack *k, mmx_t v, int bridge);
/* Right to left: flip d_x and move the start to the end of the word. */
void mmx_uv_flip(uint32_t *s_u, uint32_t *d_u_x, uint32_t *s_v, uint32_t *d_v_x);
/* UNPACK_UV_32 after the flip: uv is s_u, d_u_y1, d_u_x, d_u_y0, s_v,
 * d_v_y1, d_v_x, d_v_y0, rewritten in place as the work structure's u ...
 * d_v_y0; masks receives u_mask and v_mask. */
void mmx_unpack_uv_32(const struct texture_info *t, uint32_t uv[8], uint32_t masks[2]);

#endif
