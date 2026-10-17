# Changelog

## 0.9.0

The period BRender 1.3.2 build can now draw 15 and 16-bit triangles the way it does on a Pentium with MMX, through pentprim's MMX table. That is the 1998 path BRender 1.4 reproduces, so the two can now be compared like for like, textured included.

- The materialized harness has a new CMake option, `BRENDER_PENTPRIM_MMX` (off by default). With it on, pentprim is built with `USE_MMX=1` and the host stub reports `HOST_CAPS_MMX`, so `plib.c` turns on the rasterise buffer and `match.c` matches 15 and 16-bit triangles against `mmx_t15f.c` and `mmx_t16f.c` instead of the general tables, as the 1998 code does when CPUID reports MMX. Without the option nothing changes: the renderer rung and every existing build keep the general tables.
- The MMX family is ported line by line from the assembly, in nine new compat files. `brender-pentprim-mmx-port.h` emulates each MMX instruction the kernels use on 64-bit values (`paddw`, `psubusw`, `pmulhw`, `pmaddwd`, `packssdw`, `packsswb`, the unpacks and the shifts), with the wrapping and saturation of the real instruction. `brender-pentprim-mmx-setup-port.c` ports `gsetuptf.asm`'s `GenericSetupTriangleFloat_A` and its eight parameter setups (the float setup is `fpsetup.asm`'s, reused, with the major edge truncated to the 4-pixel stride) and `rastbuff.asm`'s rasterise buffer: a 1600-byte stack of parameter blocks drawn newest first when it fills or at `RendererFlush`, with each rasteriser's work structure laid over its block as in `rastparm.h`. `brender-pentprim-mmx-tables-port.c` holds `rastrise.asm`'s tables, `-common-port.c` `unpack.inc`'s parameter unpacking and the scanline walk and Z pass every kernel repeats, and `-uv-port.c` the U and V packing and texel fetch of the three textured kernels. `-zrgb`, `-zuv`, `-zuvc` and `-zuvrgb-port.c` port `xzrgb.inc`, `xzuv1.inc`, `xzuvc.inc` and `xzuvrgb.inc`: all 64 entry points, RGB_555 and RGB_565, plain, dithered, screendoor and both, left to right and right to left.
- Kept as found: the 16-bit dithered textured kernel does not mask green in a span of two or more words (`xzuv1.inc` tests `COLOUR_TYPE_15` twice), so green's low bits reach blue there. BlazingRenderer recorded this first, in BRender 1.4's `drivers/softprim/raster.cpp` (`MmxPackDirectDither`), and 1.4 reproduces it too.
- Checked against the 1998 assembly itself: `gsetuptf.asm`, `rastbuff.asm`, `rastrise.asm`, `setupdat.asm` and the eight `xz*.asm` files, assembled with MASM against the harness's own structure offsets and linked in place of the port, draw all 61 of brender-archival's parity scenes byte for byte as the port does. A differential test of 240,866 random triangles, in batches that exercise the buffer's order and its flush, finds no difference over all 32 kernels in both formats. One-instruction changes to the assembly (one per kernel file and one in the setup) make the random test fail in exactly the kernels they touch.
- Fed the vertices BRender 1.4's own MMX triangles receive, the port reproduces 1.4's frames byte for byte on all 34 of brender-archival's 15/16-bit scenes. The remaining modern-against-period difference on those scenes therefore comes from the vertices, not from the rasterisers.
- `RasteriseBufferDisable` moved from `brender-pentprim-c-port.c` to the setup port. The materializer writes 49 files (nine new compat files); the ladder stays at 21 targets.

## 0.8.0

The period BRender 1.3.2 build now draws into 15 and 16-bit buffers (RGB_555 and RGB_565), untextured and textured.

- The untextured z-buffered kernels of `tt15_piz.asm` and `tt16_piz.asm` are ported line by line: `TriangleRenderPIZ2_RGB_555` / `_565` (constant colour) and `TriangleRenderPIZ2I_RGB_555` / `_565` (interpolated colour). They share setup and trapezoid loops with `tt24_piz.asm`, so the 24-bit port moved out of the main file into `brender_compat/brender-pentprim-tt-port.c` and all six kernels now live there. The three files differ in the pixel size, in how a colour becomes a pixel (the 555 and 565 masks and shifts as the assembly builds them) and in `GDIVIDE`: on overflow `tt15_piz.asm` and `tt16_piz.asm` give 0 where `tt24_piz.asm` keeps the low dword of the numerator. The scanline ends are now compared as the assembly compares them, unsigned, after scaling the pixel offsets to bytes.
- The unlit power-of-two 16-bit texture kernels of `zb16p2ul.asm`, `TriangleRender_ZT_RGB565_D16_32`, `_64`, `_128`, `_256` and `_1024`, are ported in `brender-pentprim-zb16-port.c`, with `fpsetup.asm`'s `TriangleSetup_ZT` from the 0.6.0 setup port. `prm_t15f.c` lists them for RGB_555 under the 565 name: they copy 16-bit texels and never look inside them, so they serve both tables. Kept as found: the z step is compared with the decimal constant 80000000, as in `zb8.asm`.
- `t_piza.asm`'s two 16-bit instances are ported: `TrapezoidRenderPIZ2TA15` (a 16-bit map of any width) and `TrapezoidRenderPIZ2TIA_RGB_555` (an 8-bit map lit through a 16-bit shade table), which `prm_t16f.c` also uses for RGB_565. The trapezoid port moved to `brender-pentprim-awsl-port.c` with the 24-bit instances. The DBPP 2 branch reads its shade table at `[edx+ecx*2]`, a 2-byte entry, which matches the table: the 24-bit branch's stride defect has no 16-bit counterpart.
- Eleven diagnostic stubs are gone. The z-sorted 15/16-bit kernels, the perspective `TIP256` and `TIPB256` mappers and the MMX family (`mmx_t15f.c`, `mmx_t16f.c`) are not ported; the harness builds without `USE_MMX`, so pentprim uses its general tables.
- Checked against the 1998 assembly itself: `tt15_piz.asm`, `tt16_piz.asm`, `tt24_piz.asm`, `t_piza.asm`, `zb16p2ul.asm` and `fpsetup.asm`, assembled with MASM against the harness's own structure offsets and linked in place of the ports, draw all 47 of brender-archival's parity scenes byte for byte as the ports do, and a differential test of 72,000 random triangles over the fifteen kernels finds no difference. Four one-instruction changes to the assembly change exactly the scenes that use those instructions, and make the random test fail, so both comparisons can fail.
- The materializer writes 40 files (three new compat files); the ladder stays at 21 targets.
- Measured with brender-archival's parity scenes (17 October 2026, Win32 Debug period): the v0.7.0 stubs draw nothing in the twenty 15/16-bit scenes, the ports draw them, two period runs are byte-identical, and the 27 older scenes are byte-identical to 0.7.0. BRender 1.4 draws the same untextured scenes with coverage IoU 0.9869 or better but does not texture them: for 15/16-bit output it reproduces pentprim's MMX path, which the 1998 matcher uses instead of the general tables when the processor has MMX, and whose untextured rows take any map that is not 8-bit with a palette. The two sides share no 15/16-bit kernel, so brender-archival records the comparison and gates only its untextured half.

## 0.7.0

The period BRender 1.3.2 build now draws perspective-correct textures in 8-bit colour.

- The two 256 x 256 perspective kernels of `fti8pizp.asm` are ported line by line: `TriangleRender_ZPT_I8_D16_256` (unlit) and `TriangleRender_ZPTI_I8_D16_256` (lit through the shade table), each with its forward and backward trapezium and the five scanline loops the assembly picks between by the signs of the q, u and v gradients. They live in `brender_compat/brender-pentprim-i8p-port.c`. Their diagnostic stubs are gone. The other map sizes and the constant-intensity `_FLAT` variant stay stubs.
- The setup they call, `pfpsetup.asm`'s `TriangleSetup_ZPT` and `TriangleSetup_ZPTI`, is ported in `brender-pentprim-pfpsetup-port.c`: the perspective cheat test (w range times screen range against four times the smallest w, compared as the bits of two singles), the base texel by `fistp`, q' as the product of the other two w, the `maxuv` normalisation rounded up to a power of two from the float's bits, and the u, v and q parameters converted with 1.5 x 2^52. `pfpsetup.asm`'s `SETUP_FLOAT` and `SETUP_FLOAT_PARAM` do the same double operations as `fpsetup.asm`'s, so the 0.6.0 port of those is reused; it now also records the sorted vertices and the sort parity, which the perspective setup needs.
- In the cheat case the 1998 setup returns with carry set and the kernel jumps into the linear rasteriser. The ZT and ZTI ports are split at that label (`i8_rasterise_zt_256`, `i8_rasterise_zti_256`) so the perspective kernels can do the same.
- Kept as found: the per-pixel u and v loops test the exact sign of each add or subtract, as the x86 flags do, and the decreasing loop ends on the carry (`jnc`). The texel offset wraps u and v at 256 through `inc al` and `inc ah`.
- Checked against the 1998 assembly itself: `pfpsetup.asm` and `fti8pizp.asm`, assembled with MASM against the harness's own structure offsets and linked in place of the port, draw all 27 of brender-archival's parity scenes byte for byte as the port does. Changing one instruction in the assembly (v stepping) changes exactly the two unlit scenes with perspective triangles, so the comparison can fail.
- The materializer writes 37 files (three new compat files); the ladder stays at 21 targets.
- Measured with brender-archival's parity scenes (17 October 2026, Win32 Debug period against BRender 1.4 on Windows): the v0.6.0 stubs draw nothing in the six perspective scenes, the port draws them, and two period runs are byte-identical. Against BRender 1.4, coverage IoU is 0.9983 or better, 92.6% to 97.9% of pixels carry the same index and MAD is at most 1.62. The lowest scores are on the steep plane, and the same plane drawn without perspective scores the same (92.7%), so that difference is not in the perspective kernels: the vertices the two softrend versions hand the rasteriser differ by up to 0.03 pixels.

## 0.6.0

The period BRender 1.3.2 build now draws in 8-bit colour, through BRender's own palette path.

- Four INDEX_8 kernels with a 16-bit depth buffer are ported line by line: `TriangleRender_Z_I8_D16` (flat colour index) and `TriangleRender_ZTI_I8_D16_256` (256 x 256 map lit through the shade table) from `zb8.asm`, and `TriangleRender_ZI_I8_D16` (Gouraud index) and `TriangleRender_ZT_I8_D16_256` (256 x 256 map, unlit) from `fti8_piz.asm`. The diagnostic stubs for them are gone. They live in `brender_compat/brender-pentprim-i8-port.c`.
- The floating point triangle setup they call, `fpsetup.asm`'s `SETUP_FLOAT`, `SETUP_FLOAT_PARAM` and `SETUP_FLOAT_COLOUR`, is ported in `brender-pentprim-fpsetup-port.c`: the same double operations in the same order, single precision where the assembly stores a dword, the 1.5 x 2^36 conversion to 16.16 fixed point, the integer y taken from the float's bits, and the overlapping sort tables read as one array.
- Kept as found: `zb8.asm` compares the z step with the decimal constant 80000000 where `fti8_piz.asm` uses 80000000h, so the flat kernel takes one off a large positive step as well as a negative one.
- The materializer writes 34 files (three new compat files); the ladder stays at 21 targets.
- Measured with brender-archival's parity scenes (16 October 2026): nine 8-bit scenes against BRender 1.4, compared by palette index. Coverage IoU is 0.9983 or better, 96.4% to 99.9% of pixels carry the same index, and MAD is at most 0.43 levels. Two period runs are byte-identical.

## 0.5.0

The period BRender 1.3.2 build now draws textured scenes.

- Argonaut's C triangle setup for the arbitrary-width texture mappers (`drivers/pentprim/awtmz.c` with `awtmi.h`) is compiled into the harness. It was left out before, and the port supplied an approximation of one of its wrappers and empty stand-ins for three more.
- `TrapezoidRenderPIZ2TA24` (RGB_888 texture into RGB_888) and `TrapezoidRenderPIZ2TIA_RGB_888` (8-bit texture lit through the shade table) are a line-by-line C port of the `TriangleRenderZ2` macro in `t_piza.asm`, with its wrapping, its transparent zero texel and its depth test (draw where z is at most the stored value). The other trapezoid loops stay stubs.
- `SafeFixedMac2Div` now computes what `safediv.asm` computes, (a*b + c*d) / e with 0 on overflow. The port had (a*b << 16) / (c*d).
- The renderer rung expands `earth.pix` through `std.pal` into an RGB_888 map, registers it with `BrMapAdd` and `BrMapUpdate` before the material is stored, and now requires a textured frame: more than 500 coloured pixels as well as more than 500 lit ones. On Win32 Debug it reports 31,503 lit and 16,436 coloured pixels, with the same bytes on every run.
- Measured with brender-archival's parity scenes (16 October 2026): three textured scenes. Against BRender 1.4 coverage IoU is 0.9992 or better, 95.7% to 99.7% of pixels are identical, and MAD is at most 1.09. Two period runs are byte-identical.

## 0.4.0

The period BRender 1.3.2 build now draws its flat and Gouraud scenes the way the 1998 kernels do, and deterministically.

- `brender-pentprim-c-port.c`: `TriangleRenderPIZ2_RGB_888` (constant colour) and `TriangleRenderPIZ2I_RGB_888` (interpolated colour) are rewritten as a line-by-line C port of `RawTriangle_PIZ2_RGB_888` and `RawTriangle_PIZ2I_RGB_888` in BRender's `tt24_piz.asm`. The port keeps the assembly's fixed-point setup, its edge stepping with carry, its truncating divides and its depth test (draw where z is below the stored value, with far = 0xFFFF).
- Root cause of the black and broken period renders: the port redefined `C_SX`, `C_SY` and the other component slots as 0 to 8 with `#ifndef`. Those names are enum constants in `priminfo.h` (`C_SX` is 5), so the macros shadowed them and the kernels written after them read the wrong slots: the flat kernel's screen x came out as zero, and its colour came from the wrong place. The Gouraud kernel also interpolated the intensity slot as grey and shifted it 8 bits instead of 16, which saturated to white. Its depth convention was inverted.
- The renderer rung clears depth to far (0xFFFFFFFF) and returns its verdict as the exit status. Before, `main` returned 0 whatever its receipt said, so CTest passed an invalid render. With this release it renders a lit silhouette on every orbit frame (31,509 pixels in the last frame on Win32 Debug). The rung's material carries a texture, but the frames show none, so the textured path is still open.
- Measured with brender-archival's parity scenes on 16 October 2026: two period runs give identical bytes. Against BRender 1.4's software rasteriser, coverage IoU is 0.9993 or better on all six scenes, 90.7% to 100% of pixels are identical, and the mean absolute difference is under 0.2 levels.

### Catalogue

- Six engines join the catalogue, each from a source release by its rights holder under an open licence, with the licence read on 2026-10-16: Microsoft 3D Movie Maker (MIT, 2022, built on BRender), Serious Engine 1 (GPL-2.0, Croteam 2016) and Catacomb 3-D (GPL-2.0, 2014) as revival candidates; the Marathon 2 engine (Aleph One, GPL), id Tech 4 (Doom 3, GPL-3.0 with additional terms) and Torque 3D (MIT, GarageGames 2012) as maintained elsewhere. Eight sources, six tasks and six milestones come with them.
- `docs/DIRECTORY.md` lists the three maintained projects and the three candidates, and names the engines checked and left out because their source notices bar commercial use or are not open licences (Build, Descent, FreeSpace 2, Homeworld, Glide, the id Wolfenstein 3D repository), or because no primary licence source was found (Genesis3D, Allegiance).
- BRender's page links brview 0.1.2 and The Keeper's Hour 0.3.2, now for Windows and Linux, and relates BRender to 3D Movie Maker.
- Every count on the front-page drawings and in the README moves to the 35-target corpus.

## 0.3.0

- engine-revival is the single owner of the `engine_revival` package. The BRender harness from brender-archival is folded in: the materializer now writes the full 21-target ladder (31 files) instead of 12.
- The BRender C ports ship inside the package as `engine_revival/brender_compat/` (MIT, with the Argonaut 1998 notice beside them). They were read from a path outside the package before, which only worked from a source checkout.
- The record schemas ship inside the package as `engine_revival/schemas/`, so `validate` works from a regular install.
- CI installs the package without `-e` for the records and native jobs, and the native job expects 21 rungs.
- BRender records gain a dated boundary for the 21-target materializer; the 27 August 2026 boundaries stay as the record of that date.
- Package licence expression is `FSL-1.1-MIT AND MIT`.

## 0.2.0

- From v0.2.0, code is licensed FSL-1.1-MIT. Earlier releases remain under MIT.
- `LICENSE` is the FSL-1.1-MIT text from fsl.software, with licensor Zain Dana Harper and copyright 2026.
- Imported BRender Archival release media and the sanitized transcript keep their AGPL-3.0-or-later treatment (THIRD_PARTY_NOTICES.md). The upstream BRender source is still referenced and never vendored.
- `gallery/release-20260827/provenance-manifest.json` and the script that regenerates it still record `engine_revival_code_license: MIT`. That record describes the 27 August 2026 release, which was MIT, so it is left as written.
- `pyproject.toml` declares `license = "FSL-1.1-MIT"` (PEP 639, setuptools 77 or later) and version 0.2.0. No code behaviour changed.
- v0.1.0 stays MIT.

## 0.1.0

Engine Revival release of 27 August 2026, under MIT.
