# Spec: Engine Revival Publication Boundary 2026-08-27

## Objective
Make Engine Revival self-contained and consistent with the current verified public BRender boundary. Import only portable public evidence from the merged BRender Archival main release and leave a reviewed, committed candidate without pushing or merging.

## Authorization
Status: APPROVED by parent task. The parent explicitly authorized isolated worktree creation, implementation, commit, push, merge, deploy, and publication, but this task is scoped to implementation and commit only. This task must not push, merge, deploy, or publish.

## Requirements
- [ ] Work in the isolated publication worktree on `codex/engine-revival-publication-20260827` from refreshed `origin/main`.
- [ ] Import BRender evidence only from public `HarperZ9/brender-archival` main release commit `11b5a8d539e911a9c07991b751402a7d51bf1bde` / tag `v0.1.1`, not local experimental or non-public diagnostic branches.
- [ ] Add red-first tests for 21-rung parity, transcript sanitization, provenance/hash coverage, media presence, and public-boundary wording.
- [ ] Commit a structured attempt record for the 21-target native CTest result and a sanitized transcript containing `Test project <build>`.
- [ ] Capture source, release commit, command, platform, input, and output hashes in public-safe records.
- [ ] Add current first-party public-safe release media only where rights and provenance are explicit.
- [ ] Reconcile README, `docs/BRENDER-ARCHIVAL.md`, `docs/REMASTER-LANE.md`, readiness, reproduction, harness, task records, generated docs, and stale 12/20/21 drift.
- [ ] Preserve relationship boundaries: Retro Engine equals play; Engine Revival equals preservation/research/evidence; BRender Archival equals verified specific restoration; generic Retro output is never BRender proof.
- [ ] Retain non-claims: no completed textured TIA output, no x64 readiness, no production readiness, no adoption claim, no endorsement claim, and no vendored upstream source/assets.
- [ ] Remove or correct references to files not present.
- [ ] Run `engine-revival report` twice and require the second run to produce no diff.
- [ ] Run full pytest, validate, audit-public, generated-doc parity, media/hash verification, public-boundary/secret/local-path scans, diff check, and remote drift check.
- [ ] Commit all source changes; do not commit caches or scratch.

## Technical Approach
Use the existing Engine Revival record model and report generator. Prefer metadata, transcript, provenance manifest, and public media imports over pulling BRender harness source changes into Engine Revival unless tests show the record/report surface needs code changes.

## Files Expected To Change
- `tests/test_brender_publication_boundary.py` - new red-first publication boundary tests.
- `attempts/brender-v132-native-ctest-twentyone-targets-win32.json` - structured 21-target receipt.
- `attempts/transcripts/brender-v132-ctest-twentyone-targets-2026-08-27.log` - sanitized transcript.
- `gallery/release-20260827/*` - public-safe media and provenance manifest from BRender release.
- `README.md`, `docs/BRENDER-ARCHIVAL.md`, `docs/REMASTER-LANE.md`, `docs/PUBLIC-BOUNDARY.md` - public boundary prose.
- `readiness/brender-production-readiness.json`, `reproductions/brender-critical-edition-source-build.json`, `harnesses/brender-v132-portable-core-plan.json`, `tasks/brender-*.json` - structured claim alignment.
- `docs/generated/**` - generated report parity updates.

## Success Criteria
- [ ] New tests fail before implementation for the expected missing 21-rung/media/provenance boundary.
- [ ] New and existing tests pass after implementation.
- [ ] `engine-revival validate` and `engine-revival audit-public` exit 0.
- [ ] `engine-revival report` second run leaves no diff.
- [ ] Media hashes in the provenance manifest match committed files.
- [ ] Secret/local-path/public-boundary scans show no unsafe committed content beyond sanitized placeholder paths and allowed docs examples.
- [ ] Remote `origin/main` is unchanged from the refreshed base before final handoff.
- [ ] One local commit exists on the candidate branch and is not pushed by this task.

## Blockers
None identified. Full workspace `mcp__index` map timed out; this task will rely on targeted repo reads, git history, Forum route output, and explicit source commits.

## Status
IN_PROGRESS
