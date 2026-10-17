/*
 * SPDX-License-Identifier: MIT
 * Portions copyright (c) 1998 Argonaut Software Limited (BRender v1.3.2).
 * Copyright (c) 2026 Zain Dana Harper.
 * See compat/LICENSE-NOTE.md, LICENSE and LICENSES/MIT-BRender.txt.
 */
/*
 * The setup and the rasterise buffer of BRender v1.3.2's MMX triangle
 * family, ported line by line from drivers/pentprim (MIT, Argonaut Software
 * 1998): GenericSetupTriangleFloat_A and its parameter setups in
 * gsetuptf.asm, and the rasterise buffer in rastbuff.asm.
 *
 * The rasterise buffer. Setup does not draw: it lays a parameter block
 * (a tsb_header and the parameters) at the top of a 1600-byte stack that
 * grows down, and the block's rasteriser runs later, when the stack is
 * flushed: when the next block would not fit, and at RendererFlush. The
 * flush runs from the top of the stack, so the blocks of one flush are
 * drawn last-set-up first. Each rasteriser works in the bytes just below its
 * block (its work structure overlays the block, see rastparm.h), which in
 * the stack belong to blocks already drawn; the port keeps that layout, with
 * the assembly's 320 bytes of leeway below the stack.
 *
 * The float setup is fpsetup.asm's SETUP_FLOAT, which gsetuptf.asm repeats
 * instruction for instruction (brender-pentprim-fpsetup-port.c, fps_setup),
 * then two changes: the major edge's start x and its per-scanline step are
 * truncated to the rasteriser's stride (4 pixels), and the per-scanline
 * steps of every parameter are built for a step of stride or stride + 4
 * pixels. The conversions to fixed point are the assembly's: add 1.5 * 2^n
 * and keep the low 32 bits of the double.
 */
#define __BR_V1DB__ 1
#include "brender.h"

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "drv.h"
#include "brender-pentprim-fpsetup-port.h"
#include "brender-pentprim-mmx-port.h"

#if defined(_MSC_VER)
#pragma fp_contract(off)
#pragma float_control(precise, on)
#endif

/* mmx_ip.h turns these into empty macros when USE_MMX is 0; the port always
 * defines them (RasteriseBufferDisable is called either way). */
#if !USE_MMX
#undef RasteriseBufferBegin
#undef RasteriseBufferEnd
#undef RasteriseBufferFlush
#endif

/* rastbuff.asm's data: 40 qwords of leeway, rasteriseBuffer (200 qwords),
 * then the two fake headers rasteriseBufferOff and rasteriseBufferLast. */
#define RB_LEEWAY (40 * 8)
#define RB_BUFFER (RB_LEEWAY)
#define RB_OFF (RB_LEEWAY + 200 * 8)
#define RB_LAST (RB_OFF + 4)
#define RB_BYTES (RB_LAST + 4 + 8)

static uint64_t rb_store[RB_BYTES / 8];
#define RB ((uint8_t *)rb_store)
static int32_t rb_top = RB_LAST;      /* rasteriseBufferTop, as an offset */

/* Per block, what the flush needs that the assembly keeps in code: the
 * block's size, and the buffers it was set up for. Indexed by offset / 4. */
typedef struct rb_slot {
    int32_t size;
    mmx_bounds bounds;
} rb_slot;
static rb_slot rb_slots[RB_BYTES / 4];

void BR_ASM_CALL RasteriseBufferDisable(void) { rb_top = RB_OFF; }
void BR_ASM_CALL RasteriseBufferBegin(void) { rb_top = RB_LAST; }
void BR_ASM_CALL RasteriseBufferEnd(void) {}

/* Executes the stacked blocks, top first. Reaching rasteriseBufferLast
 * (RasteriseBufferDone) resets the stack; reaching rasteriseBufferOff
 * (RasteriseBufferExit) leaves it as it is. */
void BR_ASM_CALL RasteriseBufferFlush(void)
{
    int32_t p = rb_top;
    while (p != RB_LAST && p != RB_OFF && p >= RB_BUFFER && p < RB_OFF) {
        struct tsb_header *h = (struct tsb_header *)(RB + p);
        rasterise_fn *fn = (rasterise_fn *)h->function;
        int32_t size = rb_slots[p / 4].size;
        mmx_block = RB + p;
        mmx_block_bounds = &rb_slots[p / 4].bounds;
        if (fn != NULL) fn();
        if (size <= 0) break;
        p += size;
    }
    if (p == RB_LAST) rb_top = RB_LAST;
}

void *BR_ASM_CALL RasteriseBufferAllocate(br_size_t param_size, br_size_t work_size)
{
    if ((br_size_t)(rb_top - RB_BUFFER) < work_size) RasteriseBufferFlush();
    rb_top -= (int32_t)param_size;
    return RB + rb_top;
}

void BR_ASM_CALL RasteriseBufferDone(void) { rb_top = RB_LAST; }

/* The parameter setup entry points (gsetuptf.asm labels inside
 * GenericSetupTriangleFloat_A). The tables point at them; the port compares
 * the pointer. Each body differs so that a linker cannot fold them. */
static volatile int gs_seen;
void BR_ASM_CALL GenericSetupFloatZRGBUVS_A(void) { gs_seen = 1; }
void BR_ASM_CALL GenericSetupFloatZRGBUV_A(void) { gs_seen = 2; }
void BR_ASM_CALL GenericSetupFloatZRGB_A(void) { gs_seen = 3; }
void BR_ASM_CALL GenericSetupFloatZ_A(void) { gs_seen = 4; }
void BR_ASM_CALL GenericSetupFloatZRGBS_A(void) { gs_seen = 5; }
void BR_ASM_CALL GenericSetupFloatZUVC_A(void) { gs_seen = 6; }
void BR_ASM_CALL GenericSetupFloatZUV_A(void) { gs_seen = 7; }
void BR_ASM_CALL GenericSetupFloatZC_A(void) { gs_seen = 8; }

#define FP_CONV_D12 1649267441664.0  /* fp_conv_d12: 1.5 * 2^40 */
#define FP_CONV_D16 103079215104.0   /* fp_conv_d16: 1.5 * 2^36 */
#define FP_CONV_D24 402653184.0      /* fp_conv_d24: 1.5 * 2^28 */
#define FP_CONV_S 12582912.0         /* fp_conv_s:  1.5 * 2^23 (single) */
#define FP_CONV_S8 49152.0           /* fp_conv_s8: 1.5 * 2^15 (single) */

static uint32_t gs_low32(double d)
{
    volatile double v = d;
    double copy = v;
    uint64_t bits;
    memcpy(&bits, &copy, sizeof(bits));
    return (uint32_t)bits;
}

static uint32_t gs_fbits(float f)
{
    uint32_t b;
    memcpy(&b, &f, sizeof(b));
    return b;
}

/* SETUP_FLOAT_PARAM param, conv[, unsigned]: s_p and its dy1 above it, then
 * d_p_x and its dy0 above it (the param_* layout). */
static void gs_param(int comp, double conv, int is_unsigned, union brp_vertex *v0, union brp_vertex *v1,
                     union brp_vertex *v2, uint32_t *out)
{
    double dp2 = (double)v2->comp_f[comp] - (double)v0->comp_f[comp];
    double dp1 = (double)v1->comp_f[comp] - (double)v0->comp_f[comp];
    double pdx = dp1 * (double)fps.dy2_a - dp2 * (double)fps.dy1_a;
    double pdy = dp2 * (double)fps.dx1_a - dp1 * (double)fps.dx2_a;
    double pdy_0 = pdx * (double)fps.xstep_0 + pdy;
    double pdy_1 = pdx * (double)fps.xstep_1 + pdy;
    double start = (double)fps.t_dx * pdx + (double)fps.top->comp_f[comp];
    start = start + (double)fps.t_dy * pdy;
    out[0] = gs_low32(start + conv);
    out[1] = gs_low32(pdy_1 + conv);
    out[2] = gs_low32(pdx + conv);
    out[3] = gs_low32(pdy_0 + conv);
    if (is_unsigned) out[0] ^= 0x80000000u; /* change from -1..1 signed to 0..1 unsigned */
}

/* SETUP_FLOAT_CONSTANT_COLOUR: B, G, R, A bytes of vertex 0 into h.c */
static void gs_constant_colour(struct tsb_header *h, union brp_vertex *v0)
{
    uint32_t a = gs_fbits((float)((double)v0->comp_f[C_A] + FP_CONV_S8));
    uint32_t r = gs_fbits((float)((double)v0->comp_f[C_R] + FP_CONV_S));
    uint32_t g = gs_fbits((float)((double)v0->comp_f[C_G] + FP_CONV_S));
    uint32_t b = gs_fbits((float)((double)v0->comp_f[C_B] + FP_CONV_S));
    h->c = (b & 0xffu) | (g & 0xffu) << 8 | (r & 0xffu) << 16 | (a & 0xffu) << 24;
}

/* SETUP_FLOAT_CONSTANT_ALPHA */
static void gs_constant_alpha(struct tsb_header *h, union brp_vertex *v0)
{
    h->c = gs_fbits((float)((double)v0->comp_f[C_A] + FP_CONV_S8)) << 24;
}

/* SETUP_TEXTURE_INFO */
static void gs_texture_info(struct texture_info *t)
{
    t->texture_address = work.texture.base;
    t->map_address = work.texture.palette;
    t->width_s = (char)work.texture.width_s;
    t->height_s = (char)work.texture.height_s;
    t->tile_s = (char)work.texture.tile_s;
    t->_pad0[0] = (char)work.texture._pad0;
}

static void gs_rgb(struct param_zrgb *p, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    gs_param(C_R, FP_CONV_D24, 0, v0, v1, v2, &p->s_r);
    gs_param(C_G, FP_CONV_D24, 0, v0, v1, v2, &p->s_g);
    gs_param(C_B, FP_CONV_D24, 0, v0, v1, v2, &p->s_b);
}

/* The jump to setup_param: the tower of entry points in gsetuptf.asm. */
static void gs_params(setup_param_fn *fn, uint8_t *blk, union brp_vertex *v0, union brp_vertex *v1,
                      union brp_vertex *v2)
{
    struct tsb_header *h = (struct tsb_header *)blk;
    if (fn == GenericSetupFloatZRGBUVS_A || fn == GenericSetupFloatZRGBUV_A) {
        struct param_zrgbuv *p = (struct param_zrgbuv *)blk;
        if (fn == GenericSetupFloatZRGBUVS_A) gs_constant_alpha(h, v0);
        gs_param(C_U, FP_CONV_D12, 0, v0, v1, v2, &p->s_u);
        gs_param(C_V, FP_CONV_D12, 0, v0, v1, v2, &p->s_v);
        gs_texture_info(&p->tinfo);
        gs_rgb((struct param_zrgb *)blk, v0, v1, v2);
    } else if (fn == GenericSetupFloatZRGB_A) {
        gs_rgb((struct param_zrgb *)blk, v0, v1, v2);
    } else if (fn == GenericSetupFloatZRGBS_A) {
        gs_constant_alpha(h, v0);
        gs_rgb((struct param_zrgb *)blk, v0, v1, v2);
    } else if (fn == GenericSetupFloatZUVC_A || fn == GenericSetupFloatZUV_A) {
        struct param_zuv *p = (struct param_zuv *)blk;
        if (fn == GenericSetupFloatZUVC_A) gs_constant_colour(h, v0);
        gs_param(C_U, FP_CONV_D12, 0, v0, v1, v2, &p->s_u);
        gs_param(C_V, FP_CONV_D12, 0, v0, v1, v2, &p->s_v);
        gs_texture_info(&p->tinfo);
    } else if (fn == GenericSetupFloatZC_A) {
        gs_constant_colour(h, v0);
    } else if (fn != GenericSetupFloatZ_A) {
        return;
    }
    gs_param(C_SZ, FP_CONV_D16, 1, v0, v1, v2, &((struct param_z *)blk)->s_z);
}

static void gs_record(int32_t at, int32_t size)
{
    rb_slot *s = &rb_slots[at / 4];
    s->size = size;
    s->bounds.cbase = (uint8_t *)work.colour.base;
    s->bounds.cbytes = (int64_t)work.colour.stride_b * work.colour.height;
    s->bounds.zbase = (uint8_t *)work.depth.base;
    s->bounds.zbytes = (int64_t)work.depth.stride_b * work.depth.height;
    s->bounds.tex = (const uint8_t *)work.texture.base;
    s->bounds.tbytes = (int64_t)work.texture.size;
    s->bounds.pal = (const uint8_t *)work.texture.palette;
    s->bounds.pbytes = work.texture.palette != NULL ? (int64_t)work.texture.palette_size * 4 : 0;
}

/* The header: counts, start scanline, buffers, and the rasteriser chosen by
 * the area against iarea_limit and the LR/RL bit. */
static void gs_header(struct local_block *lb, struct tsb_header *h)
{
    int32_t stride = lb->setup.stride, edi;
    int32_t xm_int, xstep_0;
    h->counts.l = ((uint32_t)fps.topCount << 16) | ((uint32_t)fps.bottomCount & 0xffffu);
    h->start_scanline = fps.t_y - 1;
    h->screen_stride = work.colour.stride_b;
    h->depth_stride = work.depth.stride_b;
    h->screen_address = work.colour.base;
    h->depth_address = work.depth.base;
    edi = (gs_fbits(fps.iarea) & 0x7fffffffu) < gs_fbits(lb->setup.iarea_limit) ? -1 : 0;
    edi = edi + edi - fps.flip;
    /* [esi+edi*4].local_block.setup.rasterise_lr_s, edi = 0, -1, -2 or -3 */
    h->function = (void *)(&lb->setup.rasterise_rl_l)[3 + edi];
    h->xm = (br_int_32)fps.xm;
    h->d_xm = (br_int_32)fps.d_xm;
    h->x1 = (br_int_32)fps.x1;
    h->d_x1 = (br_int_32)fps.d_x1;
    h->x2 = (br_int_32)fps.x2;
    h->d_x2 = (br_int_32)fps.d_x2;
    /* truncate x and the major edge's x step to the rasteriser's stride */
    xm_int = (int32_t)mmx_sar32(fps.xm, 16) & -stride;
    xstep_0 = (int32_t)mmx_sar32(fps.d_xm, 16) & -stride;
    fps.t_dx = (float)((double)xm_int - (double)fps.top->comp_f[C_SX]);
    fps.xstep_0 = (float)xstep_0;
    fps.xstep_1 = (float)(xstep_0 + stride);
}

void BR_ASM_CALL GenericSetupTriangleFloat_A(brp_block *block, brp_vertex *v0, brp_vertex *v1, brp_vertex *v2)
{
    struct local_block *lb = (struct local_block *)block;
    int32_t size = (int32_t)lb->setup.param_size;
    int32_t at = rb_top - size;
    /* flush stacked triangles first if the block does not fit */
    if (at < RB_BUFFER) {
        RasteriseBufferFlush();
        at = rb_top - size;
    }
    rb_top = at;
    if (fps_setup(v0, v1, v2)) {
        rb_top += size; /* empty_triangle: back out the allocation */
        return;
    }
    gs_header(lb, (struct tsb_header *)(RB + at));
    gs_record(at, size);
    gs_params(lb->setup.setup_param, RB + at, v0, v1, v2);
}
