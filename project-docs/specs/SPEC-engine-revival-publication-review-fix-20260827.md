# Spec: Engine Revival publication review fix, 2026-08-27

## Objective

Fix the independent-review warnings on the BRender publication candidate without
changing the implementation boundary or publishing anything. The patch must keep
Engine Revival self-contained while making the 12-target local materializer and
the external BRender Archival v0.1.1 21-target release unambiguous.

## Requirements

- [x] Separate every BRender recipe and public structured record touched by the
  release packet into two explicit boundaries:
  - Engine Revival local portable materializer: 12-target metadata/scaffold path.
  - External BRender Archival v0.1.1 release: 21-target pinned public checkout
    evidence path at commit `11b5a8d539e911a9c07991b751402a7d51bf1bde`.
- [x] Use an explicit pinned external checkout recipe for BRender Archival
  instead of implying this patch ports or vendors the 21-target implementation.
- [x] Relabel the existing progress sequence as an orbit/frame sequence, with
  per-panel provenance tied to the eight verified source PPM frame hashes.
- [x] Regenerate affected release media deterministically and update manifest,
  record, and test hashes.
- [x] Add asset-level copyright/license treatment for imported BRender Archival
  media/transcript/provenance and add `THIRD_PARTY_NOTICES.md`.
- [x] Keep upstream BRender MIT source licensing distinct from Engine Revival
  MIT code and imported BRender Archival AGPL-covered release artifacts.

## Technical Approach

Add red tests in `tests/test_brender_publication_boundary.py` for boundary-role
metadata, orbit-frame sequence provenance, and third-party notices. Then update
the BRender JSON records, hand-authored docs, generated docs via
`engine-revival report`, and regenerated PNG media. The media regeneration uses
the committed release orbit PNG frames as inputs and writes deterministic PNG
outputs with stable labels.

## Files to Modify

- `tests/test_brender_publication_boundary.py` — review warning regression tests.
- `README.md`, `docs/BRENDER-ARCHIVAL.md`, `docs/PUBLIC-BOUNDARY.md`,
  `docs/REMASTER-LANE.md` — public boundary text and notices.
- `THIRD_PARTY_NOTICES.md` — third-party licensing/rights treatment.
- `attempts/`, `builds/`, `harnesses/`, `readiness/`, `reproductions/`,
  `tasks/`, `sources/`, `gallery/release-20260827/` — structured records and
  release media provenance.
- `docs/generated/` — regenerated machine docs.

## Success Criteria

- [x] New tests fail before implementation and pass after implementation.
- [x] Full `python -m pytest` passes.
- [x] `engine-revival validate` and `engine-revival audit-public` pass.
- [x] `engine-revival report` runs twice, and the second run leaves no diff.
- [x] Media and receipt hashes match the committed manifests.
- [x] Public-boundary and secret scans are clean.
- [ ] Final commit is local only, with no push, merge, or release.

## Blockers

None identified.

## Status: IMPLEMENTED

## Notes

- Added boundary regression tests before implementation. The first targeted run
  failed on missing `evidence_boundaries`, stale `progress-sequence.png`, missing
  third-party notices, and missing public prose for the two-boundary split.
- Added deterministic media regeneration script for the boundary diagram and
  orbit-frame sequence. The script requires the optional `media` extra.
- Retained BRender Archival AGPL treatment for imported or derived release
  media/transcript/provenance unless later asset-specific evidence supports a
  different grant.
