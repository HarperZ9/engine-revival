# BRender Archival

BRender (Blazing Render) was Argonaut Software's real-time 3D engine, shipped
through the 1990s and used in titles such as Carmageddon, FX Fighter, and
Microsoft 3D Movie Maker.

This packet is the Engine Revival evidence view of the public BRender Archival
v0.1.1 release. It is not a source mirror and not a claim that Engine Revival
vendors the restoration. Engine Revival preserves public-safe receipts, media,
and provenance. BRender Archival is the verified specific restoration.

## Provenance and rights

- Upstream source: public `foone/BRender-v1.3.2`, pinned at commit
  `d88d0ed41122664b9781015b517db64353e16f19`, MIT licensed. Provenance runs
  through Foone Turing, with the open-source release authorized by Argonaut's
  former CEO Jez San.
- Specific restoration: public `HarperZ9/brender-archival` release `v0.1.1`,
  merge commit `11b5a8d539e911a9c07991b751402a7d51bf1bde`, PR #9, candidate
  contents `bbf3ba2f26ee9ae265759e282dc1454b2234b6be`.
- This repository vendors none of the BRender source and none of BRender's
  assets. Model, texture, and palette datafiles are read from the public source
  checkout at run time by the restoration harness and are never copied here.

## Verified release boundary

The imported 2026-08-27 native verification result is 21/21 CTest targets
passing under a Visual Studio Win32 Debug boundary: 21 verified CTest rungs,
all passing. The sanitized transcript is committed at
`attempts/transcripts/brender-v132-ctest-twentyone-targets-2026-08-27.log` and
starts with `Test project <build>`.

The target set covers:

| Rung | What it proves |
|---|---|
| Vector math | scalar and vector core |
| Framework startup | `BrBegin` / `BrEnd` |
| Wireframe | `BrMatrix4Perspective` into a memory pixelmap |
| Scene graph | model actors projected through BRender's v1db transforms |
| Solid shaded | portable C scanline rasterizer, per-face lighting |
| Depth buffer | per-pixel occlusion in the portable rung |
| Textured | perspective-correct texture mapping in the portable rung |
| Datafile models | `BrModelLoad` renders real `.dat` models |
| UV-textured models | loaded model UV coordinates drive texture sampling |
| Multi-part assembly | `BrModelLoadMany` composites the 12-part coupe |
| Gouraud shading | per-vertex normals and smooth gradients |
| Plotter lane | hidden-line-removed SVG polylines |
| Asset audit | loaded model geometry, face indices, degenerate faces, and material attachment |
| Pixelmap audit | `BrPixelmapLoad` probes period `.pix` and `.pal` files |
| Material audit | `BrMaterialLoad` identifiers, flags, index_base, and colour-map attachment |
| Pixelmap round trip | `BrPixelmapSave` then reload, with caller-owned workfile protection |
| Material resolve | `BrMaterialLoad` material attached to loaded model faces and rendered |
| File-texture sampling | loaded period `.pix` sampled with palette-aware INDEX_8 lookup |
| Game shell | deterministic INIT/LOAD/RUN/TEARDOWN frame loop over loaded assets |
| Host semantic | host file round trips without deleting caller-owned workfiles |
| Period pipeline | softrend plus pentprim built from upstream, nonblack ZB sphere output |

The period-pipeline media source run drives
`brender_core_softrend_render` over `dat/sph32.dat`, `dat/earth.pix`, and
`dat/std.pal`, emits eight nonblack frames, and records
`final_frame_lit=19284 valid=true`.

## Release media and provenance

Current public media lives under
[`gallery/release-20260827/`](../gallery/release-20260827/). It is generated
from verified nonblack render output or from factual diagrams/cards that cite
the same boundary. It excludes black diagnostic frames and generative imitation.

- [Provenance manifest](../gallery/release-20260827/provenance-manifest.json)
  records sanitized commands, release commit, PR, candidate contents, source
  attribution, source SHA, input/output hashes, dimensions, nonblack metrics,
  and limitations.
- [Period pipeline still](../gallery/release-20260827/period-pipeline-still.png)
  is a lossless PNG from the final verified orbit frame.
- [Period pipeline orbit contact sheet](../gallery/release-20260827/period-pipeline-orbit-contact-sheet.png)
  shows the provenance-pinned eight-frame period-pipeline orbit.
- [Evidence card](../gallery/release-20260827/evidence-card.png),
  [pipeline diagram](../gallery/release-20260827/pipeline-diagram.png), and
  [1200x630 social card](../gallery/release-20260827/social-card-1200x630.png)
  are bounded release assets.

## Relationship boundary

- Retro Engine equals play.
- Engine Revival equals preservation, research, metadata, and evidence.
- BRender Archival equals the verified specific BRender restoration.
- Generic Retro output is never BRender proof.

## Current limitations

These are documented boundaries, not release claims:

- Completed textured TIA output is not claimed. The experimental TIA/PIZ2TIA
  path executes in BRender Archival, but public release notes record black
  output from a measured vertex-layout/state mismatch.
- x64 pointer-width portability is not claimed. The verified native build
  target is Win32.
- Production readiness, adoption, and endorsement are not claimed.
- Assembly-only pentprim kernels outside the exercised RGB_888 ZB and
  experimental TIA path remain linkage stubs.
- The MSVC warning set is warning-only in the verified build, but has not been
  reduced to a zero-warning portability claim.

## Reproduce the release boundary

```powershell
python -m pip install -e ".[test]"
engine-revival materialize-brender-harness `
  --source-root C:\path\to\BRender-v1.3.2 `
  --output-root C:\path\to\brender-portable-core-harness
cmake -S <harness> -B <build> -A Win32 "-DBRENDER_SOURCE_DIR=C:\path\to\BRender-v1.3.2"
cmake --build <build> --config Debug
ctest --test-dir <build> -C Debug --output-on-failure
```

For the 21-target restoration, use the BRender Archival v0.1.1 release boundary
named above. Engine Revival stores the evidence receipt, not a vendored copy of
that build tree.

## Records

The claims above are backed by structured records in this repository:
`readiness/brender-production-readiness.json`,
`harnesses/brender-v132-portable-core-plan.json`,
`tasks/brender-critical-edition-packet.json`,
`attempts/brender-v132-native-ctest-twentyone-targets-win32.json`, and
`reproductions/brender-critical-edition-source-build.json`. The generated target
dossier at `docs/generated/targets/brender.md` is the machine-updated view.
