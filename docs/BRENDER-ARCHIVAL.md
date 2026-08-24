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
renderer. The BRender work is an executed twenty-rung ladder, verified end to
end against the pinned v1.3.2 checkout at commit `d88d0ed4`.

The verified boundary is 20 executed CTest rungs under Visual Studio Win32,
backed by structured attempt records in this repository and a committed CTest
transcript at `attempts/transcripts/brender-v132-ctest-twenty-rungs-2026-08-22.log`
(attempt `brender-v132-twenty-rung-execution-win32`).

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
8. Real datafile models: `BrModelLoad` reads native binary `.dat` models and
   renders them solid and depth-buffered.
9. UV-textured render through a model's own vertex UV coordinates.
10. Multi-part assembly: `BrModelLoadMany` composites the 12-part coupe.
11. Gouraud shading with per-vertex normals.
12. Plotter lane: hidden-line-removed SVG polylines, pen-plotter ready.
13. Asset audit: model geometry validation with JSON receipts.
14. Pixelmap audit: `.pix` and palette-datafile decode probing.
15. Material-file audit: binary material save/load round trip plus honest
    text-script probes of period `.mat` files.
16. Pixelmap round trip: native datafile write path via `BrPixelmapSave`
    (count-returning semantics), reload, type and geometry compared.
17. Material resolution: a material attached to every non-degenerate face of a
    loaded model and rendered through the rasterizer.
18. File-texture sampling: perspective-correct UV sampling of a loaded period
    `.pix`, palette attached, distinct-colour proof.
19. Game shell: INIT/LOAD/RUN/TEARDOWN state machine over loaded assets with a
    deterministic frame loop and manifest.
20. Host/memory semantics: allocator pattern retention, inquire behavior, and
    an exact file round trip through BRender's fw layer.

Every stage passes under CTest on a Visual Studio Win32 target: 20/20 passed.
Execution also surfaced defects that generation alone had missed; they are
recorded in the attempt record.
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
- Continue the asset-pipeline evidence work: model, pixelmap, palette, material,
  native-write, material-resolution, and file-texture rungs are implemented but
  still need structured attempt records here before R4 can be claimed.
- Extend the portable rasterizer (Gouraud shading, materials, a wider viewer).
- Plot period models on a pen plotter: `brender_core_plotter_smoke` emits
  hidden-line-removed SVG polylines from any `.dat` model.
- Use the harness as the pattern for reviving other engines in this archive.

## Honestly deferred

These are documented, not claimed, so the revival is not oversold:

- BRender's period 386-assembly `softrend` renderer (the hard portability item).
- x64 pointer-width portability (the unreworked period code is 32-bit bound).
- R4 asset-pipeline closure: structured attempt records, command transcripts,
  artifact digests, and tolerance statements for rungs 13 through 18 are still
  missing from this repository.
- Full material/texture *resolution* for rendering loaded models: the audit
  rungs now load and describe `.mat`, `.pix`, and `.pal` files, but attaching
  them to rendered models end to end is still open.
- Fixing the partial decode of 15-bit pixelmap variants.
- Release packaging and a full interactive viewer.

## Records

The claims above are backed by structured records in this repository:
`readiness/brender-production-readiness.json`,
`harnesses/brender-v132-portable-core-plan.json`,
`attempts/brender-v132-portable-core-*.json`, and
`reproductions/brender-critical-edition-source-build.json`. The generated target
dossier at `docs/generated/targets/brender.md` is the machine-updated view.
