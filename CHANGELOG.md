# Changelog

## Unreleased

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
