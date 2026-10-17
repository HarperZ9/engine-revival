/*
 * SPDX-License-Identifier: MIT
 * Portions copyright (c) 1998 Argonaut Software Limited (BRender v1.3.2).
 * Copyright (c) 2026 Zain Dana Harper.
 * See compat/LICENSE-NOTE.md, LICENSE and LICENSES/MIT-BRender.txt.
 */
/*
 * What the four MMX kernels share, ported line by line from BRender v1.3.2
 * drivers/pentprim (MIT, Argonaut Software 1998): unpack.inc's
 * UNPACK_PARAM_16 and UNPACK_PARAM_8, and the code every
 * kernel repeats around its colour pass: the scanline walk (sl_loop,
 * next_line, next_trapezoid_1 and _2, the top-empty entry) and the Z pass,
 * for a span of two or more 4-pixel words and for a span inside one word.
 *
 * Reads and writes are checked against the buffers the block was set up for
 * (mmx_block_bounds): a byte outside reads as 0 and is not written. The
 * assembly relies on softrend having clipped the triangle, so a clipped
 * triangle never reaches the check.
 */
#define __BR_V1DB__ 1
#include "brender.h"

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "drv.h"
#include "brender-pentprim-mmx-port.h"

uint8_t *mmx_block;
const mmx_bounds *mmx_block_bounds;

static uint8_t mmx_scanline_mask[MMX_MASK_BYTES];

static int mmx_inside(const uint8_t *base, int64_t bytes, const uint8_t *p)
{
    return base != NULL && p >= base && (int64_t)(p - base) < bytes;
}

mmx_t mmx_read8(const uint8_t *base, int64_t bytes, const uint8_t *p)
{
    mmx_t v = 0;
    int i;
    if (mmx_inside(base, bytes, p) && mmx_inside(base, bytes, p + 7)) {
        memcpy(&v, p, 8);
        return v;
    }
    for (i = 0; i < 8; i++)
        if (mmx_inside(base, bytes, p + i)) v |= (mmx_t)p[i] << (8 * i);
    return v;
}

void mmx_write8(uint8_t *base, int64_t bytes, uint8_t *p, mmx_t v)
{
    int i;
    if (mmx_inside(base, bytes, p) && mmx_inside(base, bytes, p + 7)) {
        memcpy(p, &v, 8);
        return;
    }
    for (i = 0; i < 8; i++)
        if (mmx_inside(base, bytes, p + i)) p[i] = (uint8_t)(v >> (8 * i));
}

uint32_t mmx_read4(const uint8_t *base, int64_t bytes, const uint8_t *p)
{
    uint32_t v = 0;
    int i;
    for (i = 0; i < 4; i++)
        if (mmx_inside(base, bytes, p + i)) v |= (uint32_t)p[i] << (8 * i);
    return v;
}

uint8_t mmx_read1(const uint8_t *base, int64_t bytes, const uint8_t *p)
{
    return mmx_inside(base, bytes, p) ? *p : 0;
}

mmx_t mmx_mask_read(int32_t off) { return mmx_read8(mmx_scanline_mask, MMX_MASK_BYTES, mmx_scanline_mask + off); }
void mmx_mask_write(int32_t off, mmx_t v) { mmx_write8(mmx_scanline_mask, MMX_MASK_BYTES, mmx_scanline_mask + off, v); }

/* UNPACK_PARAM_16: the start for pixels 0 to 3 of the first word as 16.16
 * values, the two per-scanline steps, and the step for a whole word. */
void mmx_unpack_param_16(const uint32_t *p, uint32_t *p0, uint32_t *d_p_y1, uint32_t *d_p_y0, uint32_t *d_p_x)
{
    mmx_t mm0 = mmx_dd(p[0], p[1]), mm4 = mmx_dd(p[2], p[3]);
    mmx_t mm2 = mm0, mm1 = mm4, mm3 = mm4, mm5;
    mm0 = mmx_punpckldq(mm0, mm0);
    mm1 = mmx_punpckldq(mm1, mm1);
    mm5 = mm1;
    mm4 = mmx_psllq(mm4, 32);
    mm1 = mmx_paddd(mm1, mm1);
    mm0 = mmx_paddd(mm0, mm4);
    mm1 = mmx_paddd(mm1, mm0);
    mm5 = mmx_pslld(mm5, 2);
    mm2 = mmx_punpckhdq(mm2, mm2);
    mm3 = mmx_punpckhdq(mm3, mm3);
    p0[0] = mmx_d(mm0, 0); p0[1] = mmx_d(mm0, 1);
    p0[2] = mmx_d(mm1, 0); p0[3] = mmx_d(mm1, 1);
    d_p_x[0] = mmx_d(mm5, 0); d_p_x[1] = mmx_d(mm5, 1);
    d_p_y1[0] = mmx_d(mm2, 0); d_p_y1[1] = mmx_d(mm2, 1);
    d_p_y0[0] = mmx_d(mm3, 0); d_p_y0[1] = mmx_d(mm3, 1);
}

/* UNPACK_PARAM_8: the same, packed to four 8.8 words with saturation. */
void mmx_unpack_param_8(const uint32_t *p, uint32_t *p0, uint32_t *d_p_y1, uint32_t *d_p_y0, uint32_t *d_p_x)
{
    mmx_t mm0 = mmx_dd(p[0], p[1]), mm4 = mmx_dd(p[2], p[3]);
    mmx_t mm2 = mm0, mm1 = mm4, mm3 = mm4, mm5;
    mm0 = mmx_punpckldq(mm0, mm0);
    mm1 = mmx_punpckldq(mm1, mm1);
    mm5 = mm1;
    mm4 = mmx_psllq(mm4, 32);
    mm1 = mmx_paddd(mm1, mm1);
    mm2 = mmx_punpckhdq(mm2, mm2);
    mm0 = mmx_paddd(mm0, mm4);
    mm3 = mmx_punpckhdq(mm3, mm3);
    mm1 = mmx_paddd(mm1, mm0);
    mm0 = mmx_psrad(mm0, 16);
    mm1 = mmx_psrad(mm1, 16);
    mm5 = mmx_paddd(mm5, mm5);
    mm0 = mmx_packssdw(mm0, mm1);
    mm5 = mmx_paddd(mm5, mm5);
    mm5 = mmx_psrad(mm5, 16);
    mm5 = mmx_packssdw(mm5, mm5);
    mm2 = mmx_psrad(mm2, 16);
    mm2 = mmx_packssdw(mm2, mm2);
    mm3 = mmx_psrad(mm3, 16);
    mm3 = mmx_packssdw(mm3, mm3);
    p0[0] = mmx_d(mm0, 0); p0[1] = mmx_d(mm0, 1);
    d_p_x[0] = mmx_d(mm5, 0); d_p_x[1] = mmx_d(mm5, 1);
    d_p_y1[0] = mmx_d(mm2, 0); d_p_y1[1] = mmx_d(mm2, 1);
    d_p_y0[0] = mmx_d(mm3, 0); d_p_y0[1] = mmx_d(mm3, 1);
}

static uint32_t mmx_rol16(uint32_t v) { return (v << 16) | (v >> 16); }
static int32_t mmx_sar16(uint32_t v) { return (int32_t)mmx_sar32(v, 16); }

/* movd mm5,start_scanline ; punpckldq ; pmaddwd mm5,(screen_stride,
 * depth_stride) ; paddd mm5,(screen_address, depth_address) */
void mmx_walk_addresses(struct tsb_header *h)
{
    uint32_t s = (uint32_t)h->start_scanline;
    mmx_t m = mmx_pmaddwd(mmx_dd(s, s), mmx_dd((uint32_t)h->screen_stride, (uint32_t)h->depth_stride));
    h->screen_address = (uint8_t *)h->screen_address + (int32_t)mmx_d(m, 0);
    h->depth_address = (uint8_t *)h->depth_address + (int32_t)mmx_d(m, 1);
}

void mmx_walk_step_addresses(struct tsb_header *h)
{
    h->screen_address = (uint8_t *)h->screen_address + h->screen_stride;
    h->depth_address = (uint8_t *)h->depth_address + h->depth_stride;
}

/* The entry: eax = xm >> 16, ebx = x1 >> 16, and the top-empty switch to
 * the lower trapezoid. Returns 0 for skip_triangle. */
int mmx_walk_first(struct tsb_header *h, mmx_walk *w)
{
    uint32_t ecx = h->counts.l;
    w->x0 = mmx_sar16((uint32_t)h->xm);
    w->x1 = mmx_sar16((uint32_t)h->x1);
    w->carry = 0;
    if ((int32_t)ecx >= 0) return 1;
    ecx = mmx_rol16(ecx);
    if ((int32_t)ecx < 0) return 0;
    h->counts.l = ecx;
    h->d_x1 = h->d_x2;
    h->x1 = h->x2;
    w->x1 = mmx_sar16((uint32_t)h->x2);
    return 1;
}

/* After a span (drawn, invisible or empty): count the scanline down and step
 * the edges, switching to the lower trapezoid when the top one runs out.
 * Returns 0 for skip_triangle. Sets the carry index for the parameter
 * steps; the caller adds the steps and moves the addresses on. */
int mmx_walk_next(struct tsb_header *h, mmx_walk *w)
{
    uint32_t eax = (uint32_t)h->xm, ebx = (uint32_t)h->x1, ecx = h->counts.l, edx;
    ecx -= 0x10000u;
    if ((int32_t)ecx >= 0) {
        /* sl_loop */
        h->counts.l = ecx;
        edx = eax;
        eax += (uint32_t)h->d_xm;
        h->xm = (br_int_32)eax;
        edx ^= (uint32_t)h->d_xm ^ eax;
        ebx += (uint32_t)h->d_x1;
    } else {
        /* next_trapezoid_1, next_trapezoid_2 */
        ecx = mmx_rol16(ecx);
        if ((int32_t)ecx < 0) return 0;
        h->counts.l = ecx;
        h->d_x1 = h->d_x2;
        edx = eax;
        eax += (uint32_t)h->d_xm;
        h->xm = (br_int_32)eax;
        edx ^= (uint32_t)h->d_xm ^ eax;
        ebx = (uint32_t)h->x2;
    }
    /* sl_bottom_cont: shl edx,14 puts bit 18 in CF ; sbb edx,edx */
    w->carry = (edx >> 18) & 1u ? -1 : 0;
    h->x1 = (br_int_32)ebx;
    w->x0 = mmx_sar16(eax);
    w->x1 = mmx_sar16(ebx);
    return 1;
}

/* start: split the span into 4-pixel words. Returns 0 when it is one word
 * (one_word), 1 for two or more. J_EMPTY is the caller's. */
int mmx_span_split(mmx_span *s)
{
    int32_t eax = s->x0, ebx = s->x1;
    s->ecx = eax & 3;
    s->edx = ebx & 3;
    eax &= ~3;
    ebx &= ~3;
    s->ebx = ebx;
    s->eax = eax - ebx;
    if (s->eax == 0) return 0;
    s->eax += s->eax;                  /* convert pixel number to address offset */
    s->mask_base = s->rl ? 0 : -s->eax; /* ebp = scanline_mask (- eax for left to right) */
    return 1;
}

static mmx_t mmx_zmerge(mmx_t znew, mmx_t zold, mmx_t mask)
{
    return mmx_por(mmx_pandn(mask, zold), mmx_pand(znew, mask));
}

/* The Z pass for two or more words. depth_row + 2 * ebx is the last word. */
uint32_t mmx_zpass_words(mmx_span *s, mmx_t z01, mmx_t z23, mmx_t d_z_x)
{
    const mmx_bounds *b = mmx_block_bounds;
    const int32_t step = s->rl ? -8 : 8;
    const mmx_t *start_masks = s->rl ? mmx_right_masks : mmx_left_masks;
    const mmx_t *end_masks = s->rl ? mmx_left_masks : mmx_right_masks;
    uint8_t *esi = s->depth_row + 2 * s->ebx;
    int32_t eax = s->eax, ebp = s->mask_base;
    mmx_t mm2, mm3, mm4, mm5 = 0, mm6, mm7;

    mm2 = mmx_psrad(z01, 16);
    mm3 = mmx_psrad(z23, 16);
    z01 = s->rl ? mmx_psubd(z01, d_z_x) : mmx_paddd(z01, d_z_x);
    z23 = s->rl ? mmx_psubd(z23, d_z_x) : mmx_paddd(z23, d_z_x);
    mm2 = mmx_packssdw(mm2, mm3);
    mm7 = mmx_read8(b->zbase, b->zbytes, esi + eax);
    mm6 = mmx_pcmpeqw(mmx_psubusw(mm2, mm7), 0);
    mm6 = mmx_pand(mm6, start_masks[s->ecx]);
    if (s->screendoor) mm6 = mmx_pand(mm6, s->sd_mask);
    mm4 = mm6;
    mm6 = mmx_zmerge(mm2, mm7, mm6);
    eax += step;
    while (eax != 0) {
        mm2 = mmx_psrad(z01, 16);
        mm3 = mmx_psrad(z23, 16);
        z01 = s->rl ? mmx_psubd(z01, d_z_x) : mmx_paddd(z01, d_z_x);
        z23 = s->rl ? mmx_psubd(z23, d_z_x) : mmx_paddd(z23, d_z_x);
        mm7 = mmx_read8(b->zbase, b->zbytes, esi + eax);
        mm2 = mmx_packssdw(mm2, mm3);
        mmx_write8(b->zbase, b->zbytes, esi + eax - step, mm6);
        mmx_mask_write(ebp + eax - step, mm4);
        mm6 = mmx_pcmpeqw(mmx_psubusw(mm2, mm7), 0);
        mm5 = mmx_por(mm5, mm4);
        if (s->screendoor) mm6 = mmx_pand(mm6, s->sd_mask);
        mm4 = mm6;
        mm6 = mmx_zmerge(mm2, mm7, mm6);
        eax += step;
    }
    /* z_loop_tail */
    mm7 = mmx_read8(b->zbase, b->zbytes, esi);
    z01 = mmx_psrad(z01, 16);
    mmx_write8(b->zbase, b->zbytes, esi - step, mm6);
    z23 = mmx_psrad(z23, 16);
    mmx_mask_write(ebp - step, mm4);
    z01 = mmx_packssdw(z01, z23);
    mm6 = mmx_pcmpeqw(mmx_psubusw(z01, mm7), 0);
    mm6 = mmx_pand(mm6, end_masks[s->edx]);
    mm5 = mmx_por(mm5, mm4);
    if (s->screendoor) mm6 = mmx_pand(mm6, s->sd_mask);
    mm4 = mm6;
    mm6 = mmx_zmerge(z01, mm7, mm6);
    mmx_mask_write(ebp, mm4);
    mm5 = mmx_por(mm5, mm4);
    mmx_write8(b->zbase, b->zbytes, esi, mm6);
    s->mask_base = ebp;
    return mmx_d(mmx_packsswb(mm5, mm5), 0);
}

/* one_word: the depth test and write for a span inside one word. *mask is
 * the pixel mask the colour pass uses. */
uint32_t mmx_zpass_one(mmx_span *s, mmx_t z01, mmx_t z23, mmx_t *mask)
{
    const mmx_bounds *b = mmx_block_bounds;
    const mmx_t *start_masks = s->rl ? mmx_right_masks : mmx_left_masks;
    const mmx_t *end_masks = s->rl ? mmx_left_masks : mmx_right_masks;
    uint8_t *at = s->depth_row + 2 * s->ebx;
    mmx_t mm7 = mmx_read8(b->zbase, b->zbytes, at), mm6, mm0, mm1;
    mm0 = mmx_psrad(z01, 16);
    mm6 = s->screendoor ? mmx_pand(s->sd_mask, start_masks[s->ecx]) : start_masks[s->ecx];
    mm1 = mmx_psrad(z23, 16);
    mm6 = mmx_pand(mm6, end_masks[s->edx]);
    mm0 = mmx_packssdw(mm0, mm1);
    mm1 = mm0;
    mm0 = mmx_pcmpeqw(mmx_psubusw(mm0, mm7), 0);
    mm0 = mmx_pand(mm0, mm6);
    *mask = mm0;
    mmx_write8(b->zbase, b->zbytes, at, mmx_zmerge(mm1, mm7, mm0));
    return mmx_d(mmx_packsswb(mm0, mm0), 0);
}
