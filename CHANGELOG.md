# Changelog

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
