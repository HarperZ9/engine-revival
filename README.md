# engine-revival

Public tooling spine for reviving historical game engines, SDKs, rendering
libraries, CGI toolkits, and studio technology lineages.

This repo publishes public-safe metadata, schemas, validation tools, target
matrices, generated summaries, and evidence packets. It does not publish
proprietary SDKs, leaked source, game assets, private donor files, private
contact data, credentials, restricted media, or upstream source snapshots.

## Current Public Boundary

BRender is the current flagship evidence lane. Engine Revival now preserves the
public BRender Archival v0.1.1 boundary:

- BRender Archival release commit:
  `11b5a8d539e911a9c07991b751402a7d51bf1bde`.
- Release tag: `v0.1.1`.
- PR: `HarperZ9/brender-archival#9`.
- Candidate contents: `bbf3ba2f26ee9ae265759e282dc1454b2234b6be`.
- Upstream public source snapshot:
  `foone/BRender-v1.3.2` at
  `d88d0ed41122664b9781015b517db64353e16f19`.
- Native verification: 21/21 CTest targets under Visual Studio Win32 Debug.
- Release media source run: `brender_core_softrend_render` over `dat/sph32.dat`
  with `final_frame_lit=19284 valid=true`.

The relationship boundary is explicit:

- Retro Engine equals play.
- Engine Revival equals preservation, research, metadata, and evidence.
- BRender Archival equals the verified specific BRender restoration.
- Generic Retro output is never BRender proof.

Engine Revival imports BRender Archival's public-safe receipt, transcript,
media, and provenance. It does not import local experimental branches, private
build trees, upstream source, binaries, or assets.

## Non-Claims

The BRender evidence packet does not claim completed textured TIA output, x64
readiness, production readiness, adoption, endorsement, or vendored upstream
source/assets. The experimental textured TIA path executes in BRender Archival,
but public release notes record black output from a measured
vertex-layout/state mismatch.

## First Workflow

```powershell
python -m pip install -e ".[test]"
engine-revival seed
engine-revival validate
engine-revival audit-public
engine-revival index
engine-revival report
python -m pytest
```

## BRender Evidence

- [BRender archival packet](docs/BRENDER-ARCHIVAL.md)
- [Sanitized 21-target transcript](attempts/transcripts/brender-v132-ctest-twentyone-targets-2026-08-27.log)
- [21-target attempt record](attempts/brender-v132-native-ctest-twentyone-targets-win32.json)
- [Release media provenance](gallery/release-20260827/provenance-manifest.json)
- [Period pipeline still](gallery/release-20260827/period-pipeline-still.png)
- [Period pipeline contact sheet](gallery/release-20260827/period-pipeline-orbit-contact-sheet.png)
- [Social card](gallery/release-20260827/social-card-1200x630.png)

## BRender Harness Metadata

```powershell
engine-revival materialize-brender-harness `
  --source-root C:\path\to\BRender-v1.3.2 `
  --output-root C:\path\to\brender-v132-portable-core-harness
```

This command is retained as public harness metadata and scaffolding. The
verified 21-target restoration boundary is the BRender Archival v0.1.1 release
referenced above; Engine Revival stores the public receipt and provenance.

## Public Docs

- [Revival mission](docs/REVIVAL-MISSION.md)
- [Lost engine directory](docs/DIRECTORY.md)
- [BRender archival packet](docs/BRENDER-ARCHIVAL.md)
- [Public boundary](docs/PUBLIC-BOUNDARY.md)
- [Recovery workflow](docs/RECOVERY-WORKFLOW.md)
- [Remaster lane](docs/REMASTER-LANE.md)
- [Generated public index](docs/generated/index.md)
- [Generated corpus database](docs/generated/database.json)
- [Generated targets](docs/generated/targets.md)
- [Generated sources](docs/generated/sources.md)
- [Generated artifacts](docs/generated/artifacts.md)
- [Generated accessions](docs/generated/accessions.md)
- [Generated tasks](docs/generated/tasks.md)
- [Generated milestones](docs/generated/milestones.md)
- [Generated reproductions](docs/generated/reproductions.md)
- [Generated snapshots](docs/generated/snapshots.md)
- [Generated production readiness](docs/generated/production-readiness.md)
- [Generated build environments](docs/generated/builds.md)
- [Generated harnesses](docs/generated/harnesses.md)
- [Generated attempts](docs/generated/attempts.md)
- [Generated coverage](docs/generated/coverage.md)
- [Generated rights summary](docs/generated/rights-summary.md)
- [Contributing](CONTRIBUTING.md)

---

**[Zentropy Labs](https://github.com/ZentropyLabs-ai)** · order out of entropy.
An independent lab building evidence-first tools that leave a re-checkable
artifact behind. Built by Zain Dana Harper in Seattle. The full workbench is at
[Project Telos](https://harperz9.github.io).
