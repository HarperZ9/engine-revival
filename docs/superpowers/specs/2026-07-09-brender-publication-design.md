# BRender Provenance-First Publication Design

Date: 2026-07-09

Status: Recommended design selected under the operator's publication request;
written specification awaiting explicit operator review

## Objective

Publish the verified BRender portable-memory compatibility work as a coherent,
reviewable source release across both `engine-revival` and the standalone
`brender-archival` repository. The publication must include current structured
evidence, hand-written documentation, one real engine-generated media proof,
durable demo instructions, outcome-marked specifications, and remote draft
pull requests.

This is a source-and-evidence publication, not a binary package or claim that
BRender is fully ported. The active long-term engine-revival goal remains open
after publication.

## Selected Approach

Use a provenance-first release surface:

1. Promote the verified 13/13 checkpoint atomically in the five canonical
   current-state carriers.
2. Present the capability through concise README text, a dedicated demo guide,
   and the existing archival packet.
3. Publish one byte-identical SVG produced by the real BRender plotter smoke,
   with adjacent input, source, command, size, and hash provenance.
4. Regenerate structured public views from records; never hand-edit them.
5. Mirror reviewed non-protected canonical paths byte-for-byte into
   `brender-archival`.
6. Update archive-owned README/provenance separately while preserving its AGPL
   identity and every existing gallery byte.
7. Push feature branches and open draft PRs against each repository's `main`;
   do not merge or publish binaries in this operation.

### Alternatives considered

**Expanded raster release.** Convert texture and Gouraud PPM output into PNG in
addition to the SVG. This gives a richer visual page but adds conversion-tool,
metadata, and derivative-hash obligations. It is deferred until the next media
slice.

**Evidence-only release.** Publish records and prose without new media. This is
smaller but does not satisfy the operator's request for user-facing media and
leaves the canonical repository visually opaque.

The selected approach is the smallest release that provides direct visual
proof without inventing imagery or adding lossy derivatives.

Only this design document is committed before review. Checkpoint promotion,
documentation/media changes, archive synchronization, pushes, and pull-request
creation begin only after the operator approves this written specification and
the corresponding implementation plan is written.

## Verified Input State

- Canonical branch: `feat/brender-memory-compat`.
- Canonical reviewed implementation head before this design:
  `cdb985f66e2ea9dfa9b42b610979c0bffb8b90bc`.
- Public BRender source snapshot:
  `d88d0ed41122664b9781015b517db64353e16f19`.
- Full Win32 Debug transcript:
  `C:\dev\public\engine-revival-workspaces\brender-v132-portable-core-ctest-memory-compat-2026-07-09.txt`.
- Full transcript SHA-256:
  `592559a41e77a0d51c378e678f8f17884be439c14954ba16ec4b5e90790be1f4`.
- Full transcript proof: exactly thirteen passed-test lines and
  `100% tests passed, 0 tests failed out of 13`.
- Pre-fix RED transcript SHA-256:
  `7ec18fe16bc591980ea5eafc970abec25a894338f759fce3cb0e1f9cd9cd6b60`.
- Archive synchronization base:
  `500d9bc16281e966373f6cf87bc3fa569f55a32f`.
- Canonical remote: public `HarperZ9/engine-revival`, default branch `main`.
- Archive remote: public `HarperZ9/brender-archival`, default branch `main`.
- Reproduction shell: Windows PowerShell Desktop `5.1.26100.8764` through
  `powershell.exe`; `pwsh.exe` was not present in `PATH` on the verified host.

## Publication Boundaries

The publication may claim only:

- a successful out-of-tree Visual Studio Win32 Debug FLOAT-core build;
- thirteen passing CTests at the pinned source snapshot;
- direct 1-, 2-, 3-, and 4-byte pixel set/get semantics;
- non-black RGB888 fill byte order;
- positive-stride rectangular fill with padding preservation;
- nonzero-start copy-bits through `_MemCopyBits_A` and
  `BrPixelmapCopyBits` public dispatch;
- the earlier visual render, model, Gouraud, and hidden-line plotter rungs.

It must continue to defer:

- Host/DOS fallback policy parity;
- overlapping-copy behavior;
- negative strides;
- colour-key and FPU-wrapper parity;
- widths above four bytes;
- x64 runtime safety;
- period `softrend` translation;
- native `.mat/.pal/.pix` resolution;
- FIXED variants and drivers;
- packaged binaries and a full interactive viewer;
- the separate 3D Movie Maker build step.

Flagship score stays `86`. Packaging stays `not-started`.

## Component Design

### 1. Release-command hardening

Resolve the remaining nonblocking review observation before publication. The
attempt record's `Tee-Object` invocation must include `-ErrorAction Stop`, and
its exact-command regression test must require that flag. This prevents a
future transcript-write failure from being treated as a successful
reproduction.

The final and RED transcripts must remain byte-identical during this static
command hardening.

### 2. Atomic canonical checkpoint promotion

The following records must receive one identical checkpoint:

- `tasks/brender-critical-edition-packet.json`
- `reproductions/brender-critical-edition-source-build.json`
- `builds/brender-v132-build-environment.json`
- `harnesses/brender-v132-portable-core-plan.json`
- `readiness/brender-production-readiness.json`

Exact checkpoint:

```json
{
  "id": "brender-v132-portable-core-memory-compat-2026-07-09",
  "stage": "portable-core-memory-compat-lane-passing",
  "passed": 13,
  "total": 13,
  "source_snapshot": "d88d0ed41122664b9781015b517db64353e16f19"
}
```

Exact current statuses:

- task: `portable-memory-compat-lane-verified`;
- reproduction: `v132-portable-memory-compat-verified-3dmm-pending`;
- build and harness: `portable-core-memory-compat-lane-passing`;
- readiness stage: `portable-core-memory-compat-lane-passing`;
- readiness build: `portable-core-memory-compat-lane-built`;
- readiness runtime/test: `memory-compat-lane-passing`;
- readiness modernization:
  `portable-memory-compat-verified-x64-port-scoped`.

The new attempt ID is promoted into readiness only after its existing 13/13
transcript is revalidated.

Checkpoint replacement alone is insufficient. Update every current claim
carrier so its narrative and operational arrays agree with the checkpoint:

- task `public_notes` and `outputs`;
- reproduction `public_notes`;
- build `blockers`, `next_actions`, and `public_notes`;
- harness `expected_outputs`, `implementation_units`, `next_actions`,
  `public_notes`, and `steps`;
- readiness `blockers`, `evidence`, `next_actions`, and `public_notes`.

Those fields must describe thirteen rungs and narrow the former blanket
semantic-compatibility deferral to the four delivered memory contracts. They
must preserve the Host/DOS, overlap, negative-stride, colour-key, FPU-wrapper,
widths-above-four, x64, `softrend`, native-material, FIXED, driver, packaging,
viewer, and 3D Movie Maker boundaries. Any retained 12/12 readiness evidence
must be explicitly labeled as the earlier dated plotter checkpoint, never as
current state. Regression tests must reject stale `12/12`, `twelve-rung`, or
blanket semantic-coverage language in current summary/output fields.

Tests must also be positive and field-complete. For each of the five carriers,
explicitly enumerate the claim-bearing fields above and require they are
non-empty; require the exact shared checkpoint and exact status; require a
thirteen-rung/13-of-13 current claim; require all four delivered contracts
(1-4-byte pixel set/get, non-black RGB888 fill, positive-stride rectangular
fill, and nonzero-start copy-bits through raw plus public dispatch); and require
the retained deferral set. A carrier cannot pass merely by omitting a field or
avoiding a banned stale phrase.

### 3. Canonical user-facing documentation

Update `README.md` with a compact BRender release section that states the
pinned snapshot, observed Win32 13/13 result, four memory contracts, demo link,
an inline rendering of the tracked SVG with useful Markdown alt text and a
provenance link, and unsupported boundaries.

Update `docs/BRENDER-ARCHIVAL.md` to list thirteen rungs and distinguish proven
memory primitives from unverified Host/DOS and other policies.

Add `docs/BRENDER-DEMO.md` with public-source acquisition and pin verification,
an explicit instruction to use an external working directory, and reproducible
commands based on explicit PowerShell variables:

```powershell
$source = (Resolve-Path .\BRender-v1.3.2).Path
$harness = Join-Path $PWD '.work\brender-harness'
$build = Join-Path $PWD '.work\brender-build'

engine-revival materialize-brender-harness `
  --source-root $source --output-root $harness
cmake -S $harness -B $build -A Win32 "-DBRENDER_SOURCE_DIR=$source"
cmake --build $build --config Debug
ctest --test-dir $build -C Debug --output-on-failure
```

The guide must include direct plotter, Gouraud, texture, and memory-semantic
commands. It must label PPM as native raw evidence output and the tracked SVG as
a byte-identical real-run artifact. Every direct command must write only beneath
the external demo work directory; the demo workflow copies no source model or
build product into this repository. The separately reviewed publication step in
Component 5 is the sole exception: it copies the one selected SVG output
byte-for-byte into the tracked media path.

### 4. Generated harness README completeness

The tracked README generator must list every visual target already present in
the manifest, including:

- `brender_core_material_smoke`;
- `brender_core_multimodel_smoke`;
- `brender_core_gouraud_smoke`;
- `brender_core_plotter_smoke`;
- `brender_core_memory_compat_smoke`.

Materializer tests must require those commands and preserve the Host/DOS
non-claim.

### 5. Real media and provenance

Copy this external file byte-for-byte:

`C:\dev\public\engine-revival-workspaces\brender-v132-portable-core-build-memory-compat-2026-07-09\brender-core-plotter-smoke.svg`

to:

`docs/media/brender/brender-core-plotter-smoke.svg`

Before copying it, create a dedicated media-generation code checkpoint after
all command hardening and harness/template changes are committed but before any
media/provenance file is added. Run the focused/full tests and bind an
independent code review to that exact checkpoint. Then perform a clean
provenance reproduction in a new external workspace: materialize the harness
from that exact reviewed code checkpoint, verify the pinned public source is clean at
`d88d0ed41122664b9781015b517db64353e16f19`, configure/build Win32 Debug, and
direct-run `brender_core_plotter_smoke` with that snapshot's `dat/teapot.dat`
into a fresh SVG path. The reproduced SVG must have the same exact SHA-256 as
the selected file. Capture an external transcript containing the canonical
code-checkpoint commit, source HEAD, tool versions, ordered fail-fast commands,
exit codes, and output hash; record the transcript path and SHA-256 in the
adjacent provenance page. A hash mismatch blocks publication rather than being
normalized away.

After that checkpoint, no generation-relevant implementation path may change.
The final canonical review must prove a zero diff from the code checkpoint to
final HEAD for `src/engine_revival/brender_harness.py`,
`src/engine_revival/brender_harness_templates.py`, every
`src/engine_revival/brender*_sources.py` file, and the materializer dispatch in
`src/engine_revival/cli.py`. Documentation, structured state, generated
reports, tests, and tracked media may advance; a later change to any listed
path invalidates the transcript and requires a new checkpoint/reproduction.

Required immutable media facts:

- SVG dimensions: 640 by 480;
- file size: 56,287 bytes;
- line elements: 1,002;
- SHA-256:
  `e631561322d4ac3445d3b57e5d643bfd1bed6e59da57144c99738c00e0f61885`;
- input: `dat/teapot.dat` from the pinned public source snapshot;
- no `<text>`, `<metadata>`, external reference, script, filesystem path, or
  username content.

Add `docs/media/brender/LICENSE-BRENDER-MIT.txt` as the byte-identical raw Git
blob of `LICENSE` at the pinned source snapshot. Add
`docs/media/brender/README.md` recording those facts, the generation
command, source snapshot, harness commit
`ccd859efa4e24b11844a422d3d62199cf8d4ba1e`, compatibility-fix commit
`b54d60fab4346810687bc892badf31b3df392ec9`, output hash, upstream license
identity `MIT License`, attribution `Copyright (c) 1998 Argonaut Software
Limited`, public source URL `https://github.com/foone/BRender-v1.3.2`, source
snapshot, and raw source `LICENSE` Git-blob SHA-256
`f604769ea503e060b146607f1a8ebc1ce24daec3f75a2b1cbd9e2ab8ac3223fa`.
The page must assign the SVG to those upstream MIT terms, link the complete
notice file, and state that no source model or binary is vendored. It must
explain that the file-specific MIT notice and attribution remain attached to
the media whether it is presented inside canonical MIT-licensed
`engine-revival` or the AGPL-3.0-or-later standalone archive; the archive's
project license does not replace the media's notice.

No PNG conversion belongs in this release.

### 6. Outcome-marked specifications

Append dated outcome blocks to:

- `docs/superpowers/specs/2026-07-09-brender-memory-compatibility-design.md`;
- `docs/superpowers/plans/2026-07-09-brender-memory-compatibility.md`.

The blocks must identify verified commits, 13/13 transcript/hash, review state,
publication scope, and remaining deferrals. Original design and plan text stays
historically intact.

The historical evidence-consistency design and plan remain byte-for-byte
unchanged. Any supersession context belongs in this publication design and the
new memory-compatibility outcome blocks, never in the earlier 12/12 documents.

### 7. Deterministic generated views

Update structured records first, run the worktree-bound
`engine-revival report --root .`, and commit generator output. The new attempt
must appear in its generated page, attempt index, coverage, database, and BRender
target dossier. Current task, reproduction, build, harness, readiness, and
artifact-derived pages must show 13/13.

A second report run must produce byte-identical output. Historical 1/1 through
12/12 attempt records/pages remain untouched.

### 8. Canonical publication-surface tests

Add `tests/test_brender_publish_surface.py` to require:

- current README and demo guide state 13/13 and Win32 scope;
- exact non-claim language is present;
- the README embeds the exact relative SVG path with meaningful alt text and
  links its adjacent provenance page;
- the demo guide names the required Git, Python, CMake, Visual Studio C++/Win32,
  and PowerShell prerequisites; clones the public URL; verifies the exact source
  commit; and keeps source, harness, build, and output roots external;
- the demo guide contains full-build, plotter, Gouraud, texture, and targeted
  memory commands;
- the tracked SVG has the exact size and SHA-256 above;
- its provenance page names the input, source snapshot, hash, and no-vendoring
  boundary, plus the exact upstream license identity, copyright attribution,
  source URL, raw source-license blob hash, complete notice link, explicit SVG
  distribution terms, and canonical/archive license distinction;
- the tracked `LICENSE-BRENDER-MIT.txt` raw Git blob has exact SHA-256
  `f604769ea503e060b146607f1a8ebc1ce24daec3f75a2b1cbd9e2ab8ac3223fa`;
- the memory spec and plan contain their dated verified-outcome markers.

Write these expectations before the documentation/media implementation and
observe a missing/stale-surface RED.

### 9. Canonical review gate

Run:

- full isolated pytest;
- worktree-bound validate and audit-public;
- deterministic report twice;
- full external transcript/hash check;
- clean media provenance reproduction and generation-transcript hash check;
- media hash/content scan;
- historical attempt and transcript immutability check;
- diff and staged-scope checks.

An independent review must bind its approval to the exact canonical HEAD before
archive synchronization. It must inspect the complete prospective canonical PR
range `6f2361d478c85a39b2cd146e3c94acd2127870f0...canonical_HEAD`, including
commit ancestry/order, changed paths, licensing, public-safety/secret scans,
claims, generated files, and all earlier evidence-consistency plus memory work.
No unexpected commit or path may remain merely because a narrower feature delta
was reviewed earlier.

### 10. Shared archive synchronization

Create archive branch `feat/brender-memory-compat` from exact base
`500d9bc16281e966373f6cf87bc3fa569f55a32f`.

Define the shared delta as every reviewed canonical path changed from
`fde1f9a4621ea37f1a3f6dd4aa03e6943238d17b` through the exact reviewed
canonical HEAD, except:

- `README.md`;
- `LICENSE`;
- `pyproject.toml`;
- `gallery/**`.

The shared delta must contain no deletion; an unexpected deletion blocks the
sync for explicit review. Require staged-path equality with this exact reviewed
non-protected delta and SHA-256 equality for every copied file. The first
archive commit must not touch any protected file.

The following raw Git-blob-content SHA-256 baseline is authoritative for
archive HEAD `500d9bc`; it is intentionally independent of checkout EOL
filters such as `core.autocrlf`. Hash bytes obtained from the index/commit
object, not the working-tree representation.
Verify all fourteen paths before branching and after shared synchronization.
The original `README.md` hash must match through that first commit and may change
only in the second archive commit. After the presentation commit, bind the new
README blob hash to the exact-HEAD archive review and verify that reviewed hash
immediately before push. `LICENSE`, `pyproject.toml`, and all eleven existing
media blobs must keep the baseline hashes throughout every phase.

| Protected archive path | Raw Git-blob SHA-256 at `500d9bc` |
|---|---|
| `README.md` | `78a90dbaef31f2a1e5ea5c5af0e04d53b2a0b75c6597283ad46b821e93a074cc` |
| `LICENSE` | `0d96a4ff68ad6d4b6f1f30f713b18d5184912ba8dd389f86aa7710db079abcb0` |
| `pyproject.toml` | `0ea27ff98d7226dd2b165aa9a35a33b5cb63936d96a81a1889610e624bce0ced` |
| `gallery/01-wireframe-cube.png` | `7caa3fdffba2666ead882f0ad4aca5cd9e3c9d2a669c7867eebd170c5956f1ce` |
| `gallery/02-scene-graph-cube.png` | `e0d61cd295ce3115065ac1d2bef07acf1e4788c5bba05c4adb80977ea95c59a1` |
| `gallery/03-solid-shaded.png` | `99263cabcea1b273af0018b8ec94bcfd38c03155c2d04027dade846f4f5c1d66` |
| `gallery/04-depth-buffer.png` | `1417dde07f043fb94d8866a3c9a6aa77ece0d7707e787f0a9a887e404cea07a2` |
| `gallery/05-texture-mapped.png` | `d4ebf0d9db580e802ec01f2585375cbbac0772229182d0010eafe777f3930c92` |
| `gallery/06-datafile-models.png` | `6905626a2151ba7f0ff40eb6acfa16388e226d096b4f48cab63763cda0b97972` |
| `gallery/07-uv-textured-globe.png` | `550ac52aa751d759002b0fb4fb5054541049ff9165f8db327322ac1724e55522` |
| `gallery/08-multipart-coupe.png` | `d2be455e1110139068d449d3a083576130dfeffabc287f2bb70e1e49a8fb4745` |
| `gallery/09-gouraud-sphere.png` | `66fb2058d3d4e3f920c2ee11f4f30fec1bc6ce17f06222fa75067ea87e6bbac0` |
| `gallery/10-teapot-plotter.png` | `c7f0a0df1cab4c5e3ec42cf034bca1fd91934ce1f83318f53faff5891d5d095c` |
| `gallery/10-teapot-plotter.svg` | `e631561322d4ac3445d3b57e5d643bfd1bed6e59da57144c99738c00e0f61885` |

### 11. Archive-specific presentation

In a second archive commit:

- update archive `README.md` deliberately for thirteen rungs, 13/13, demo/media
  links, an inline SVG rendering with useful alt text, and explicit deferrals;
- add `gallery/README.md` with an inventory and SHA-256 for all eleven existing
  gallery assets (using raw committed-blob bytes), known input/origin, source
  snapshot, license context, and `unknown` for any historical conversion detail
  that cannot be verified.

Do not change archive `LICENSE`, `pyproject.toml`, or existing gallery media.
The archive remains AGPL-3.0-or-later and retains its standalone identity.

The final archive review must inspect the complete prospective PR range
`4ef0025aff3d733a5433364b9e8b720de40e49dc...archive_HEAD`, not only the new
sync/presentation commits. It must verify that `500d9bc` is the sole intentional
pre-feature commit, is a descendant of that remote base, and still matches its
reviewed evidence-consistency purpose. Review the cumulative commit list,
changed paths, blob/license boundaries, generated files, claims, secrets/public
safety, protected hashes, and absence of unexpected commits before approval.

### 12. Remote publication

After both repositories pass local gates and whole-branch review:

1. immediately before *each repository's* push, fetch/prune that `origin` and
   verify its remote `main` by `git ls-remote`: canonical must still be
   `6f2361d478c85a39b2cd146e3c94acd2127870f0`; archive must still be
   `4ef0025aff3d733a5433364b9e8b720de40e49dc`;
2. at that same just-in-time check, verify the repository's remote
   `feat/brender-memory-compat` ref is absent, or—only when safely resuming a
   partial publication—already equals the exact locally reviewed HEAD; any
   divergent feature ref blocks publication;
3. re-check that repository's clean worktree, exact reviewed HEAD, full
   base-to-HEAD PR diff, transcript hashes, media hashes, and—where
   applicable—protected hashes;
4. if the feature ref is absent, create it with an explicit compare-and-swap
   lease that requires absence
   (`--force-with-lease=refs/heads/feat/brender-memory-compat:`); this scoped
   create-only lease prevents a newly appeared ref from being fast-forwarded or
   overwritten. Unconditional `--force`, a non-empty overwrite lease, and every
   push to `main` are forbidden. If the remote feature already equals reviewed
   HEAD, skip the push;
5. after that push and again immediately before creating its PR, re-run
   `git ls-remote` and verify the remote feature ref equals the local reviewed
   HEAD and remote `main` still equals the expected base;

6. push canonical `feat/brender-memory-compat` to `origin` with tracking;
7. open a draft PR against `HarperZ9/engine-revival:main`;
8. push archive `feat/brender-memory-compat` to `origin` with tracking;
9. open a draft PR against `HarperZ9/brender-archival:main`;
10. state explicitly that the archive PR includes local evidence-consistency
   commit `500d9bc`, because remote `main` is still at `4ef0025`;
11. include checks, media provenance, non-claims, and the verified host-shell
   note in both PR descriptions: `pwsh.exe` was absent from `PATH`, while
   Windows PowerShell Desktop `5.1.26100.8764` executed the documented commands;
   this was not a BRender build or CTest failure.

The PRs remain draft. Do not merge, tag, create a release, upload binaries, or
push directly to `main`.

Remote `main` cannot be transactionally frozen for the lifetime of a draft PR.
The feature-ref lease protects branch integrity; the repeated pre/post checks
protect the reviewed base. If `main` moves after feature creation, leave the
feature branch intact but stop before PR creation and re-review the new
base-to-HEAD diff. Record the exact base SHA in each PR body and re-run the
whole-branch gate whenever that draft PR's base changes.

## Failure Handling

- If the 13/13 transcript or either transcript hash drifts, stop publication and
  investigate before changing records.
- If the media source hash/content scan differs, do not copy it; regenerate or
  explain the mismatch under review.
- If the clean plotter reproduction does not yield the exact selected SVG hash,
  do not publish that media or assert its generation provenance.
- If report generation is not deterministic, do not mirror generated views.
- If a historical attempt/transcript changes, restore the task scope and block
  publication.
- If a protected archive path changes during shared sync, abort before commit.
- If copied shared blobs differ, do not create the archive commit.
- If either remote `main` or feature ref differs from the expected just-in-time
  value, do not push or open a PR; review the new remote state first.
- If GitHub authentication, push, or PR creation fails, preserve local reviewed
  branches and report the exact remote blocker without rewriting history.

## Acceptance Criteria

- Canonical Task 3 command hardening includes fail-closed transcript writes.
- Five current carriers share the exact 13/13 memory checkpoint.
- Every current carrier summary/output field says thirteen rungs, contains no
  stale current-state 12/12 or blanket semantic-coverage deferral, and keeps
  earlier 12/12 evidence explicitly historical.
- Score is 86, packaging is `not-started`, and 3D Movie Maker remains pending.
- README, archival packet, demo guide, harness README, spec, and plan agree on
  verified scope and deferrals.
- Canonical and archive READMEs render the tracked SVG inline with useful alt
  text and link to its adjacent provenance record.
- Real SVG and provenance page pass exact hash/content checks.
- A clean external materialize/build/direct-run reproduces the exact SVG hash,
  and the adjacent page records a generation transcript bound to the reviewed
  media-generation code checkpoint and pinned source commit; the final review
  proves generation-relevant paths did not change afterward.
- Generated views are complete, deterministic, and historically honest.
- Canonical full tests, validate, audit, transcript, media, and review gates pass.
- Archive shared files match canonical by path and hash.
- Archive README and gallery provenance are current while license/package/media
  identity is preserved.
- The exact committed archive protected-byte baseline passes before and after
  shared sync; its immutable 13-path subset passes after presentation and before
  push, while the new README hash is bound to final archive review.
- Archive full tests, validate, audit, deterministic report, hash, and review
  gates pass.
- Both feature branches are pushed with upstream tracking.
- Neither push can overwrite a remote branch: new feature refs use only the
  expected-absence create lease, existing exact refs are skipped, expected
  remote `main` heads are unchanged, and remote feature refs equal the reviewed
  local heads.
- Whole-branch reviews cover the exact canonical and archive remote-main-to-HEAD
  PR ranges, including the cumulative archive evidence-consistency commit.
- Two draft PRs target `main` and contain reproducibility, media, review, and
  known-limit information.
- No Fable-owned path, credential, restricted asset, source checkout, datafile,
  binary, library, PDB, or CMake build tree is committed.

## Spec Self-Review

- [x] Evidence hashes, commits, checkpoint values, score, packaging state, and
  source snapshot match the verified local records and external artifacts.
- [x] Historical attempts, transcripts, and prior 12/12 design records are
  immutable.
- [x] Canonical versus archive-owned files, licenses, gallery assets, and remote
  branch bases have explicit boundaries.
- [x] User-facing documentation, demo commands, real media, alt text, and media
  provenance have testable acceptance criteria.
- [x] Pushes stop at feature branches and draft PRs; merges, tags, releases,
  binaries, and direct default-branch writes remain outside authorization.
- [x] No placeholder, guessed provenance, or unresolved publication decision
  remains in this specification.

## Follow-On

After these PRs are published, return to the engine-revival roadmap. The next
BRender slices remain Host/DOS policy contracts, colour-key semantics,
negative-stride behavior, faithful public material resolution, packaging, x64
porting, and period `softrend`; the broader active goal then advances to the
next engine target rather than treating this source publication as completion
of every engine revival.
