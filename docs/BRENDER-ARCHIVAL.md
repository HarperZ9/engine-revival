# BRender Archival

BRender (Blazing Render) was Argonaut Software's real-time 3D engine, shipped
through the 1990s and used in titles such as Carmageddon and FX Fighter and in
Microsoft 3D Movie Maker. This packet is the revival of BRender v1.3.2: not a
mirror of the source, but a demonstration that the engine still builds and
renders, reproduced from public material with nothing proprietary vendored in.

## Provenance and rights

- Source: the public `foone/BRender-v1.3.2` snapshot, pinned at commit
  `d88d0ed4`, MIT licensed. Provenance runs through Foone Turing, with the
  open-source release authorized by Argonaut's former CEO Jez San.
- This repository vendors none of that source and none of BRender's assets. The
  materializer generates a build harness that references a public checkout in
  place. Model datafiles are read from the checkout at run time, never copied.

## What the revival delivers

A materializer turns the period makefile topology into an out-of-tree CMake
harness. It builds the FLOAT core library through BRender's own pure-C
memory-pixelmap path, with no dependence on the period 386-assembly software
renderer, and stands up a sixteen-target ladder of self-verifying rungs:

1. Vector math smoke (`BrVector3`, `BrScalar`).
2. Framework startup smoke (`BrBegin` / `BrEnd`).
3. Wireframe render: a projected cube via `BrMatrix4Perspective` into a memory
   pixelmap.
4. Scene-graph render: a model out of the v1db scene database, projected by the
   engine's own `BrActorToScreenMatrix4`.
5. Solid flat-shaded render: a portable C scanline rasterizer with per-face
   lighting.
6. Per-pixel depth buffer: correct occlusion for arbitrary multi-object scenes.
7. Perspective-correct texture mapping.
8. Real datafile models: `BrModelLoad` reads native binary `.dat` models
   (duck, teapot, skull, torus) and renders them solid and depth-buffered.
9. UV-textured render through a model's own vertex UV coordinates.
10. Multi-part assembly: `BrModelLoadMany` composites the 12-part coupe.
11. Gouraud shading with per-vertex normals.
12. Plotter lane: hidden-line-removed SVG polylines, pen-plotter ready.
13. Asset audit: `BrModelLoad` geometry validation (finite vertices, face index
    ranges, degenerate faces, face-material attachment) with one JSON summary
    per model.
14. Pixelmap audit: `BrPixelmapLoad` decode probe over period `.pix` files and
    palette datafiles (`.pal` is a pixelmap datafile; there is no separate
    palette chunk in this BRender version), reporting type, geometry, and
    whether pixels decoded.
15. Material audit: `BrMaterialLoad` over `std.mat`/`winstd.mat` reporting
    identifier, flags, index_base, and colour-map attachment.
16. Pixelmap round trip: the native datafile write path proven end to end
    (`BrPixelmapSave` then reload, type and geometry compared).
17. Material resolution: a `BrMaterialLoad`-loaded material attached to every
    non-degenerate face of a loaded model and rendered through the portable
    rasterizer, proving material-to-face association on the render path.
18. File-texture sampling: perspective-correct UV sampling of a loaded period
    `.pix` through `BrPixelmapPixelGet`, with an externally loaded palette
    attached for indexed variants, and a distinct-colour proof that real
    texture data drove the frame. This closes R4's substance: original
    material/texture files now load, describe themselves, attach, write back,
    and render.
19. Game shell (R5 opened): an explicit INIT/LOAD/RUN/TEARDOWN state machine
    driving a fixed-length deterministic orbit frame loop over loaded assets.
    One numbered PPM per frame plus a JSON manifest. Real display and input
    drivers bind onto this lifecycle later; the loop itself is proven.

Every stage passes under CTest on a Visual Studio Win32 target. The render
captures are generated as a public-safe release artifact.

The four audit and round-trip rungs were grounded in the pinned upstream tree:
loader locations, struct fields, and the 68-file `dat/` inventory were verified
from the `d88d0ed4` snapshot before the code was written.

## Reproduce it

```powershell
python -m pip install -e ".[test]"
engine-revival materialize-brender-harness `
  --source-root C:\path\to\BRender-v1.3.2 `
  --output-root C:\path\to\brender-portable-core-harness
cmake -S <harness> -B <build> -A Win32 "-DBRENDER_SOURCE_DIR=C:\path\to\BRender-v1.3.2"
cmake --build <build> --config Debug --target brender_core_model_smoke
ctest --test-dir <build> -C Debug --output-on-failure
```

The `brender_core_model_smoke` executable takes any `.dat` model path and writes
a PPM, so it doubles as a minimal model viewer for the period asset library.

## What you can do with it today

- Build BRender's core from a public checkout on a modern MSVC toolchain.
- Load and render BRender's own period models straight from their datafiles.
- Audit any period asset before use: models for geometry defects, pixelmaps and
  palettes for decode status, materials for map attachment.
- Write BRender-native datafile pixelmaps via `BrPixelmapSave` (verified round
  trip), the first step of a re-created-asset pipeline.
- Extend the portable rasterizer (Gouraud shading, materials, a wider viewer).
- Plot period models on a pen plotter: `brender_core_plotter_smoke` emits
  hidden-line-removed SVG polylines from any `.dat` model.
- Use the harness as the pattern for reviving other engines in this archive.

## Honestly deferred

These are documented, not claimed, so the revival is not oversold:

- BRender's period 386-assembly `softrend` renderer (the hard portability item).
- x64 pointer-width portability (the unreworked period code is 32-bit bound).
- Full material/texture *resolution* for rendering loaded models: the audit
  rungs now load and describe `.mat`, `.pix`, and `.pal` files, but attaching
  them to rendered models end to end is still open.
- Note: an earlier session recorded 15-bit pixelmap variants as partially decoding; execution against the pinned tree shows `earth15.pix` (type 4) loading completely, so that item is resolved rather than deferred.
- Release packaging and a full interactive viewer.

## Records

The claims above are backed by structured records in this repository:
`readiness/brender-production-readiness.json`,
`harnesses/brender-v132-portable-core-plan.json`,
`attempts/brender-v132-portable-core-*.json`, and
`reproductions/brender-critical-edition-source-build.json`. The generated target
dossier at `docs/generated/targets/brender.md` is the machine-updated view.
