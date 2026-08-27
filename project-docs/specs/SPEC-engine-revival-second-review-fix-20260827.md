# Spec: Engine Revival second-review fix, 2026-08-27

## Objective

Fix the two independent-review warnings on head
`4682fdc85f2eec2f8d9828feb240405779e2f77d` without push, merge, or release.

## Requirements

- [x] The canonical machine-readable local 12-target materializer recipe must
  order configure, build, then test:
  1. `engine-revival materialize-brender-harness ...`
  2. `cmake -S ... -B <build> ...`
  3. `cmake --build <build> --config Debug`
  4. `ctest --test-dir <build> -C Debug --output-on-failure ...`
- [x] Regenerate structured records and release manifest from the generator.
- [x] Add canonical AGPL-3.0-or-later license text at
  `LICENSES/AGPL-3.0-or-later.txt`.
- [x] Link the canonical AGPL text from `THIRD_PARTY_NOTICES.md`.
- [x] Add regression tests for ordered configure/build/test and AGPL file
  presence/digest.

## Technical Approach

Strengthen `tests/test_brender_publication_boundary.py` first. Then patch
`scripts/regenerate_brender_publication_boundary.py`, add the license text, run
the regenerator, run `engine-revival report`, and complete the full verification
gates.

## Files to Modify

- `tests/test_brender_publication_boundary.py`
- `scripts/regenerate_brender_publication_boundary.py`
- `LICENSES/AGPL-3.0-or-later.txt`
- `THIRD_PARTY_NOTICES.md`
- regenerated BRender records and generated docs

## Success Criteria

- [x] Targeted tests fail before implementation for both review warnings.
- [x] Targeted tests pass after implementation.
- [x] Full `python -m pytest` passes.
- [x] `engine-revival validate` and `engine-revival audit-public` pass.
- [x] Regenerator and report runs are idempotent.
- [x] Final tree is clean after a local commit only.

## Blockers

None identified.

## Status: IMPLEMENTED
