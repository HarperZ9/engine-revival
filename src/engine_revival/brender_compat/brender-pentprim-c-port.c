/*
 * SPDX-License-Identifier: MIT
 * Portions copyright (c) 1998 Argonaut Software Limited (BRender v1.3.2).
 * Copyright (c) 2026 Zain Dana Harper.
 * See compat/LICENSE-NOTE.md, LICENSE and LICENSES/MIT-BRender.txt.
 */
/*
 * Pentprim C port: DIAGNOSTIC build - every stub prints when called.
 * This traces which rasterizer functions fire during BrZbSceneRender.
 */
#define __BR_V1DB__ 1
#include "brender.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "drv.h"

#if defined(_DEBUG)
#include <crtdbg.h>
#endif

/* ---- real-semantics helpers ---- */

int _sar16(int v)
{
    return v >> 16;
}

#undef BrFixedDiv
br_int_32 BrFixedDiv(br_int_32 a, br_int_32 b)
{
    if (b == 0) return (a >= 0) ? 0x7fffffff : -0x7fffffff;
    return (br_int_32)(((long long)a << 16) / b);
}

/* SafeFixedMac2Div(a,b,c,d,e) = (a*b + c*d) / e, from safediv.asm: a 64-bit
 * sum, an unsigned divide of the magnitudes, the sign restored afterwards,
 * and 0 when e is 0 or the quotient would not fit in 32 bits. */
br_int_32 SafeFixedMac2Div(br_int_32 a, br_int_32 b, br_int_32 c, br_int_32 d, br_int_32 e)
{
    long long num = (long long)a * b + (long long)c * d;
    unsigned long long m = num < 0 ? 0ull - (unsigned long long)num : (unsigned long long)num;
    unsigned int sign = (unsigned int)e ^ (unsigned int)((unsigned long long)num >> 32);
    unsigned int divisor = e < 0 ? 0u - (unsigned int)e : (unsigned int)e;
    unsigned int q;
    if (divisor == 0 || (unsigned int)(m >> 32) >= divisor) return 0;
    q = (unsigned int)(m / divisor);
    return (br_int_32)((sign & 0x80000000u) ? 0u - q : q);
}

/* RasteriseBufferDisable and the rest of rastbuff.asm: brender-pentprim-mmx-setup-port.c */

/* ---- diagnostic stubs: print name when called ---- */

void BR_ASM_CALL TrapezoidRenderPIZ2TA(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2, br_uint_16 (*fp_vertices)[3]) { (void)block; (void)v0; (void)v1; (void)v2; (void)fp_vertices; }
void BR_ASM_CALL TrapezoidRenderPIZ2TIA(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2, br_uint_16 (*fp_vertices)[3]) { (void)block; (void)v0; (void)v1; (void)v2; (void)fp_vertices; }
void BR_ASM_CALL TrapezoidRenderPIZ2TAN(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2, br_uint_16 (*fp_vertices)[3]) { (void)block; (void)v0; (void)v1; (void)v2; (void)fp_vertices; }

void BR_ASM_CALL TrapezoidRenderPITA(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2, br_uint_16 (*fp_vertices)[3])
{
    fprintf(stderr, "CALL TrapezoidRenderPITA\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2; (void)fp_vertices;
}

void BR_ASM_CALL TrapezoidRenderPITAN(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2, br_uint_16 (*fp_vertices)[3])
{
    fprintf(stderr, "CALL TrapezoidRenderPITAN\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2; (void)fp_vertices;
}

void BR_ASM_CALL TrapezoidRenderPITIA(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2, br_uint_16 (*fp_vertices)[3])
{
    fprintf(stderr, "CALL TrapezoidRenderPITIA\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2; (void)fp_vertices;
}















void BR_ASM_CALL TriangleRenderPIZ2TPD1024(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRenderPIZ2TPD1024\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRenderPIZ2TPD128(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRenderPIZ2TPD128\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRenderPIZ2TPD256(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRenderPIZ2TPD256\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRenderPIZ2TPD64(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRenderPIZ2TPD64\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}



/* TriangleRenderPIZ2(I)_RGB_888, _555 and _565: brender-pentprim-tt-port.c. */

void BR_ASM_CALL TriangleRender_I8(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_I8\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_I_I8(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_I_I8\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_PTI_I8_1024(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_PTI_I8_1024\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_PTI_I8_1024_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_PTI_I8_1024_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_PTI_I8_128(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_PTI_I8_128\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_PTI_I8_128_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_PTI_I8_128_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_PTI_I8_256(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_PTI_I8_256\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_PTI_I8_256_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_PTI_I8_256_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_PTI_I8_64(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_PTI_I8_64\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_PTI_I8_64_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_PTI_I8_64_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_PT_I8_1024(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_PT_I8_1024\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_PT_I8_128(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_PT_I8_128\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_PT_I8_256(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_PT_I8_256\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_PT_I8_32(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_PT_I8_32\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_PT_I8_64(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_PT_I8_64\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_TID_I8(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_TID_I8\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_TID_I8_256(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_TID_I8_256\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_TID_I8_256_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_TID_I8_256_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_TID_I8_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_TID_I8_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_TI_I8(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_TI_I8\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_TI_I8_256(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_TI_I8_256\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_TI_I8_256_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_TI_I8_256_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_TI_I8_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_TI_I8_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_T_I8(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_T_I8\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_T_I8_1024(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_T_I8_1024\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_T_I8_128(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_T_I8_128\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_T_I8_256(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_T_I8_256\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_T_I8_32(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_T_I8_32\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_T_I8_64(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_T_I8_64\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZIF_I8_D16(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZIF_I8_D16\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZIF_I8_D16_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZIF_I8_D16_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZI_I8_D16_ShadeTable(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZI_I8_D16_ShadeTable\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTB_I8_D16_1024(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTB_I8_D16_1024\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTB_I8_D16_128(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTB_I8_D16_128\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTB_I8_D16_256(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTB_I8_D16_256\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTB_I8_D16_32(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTB_I8_D16_32\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTB_I8_D16_64(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTB_I8_D16_64\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTFB_I8_D16_1024(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTFB_I8_D16_1024\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTFB_I8_D16_128(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTFB_I8_D16_128\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTFB_I8_D16_256(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTFB_I8_D16_256\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTFB_I8_D16_32(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTFB_I8_D16_32\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTFB_I8_D16_64(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTFB_I8_D16_64\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTF_I8_D16_1024(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTF_I8_D16_1024\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTF_I8_D16_128(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTF_I8_D16_128\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTF_I8_D16_256(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTF_I8_D16_256\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTF_I8_D16_32(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTF_I8_D16_32\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTF_I8_D16_64(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTF_I8_D16_64\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTIB_I8_D16_1024(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTIB_I8_D16_1024\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTIB_I8_D16_1024_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTIB_I8_D16_1024_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTIB_I8_D16_128(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTIB_I8_D16_128\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTIB_I8_D16_128_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTIB_I8_D16_128_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTIB_I8_D16_256(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTIB_I8_D16_256\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTIB_I8_D16_256_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTIB_I8_D16_256_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTIB_I8_D16_32(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTIB_I8_D16_32\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTIB_I8_D16_32_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTIB_I8_D16_32_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTIB_I8_D16_64(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTIB_I8_D16_64\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTIB_I8_D16_64_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTIB_I8_D16_64_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTIFB_I8_D16_1024(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTIFB_I8_D16_1024\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTIFB_I8_D16_1024_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTIFB_I8_D16_1024_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTIFB_I8_D16_128(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTIFB_I8_D16_128\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTIFB_I8_D16_128_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTIFB_I8_D16_128_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTIFB_I8_D16_256(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTIFB_I8_D16_256\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTIFB_I8_D16_256_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTIFB_I8_D16_256_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTIFB_I8_D16_32(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTIFB_I8_D16_32\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTIFB_I8_D16_32_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTIFB_I8_D16_32_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTIFB_I8_D16_64(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTIFB_I8_D16_64\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTIFB_I8_D16_64_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTIFB_I8_D16_64_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTIF_I8_D16_1024(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTIF_I8_D16_1024\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTIF_I8_D16_1024_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTIF_I8_D16_1024_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTIF_I8_D16_128(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTIF_I8_D16_128\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTIF_I8_D16_128_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTIF_I8_D16_128_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTIF_I8_D16_256(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTIF_I8_D16_256\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTIF_I8_D16_256_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTIF_I8_D16_256_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTIF_I8_D16_32(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTIF_I8_D16_32\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTIF_I8_D16_32_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTIF_I8_D16_32_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTIF_I8_D16_64(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTIF_I8_D16_64\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTIF_I8_D16_64_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTIF_I8_D16_64_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTI_I8_D16_1024(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTI_I8_D16_1024\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTI_I8_D16_1024_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTI_I8_D16_1024_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTI_I8_D16_128(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTI_I8_D16_128\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTI_I8_D16_128_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTI_I8_D16_128_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTI_I8_D16_256_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTI_I8_D16_256_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTI_I8_D16_32(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTI_I8_D16_32\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTI_I8_D16_32_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTI_I8_D16_32_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTI_I8_D16_64(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTI_I8_D16_64\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPTI_I8_D16_64_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPTI_I8_D16_64_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPT_I8_D16_1024(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPT_I8_D16_1024\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPT_I8_D16_128(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPT_I8_D16_128\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPT_I8_D16_32(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPT_I8_D16_32\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZPT_I8_D16_64(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZPT_I8_D16_64\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTB_I8_D16(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTB_I8_D16\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTB_I8_D16_1024(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTB_I8_D16_1024\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTB_I8_D16_1024_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTB_I8_D16_1024_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTB_I8_D16_128(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTB_I8_D16_128\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTB_I8_D16_128_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTB_I8_D16_128_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTB_I8_D16_16(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTB_I8_D16_16\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTB_I8_D16_16_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTB_I8_D16_16_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTB_I8_D16_256(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTB_I8_D16_256\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTB_I8_D16_256_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTB_I8_D16_256_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTB_I8_D16_32(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTB_I8_D16_32\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTB_I8_D16_32_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTB_I8_D16_32_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTB_I8_D16_64(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTB_I8_D16_64\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTB_I8_D16_64_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTB_I8_D16_64_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTB_I8_D16_8(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTB_I8_D16_8\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTB_I8_D16_8_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTB_I8_D16_8_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTFB_I8_D16(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTFB_I8_D16\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTF_I8_D16(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTF_I8_D16\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTF_I8_D16_256(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTF_I8_D16_256\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTIB_I8_D16(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTIB_I8_D16\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTIB_I8_D16_1024(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTIB_I8_D16_1024\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTIB_I8_D16_128(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTIB_I8_D16_128\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTIB_I8_D16_16(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTIB_I8_D16_16\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTIB_I8_D16_256(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTIB_I8_D16_256\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTIB_I8_D16_32(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTIB_I8_D16_32\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTIB_I8_D16_64(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTIB_I8_D16_64\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTIB_I8_D16_8(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTIB_I8_D16_8\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTIB_I8_D16_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTIB_I8_D16_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTID_I8_D16(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTID_I8_D16\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTID_I8_D16_1024(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTID_I8_D16_1024\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTID_I8_D16_1024_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTID_I8_D16_1024_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTID_I8_D16_128(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTID_I8_D16_128\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTID_I8_D16_128_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTID_I8_D16_128_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTID_I8_D16_256(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTID_I8_D16_256\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTID_I8_D16_256_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTID_I8_D16_256_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTID_I8_D16_32(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTID_I8_D16_32\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTID_I8_D16_32_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTID_I8_D16_32_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTID_I8_D16_64(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTID_I8_D16_64\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTID_I8_D16_64_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTID_I8_D16_64_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTID_I8_D16_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTID_I8_D16_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTIFB_I8_D16(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTIFB_I8_D16\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTIFB_I8_D16_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTIFB_I8_D16_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTIF_I8_D16(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTIF_I8_D16\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTIF_I8_D16_256(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTIF_I8_D16_256\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTIF_I8_D16_256_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTIF_I8_D16_256_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTIF_I8_D16_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTIF_I8_D16_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTI_I8_D16(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTI_I8_D16\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTI_I8_D16_1024(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTI_I8_D16_1024\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTI_I8_D16_128(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTI_I8_D16_128\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTI_I8_D16_16(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTI_I8_D16_16\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTI_I8_D16_32(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTI_I8_D16_32\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTI_I8_D16_64(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTI_I8_D16_64\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTI_I8_D16_8(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTI_I8_D16_8\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZTI_I8_D16_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZTI_I8_D16_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZT_I8_D16(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZT_I8_D16\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZT_I8_D16_1024(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZT_I8_D16_1024\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZT_I8_D16_1024_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZT_I8_D16_1024_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZT_I8_D16_128(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZT_I8_D16_128\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZT_I8_D16_128_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZT_I8_D16_128_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZT_I8_D16_16(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZT_I8_D16_16\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZT_I8_D16_16_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZT_I8_D16_16_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZT_I8_D16_256_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZT_I8_D16_256_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZT_I8_D16_32(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZT_I8_D16_32\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZT_I8_D16_32_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZT_I8_D16_32_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZT_I8_D16_64(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZT_I8_D16_64\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZT_I8_D16_64_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZT_I8_D16_64_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZT_I8_D16_8(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZT_I8_D16_8\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_ZT_I8_D16_8_FLAT(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_ZT_I8_D16_8_FLAT\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void BR_ASM_CALL TriangleRender_Z_I8_D16_ShadeTable(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2)
{
    fprintf(stderr, "CALL TriangleRender_Z_I8_D16_ShadeTable\n"); fflush(stderr);
    (void)block; (void)v0; (void)v1; (void)v2;
}

void sar16(void)
{
    fprintf(stderr, "CALL sar16\n"); fflush(stderr);
    
}

/* TrapezoidRenderPIZ2TA24, TIA_RGB_888, TA15 and TIA_RGB_555 (t_piza.asm):
 * brender-pentprim-awsl-port.c. */
