/*
 * SPDX-License-Identifier: MIT
 * Portions copyright (c) 1998 Argonaut Software Limited (BRender v1.3.2).
 * Copyright (c) 2026 Zain Dana Harper.
 * See compat/LICENSE-NOTE.md, LICENSE and LICENSES/MIT-BRender.txt.
 */
/*
 * The 8-bit perspective correct texture kernels (INDEX_8, 16-bit depth,
 * square maps 32, 64, 128, 256 and 1024 wide), ported line by line from
 * BRender v1.3.2 drivers/pentprim/fti8pizp.asm (MIT, Argonaut Software 1998):
 *
 *   TriangleRender_ZPT_I8_D16_<n>   TrapeziumRender_ZPT_I8_D16 n,f and n,b
 *                                   with ScanlineRender_ZPT_I8_D16, unlit
 *   TriangleRender_ZPTI_I8_D16_<n>  TrapeziumRender_ZPTI_I8_D16 n,f and n,b
 *                                   with ScanlineRender_ZPTI_I8_D16, lit
 *                                   through the shade table
 *
 * with the setup they call, pfpsetup.asm's TriangleSetup_ZPT and
 * TriangleSetup_ZPTI, in brender-pentprim-pfpsetup-port.c. When the setup
 * finds the triangle flat enough in w it returns with carry set and the
 * kernel jumps into the linear rasteriser: for 256, fti8_piz.asm ZT and
 * zb8.asm ZTI; for the other sizes, zb8p2unl.asm and zb8p2lit.asm
 * (brender-pentprim-i8pow2-port.c).
 *
 * The texel offset is v * n + u, and u and v each wrap at n: inc al and
 * inc ah for 256; for the others the offset is shifted up so u and v fill
 * whole bytes (words for 1024), stepped, shifted back and masked. Each of u
 * and v carries an error term, a numerator kept in 0..q (q, the denominator,
 * is 1/w scaled), stepped per pixel and scanline; the kernel picks one of
 * five scanline loops by the signs of the x gradients of q, u and v.
 *
 * x86 conditions as the assembly takes them: a signed jump after add, sub or
 * cmp tests the exact result, jnc the carry; registers wrap at 32 bits.
 *
 * Additions, none of which changes a pixel the assembly would draw inside
 * its buffers: a pixel or depth value outside its buffer is not written, a
 * texel outside its map is not read (the pixel is skipped), and an error
 * loop that has not ended after 2^24 steps stops (the assembly would keep
 * going; no scene reaches it).
 */
#define __BR_V1DB__ 1
#include "brender.h"

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "drv.h"
#include "brender-pentprim-fpsetup-port.h"
#include "brender-pentprim-pfpsetup-port.h"

#define P8_LOOP_LIMIT (1u << 24)

enum { P8_D, P8_I, P8_B }; /* the udirn and vdirn of a scanline loop: d, i, b */

/* The rasteriser's own state: workspace.scanAddress and depthAddress as byte
 * offsets from the buffer bases, the major edge with its integer and
 * fraction swapped, and work.tsl's start, end, source and steps. */
static struct {
    uint8_t *cbase, *zbase;
    int32_t climit, zlimit, cstride, zstride;
    int32_t scan, depth, start, end, zstart;
    uint32_t main_i, main_d_i, source, dz, di, ddenominator;
    uint32_t pow2;                                   /* the map is 1 << pow2 wide */
} p8;

static uint32_t p8_ror16(uint32_t v) { return (v >> 16) | (v << 16); }

/* cmp g,80000000h ; adc g,-1 */
static uint32_t p8_adjust(uint32_t g) { return g >= 0x80000000u ? g - 1u : g; }

static int p8_lt_sum(uint32_t a, uint32_t b) { return (int64_t)(int32_t)a + (int32_t)b < 0; }
static int p8_lt_diff(uint32_t a, uint32_t b) { return (int64_t)(int32_t)a - (int32_t)b < 0; }

/* pre ; incu / decu (shift 0) or incv / decv (shift 8) ; post1 ; post2:
 * u or v of the offset v * n + u, one up or down, wrapping at n. */
static void p8_texel_step(uint32_t *src, int shift, int up)
{
    uint32_t m = (1u << p8.pow2) - 1u, u = *src & m, v = (*src >> p8.pow2) & m, d = up ? 1u : m;
    if (shift == 0) u = (u + d) & m;
    else v = (v + d) & m;
    *src = v << p8.pow2 | u;
}

static int p8_open(void)
{
    p8.cbase = (uint8_t *)work.colour.base;
    p8.zbase = (uint8_t *)work.depth.base;
    p8.cstride = work.colour.stride_b;
    p8.zstride = work.depth.stride_b;
    p8.climit = work.colour.stride_b * (int32_t)work.colour.height;
    p8.zlimit = work.depth.stride_b * (int32_t)work.depth.height;
    return p8.cbase != NULL && p8.zbase != NULL;
}

/* next_pixel: z test (cmp bx,dx ; ja nodraw), transparent zero texel, then
 * the texel, or for the lit kernel its shade table entry at row i >> 16. */
static void p8_pixel(int lit, int32_t dest, int32_t zdest, uint32_t z, uint32_t i, uint32_t src)
{
    const uint8_t *tex = (const uint8_t *)work.texture.base;
    uint16_t zb;
    uint8_t texel;
    if (zdest < 0 || zdest + 2 > p8.zlimit || dest < 0 || dest >= p8.climit) return;
    if (tex == NULL || src >= work.texture.size) return;
    memcpy(&zb, p8.zbase + zdest, 2);
    texel = tex[src];
    if ((z & 0xffffu) > zb || texel == 0) return;
    if (lit) {
        if (work.shade_table == NULL) return;
        texel = work.shade_table[((i >> 16) & 0xffu) << 8 | texel]; /* mov ch,byte ptr (tsl.i+2) */
    }
    memcpy(p8.zbase + zdest, &z, 2);
    p8.cbase[dest] = texel;
}

/* The u (shift 0) or v (shift 8) section of a scanline loop: step the
 * numerator by its gradient, then bring it back into 0..q with the
 * denominator den already stepped, moving the texel one at a time. */
static void p8_span_step(uint32_t *num, uint32_t *dnum, uint32_t den, uint32_t dden, int mode, int b, uint32_t *src, int shift)
{
    uint32_t ebx = *num, edi = *dnum, n = 0;
    int neg = b ? p8_lt_diff(ebx, edi) : p8_lt_sum(ebx, edi); /* flags of add_dirn ebx,edi */
    ebx = b ? ebx - edi : ebx + edi;
    if (mode == P8_B) {
        if (neg) {                                   /* deculoop */
            do { p8_texel_step(src, shift, 0); edi += dden; neg = p8_lt_sum(ebx, den); ebx += den; }
            while (neg && ++n < P8_LOOP_LIMIT);
            *dnum = edi;
        } else if (!((int32_t)ebx < (int32_t)den)) { /* inculoop */
            do { p8_texel_step(src, shift, 1); edi -= dden; ebx -= den; }
            while (!((int32_t)ebx < (int32_t)den) && ++n < P8_LOOP_LIMIT);
            *dnum = edi;
        }
    } else if (mode == P8_I) {
        if (!neg) {                                  /* jl_i doneu ; stepuloop, jge_i */
            do { p8_texel_step(src, shift, 1); edi -= dden; neg = p8_lt_diff(ebx, den); ebx -= den; }
            while (!neg && ++n < P8_LOOP_LIMIT);
            *dnum = edi;
        }
    } else if (neg) {                                /* jl_d (jge) doneu ; stepuloop, jge_d (jnc) */
        uint32_t cf;
        do { p8_texel_step(src, shift, 0); edi += dden; cf = (uint32_t)(((uint64_t)ebx + den) >> 32); ebx += den; }
        while (!cf && ++n < P8_LOOP_LIMIT);
        *dnum = edi;
    }
    *num = ebx;
}

/* ScanlineRender_ZPT_I8_D16 / ScanlineRender_ZPTI_I8_D16 size,dirn,udirn,vdirn */
static void p8_scanline(int lit, int b, int umode, int vmode)
{
    uint32_t q = pfps.pq.current, dq = pfps.pq.grad_x, den = q, src = p8.source;
    uint32_t un = pfps.pu.current, dun = pfps.pu.grad_x, vn = pfps.pv.current, dvn = pfps.pv.grad_x;
    uint32_t z = p8_ror16(fps.s_z), i = fps.s_i;
    int32_t dest = p8.start, zdest = p8.zstart;
    if (umode == P8_I) { un -= q; dun -= dq; }       /* move error into the range -1..0 */
    if (vmode == P8_I) { vn -= q; dvn -= dq; }
    for (;;) {
        p8_pixel(lit, dest, zdest, z, i, src);
        dest += b ? -1 : 1;
        zdest += b ? -2 : 2;
        if (b ? dest < p8.end : dest > p8.end) return;
        if (!b) { uint64_t s = (uint64_t)z + p8.dz; z = (uint32_t)s + (uint32_t)(s >> 32); } /* add ; adc 0 */
        else { uint32_t cf = z < p8.dz; z -= p8.dz; z -= cf; }                               /* sub ; sbb 0 */
        i = b ? i - p8.di : i + p8.di;
        den = b ? den - p8.ddenominator : den + p8.ddenominator;
        p8_span_step(&un, &dun, den, p8.ddenominator, umode, b, &src, 0);
        p8_span_step(&vn, &dvn, den, p8.ddenominator, vmode, b, &src, 8);
    }
}

/* The trapezium's "Fix up the error values" loops (uiloop/udloop and
 * viloop/vdloop): bring the scanline's starting numerator into 0..q. */
static void p8_fixup(pfps_param *p, int shift)
{
    uint32_t ebx = pfps.pq.current, ecx = pfps.pq.grad_x, edx = pfps.pq.d_nocarry;
    uint32_t ebp = p->current, edi = p->grad_x, esi = p->d_nocarry, n = 0;
    int neg;
    if (!((int32_t)ebp < (int32_t)ebx)) {
        do { p8_texel_step(&p8.source, shift, 1); edi -= ecx; esi -= edx; ebp -= ebx; }
        while (!((int32_t)ebp < (int32_t)ebx) && ++n < P8_LOOP_LIMIT);
    } else if ((int32_t)ebp < 0) {
        do { p8_texel_step(&p8.source, shift, 0); edi += ecx; esi += edx; neg = p8_lt_sum(ebp, ebx); ebp += ebx; }
        while (neg && ++n < P8_LOOP_LIMIT);
    }
    p->current = ebp;
    p->grad_x = edi;
    p->d_nocarry = esi;
}

/* "Select scanline loop": by the signs of the q, u and v x gradients, and
 * where q falls while u or v rises, by comparing them. jl_dirn, jle_dirn and
 * jg_dirn are the forward jumps for dirn f and the mirrored ones for b. */
static void p8_select(int b, int *um, int *vm)
{
    int32_t esi = (int32_t)pfps.pu.grad_x, ebp = (int32_t)pfps.pq.grad_x, edi = (int32_t)pfps.pv.grad_x;
#define P8_LE0(x) (b ? (x) >= 0 : (x) <= 0)
#define P8_GT0(x) (b ? (x) < 0 : (x) > 0)
#define P8_LE(x, y) (b ? (x) >= (y) : (x) <= (y))
    if (!(b ? ebp > 0 : ebp < 0)) {
        *um = P8_LE0(esi) ? P8_D : P8_I;
        *vm = P8_LE0(edi) ? P8_D : P8_I;
    } else if (!P8_LE0(esi)) {                       /* qd_ui */
        *um = P8_I;
        *vm = P8_GT0(edi) ? P8_I : P8_LE(edi, ebp) ? P8_D : P8_B;
        if (*vm == P8_B) *um = P8_B;
    } else if (!P8_LE(esi, ebp)) {                   /* qd_ud: cmp esi,ebp ; jg_dirn */
        *um = *vm = P8_B;
    } else {
        *um = P8_D;
        *vm = P8_GT0(edi) ? P8_I : P8_LE(edi, ebp) ? P8_D : P8_B;
        if (*vm == P8_B) *um = P8_B;
    }
#undef P8_LE0
#undef P8_GT0
#undef P8_LE
}

/* The per-scanline steps after no_pixels: buffers, the major edge with the
 * carry out of its fraction added into its integer (jc carry ; adc ebp,0),
 * the minor edge, and every parameter by its carry or no-carry step. */
static void p8_next_line(uint32_t *top_i, uint32_t top_d_i, int lit)
{
    uint32_t carry = (uint32_t)(((uint64_t)p8.main_i + p8.main_d_i) >> 32);
    p8.scan += p8.cstride;
    p8.depth += p8.zstride;
    p8.main_i += p8.main_d_i + carry;
    *top_i += top_d_i;
    pfps.pq.current += carry ? pfps.pq.d_carry : pfps.pq.d_nocarry;
    fps.s_z += carry ? fps.d_z_y_1 : fps.d_z_y_0;
    if (lit) fps.s_i += carry ? fps.d_i_y_1 : fps.d_i_y_0;
    pfps.pu.current += pfps.pu.d_nocarry;
    pfps.pv.current += pfps.pv.d_nocarry;
    if (carry) {                                     /* avoids the need to fix up nocarry and carry */
        pfps.pu.current += pfps.pu.grad_x;
        pfps.pv.current += pfps.pv.grad_x;
    }
}

/* TrapeziumRender_ZPT_I8_D16 / TrapeziumRender_ZPTI_I8_D16 n,dirn */
static void p8_trapezium(uint32_t *top_i, uint32_t top_d_i, int32_t *count, int lit, int b)
{
    int32_t ebp, edi;
    int um, vm;
    if (*count < 0) return;
    edi = (int32_t)(*top_i >> 16);                   /* integer part of the end */
    ebp = (int32_t)(p8.main_i & 0xffffu);            /* integer part of the start */
    for (;;) {
        if (!(b ? ebp < edi : ebp > edi)) {          /* cmp ebp,edi ; jg_dirn no_pixels */
            p8.end = p8.scan + edi;
            p8.start = p8.scan + ebp;
            p8.zstart = p8.depth + 2 * ebp;
            p8_fixup(&pfps.pu, 0);
            p8_fixup(&pfps.pv, 8);
            p8_select(b, &um, &vm);
            p8_scanline(lit, b, um, vm);
        }
        p8_next_line(top_i, top_d_i, lit);
        edi = (int32_t)(*top_i >> 16);
        ebp = (int32_t)(p8.main_i & 0xffffu);
        if (--*count < 0) break;
    }
}

static void p8_triangle(union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2, int lit, uint32_t pow2)
{
    int r, b;
    uint32_t m = (1u << pow2) - 1u;
    if (!p8_open()) return;
    p8.pow2 = pow2;
    r = pfps_setup(v0, v1, v2, lit);
    if (r < 0) return;
    if (r > 0) { /* jc TriangleRasterise_ZT(I)_I8_D16_<n> */
        if (pow2 == 8) { if (lit) i8_rasterise_zti_256(); else i8_rasterise_zt_256(); }
        else if (lit) i8_rasterise_zti_pow2(pow2);
        else i8_rasterise_zt_pow2(pow2);
        return;
    }
    p8.scan = p8.cstride * (fps.t_y - 1) - 1;        /* colour.base + stride * (t_y - 1) - 1 */
    p8.depth = p8.zstride * (fps.t_y - 1) - 2;
    p8.main_i = p8_ror16(fps.xm);
    p8.main_d_i = p8_ror16(p8_adjust(fps.d_xm));
    p8.dz = p8_ror16(p8_adjust(fps.d_z_x));
    p8.di = p8_adjust(fps.d_i_x);                    /* ZPTI only: adjusted, not swapped */
    /* (v mod n) * n + u mod n, as each size's instance computes it */
    p8.source = ((pfps.v_current & m) << pow2) | (pfps.u_current & m);
    p8.ddenominator = pfps.pq.grad_x;
    b = pfps.direction != 0;
    p8_trapezium(&fps.x1, fps.d_x1, &fps.topCount, lit, b);
    p8_trapezium(&fps.x2, fps.d_x2, &fps.bottomCount, lit, b);
}

#define P8_KERNEL(name, lit, pow2) void BR_ASM_CALL name(struct brp_block *block, union brp_vertex *v0, \
    union brp_vertex *v1, union brp_vertex *v2) { (void)block; p8_triangle(v0, v1, v2, lit, pow2); }
#define P8_KERNELS(n, pow2) \
    P8_KERNEL(TriangleRender_ZPT_I8_D16_##n, 0, pow2) P8_KERNEL(TriangleRender_ZPTI_I8_D16_##n, 1, pow2)
P8_KERNELS(32, 5)
P8_KERNELS(64, 6)
P8_KERNELS(128, 7)
P8_KERNELS(256, 8)
P8_KERNELS(1024, 10)
