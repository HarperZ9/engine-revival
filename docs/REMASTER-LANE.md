# Remaster Lane

The revival archive does not stop at building old engines. The end state for
each revived engine is a playable, improved surface: lost games running again,
at higher fidelity than their original hardware allowed where the source
material supports it.

This lane defines how an engine moves from restored to remastered, and what is
honestly claimable at each rung.

## Rungs

| Rung | Name | Claim it earns |
|---|---|---|
| R0 | Dossier | lead recorded; rights posture stated; nothing buildable claimed |
| R1 | Source secured | authorized, license-verified source pinned by commit or archive ID |
| R2 | Build ladder green | reproducible out-of-tree harness; every rung self-verifies (BRender pattern) |
| R3 | Render parity | reference frames match documented original output within stated tolerance |
| R4 | Asset pipeline | original data formats load, validate, and render from redump-legal or recreated assets |
| R5 | Game shell | title flow runs start to finish in the restored engine |
| R6 | Remaster pass | measured improvements: resolution independence, MSAA/anisotropy, float color, widescreen, input latency reductions |
| R7 | Lost-game recovery | an unreleased or platform-lost title is made playable through the restored engine, with provenance for every asset |

## Honesty rules

- A rung claim exists only with its evidence: captured command transcript,
  artifact digests, and tolerance statements. No transcript, no claim.
- Remaster quality claims are measurements against the R3 baseline, never adjectives.
- Rights gate every rung: no rung proceeds past R1 without a stated license or
  rightsholder basis for the specific material in use.
- Assets are never committed to git. The archive stores metadata, digests, and
  public-safe captures only.
- "Better quality where possible" means where the source material and rights
  allow; some engines will honestly stop at R3.
- Retro Engine equals play. Engine Revival equals preservation, research,
  metadata, and evidence. BRender Archival equals the verified specific
  BRender restoration. Generic Retro output is never BRender proof.

## Current lane state

| Engine | Rung | Evidence |
|---|---|---|
| Argonaut BRender v1.3.2 | R2/R4 evidence imported; release boundary is preservation evidence, not a remaster-play claim. | Engine Revival local 12-target portable materializer remains scaffold metadata. External pinned BRender Archival v0.1.1 21-target release commit `11b5a8d539e911a9c07991b751402a7d51bf1bde` supplies the sanitized CTest transcript, readiness score 88, and `gallery/release-20260827/provenance-manifest.json` |
| All other targets | R0 | target + accession records |

## Next candidates

Ranked by rung distance: engines with open or released sources move fastest
(Crystal Space, librw/RenderWare substrate, Aqsis/Pixie). Proprietary-console
SDKs stay dossiers until an authorized basis appears.
