from __future__ import annotations


def memory_compat_smoke_source() -> str:
    """C source for direct and public-dispatch BRender memory semantics."""
    return r"""/*
 * BRender v1.3.2 portable memory compatibility smoke.
 *
 * Proves only pixel widths 1-4, RGB888 fill, positive-stride rectangular
 * fill, and nonzero-start copy-bits. Host/DOS and wider compatibility are
 * intentionally outside this executable.
 */
#define __BR_V1DB__ 0
#include "brender.h"
#include "pm.h"
#include "pm_ip.h"

#include <stdio.h>
#include <string.h>

#define SENTINEL ((br_uint_8)0x5A)
#define PIXEL_COLOUR 0xA1B2C3D4u
#define FILL_COLOUR 0x00112233u
#define COPY_SOURCE ((br_uint_8)0x2A)
#define COPY_COLOUR ((br_uint_32)0xE7)
#define COPY_START_BIT 2
#define COPY_END_BIT 6

static int fail(int code, const char *label)
{
    fprintf(stderr, "memory-compat:%s\n", label);
    return code;
}

static int check_pixel_widths(void)
{
    static const br_uint_32 expected[] = {
        0x000000D4u,
        0x0000C3D4u,
        0x00B2C3D4u,
        0xA1B2C3D4u
    };
    br_uint_8 buffer[6];
    br_uint_32 bytes;
    br_uint_32 i;

    for (bytes = 1; bytes <= 4; bytes++) {
        memset(buffer, SENTINEL, sizeof(buffer));
        _MemPixelSet((char *)(buffer + 1), 0, bytes, PIXEL_COLOUR);
        if (_MemPixelGet((char *)(buffer + 1), 0, bytes) != expected[bytes - 1]) {
            return fail(10 + (int)bytes, "pixel-value");
        }
        if (buffer[0] != SENTINEL) {
            return fail(15, "pixel-prefix-canary");
        }
        for (i = 1 + bytes; i < (br_uint_32)sizeof(buffer); i++) {
            if (buffer[i] != SENTINEL) {
                return fail(16, "pixel-suffix-canary");
            }
        }
    }
    return 0;
}

static int check_rgb888_fill(void)
{
    static const br_uint_8 expected[] = {
        0x33, 0x22, 0x11, 0x33, 0x22, 0x11
    };
    br_uint_8 buffer[8];

    memset(buffer, SENTINEL, sizeof(buffer));
    _MemFill_A((char *)(buffer + 1), 0, 2, 3, FILL_COLOUR);
    if (memcmp(buffer + 1, expected, sizeof(expected)) != 0) {
        return fail(20, "rgb888-fill-value");
    }
    if (buffer[0] != SENTINEL || buffer[7] != SENTINEL) {
        return fail(21, "rgb888-fill-canary");
    }
    return 0;
}

static int check_positive_stride_rect_fill(void)
{
    static const br_uint_8 expected_row[] = {
        0x33, 0x22, 0x11, 0x33, 0x22, 0x11
    };
    br_uint_8 buffer[16];
    br_uint_8 *row_start;
    br_uint_32 row;

    memset(buffer, SENTINEL, sizeof(buffer));
    _MemRectFill_A((char *)buffer, 0, 2, 2, 8, 3, FILL_COLOUR);
    for (row = 0; row < 2; row++) {
        row_start = buffer + row * 8;
        if (memcmp(row_start, expected_row, sizeof(expected_row)) != 0) {
            return fail(30 + (int)row, "rect-fill-value");
        }
        if (row_start[6] != SENTINEL || row_start[7] != SENTINEL) {
            return fail(32 + (int)row, "rect-fill-padding");
        }
    }
    return 0;
}

static int check_copy_bits_raw(void)
{
    br_uint_8 source[1] = {COPY_SOURCE};
    br_uint_8 destination[8];
    br_uint_8 expected;
    br_uint_32 i;

    memset(destination, SENTINEL, sizeof(destination));
    _MemCopyBits_A(
        (char *)destination, 0, 8, source, 1,
        COPY_START_BIT, COPY_END_BIT, 1, 1, COPY_COLOUR);

    for (i = 0; i < (br_uint_32)sizeof(destination); i++) {
        expected = (i == 2 || i == 4)
            ? (br_uint_8)COPY_COLOUR
            : SENTINEL;
        if (destination[i] != expected) {
            return fail(40, "copy-bits-raw-offset");
        }
    }
    return 0;
}

static int check_copy_bits_public(void)
{
    br_pixelmap *pixelmap = NULL;
    br_uint_8 source[1] = {COPY_SOURCE};
    br_uint_32 expected;
    br_uint_32 x;
    int code = 0;

    if (BrBegin() != BRE_OK) {
        return fail(50, "copy-bits-public-begin");
    }

    pixelmap = BrPixelmapAllocate(
        BR_PMT_INDEX_8, 10, 1, NULL, BR_PMAF_NORMAL);
    if (pixelmap == NULL) {
        code = fail(51, "copy-bits-public-allocate");
    } else {
        BrPixelmapFill(pixelmap, SENTINEL);
        BrPixelmapCopyBits(
            pixelmap, 3, 0, source, 1,
            COPY_START_BIT, COPY_END_BIT, 1, COPY_COLOUR);

        for (x = 0; x < 10; x++) {
            expected = (x == 3 || x == 5)
                ? COPY_COLOUR
                : (br_uint_32)SENTINEL;
            if (BrPixelmapPixelGet(pixelmap, (br_int_32)x, 0) != expected) {
                code = fail(52, "copy-bits-public-offset");
                break;
            }
        }
    }

    if (pixelmap != NULL) {
        BrPixelmapFree(pixelmap);
    }
    if (BrEnd() != BRE_OK && code == 0) {
        return fail(53, "copy-bits-public-end");
    }
    return code;
}

int main(void)
{
    int code;

    code = check_pixel_widths();
    if (code != 0) return code;

    code = check_rgb888_fill();
    if (code != 0) return code;

    code = check_positive_stride_rect_fill();
    if (code != 0) return code;

    code = check_copy_bits_raw();
    if (code != 0) return code;

    return check_copy_bits_public();
}
"""
