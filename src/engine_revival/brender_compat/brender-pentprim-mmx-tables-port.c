/*
 * SPDX-License-Identifier: MIT
 * Portions copyright (c) 1998 Argonaut Software Limited (BRender v1.3.2).
 * Copyright (c) 2026 Zain Dana Harper.
 * See compat/LICENSE-NOTE.md, LICENSE and LICENSES/MIT-BRender.txt.
 */
/*
 * The data of BRender v1.3.2 drivers/pentprim/rastrise.asm (MIT, Argonaut
 * Software 1998) that the MMX kernels read: the colour masks, the span edge
 * masks, the 4x4 dither squares, the texture fraction and U/V masks, and the
 * sixteen screendoor levels. Each qword is the assembly's four words (or two
 * dwords), lowest first.
 */
#define __BR_V1DB__ 1
#include "brender.h"

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "drv.h"
#include "brender-pentprim-mmx-port.h"

const mmx_t mmx_mask_5 = 0xf800f800f800f800ull;
const mmx_t mmx_mask_6 = 0xfc00fc00fc00fc00ull;
const mmx_t mmx_mask_5d = 0x3e003e003e003e00ull;
const mmx_t mmx_mask_6d = 0x3f003f003f003f00ull;
const mmx_t mmx_right_masks[4] = {
    0x000000000000ffffull, 0x00000000ffffffffull, 0x0000ffffffffffffull,
    0xffffffffffffffffull, /* right_masks[3] runs into left_masks[0] */
};
const mmx_t mmx_left_masks[4] = {
    0xffffffffffffffffull, 0xffffffffffff0000ull, 0xffffffff00000000ull, 0xffff000000000000ull,
};
/* 4x4 magic squares for dithered 5.11 and 6.10, a row per scanline */
const mmx_t mmx_dither_table_11[4] = {
    0x07ff04ff037f007full, 0x027f017f06ff05ffull, 0x047f077f00ff03ffull, 0x01ff02ff057f067full,
};
const mmx_t mmx_dither_table_10[4] = {
    0x03ff027f01bf003full, 0x013f00bf037f02ffull, 0x023f03bf007f01ffull, 0x00ff017f02bf033full,
};
/* 4x4 magic squares for dithered 7.9 and 8.8 */
const mmx_t mmx_dither_table_9[4] = {
    0x01ff013f00df001full, 0x009f005f01bf017full, 0x011f01df003f00ffull, 0x007f00bf015f019full,
};
const mmx_t mmx_dither_table_8[4] = {
    0x00ff009f006f000full, 0x004f002f00df00bfull, 0x008f00ef001f007full, 0x003f005f00af00cfull,
};
const mmx_t mmx_fraction_mask = ((mmx_t)((1u << MMX_FRACTION_BITS) - 1) << 32) | ((1u << MMX_FRACTION_BITS) - 1);
const mmx_t mmx_fraction_bit = ((mmx_t)(1u << MMX_FRACTION_BITS) << 32) | (1u << MMX_FRACTION_BITS);
#define UVM(x) ((((mmx_t)((1u << (x)) - 1u) << (MMX_FRACTION_BITS + 1)) & 0xffffffffull) * 0x100000001ull)
const mmx_t mmx_uv_masks[21] = {
    UVM(0), UVM(1), UVM(2), UVM(3), UVM(4), UVM(5), UVM(6), UVM(7), UVM(8), UVM(9), UVM(10),
    UVM(11), UVM(12), UVM(13), UVM(14), UVM(15), UVM(16), UVM(17), UVM(18), UVM(19), UVM(20),
};
/* rastrise.asm's 16 levels of 4x4 screendoor masks, four rows of four words each */
#define SDR(a, b, c, d) ((mmx_t)(a) | (mmx_t)(b) << 16 | (mmx_t)(c) << 32 | (mmx_t)(d) << 48)
#define F 0xffffu
const mmx_t mmx_screendoor_masks[16 * 4] = {
    SDR(0, 0, 0, 0), SDR(0, 0, 0, 0), SDR(0, 0, 0, 0), SDR(0, 0, 0, 0),
    SDR(F, 0, 0, 0), SDR(0, 0, 0, 0), SDR(0, 0, 0, 0), SDR(0, 0, 0, 0),
    SDR(F, 0, 0, 0), SDR(0, 0, 0, 0), SDR(0, 0, F, 0), SDR(0, 0, 0, 0),
    SDR(F, 0, F, 0), SDR(0, 0, 0, 0), SDR(0, 0, F, 0), SDR(0, 0, 0, 0),
    SDR(F, 0, F, 0), SDR(0, 0, 0, 0), SDR(F, 0, F, 0), SDR(0, 0, 0, 0),
    SDR(F, 0, F, 0), SDR(0, F, 0, 0), SDR(F, 0, F, 0), SDR(0, 0, 0, 0),
    SDR(F, 0, F, 0), SDR(0, F, 0, 0), SDR(F, 0, F, 0), SDR(0, 0, 0, F),
    SDR(F, 0, F, 0), SDR(0, F, 0, F), SDR(F, 0, F, 0), SDR(0, 0, 0, F),
    SDR(F, 0, F, 0), SDR(0, F, 0, F), SDR(F, 0, F, 0), SDR(0, F, 0, F),
    SDR(F, F, F, 0), SDR(0, F, 0, F), SDR(F, 0, F, 0), SDR(0, F, 0, F),
    SDR(F, F, F, 0), SDR(0, F, 0, F), SDR(F, 0, F, F), SDR(0, F, 0, F),
    SDR(F, F, F, F), SDR(0, F, 0, F), SDR(F, 0, F, F), SDR(0, F, 0, F),
    SDR(F, F, F, F), SDR(0, F, 0, F), SDR(F, F, F, F), SDR(0, F, 0, F),
    SDR(F, F, F, F), SDR(F, F, 0, F), SDR(F, F, F, F), SDR(0, F, 0, F),
    SDR(F, F, F, F), SDR(F, F, 0, F), SDR(F, F, F, F), SDR(0, F, F, F),
    SDR(F, F, F, F), SDR(F, F, F, F), SDR(F, F, F, F), SDR(0, F, F, F),
};
#undef F
