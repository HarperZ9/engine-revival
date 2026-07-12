# BRender Provenance-First Publication Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Publish the verified BRender Win32 13-rung portable-core work as two reviewed draft pull requests with coherent state records, reproducible demos, real engine-generated media, exact provenance, and preserved archive identity.

**Architecture:** Finish and freeze the generation-relevant code first, then reproduce the plotter SVG from that exact checkpoint in a clean external workspace. Promote the five structured carriers and deterministic generated views before adding the public demo/media surface. Mirror only the exact reviewed canonical delta into `brender-archival`, update archive-owned presentation separately, and use compare-and-swap feature-branch creation plus draft PRs for remote publication.

**Tech Stack:** Python 3.11+, pytest, JSON Schema validation, the `engine-revival` CLI, PowerShell 5.1, Git, CMake 3.20+, Visual Studio/MSVC Win32 Debug, CTest, GitHub HTTPS remotes, and the GitHub connector.

## Global Constraints

- Canonical worktree: `C:\dev\worktrees\engine-revival-brender-evidence` on `feat/brender-memory-compat`.
- Approved design commit: `37efdcbe8f9379de3e2a10b78f57e54076a57040`; execution starts from the clean committed plan whose only delta after that design is this plan file.
- Canonical remote `main` must remain `6f2361d478c85a39b2cd146e3c94acd2127870f0` through publication.
- Archive worktree: `C:\dev\worktrees\brender-archival-evidence`; create `feat/brender-memory-compat` from exact `500d9bc16281e966373f6cf87bc3fa569f55a32f`.
- Archive remote `main` must remain `4ef0025aff3d733a5433364b9e8b720de40e49dc` through publication.
- Public source: clean `C:\dev\public\engine-revival-workspaces\brender-v132` at `d88d0ed41122664b9781015b517db64353e16f19` only.
- Use only the canonical isolated environment `C:\dev\worktrees\.venvs\engine-revival-brender-evidence` and archive environment `C:\dev\worktrees\.venvs\brender-archival-evidence`; prove import paths before CLI gates.
- Configure only Visual Studio `-A Win32`; do not claim x64 runtime safety.
- The exact checkpoint is `brender-v132-portable-core-memory-compat-2026-07-09`, stage `portable-core-memory-compat-lane-passing`, source snapshot `d88d0ed41122664b9781015b517db64353e16f19`, and 13/13.
- The only newly verified contracts are 1-4-byte pixel set/get, non-black RGB888 fill, positive-stride rectangular fill, and nonzero-start copy-bits through `_MemCopyBits_A` plus `BrPixelmapCopyBits`.
- Preserve explicit deferrals: Host/DOS fallback parity, overlap, negative strides, colour-key and FPU-wrapper parity, widths above four, x64, period `softrend`, native `.mat/.pal/.pix`, FIXED, drivers, packaging, a full viewer, and 3D Movie Maker.
- Flagship score remains `86`; packaging remains `not-started`.
- Preserve every historical BRender attempt record/transcript and every earlier 1/1-12/12 generated attempt page byte-for-byte.
- After the media-generation checkpoint, do not change `src/engine_revival/brender_harness.py`, `src/engine_revival/brender_harness_templates.py`, any `src/engine_revival/brender*_sources.py`, or the materializer dispatch in `src/engine_revival/cli.py` without invalidating and repeating media reproduction.
- Canonical media is one real SVG only; no PNG conversion, binaries, libraries, PDBs, source checkout, datafile, or CMake build tree enters either Git repository.
- Canonical repository remains MIT. Archive remains AGPL-3.0-or-later. The SVG carries the upstream BRender MIT notice separately.
- Archive shared sync excludes canonical `README.md`, `LICENSE`, `pyproject.toml`, and `gallery/**`. Existing archive `LICENSE`, `pyproject.toml`, and eleven gallery blobs never change.
- Never modify `C:\dev\local-model`, `E:\local-model-run`, `public/index`, `forum`, `gather`, `crucible`, `telos`, `mneme`, `relay`, `plexus`, `telos-v2`, `portfolio-site`, `profile`, or either live Fable workflow/session tree.
- Never print child-process command lines that may contain secrets; never print credentials, `.env` contents, private keys, or the exposed Hugging Face credential.
- Push only `feat/brender-memory-compat`; never push directly to `main`, merge, tag, create a release, or upload binaries.
- Create draft PRs only. A remote ref mismatch or remote `main` drift stops publication for re-review.

## File and Responsibility Map

### Canonical tracked changes

- `attempts/brender-v132-portable-core-memory-compat-smoke.json`: fail-closed reproduction command.
- `tests/test_brender_memory_compat_evidence.py`: exact command regression.
- `src/engine_revival/brender_harness_templates.py`: complete generated harness README target list.
- `tests/test_brender_harness_materializer.py`: generated README regression.
- `tests/test_brender_state_consistency.py`: exact checkpoint/status and positive field-complete carrier contract.
- `tasks/brender-critical-edition-packet.json`: current task claim/output.
- `reproductions/brender-critical-edition-source-build.json`: 13/13 reproduction with 3D Movie Maker pending.
- `builds/brender-v132-build-environment.json`: build state, narrowed blocker, and next action.
- `harnesses/brender-v132-portable-core-plan.json`: thirteenth output/unit/step and narrowed blocker.
- `readiness/brender-production-readiness.json`: 13/13 evidence, attempt promotion, statuses, blockers, and next actions.
- `docs/BRENDER-ARCHIVAL.md`: thirteen-rung packet and honest deferrals.
- `README.md`: public BRender result, inline SVG, demo/provenance links, and non-claims.
- `docs/BRENDER-DEMO.md`: prerequisites, pinned public checkout, full build/CTest, and direct demo commands.
- `.gitattributes`: force exact LF/raw checkout bytes for the two media/license files.
- `docs/media/brender/brender-core-plotter-smoke.svg`: byte-identical real output.
- `docs/media/brender/LICENSE-BRENDER-MIT.txt`: byte-identical upstream MIT license blob.
- `docs/media/brender/README.md`: generation transcript, commits, hashes, input, and cross-repository license context.
- `tests/test_brender_publish_surface.py`: durable docs/demo/media/license/gallery publication contract.
- `docs/superpowers/specs/2026-07-09-brender-memory-compatibility-design.md`: dated verified outcome appended without rewriting history.
- `docs/superpowers/plans/2026-07-09-brender-memory-compatibility.md`: dated verified outcome appended without rewriting history.
- `docs/superpowers/specs/2026-07-09-brender-publication-design.md`: operator-approved status and implementation-plan link.
- `docs/generated/**`: generator-owned current views only.

### Archive-specific tracked changes

- Shared canonical delta above, copied byte-for-byte except protected paths.
- `README.md`: archive-specific thirteen-rung landing page and inline media.
- `gallery/README.md`: exact eleven-blob provenance inventory.

### Ignored verification helpers

- `C:\dev\worktrees\engine-revival-brender-evidence\.superpowers\sdd\verify_archive_protected_blobs.py`: raw Git-blob SHA-256 verifier shared by archive Tasks 6-8; canonical `.superpowers/sdd` is already ignored and the helper is never copied or committed.

### External evidence paths

- Harness: `C:\dev\public\engine-revival-workspaces\brender-v132-portable-core-harness-publication-2026-07-10`.
- Build: `C:\dev\public\engine-revival-workspaces\brender-v132-portable-core-build-publication-2026-07-10`.
- Fresh output: `C:\dev\public\engine-revival-workspaces\brender-v132-publication-media-2026-07-10`.
- Generation transcript: `C:\dev\public\engine-revival-workspaces\brender-v132-plotter-provenance-2026-07-10.txt`.
- Existing full transcript: `C:\dev\public\engine-revival-workspaces\brender-v132-portable-core-ctest-memory-compat-2026-07-09.txt`.
- Existing RED transcript: `C:\dev\public\engine-revival-workspaces\brender-v132-portable-core-ctest-memory-compat-red-2026-07-09.txt`.

---

## Execution Preflight

- [ ] **Verify exact local state and immutable inputs**

Run from the canonical worktree:

```powershell
$canonical = 'C:\dev\worktrees\engine-revival-brender-evidence'
$archive = 'C:\dev\worktrees\brender-archival-evidence'
$source = 'C:\dev\public\engine-revival-workspaces\brender-v132'
$canonicalPython = 'C:\dev\worktrees\.venvs\engine-revival-brender-evidence\Scripts\python.exe'
$canonicalCli = 'C:\dev\worktrees\.venvs\engine-revival-brender-evidence\Scripts\engine-revival.exe'

$planBase = git -C $canonical rev-parse HEAD
git -C $canonical merge-base --is-ancestor 37efdcbe8f9379de3e2a10b78f57e54076a57040 $planBase
if ($LASTEXITCODE -ne 0) { throw 'Approved design is not an ancestor of plan base' }
$planDelta = @(git -C $canonical diff --name-only 37efdcbe8f9379de3e2a10b78f57e54076a57040..$planBase)
if ($planDelta.Count -ne 1 -or $planDelta[0] -ne 'docs/superpowers/plans/2026-07-10-brender-publication.md') {
  throw "Unexpected plan-base delta: $($planDelta -join ', ')"
}
if ((git -C $canonical show -s --format='%s' $planBase) -ne 'docs: plan BRender publication implementation') {
  throw 'Unexpected plan commit subject'
}
if ((git -C $canonical branch --show-current) -ne 'feat/brender-memory-compat') {
  throw 'Unexpected canonical branch'
}
if (@(git -C $canonical status --porcelain)) { throw 'Canonical worktree is dirty' }
if ((git -C $archive rev-parse HEAD) -ne '500d9bc16281e966373f6cf87bc3fa569f55a32f') {
  throw 'Archive base drifted'
}
if (@(git -C $archive status --porcelain)) { throw 'Archive worktree is dirty' }
if ((git -C $source rev-parse HEAD) -ne 'd88d0ed41122664b9781015b517db64353e16f19') {
  throw 'Pinned source drifted'
}
if (@(git -C $source status --porcelain)) { throw 'Pinned source is dirty' }

& $canonicalPython -c "import pathlib, engine_revival.cli; p=pathlib.Path(engine_revival.cli.__file__).resolve(); r=pathlib.Path(r'$canonical\src').resolve(); assert r in p.parents, p; print(p)"
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
& $canonicalCli validate --root $canonical
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
& $canonicalCli audit-public --root $canonical
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
& $canonicalPython -m pytest -q $canonical
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
```

Expected: canonical and archive are clean at the exact bases, source is clean at the pinned public commit, CLI resolves inside the isolated canonical worktree, and 129 tests plus validate/audit pass.

- [ ] **Verify transcript and media source fixity before edits**

```powershell
$final = 'C:\dev\public\engine-revival-workspaces\brender-v132-portable-core-ctest-memory-compat-2026-07-09.txt'
$red = 'C:\dev\public\engine-revival-workspaces\brender-v132-portable-core-ctest-memory-compat-red-2026-07-09.txt'
$svg = 'C:\dev\public\engine-revival-workspaces\brender-v132-portable-core-build-memory-compat-2026-07-09\brender-core-plotter-smoke.svg'
if ((Get-FileHash $final -Algorithm SHA256).Hash.ToLowerInvariant() -ne '592559a41e77a0d51c378e678f8f17884be439c14954ba16ec4b5e90790be1f4') { throw 'Final transcript drift' }
if ((Get-FileHash $red -Algorithm SHA256).Hash.ToLowerInvariant() -ne '7ec18fe16bc591980ea5eafc970abec25a894338f759fce3cb0e1f9cd9cd6b60') { throw 'RED transcript drift' }
if ((Get-FileHash $svg -Algorithm SHA256).Hash.ToLowerInvariant() -ne 'e631561322d4ac3445d3b57e5d643bfd1bed6e59da57144c99738c00e0f61885') { throw 'Selected SVG drift' }
$fullText = Get-Content -Raw -LiteralPath $final
if (@(Select-String -LiteralPath $final -Pattern 'Test\s+#\d+:.*Passed').Count -ne 13) { throw 'Final transcript lacks thirteen passed lines' }
if ($fullText -notmatch '100% tests passed, 0 tests failed out of 13') { throw 'Final transcript lacks 13/13 summary' }
```

Expected: both transcripts and the selected real SVG have their exact approved hashes and the final transcript proves 13/13.

- [ ] **Require clean external reproduction destinations**

```powershell
$externalPaths = @(
  'C:\dev\public\engine-revival-workspaces\brender-v132-portable-core-harness-publication-2026-07-10',
  'C:\dev\public\engine-revival-workspaces\brender-v132-portable-core-build-publication-2026-07-10',
  'C:\dev\public\engine-revival-workspaces\brender-v132-publication-media-2026-07-10',
  'C:\dev\public\engine-revival-workspaces\brender-v132-plotter-provenance-2026-07-10.txt'
)
$existing = @($externalPaths | Where-Object { Test-Path -LiteralPath $_ })
if ($existing) { throw "Publication reproduction destination already exists: $($existing -join ', ')" }
```

Expected: all four exact external paths are absent. Do not delete or reuse a prior path; stop for explicit review if one appears.

---

### Task 1: Harden publication commands and freeze generation code

**Files:**

- Modify: `tests/test_brender_memory_compat_evidence.py`
- Modify: `attempts/brender-v132-portable-core-memory-compat-smoke.json`
- Modify: `tests/test_brender_harness_materializer.py`
- Modify: `src/engine_revival/brender_harness_templates.py`

**Interfaces:**

- Consumes: immutable Task 3 attempt at `cdb985f66e2ea9dfa9b42b610979c0bffb8b90bc` and the existing 13-target manifest.
- Produces: fail-closed transcript write, complete generated README target commands, and the exact media-generation code checkpoint consumed by Task 2.
- Preserves: every CMake target/smoke source, both existing transcript bytes, and all runtime semantics.

- [ ] **Step 1: Write the two failing regressions**

In `tests/test_brender_memory_compat_evidence.py`, replace `TEE_FINAL` with:

```python
TEE_FINAL = (
    f"$fullOutput | Tee-Object -FilePath {FINAL_TRANSCRIPT} -ErrorAction Stop"
)
```

In `tests/test_brender_harness_materializer.py`, after reading the generated README, add:

```python
    for target in (
        "brender_core_material_smoke",
        "brender_core_multimodel_smoke",
        "brender_core_gouraud_smoke",
        "brender_core_plotter_smoke",
        "brender_core_memory_compat_smoke",
    ):
        assert f"--target {target}" in readme
```

- [ ] **Step 2: Run focused tests and verify RED**

```powershell
$python = 'C:\dev\worktrees\.venvs\engine-revival-brender-evidence\Scripts\python.exe'
& $python -m pytest tests/test_brender_memory_compat_evidence.py tests/test_brender_harness_materializer.py -q
```

Expected: exactly the command equality fails because the attempt lacks `-ErrorAction Stop`, and README target assertions fail for material, multimodel, Gouraud, and plotter. Import/fixture failures are not acceptable RED.

- [ ] **Step 3: Make the exact minimal implementation**

In the attempt record, replace:

```text
$fullOutput | Tee-Object -FilePath C:/dev/public/engine-revival-workspaces/brender-v132-portable-core-ctest-memory-compat-2026-07-09.txt
```

with:

```text
$fullOutput | Tee-Object -FilePath C:/dev/public/engine-revival-workspaces/brender-v132-portable-core-ctest-memory-compat-2026-07-09.txt -ErrorAction Stop
```

In `readme_source()` insert these commands after `brender_core_model_smoke` and before `brender_core_memory_compat_smoke`:

```text
cmake --build build --config Debug --target brender_core_material_smoke
cmake --build build --config Debug --target brender_core_multimodel_smoke
cmake --build build --config Debug --target brender_core_gouraud_smoke
cmake --build build --config Debug --target brender_core_plotter_smoke
```

- [ ] **Step 4: Run focused GREEN and full code-checkpoint gates**

```powershell
$python = 'C:\dev\worktrees\.venvs\engine-revival-brender-evidence\Scripts\python.exe'
$cli = 'C:\dev\worktrees\.venvs\engine-revival-brender-evidence\Scripts\engine-revival.exe'
& $python -m pytest tests/test_brender_memory_compat_evidence.py tests/test_brender_harness_materializer.py -q
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
& $python -m pytest -q
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
& $cli validate --root .
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
& $cli audit-public --root .
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
git diff --check
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
```

Expected: focused tests and the complete suite pass; validation/audit/diff checks are clean.

- [ ] **Step 5: Prove external evidence remained byte-identical and commit**

```powershell
$final = 'C:\dev\public\engine-revival-workspaces\brender-v132-portable-core-ctest-memory-compat-2026-07-09.txt'
$red = 'C:\dev\public\engine-revival-workspaces\brender-v132-portable-core-ctest-memory-compat-red-2026-07-09.txt'
if ((Get-FileHash $final -Algorithm SHA256).Hash.ToLowerInvariant() -ne '592559a41e77a0d51c378e678f8f17884be439c14954ba16ec4b5e90790be1f4') { throw 'Final transcript changed' }
if ((Get-FileHash $red -Algorithm SHA256).Hash.ToLowerInvariant() -ne '7ec18fe16bc591980ea5eafc970abec25a894338f759fce3cb0e1f9cd9cd6b60') { throw 'RED transcript changed' }
git add attempts/brender-v132-portable-core-memory-compat-smoke.json `
  tests/test_brender_memory_compat_evidence.py `
  src/engine_revival/brender_harness_templates.py `
  tests/test_brender_harness_materializer.py
git diff --cached --check
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
git commit -m "fix: harden BRender publication commands"
```

Expected: one four-file commit. Its exact HEAD becomes the candidate media-generation code checkpoint.

- [ ] **Step 6: Obtain a HEAD-bound code review and freeze paths**

An independent reviewer checks the Task 1 commit against Component 1 and Component 4 of the approved design and writes ignored report `.superpowers/sdd/brender-publication-code-review.md`. The report's penultimate line is `Reviewed code checkpoint: ` immediately followed by the exact 40-character Task 1 HEAD, and its final line is exactly `Review result: APPROVED`.

The reviewer must find no Critical or Important issue. Record that hash as `CODE_CHECKPOINT`. From this point onward, any diff to these paths invalidates Task 2 evidence:

```text
src/engine_revival/brender_harness.py
src/engine_revival/brender_harness_templates.py
src/engine_revival/brender*_sources.py
src/engine_revival/cli.py
```

---

### Task 2: Reproduce and bind the real SVG to the code checkpoint

**Files:**

- External create: `C:\dev\public\engine-revival-workspaces\brender-v132-portable-core-harness-publication-2026-07-10/**`
- External create: `C:\dev\public\engine-revival-workspaces\brender-v132-portable-core-build-publication-2026-07-10/**`
- External create: `C:\dev\public\engine-revival-workspaces\brender-v132-publication-media-2026-07-10/brender-core-plotter-smoke.svg`
- External create: `C:\dev\public\engine-revival-workspaces\brender-v132-publication-media-2026-07-10/brender-core-plotter-preview.ppm`
- External create: `C:\dev\public\engine-revival-workspaces\brender-v132-plotter-provenance-2026-07-10.txt`
- Ignored record: `.superpowers/sdd/brender-publication-media-evidence.md`

**Interfaces:**

- Consumes: exact `CODE_CHECKPOINT`, clean source `d88d0ed...`, and worktree-bound materializer.
- Produces: clean 13/13 build evidence, exact reproduced SVG `e631...`, and `MEDIA_TRANSCRIPT_SHA256` for Task 4.
- Does not modify: any tracked file.

- [ ] **Step 1: Verify the reviewed checkpoint and frozen tree before reproduction**

```powershell
$canonical = 'C:\dev\worktrees\engine-revival-brender-evidence'
$review = Get-Content -Raw -LiteralPath (Join-Path $canonical '.superpowers\sdd\brender-publication-code-review.md')
if ($review -notmatch '(?m)^Reviewed code checkpoint: ([0-9a-f]{40})\r?\nReview result: APPROVED\r?\n?\z') { throw 'Code review is missing or malformed' }
$codeCheckpoint = $Matches[1]
if ((git -C $canonical rev-parse HEAD) -ne $codeCheckpoint) { throw 'Code review is stale' }
if (@(git -C $canonical status --porcelain)) { throw 'Canonical worktree is dirty' }
if ((git -C 'C:\dev\public\engine-revival-workspaces\brender-v132' rev-parse HEAD) -ne 'd88d0ed41122664b9781015b517db64353e16f19') { throw 'Source commit drifted' }
if (@(git -C 'C:\dev\public\engine-revival-workspaces\brender-v132' status --porcelain)) { throw 'Pinned source dirty' }
```

Expected: the clean canonical HEAD equals the approved code checkpoint and source is pinned.

- [ ] **Step 2: Materialize, configure, build, test, and direct-run with ordered fail-fast logging**

Run this single PowerShell process:

```powershell
$ErrorActionPreference = 'Stop'
$canonical = 'C:\dev\worktrees\engine-revival-brender-evidence'
$source = 'C:\dev\public\engine-revival-workspaces\brender-v132'
$harness = 'C:\dev\public\engine-revival-workspaces\brender-v132-portable-core-harness-publication-2026-07-10'
$build = 'C:\dev\public\engine-revival-workspaces\brender-v132-portable-core-build-publication-2026-07-10'
$media = 'C:\dev\public\engine-revival-workspaces\brender-v132-publication-media-2026-07-10'
$transcript = 'C:\dev\public\engine-revival-workspaces\brender-v132-plotter-provenance-2026-07-10.txt'
$cli = 'C:\dev\worktrees\.venvs\engine-revival-brender-evidence\Scripts\engine-revival.exe'
$python = 'C:\dev\worktrees\.venvs\engine-revival-brender-evidence\Scripts\python.exe'
$codeCheckpoint = git -C $canonical rev-parse HEAD
$sourceHead = git -C $source rev-parse HEAD
$log = [System.Collections.Generic.List[string]]::new()

if ($sourceHead -ne 'd88d0ed41122664b9781015b517db64353e16f19') {
  throw 'Pinned source HEAD drifted immediately before reproduction'
}
if (@(git -C $source status --porcelain)) {
  throw 'Pinned source dirty immediately before reproduction'
}

function Invoke-LoggedNative([string]$display, [scriptblock]$command) {
  $script:log.Add("COMMAND: $display")
  $output = & $command 2>&1
  $exitCode = $LASTEXITCODE
  foreach ($line in $output) { $script:log.Add([string]$line) }
  $script:log.Add("EXIT: $exitCode")
  if ($exitCode -ne 0) {
    $script:log | Set-Content -LiteralPath $script:transcript -Encoding UTF8 -ErrorAction Stop
    throw "Native command failed: $display"
  }
}

$log.Add("canonical_code_checkpoint=$codeCheckpoint")
$log.Add("source_head=$sourceHead")
$log.Add("powershell=$($PSVersionTable.PSVersion)")
Invoke-LoggedNative 'git --version' { git --version }
Invoke-LoggedNative 'isolated python --version' { & $python --version }
Invoke-LoggedNative 'cmake --version' { cmake --version }
Invoke-LoggedNative 'materialize-brender-harness' { & $cli materialize-brender-harness --source-root $source --output-root $harness }
Invoke-LoggedNative 'cmake configure Win32' { cmake -S $harness -B $build -A Win32 "-DBRENDER_SOURCE_DIR=$source" }
Invoke-LoggedNative 'cmake build Debug all' { cmake --build $build --config Debug }
Invoke-LoggedNative 'ctest Debug complete ladder' { ctest --test-dir $build -C Debug --output-on-failure }
New-Item -ItemType Directory -Path $media -ErrorAction Stop | Out-Null
$svg = Join-Path $media 'brender-core-plotter-smoke.svg'
$ppm = Join-Path $media 'brender-core-plotter-preview.ppm'
$plotter = Join-Path $build 'Debug\brender_core_plotter_smoke.exe'
Invoke-LoggedNative 'direct plotter teapot.dat -> SVG + PPM' { & $plotter (Join-Path $source 'dat\teapot.dat') $svg $ppm }
$svgHash = (Get-FileHash -LiteralPath $svg -Algorithm SHA256).Hash.ToLowerInvariant()
$log.Add("svg_sha256=$svgHash")
$log.Add("svg_bytes=$((Get-Item -LiteralPath $svg).Length)")
$log | Set-Content -LiteralPath $transcript -Encoding UTF8 -ErrorAction Stop
```

Expected: clean materialization/configure/build succeeds, full CTest says 13/13, direct plotter exits 0, and transcript is written fail-closed without `Start-Transcript` user metadata.

- [ ] **Step 3: Verify exact media, transcript, and build facts**

```powershell
$build = 'C:\dev\public\engine-revival-workspaces\brender-v132-portable-core-build-publication-2026-07-10'
$media = 'C:\dev\public\engine-revival-workspaces\brender-v132-publication-media-2026-07-10'
$transcript = 'C:\dev\public\engine-revival-workspaces\brender-v132-plotter-provenance-2026-07-10.txt'
$svg = Join-Path $media 'brender-core-plotter-smoke.svg'
$selected = 'C:\dev\public\engine-revival-workspaces\brender-v132-portable-core-build-memory-compat-2026-07-09\brender-core-plotter-smoke.svg'
$hash = (Get-FileHash $svg -Algorithm SHA256).Hash.ToLowerInvariant()
if ($hash -ne 'e631561322d4ac3445d3b57e5d643bfd1bed6e59da57144c99738c00e0f61885') { throw "Reproduced SVG hash mismatch: $hash" }
if ((Get-Item $svg).Length -ne 56287) { throw 'Reproduced SVG size mismatch' }
$svgText = Get-Content -Raw -LiteralPath $svg
if ([regex]::Matches($svgText, '<line\b').Count -ne 1002) { throw 'SVG line count mismatch' }
foreach ($unsafe in @('<text', '<metadata', '<script', 'href=', 'file:', 'C:\\', 'Zain')) {
  if ($svgText -match [regex]::Escape($unsafe)) { throw "Unsafe SVG token: $unsafe" }
}
if ((Get-FileHash $selected -Algorithm SHA256).Hash.ToLowerInvariant() -ne $hash) { throw 'Selected/reproduced SVG mismatch' }
$selectedBytes = [IO.File]::ReadAllBytes($selected)
$reproducedBytes = [IO.File]::ReadAllBytes($svg)
if ([Convert]::ToBase64String($selectedBytes) -cne [Convert]::ToBase64String($reproducedBytes)) { throw 'Selected/reproduced SVG bytes differ' }
if ((Select-String -LiteralPath (Join-Path $build 'CMakeCache.txt') -SimpleMatch 'CMAKE_GENERATOR_PLATFORM:INTERNAL=Win32').Count -ne 1) { throw 'Build is not exact Win32 platform' }
$transcriptText = Get-Content -Raw -LiteralPath $transcript
if ($transcriptText -notmatch '100% tests passed, 0 tests failed out of 13') { throw 'Generation transcript lacks 13/13' }
$transcriptHash = (Get-FileHash $transcript -Algorithm SHA256).Hash.ToLowerInvariant()
"MEDIA_TRANSCRIPT_SHA256=$transcriptHash"
```

Expected: exact SVG hash/size/1,002-line content, safe SVG scan, byte-equivalent selected output, Win32 cache, 13/13 transcript, and one printed transcript SHA-256.

- [ ] **Step 4: Record durable ignored evidence and confirm no tracked changes**

Write `.superpowers/sdd/brender-publication-media-evidence.md` with exact `CODE_CHECKPOINT`, source commit, external paths, `MEDIA_TRANSCRIPT_SHA256`, SVG hash, size, line count, CTest summary, and the commands above. End with:

```text
Media evidence result: VERIFIED
```

Then run:

```powershell
if (@(git status --porcelain)) { throw 'Task 2 changed tracked files' }
```

Expected: evidence is durable in the ignored ledger; canonical tracked tree remains clean at the code checkpoint.

---

### Task 3: Promote the exact 13/13 state and deterministic public views

**Files:**

- Modify: `tests/test_brender_state_consistency.py`
- Modify: `tasks/brender-critical-edition-packet.json`
- Modify: `reproductions/brender-critical-edition-source-build.json`
- Modify: `builds/brender-v132-build-environment.json`
- Modify: `harnesses/brender-v132-portable-core-plan.json`
- Modify: `readiness/brender-production-readiness.json`
- Modify: `docs/BRENDER-ARCHIVAL.md`
- Regenerate: the exact 18 BRender-dependent paths under `docs/generated/`

**Interfaces:**

- Consumes: immutable successful attempt, final/RED transcript hashes, and Task 2 verified media evidence.
- Produces: identical checkpoint across five carriers, exact current statuses, positive field-complete claims/deferrals, thirteen-rung archival packet, and deterministic generated views.
- Preserves: score 86, packaging `not-started`, 3D Movie Maker pending, historical attempts/transcripts/pages, and frozen generation code.

- [ ] **Step 1: Replace the state test with an exact positive contract**

Keep `_load()`, but place `STATE_PATHS` before every mapping that consumes it. Replace the top-level constants, status tests, and current-claim assertions with this structure:

```python
ROOT = Path(__file__).resolve().parents[1]
STATE_PATHS = (
    "tasks/brender-critical-edition-packet.json",
    "reproductions/brender-critical-edition-source-build.json",
    "builds/brender-v132-build-environment.json",
    "harnesses/brender-v132-portable-core-plan.json",
    "readiness/brender-production-readiness.json",
)
EXPECTED_CHECKPOINT = {
    "id": "brender-v132-portable-core-memory-compat-2026-07-09",
    "stage": "portable-core-memory-compat-lane-passing",
    "passed": 13,
    "total": 13,
    "source_snapshot": "d88d0ed41122664b9781015b517db64353e16f19",
}
MEMORY_ATTEMPT_ID = "brender-v132-portable-core-memory-compat-smoke"
STATUS_EXPECTATIONS = {
    STATE_PATHS[0]: {"status": "portable-memory-compat-lane-verified"},
    STATE_PATHS[1]: {
        "status": "v132-portable-memory-compat-verified-3dmm-pending"
    },
    STATE_PATHS[2]: {"status": "portable-core-memory-compat-lane-passing"},
    STATE_PATHS[3]: {"status": "portable-core-memory-compat-lane-passing"},
    STATE_PATHS[4]: {
        "readiness_stage": "portable-core-memory-compat-lane-passing",
        "build_status": "portable-core-memory-compat-lane-built",
        "runtime_status": "memory-compat-lane-passing",
        "test_status": "memory-compat-lane-passing",
        "modernization_status": "portable-memory-compat-verified-x64-port-scoped",
    },
}
CLAIM_FIELDS = {
    STATE_PATHS[0]: ("public_notes", "outputs"),
    STATE_PATHS[1]: ("public_notes",),
    STATE_PATHS[2]: ("blockers", "next_actions", "public_notes"),
    STATE_PATHS[3]: (
        "expected_outputs",
        "implementation_units",
        "next_actions",
        "public_notes",
        "steps",
    ),
    STATE_PATHS[4]: ("blockers", "evidence", "next_actions", "public_notes"),
}
DELIVERED_TOKENS = (
    "1-4 byte pixel set/get",
    "non-black rgb888 fill",
    "positive-stride rectangular fill",
    "nonzero-start copy-bits",
    "_memcopybits_a",
    "brpixelmapcopybits",
)
DEFERRED_TOKENS = (
    "host/dos",
    "overlap",
    "negative",
    "colour-key",
    "fpu",
    "widths above four",
    "x64",
    "softrend",
    ".mat/.pal/.pix",
    "fixed",
    "drivers",
    "packag",
    "full viewer",
    "3d movie maker",
)


def _field_text(value):
    if isinstance(value, list):
        return " || ".join(value)
    return value


def test_brender_state_records_share_verified_memory_compat_checkpoint():
    for relative in STATE_PATHS:
        assert _load(relative)["evidence_checkpoint"] == EXPECTED_CHECKPOINT, relative


def test_brender_current_statuses_match_verified_scope():
    for relative, expected in STATUS_EXPECTATIONS.items():
        record = _load(relative)
        for key, value in expected.items():
            assert record[key] == value, (relative, key)
    readiness = _load(STATE_PATHS[4])
    assert readiness["flagship_score"] == 86
    assert readiness["packaging_status"] == "not-started"


def test_every_current_carrier_is_positive_complete_and_honest():
    for relative, fields in CLAIM_FIELDS.items():
        record = _load(relative)
        values = []
        for field in fields:
            value = record[field]
            assert value, (relative, field)
            text = _field_text(value)
            assert text.strip(), (relative, field)
            values.append(text)
            if not (relative == STATE_PATHS[4] and field == "evidence"):
                lowered = text.lower()
                assert "ctest 12/12" not in lowered, (relative, field)
                assert "twelve-rung" not in lowered, (relative, field)
                assert "twelve verifying" not in lowered, (relative, field)
                assert "semantic compatibility-stub coverage" not in lowered, (
                    relative,
                    field,
                )
        combined = " || ".join(values).lower()
        assert "thirteen" in combined and "13/13" in combined, relative
        for token in DELIVERED_TOKENS + DEFERRED_TOKENS:
            assert token in combined, (relative, token)


def test_memory_compat_attempt_is_promoted_and_plotter_evidence_is_historical():
    readiness = _load(STATE_PATHS[4])
    assert MEMORY_ATTEMPT_ID in readiness["attempt_ids"]
    assert any("CTest 13/13" in item for item in readiness["evidence"])
    old = [item for item in readiness["evidence"] if "CTest 12/12" in item]
    assert len(old) == 1
    assert old[0].startswith("Earlier 2026-07-03 plotter checkpoint:")


def test_brender_public_packet_describes_thirteen_rungs_and_deferrals():
    task = _load(STATE_PATHS[0])
    assert "CTest 13/13" in task["public_notes"]
    assert any("thirteen verifying" in output for output in task["outputs"])
    packet = (ROOT / "docs/BRENDER-ARCHIVAL.md").read_text(encoding="utf-8")
    assert "thirteen self-verifying portable-core smokes" in packet
    assert "13. Memory compatibility semantics" in packet
    assert "_MemCopyBits_A" in packet
    assert "BrPixelmapCopyBits" in packet
    deferred = packet.split("## Honestly deferred", 1)[1].split("## Records", 1)[0]
    for token in (
        "Host/DOS",
        "overlap",
        "negative",
        "colour-key",
        "FPU",
        "widths above four",
        "x64",
        "softrend",
        ".mat/.pal/.pix",
        "FIXED",
        "drivers",
        "packaging",
        "full viewer",
        "3D Movie Maker",
    ):
        assert token in deferred
```

Retain the existing isolated-CLI provenance test and archive reproduction assertions.

- [ ] **Step 2: Run the state test and verify RED**

```powershell
& 'C:\dev\worktrees\.venvs\engine-revival-brender-evidence\Scripts\python.exe' `
  -m pytest tests/test_brender_state_consistency.py -q
```

Expected: failures on the old checkpoint/status, missing positive contract text, old 12/12 summary, missing attempt promotion, and missing thirteenth archival rung. Import failure is not acceptable.

- [ ] **Step 3: Apply the exact checkpoint and statuses**

Set this exact object in task, reproduction, build, harness, and readiness:

```json
"evidence_checkpoint": {
  "id": "brender-v132-portable-core-memory-compat-2026-07-09",
  "stage": "portable-core-memory-compat-lane-passing",
  "passed": 13,
  "total": 13,
  "source_snapshot": "d88d0ed41122664b9781015b517db64353e16f19"
}
```

Apply every key/value in `STATUS_EXPECTATIONS`. Leave readiness `flagship_score` at `86` and `packaging_status` at `not-started`.

- [ ] **Step 4: Reconcile task and reproduction fields with complete claims**

Set task `public_notes` to:

```text
The BRender critical-edition portable pure-C lane remains published as the standalone brender-archival repository, while this feature branch verifies a thirteenth CTest rung at public v1.3.2 snapshot d88d0ed41122664b9781015b517db64353e16f19. CTest 13/13 passed on Win32 Debug. The new rung directly proves 1-4 byte pixel set/get, non-black RGB888 fill, positive-stride rectangular fill, and nonzero-start copy-bits through _MemCopyBits_A and BrPixelmapCopyBits; score remains 86. Deferred work remains explicit: Host/DOS fallback parity, overlap, negative strides, colour-key and FPU-wrapper parity, widths above four, x64 runtime safety, period softrend, native .mat/.pal/.pix resolution, FIXED variants, drivers, packaging, a full viewer, and the separate 3D Movie Maker build.
```

Append `"memory-compatibility CTest transcript"` to task `inputs`; replace its twelve-smoke output with:

```json
"thirteen verifying portable-core smokes (vector math through memory compatibility semantics; CTest 13/13, including 1-4 byte pixel set/get, non-black RGB888 fill, positive-stride rectangular fill, and nonzero-start copy-bits through _MemCopyBits_A and BrPixelmapCopyBits; Host/DOS, overlap, negative strides, colour-key, FPU, widths above four, x64, softrend, .mat/.pal/.pix, FIXED, drivers, packaging, full viewer, and 3D Movie Maker deferred)"
```

Set reproduction `public_notes` to:

```text
The public BRender v1.3.2 portable-core lane is verified through a thirteen-rung CTest 13/13 run at snapshot d88d0ed41122664b9781015b517db64353e16f19. The memory rung directly proves 1-4 byte pixel set/get, non-black RGB888 fill, positive-stride rectangular fill, and nonzero-start copy-bits through _MemCopyBits_A and BrPixelmapCopyBits. This reproduction does not claim Host/DOS fallback parity, overlap, negative strides, colour-key or FPU-wrapper parity, widths above four, x64, period softrend, native .mat/.pal/.pix, FIXED, drivers, packaging, or a full viewer. The separate 3D Movie Maker build remains pending and its step remains in the sequence.
```

- [ ] **Step 5: Reconcile build and harness operational arrays**

In the build record, replace the old blanket semantic blocker with:

```text
the thirteen-rung CTest 13/13 ladder directly covers 1-4 byte pixel set/get, non-black RGB888 fill, positive-stride rectangular fill, and nonzero-start copy-bits through _MemCopyBits_A and BrPixelmapCopyBits; Host/DOS fallback parity, overlap, negative strides, colour-key and FPU-wrapper parity, widths above four, drivers, period softrend, full renderer parity, x64, native .mat/.pal/.pix, FIXED, packaging, a full viewer, and 3D Movie Maker remain deferred
```

Replace the host/memory next action with:

```text
add separately named Host/DOS, overlap, negative-stride, colour-key, FPU-wrapper, and widths-above-four contracts; the four current memory primitives are covered by the thirteen-rung CTest 13/13 ladder while x64, softrend, .mat/.pal/.pix, FIXED, drivers, packaging, a full viewer, and 3D Movie Maker remain deferred
```

Set build `public_notes` to:

```text
BRender v1.3.2 public source at d88d0ed41122664b9781015b517db64353e16f19 builds out-of-tree as a CMake/MSVC Win32 FLOAT core and passes a thirteen-rung CTest 13/13 ladder. The new rung proves 1-4 byte pixel set/get, non-black RGB888 fill, positive-stride rectangular fill, and nonzero-start copy-bits through _MemCopyBits_A and BrPixelmapCopyBits. This build does not claim Host/DOS fallback parity, overlap, negative strides, colour-key or FPU-wrapper parity, widths above four, x64, period softrend, native .mat/.pal/.pix, FIXED, drivers, packaging, a full viewer, or the separate 3D Movie Maker build.
```

In the harness record:

- replace the compatibility blocker with the same complete delivered/deferred scope;
- append expected output `"memory compatibility smoke executable and CTest 13/13 transcript proving 1-4 byte pixel set/get, non-black RGB888 fill, positive-stride rectangular fill, and nonzero-start copy-bits through _MemCopyBits_A and BrPixelmapCopyBits; Host/DOS, overlap, negative strides, colour-key, FPU, widths above four, x64, softrend, .mat/.pal/.pix, FIXED, drivers, packaging, full viewer, and 3D Movie Maker deferred"`;
- append implementation unit `"core memory compatibility target: brender_core_memory_compat_smoke directly verifies 1-4 byte pixel set/get, non-black RGB888 fill, positive-stride rectangular fill, and nonzero-start copy-bits through _MemCopyBits_A and BrPixelmapCopyBits as rung thirteen of CTest 13/13; Host/DOS, overlap, negative strides, colour-key, FPU, widths above four, x64, softrend, .mat/.pal/.pix, FIXED, drivers, packaging, full viewer, and 3D Movie Maker deferred"`;
- replace the host/memory next action with `"add separately named Host/DOS, overlap, negative-stride, colour-key, FPU-wrapper, and widths-above-four contracts; the four current memory primitives are covered by the thirteen-rung CTest 13/13 ladder while x64, softrend, .mat/.pal/.pix, FIXED, drivers, packaging, a full viewer, and 3D Movie Maker remain deferred"`;
- replace the build step with `"build all thirteen brender_core_* smoke targets and run CTest 13/13 with the selected multi-config build configuration; the memory rung proves 1-4 byte pixel set/get, non-black RGB888 fill, positive-stride rectangular fill, and nonzero-start copy-bits through _MemCopyBits_A and BrPixelmapCopyBits while Host/DOS, overlap, negative strides, colour-key, FPU, widths above four, x64, softrend, .mat/.pal/.pix, FIXED, drivers, packaging, full viewer, and 3D Movie Maker remain deferred"`;
- set `public_notes` to:

```text
This public BRender harness materializes the pinned v1.3.2 source at d88d0ed41122664b9781015b517db64353e16f19 as an out-of-tree CMake/MSVC Win32 FLOAT core and passes a thirteen-rung CTest 13/13 ladder. The memory rung directly proves 1-4 byte pixel set/get, non-black RGB888 fill, positive-stride rectangular fill, and nonzero-start copy-bits through _MemCopyBits_A and BrPixelmapCopyBits. It does not claim Host/DOS fallback parity, overlap, negative strides, colour-key or FPU-wrapper parity, widths above four, x64, period softrend, native .mat/.pal/.pix, FIXED, drivers, packaging, a full viewer, or the separate 3D Movie Maker build.
```

- [ ] **Step 6: Reconcile readiness evidence and boundaries**

Replace the blanket compatibility blocker with:

```text
the thirteen-rung CTest 13/13 ladder directly verifies 1-4 byte pixel set/get, non-black RGB888 fill, positive-stride rectangular fill, and nonzero-start copy-bits through _MemCopyBits_A and BrPixelmapCopyBits; Host/DOS fallback parity, overlap, negative strides, colour-key and FPU-wrapper parity, widths above four, x64, period softrend, native .mat/.pal/.pix, FIXED, drivers, packaging, a full viewer, and 3D Movie Maker remain deferred
```

Prefix the existing 12/12 plotter evidence item exactly with:

```text
Earlier 2026-07-03 plotter checkpoint:
```

Append evidence:

```text
2026-07-09 rung thirteen reproduced the nonzero-start _MemCopyBits_A destination-offset defect, passed after the narrow index fix through both _MemCopyBits_A and BrPixelmapCopyBits, directly verified 1-4 byte pixel set/get, non-black RGB888 fill, and positive-stride rectangular fill, and completed CTest 13/13; Host/DOS, overlap, negative strides, colour-key, FPU, widths above four, x64, softrend, .mat/.pal/.pix, FIXED, drivers, packaging, full viewer, and 3D Movie Maker remain deferred
```

Append attempt ID `brender-v132-portable-core-memory-compat-smoke`. Replace the combined material/semantic next action with these two exact actions:

```json
"add separately named Host/DOS, overlap, negative-stride, colour-key, FPU-wrapper, and widths-above-four contract smokes after the thirteen-rung CTest 13/13 memory result; x64, softrend, FIXED, drivers, packaging, a full viewer, and 3D Movie Maker remain separate",
"resolve original materials/textures from period .mat/.pal/.pix files while preserving the verified 1-4 byte pixel, RGB888 fill, positive-stride rectangle, and _MemCopyBits_A/BrPixelmapCopyBits contracts"
```

Set readiness `public_notes` to:

```text
BRender's pinned public v1.3.2 snapshot now has a successful out-of-tree CMake/MSVC Win32 FLOAT core build and a thirteen-rung self-verifying CTest 13/13 ladder. Rung thirteen directly verifies 1-4 byte pixel set/get, non-black RGB888 fill, positive-stride rectangular fill, and nonzero-start copy-bits through _MemCopyBits_A and BrPixelmapCopyBits; flagship score remains 86 and packaging remains not-started. The portable path does not claim Host/DOS fallback parity, overlap, negative strides, colour-key or FPU-wrapper parity, widths above four, x64, period softrend, native .mat/.pal/.pix, FIXED, drivers, packaging, a full viewer, or the separate 3D Movie Maker build.
```

- [ ] **Step 7: Update the archival packet to thirteen rungs**

Change the ladder introduction to `thirteen self-verifying portable-core smokes` and add:

```markdown
13. Memory compatibility semantics: direct 1-4-byte pixel set/get, non-black
    RGB888 fill, positive-stride rectangular fill, and nonzero-start copy-bits
    through both `_MemCopyBits_A` and `BrPixelmapCopyBits`.
```

Add current capability:

```markdown
- Use directly verified portable memory primitives for 1-4-byte pixel access,
  RGB888 fill, positive-stride rectangle fill, and raw/public copy-bits dispatch.
```

Expand `Honestly deferred` with Host/DOS, overlap, negative strides, colour-key, FPU wrappers, widths above four, x64, `softrend`, `.mat/.pal/.pix`, FIXED, drivers, packaging, full viewer, and 3D Movie Maker.

- [ ] **Step 8: Run focused GREEN, validation, audit, and deterministic report generation**

```powershell
$python = 'C:\dev\worktrees\.venvs\engine-revival-brender-evidence\Scripts\python.exe'
$cli = 'C:\dev\worktrees\.venvs\engine-revival-brender-evidence\Scripts\engine-revival.exe'
& $python -m pytest tests/test_brender_state_consistency.py tests/test_brender_memory_compat_evidence.py tests/test_evidence_checkpoint_validate.py -q
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
& $cli validate --root .
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
& $cli audit-public --root .
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
& $cli report --root . | Out-Null
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
$first = Get-ChildItem docs\generated -Recurse -File | ForEach-Object {
  [pscustomobject]@{ Path=$_.FullName; Hash=(Get-FileHash $_.FullName -Algorithm SHA256).Hash }
}
& $cli report --root . | Out-Null
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
$second = Get-ChildItem docs\generated -Recurse -File | ForEach-Object {
  [pscustomobject]@{ Path=$_.FullName; Hash=(Get-FileHash $_.FullName -Algorithm SHA256).Hash }
}
if (Compare-Object $first $second -Property Path,Hash) { throw 'Second canonical report changed bytes' }
```

Expected: focused tests and CLI gates pass; second report is byte-identical.

- [ ] **Step 9: Require the exact generated-path delta and historical immutability**

```powershell
$expectedGenerated = @(
  'docs/generated/artifacts/brender-3dmm-source.md',
  'docs/generated/artifacts/brender-preservation-index.md',
  'docs/generated/artifacts/brender-v132-source.md',
  'docs/generated/attempts.md',
  'docs/generated/attempts/brender-v132-portable-core-memory-compat-smoke.md',
  'docs/generated/builds.md',
  'docs/generated/builds/brender-v132-build-environment.md',
  'docs/generated/coverage.md',
  'docs/generated/database.json',
  'docs/generated/harnesses.md',
  'docs/generated/harnesses/brender-v132-portable-core-plan.md',
  'docs/generated/packets.md',
  'docs/generated/packets/brender-critical-edition-packet.md',
  'docs/generated/production-readiness.md',
  'docs/generated/reproductions.md',
  'docs/generated/reproductions/brender-critical-edition-source-build.md',
  'docs/generated/targets/brender.md',
  'docs/generated/tasks.md'
) | Sort-Object
$actualGenerated = @(
  @(git diff --name-only -- docs/generated) +
  @(git ls-files --others --exclude-standard -- docs/generated)
) | Sort-Object -Unique
if (Compare-Object $expectedGenerated $actualGenerated) { throw 'Unexpected generated path set' }
$attemptDiff = @(git diff --name-only fde1f9a4621ea37f1a3f6dd4aa03e6943238d17b...HEAD -- attempts)
if ($attemptDiff.Count -ne 1 -or $attemptDiff[0] -ne 'attempts/brender-v132-portable-core-memory-compat-smoke.json') { throw 'Historical attempt changed' }
$historicalPageDiff = @(git diff --name-only -- docs/generated/attempts | Where-Object { $_ -ne 'docs/generated/attempts/brender-v132-portable-core-memory-compat-smoke.md' })
if ($historicalPageDiff) { throw "Historical attempt page changed: $($historicalPageDiff -join ', ')" }
```

Expected: exactly 18 generated paths change, only the new attempt record differs across the feature range, and no earlier generated attempt page changes.

- [ ] **Step 10: Run full gates and commit the promoted state**

```powershell
$python = 'C:\dev\worktrees\.venvs\engine-revival-brender-evidence\Scripts\python.exe'
$cli = 'C:\dev\worktrees\.venvs\engine-revival-brender-evidence\Scripts\engine-revival.exe'
& $python -m pytest -q
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
& $cli validate --root .
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
& $cli audit-public --root .
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
git diff --check
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
git add tests/test_brender_state_consistency.py `
  tasks/brender-critical-edition-packet.json `
  reproductions/brender-critical-edition-source-build.json `
  builds/brender-v132-build-environment.json `
  harnesses/brender-v132-portable-core-plan.json `
  readiness/brender-production-readiness.json `
  docs/BRENDER-ARCHIVAL.md docs/generated
git diff --cached --check
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
git commit -m "docs: publish BRender 13-rung checkpoint"
```

Expected: one state/docs/generated commit; frozen generation paths remain unchanged from `CODE_CHECKPOINT`.

---

### Task 4: Publish the canonical demo, media, license, and outcome surface

**Files:**

- Create: `tests/test_brender_publish_surface.py`
- Create: `.gitattributes`
- Modify: `README.md`
- Create: `docs/BRENDER-DEMO.md`
- Create: `docs/media/brender/brender-core-plotter-smoke.svg`
- Create: `docs/media/brender/LICENSE-BRENDER-MIT.txt`
- Create: `docs/media/brender/README.md`
- Modify: `docs/superpowers/specs/2026-07-09-brender-memory-compatibility-design.md`
- Modify: `docs/superpowers/plans/2026-07-09-brender-memory-compatibility.md`
- Modify: `docs/superpowers/specs/2026-07-09-brender-publication-design.md`

**Interfaces:**

- Consumes: exact `CODE_CHECKPOINT`, `MEDIA_TRANSCRIPT_SHA256`, source snapshot, existing transcript hashes, and reproduced SVG from Task 2.
- Produces: durable public README/demo/media/license/provenance/outcome contract shared with the archive.
- Preserves: original memory design/plan text, historical evidence-consistency documents, frozen generation paths, and no-vendoring boundary.

- [ ] **Step 1: Add the failing publication-surface test**

Create `tests/test_brender_publish_surface.py` with:

```python
import hashlib
import re
import subprocess
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
README = ROOT / "README.md"
DEMO = ROOT / "docs" / "BRENDER-DEMO.md"
MEDIA_DIR = ROOT / "docs" / "media" / "brender"
SVG = MEDIA_DIR / "brender-core-plotter-smoke.svg"
MEDIA_README = MEDIA_DIR / "README.md"
MEDIA_LICENSE = MEDIA_DIR / "LICENSE-BRENDER-MIT.txt"
SOURCE_HEAD = "d88d0ed41122664b9781015b517db64353e16f19"
SVG_SHA256 = "e631561322d4ac3445d3b57e5d643bfd1bed6e59da57144c99738c00e0f61885"
LICENSE_SHA256 = "f604769ea503e060b146607f1a8ebc1ce24daec3f75a2b1cbd9e2ab8ac3223fa"
OUTCOME_MARKER = "## Verified outcome — 2026-07-10"
NON_CLAIMS = (
    "Host/DOS fallback parity",
    "overlap",
    "negative strides",
    "colour-key",
    "FPU-wrapper",
    "widths above four",
    "x64",
    "softrend",
    ".mat/.pal/.pix",
    "FIXED",
    "drivers",
    "packaging",
    "full viewer",
    "3D Movie Maker",
)
GALLERY_BLOBS = {
    "gallery/01-wireframe-cube.png": "7caa3fdffba2666ead882f0ad4aca5cd9e3c9d2a669c7867eebd170c5956f1ce",
    "gallery/02-scene-graph-cube.png": "e0d61cd295ce3115065ac1d2bef07acf1e4788c5bba05c4adb80977ea95c59a1",
    "gallery/03-solid-shaded.png": "99263cabcea1b273af0018b8ec94bcfd38c03155c2d04027dade846f4f5c1d66",
    "gallery/04-depth-buffer.png": "1417dde07f043fb94d8866a3c9a6aa77ece0d7707e787f0a9a887e404cea07a2",
    "gallery/05-texture-mapped.png": "d4ebf0d9db580e802ec01f2585375cbbac0772229182d0010eafe777f3930c92",
    "gallery/06-datafile-models.png": "6905626a2151ba7f0ff40eb6acfa16388e226d096b4f48cab63763cda0b97972",
    "gallery/07-uv-textured-globe.png": "550ac52aa751d759002b0fb4fb5054541049ff9165f8db327322ac1724e55522",
    "gallery/08-multipart-coupe.png": "d2be455e1110139068d449d3a083576130dfeffabc287f2bb70e1e49a8fb4745",
    "gallery/09-gouraud-sphere.png": "66fb2058d3d4e3f920c2ee11f4f30fec1bc6ce17f06222fa75067ea87e6bbac0",
    "gallery/10-teapot-plotter.png": "c7f0a0df1cab4c5e3ec42cf034bca1fd91934ce1f83318f53faff5891d5d095c",
    "gallery/10-teapot-plotter.svg": SVG_SHA256,
}


def _sha256(data):
    return hashlib.sha256(data).hexdigest()


def _head_blob(relative):
    return subprocess.check_output(
        ["git", "show", f"HEAD:{relative}"], cwd=ROOT
    )


def test_root_readme_presents_verified_brender_release():
    text = README.read_text(encoding="utf-8")
    assert SOURCE_HEAD in text
    assert "CTest 13/13" in text
    for token in (
        "1-4 byte pixel set/get",
        "non-black RGB888 fill",
        "positive-stride rectangular fill",
        "nonzero-start copy-bits",
    ) + NON_CLAIMS:
        assert token in text
    assert "![BRender teapot hidden-line plotter output]" in text
    assert "docs/media/brender/brender-core-plotter-smoke.svg" in text
    assert "docs/BRENDER-DEMO.md" in text
    assert "docs/media/brender/README.md" in text


def test_demo_is_pinned_external_and_complete():
    text = DEMO.read_text(encoding="utf-8")
    for prerequisite in (
        "Git",
        "Python 3.11",
        "CMake 3.20",
        "Visual Studio",
        "Desktop development with C++",
        "Win32",
        "Windows PowerShell 5.1",
    ):
        assert prerequisite in text
    assert "https://github.com/foone/BRender-v1.3.2.git" in text
    assert SOURCE_HEAD in text
    assert "[IO.Path]::GetTempPath()" in text
    for variable in ("$source", "$harness", "$build", "$output"):
        assert variable in text
    for target in (
        "brender_core_plotter_smoke",
        "brender_core_gouraud_smoke",
        "brender_core_texture_smoke",
        "brender_core_memory_compat_smoke",
    ):
        assert target in text
    assert "--no-tests=error" in text
    assert "PPM is the native raw evidence format" in text
    assert "byte-identical real-run artifact" in text


def test_media_and_file_specific_license_are_exact():
    assert SVG.stat().st_size == 56287
    assert _sha256(SVG.read_bytes()) == SVG_SHA256
    svg = SVG.read_text(encoding="utf-8")
    assert 'width="640" height="480"' in svg
    assert len(re.findall(r"<line\b", svg)) == 1002
    for unsafe in ("<text", "<metadata", "<script", "href=", "file:", "C:\\", "Zain"):
        assert unsafe not in svg
    assert _sha256(MEDIA_LICENSE.read_bytes()) == LICENSE_SHA256
    license_text = MEDIA_LICENSE.read_text(encoding="utf-8")
    assert "MIT License" in license_text
    assert "Copyright (c) 1998 Argonaut Software Limited" in license_text
    attrs = (ROOT / ".gitattributes").read_text(encoding="utf-8")
    assert "docs/media/brender/brender-core-plotter-smoke.svg -text" in attrs
    assert "docs/media/brender/LICENSE-BRENDER-MIT.txt -text" in attrs


def test_media_provenance_is_complete_and_transcript_bound():
    text = MEDIA_README.read_text(encoding="utf-8")
    for token in (
        "dat/teapot.dat",
        SOURCE_HEAD,
        SVG_SHA256,
        LICENSE_SHA256,
        "ccd859efa4e24b11844a422d3d62199cf8d4ba1e",
        "b54d60fab4346810687bc892badf31b3df392ec9",
        "https://github.com/foone/BRender-v1.3.2",
        "LICENSE-BRENDER-MIT.txt",
        "MIT License",
        "Copyright (c) 1998 Argonaut Software Limited",
        "AGPL-3.0-or-later",
        "No BRender source model or binary is vendored",
        "brender-v132-plotter-provenance-2026-07-10.txt",
    ):
        assert token in text
    assert re.search(r"Generation transcript SHA-256: `[0-9a-f]{64}`", text)
    assert re.search(r"Media-generation code checkpoint: `[0-9a-f]{40}`", text)


def test_memory_design_and_plan_have_verified_outcomes():
    for relative in (
        "docs/superpowers/specs/2026-07-09-brender-memory-compatibility-design.md",
        "docs/superpowers/plans/2026-07-09-brender-memory-compatibility.md",
    ):
        text = (ROOT / relative).read_text(encoding="utf-8")
        assert OUTCOME_MARKER in text, relative
        assert "CTest 13/13" in text, relative
        assert "592559a41e77a0d51c378e678f8f17884be439c14954ba16ec4b5e90790be1f4" in text, relative


def test_publication_design_records_operator_approval_and_plan():
    text = (
        ROOT
        / "docs/superpowers/specs/2026-07-09-brender-publication-design.md"
    ).read_text(encoding="utf-8")
    assert "Status: Approved by operator on 2026-07-10" in text
    assert "../plans/2026-07-10-brender-publication.md" in text


def test_archive_gallery_has_exact_committed_blob_inventory_when_present():
    gallery = ROOT / "gallery"
    if not gallery.is_dir():
        return
    inventory = gallery / "README.md"
    assert inventory.exists()
    text = inventory.read_text(encoding="utf-8")
    for relative, expected in GALLERY_BLOBS.items():
        assert relative.removeprefix("gallery/") in text
        assert expected in text
        assert _sha256(_head_blob(relative)) == expected
    assert "unknown" in text
```

- [ ] **Step 2: Run the new test and verify the intended RED**

```powershell
& 'C:\dev\worktrees\.venvs\engine-revival-brender-evidence\Scripts\python.exe' `
  -m pytest tests/test_brender_publish_surface.py -q
```

Expected: missing `docs/BRENDER-DEMO.md`, media, provenance, license, and outcome marker failures. Import failure is not acceptable.

- [ ] **Step 3: Add stable media attributes and the exact upstream license blob**

Create `.gitattributes` with exactly:

```gitattributes
docs/media/brender/brender-core-plotter-smoke.svg -text
docs/media/brender/LICENSE-BRENDER-MIT.txt -text -diff
```

The `-diff` attribute is required because the upstream blob intentionally has
one trailing space on its copyright line; it preserves the exact public notice
without making `git diff --check` misclassify that immutable byte as an edit.
Extract the raw license blob mechanically with `git cat-file blob` captured and
written as bytes, never through the CRLF-filtered source worktree or a filtered
archive export (which applies the host checkout conversion here):

```powershell
$source = 'C:\dev\public\engine-revival-workspaces\brender-v132'
$python = 'C:\dev\worktrees\.venvs\engine-revival-brender-evidence\Scripts\python.exe'
$copyRaw = @'
import pathlib, subprocess, sys
source, target = sys.argv[1:]
data = subprocess.check_output([
    "git", "-C", source, "cat-file", "blob",
    "d88d0ed41122664b9781015b517db64353e16f19:LICENSE",
])
path = pathlib.Path(target)
path.parent.mkdir(parents=True, exist_ok=True)
path.write_bytes(data)
'@
& $python -c $copyRaw $source 'docs\media\brender\LICENSE-BRENDER-MIT.txt'
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
```

This narrow mechanical byte copy is intentional: `apply_patch` and ordinary
checkout/archive copies cannot preserve the upstream blob's exact filtered and
trailing-byte semantics on this Windows host.

Copy the reproduced SVG as a bulk mechanical asset copy:

```powershell
$sourceSvg = 'C:\dev\public\engine-revival-workspaces\brender-v132-publication-media-2026-07-10\brender-core-plotter-smoke.svg'
$targetSvg = 'C:\dev\worktrees\engine-revival-brender-evidence\docs\media\brender\brender-core-plotter-smoke.svg'
Copy-Item -LiteralPath $sourceSvg -Destination $targetSvg
```

Immediately require both exact raw hashes:

```powershell
if ((Get-FileHash docs\media\brender\brender-core-plotter-smoke.svg -Algorithm SHA256).Hash.ToLowerInvariant() -ne 'e631561322d4ac3445d3b57e5d643bfd1bed6e59da57144c99738c00e0f61885') { throw 'Tracked SVG bytes drifted' }
if ((Get-FileHash docs\media\brender\LICENSE-BRENDER-MIT.txt -Algorithm SHA256).Hash.ToLowerInvariant() -ne 'f604769ea503e060b146607f1a8ebc1ce24daec3f75a2b1cbd9e2ab8ac3223fa') { throw 'Tracked MIT notice bytes drifted' }
```

- [ ] **Step 4: Add the canonical README release section**

Replace the existing short `BRender Harness` section with:

```markdown
## BRender: verified 13-rung portable core

At public BRender v1.3.2 snapshot
`d88d0ed41122664b9781015b517db64353e16f19`, the out-of-tree MSVC Win32
FLOAT-core harness passes CTest 13/13. The newest rung verifies 1-4 byte pixel
set/get, non-black RGB888 fill, positive-stride rectangular fill, and
nonzero-start copy-bits through raw and public dispatch.

![BRender teapot hidden-line plotter output](docs/media/brender/brender-core-plotter-smoke.svg)

[Run the complete demo](docs/BRENDER-DEMO.md) ·
[Media provenance and license](docs/media/brender/README.md) ·
[Archival packet](docs/BRENDER-ARCHIVAL.md)

This is a verified portable-core milestone, not a complete engine port. It does
not claim Host/DOS fallback parity, overlap, negative strides, colour-key or
FPU-wrapper parity, widths above four, x64, period `softrend`, native
`.mat/.pal/.pix`, FIXED variants, drivers, packaging, a full viewer, or the
separate 3D Movie Maker build.

```powershell
engine-revival materialize-brender-harness `
  --source-root C:\path\to\BRender-v1.3.2 `
  --output-root C:\path\to\brender-v132-portable-core-harness
```
```

- [ ] **Step 5: Create the complete external-only demo guide**

Create `docs/BRENDER-DEMO.md` with these exact sections and commands:

```markdown
# BRender Portable-Core Demo

This demo reproduces the verified Win32 FLOAT-core ladder from public source.
All source, harness, build, and output files stay beneath a temporary external
directory; none is written into the `engine-revival` checkout.

## Prerequisites

- Git.
- Python 3.11 or newer.
- CMake 3.20 or newer.
- Visual Studio with **Desktop development with C++** and the **Win32** target.
- Windows PowerShell 5.1 (`powershell.exe`). `pwsh.exe` was not present on the
  verification host, but its absence was not a BRender build or CTest failure.

Run the following from the root of a clean `engine-revival` checkout:

```powershell
$ErrorActionPreference = 'Stop'
$engineRevival = (Resolve-Path .).Path
$work = Join-Path ([IO.Path]::GetTempPath()) 'brender-v132-demo'
if (Test-Path -LiteralPath $work) { throw "Choose a fresh demo root: $work" }
New-Item -ItemType Directory -Path $work | Out-Null
$source = Join-Path $work 'BRender-v1.3.2'
$harness = Join-Path $work 'harness'
$build = Join-Path $work 'build'
$output = Join-Path $work 'output'
$venv = Join-Path $work '.venv'

git clone https://github.com/foone/BRender-v1.3.2.git $source
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
git -C $source checkout --detach d88d0ed41122664b9781015b517db64353e16f19
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
if ((git -C $source rev-parse HEAD) -ne 'd88d0ed41122664b9781015b517db64353e16f19') { throw 'Source pin failed' }

python -m venv $venv
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
& (Join-Path $venv 'Scripts\python.exe') -m pip install -e "$engineRevival[test]"
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
$cli = Join-Path $venv 'Scripts\engine-revival.exe'
& $cli materialize-brender-harness --source-root $source --output-root $harness
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
cmake -S $harness -B $build -A Win32 "-DBRENDER_SOURCE_DIR=$source"
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
cmake --build $build --config Debug
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
ctest --test-dir $build -C Debug --output-on-failure
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
New-Item -ItemType Directory -Path $output | Out-Null
```

## Direct visual demos

```powershell
& (Join-Path $build 'Debug\brender_core_plotter_smoke.exe') `
  (Join-Path $source 'dat\teapot.dat') `
  (Join-Path $output 'teapot-plotter.svg') `
  (Join-Path $output 'teapot-plotter-preview.ppm')
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

& (Join-Path $build 'Debug\brender_core_gouraud_smoke.exe') `
  (Join-Path $source 'dat\sph32.dat') `
  (Join-Path $output 'gouraud-sphere.ppm')
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

& (Join-Path $build 'Debug\brender_core_texture_smoke.exe') `
  (Join-Path $output 'texture-cube.ppm')
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
```

PPM is the native raw evidence format emitted by the raster demos. The tracked
SVG is a byte-identical real-run artifact; no PNG conversion belongs to this
release.

## Memory-semantic demo

```powershell
& (Join-Path $build 'Debug\brender_core_memory_compat_smoke.exe')
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
ctest --test-dir $build -C Debug -R '^brender_core_memory_compat_smoke$' `
  --no-tests=error --output-on-failure
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
```

This target proves only 1-4 byte pixel set/get, non-black RGB888 fill,
positive-stride rectangular fill, and nonzero-start copy-bits through
`_MemCopyBits_A` and `BrPixelmapCopyBits`. Host/DOS fallback parity, overlap,
negative strides, colour-key and FPU-wrapper parity, widths above four, x64,
period `softrend`, native `.mat/.pal/.pix`, FIXED, drivers, packaging, a full
viewer, and 3D Movie Maker remain deferred.
```

- [ ] **Step 6: Create exact media provenance using Task 2 outputs**

Read `CODE_CHECKPOINT` from `.superpowers/sdd/brender-publication-code-review.md` and compute `MEDIA_TRANSCRIPT_SHA256` from the external generation transcript. Create `docs/media/brender/README.md` with:

```markdown
# BRender Plotter Media Provenance

![BRender teapot hidden-line plotter output](brender-core-plotter-smoke.svg)

- Output: `brender-core-plotter-smoke.svg`.
- Output SHA-256: `e631561322d4ac3445d3b57e5d643bfd1bed6e59da57144c99738c00e0f61885`.
- Dimensions: 640 by 480; 56,287 bytes; 1,002 `<line>` elements.
- Input: `dat/teapot.dat`, read in place from the public checkout.
- Public source: https://github.com/foone/BRender-v1.3.2.
- Source snapshot: `d88d0ed41122664b9781015b517db64353e16f19`.
- Harness feature commit: `ccd859efa4e24b11844a422d3d62199cf8d4ba1e`.
- Compatibility-fix commit: `b54d60fab4346810687bc892badf31b3df392ec9`.
- Media-generation code checkpoint: the exact reviewed `CODE_CHECKPOINT` from Task 1.
- Generation transcript: `external-workspace:C:\dev\public\engine-revival-workspaces\brender-v132-plotter-provenance-2026-07-10.txt`.
- Generation transcript SHA-256: the exact lowercase `MEDIA_TRANSCRIPT_SHA256` produced by Task 2.

The clean reproduction materialized the harness from the code checkpoint,
configured Visual Studio Win32 Debug, built the complete ladder, passed CTest
13/13, and direct-ran `brender_core_plotter_smoke` with `dat/teapot.dat`. The
reproduced output matched the tracked SVG byte-for-byte by SHA-256.

The SVG is distributed under the upstream [MIT License](LICENSE-BRENDER-MIT.txt),
raw license Git-blob SHA-256
`f604769ea503e060b146607f1a8ebc1ce24daec3f75a2b1cbd9e2ab8ac3223fa`,
Copyright (c) 1998 Argonaut Software Limited. That file-specific MIT notice
remains attached in the MIT-licensed canonical repository and in the
AGPL-3.0-or-later standalone archive; the archive project license does not
replace it.

No BRender source model or binary is vendored. The source checkout, model,
harness, build tree, executable, library, PDB, PPM preview, and transcript stay
external.
```

Replace the two explanatory Task-value phrases with the actual 40- and 64-character lowercase values before saving. Do not leave symbolic names in the tracked document.

- [ ] **Step 7: Append verified outcomes without rewriting historical text**

Append this block to both the memory compatibility design and plan, substituting the actual `CODE_CHECKPOINT` and `MEDIA_TRANSCRIPT_SHA256` values:

```markdown
## Verified outcome — 2026-07-10

- Implementation commits: `ccd859efa4e24b11844a422d3d62199cf8d4ba1e`,
  `b54d60fab4346810687bc892badf31b3df392ec9`,
  `0a7f0de1bba73340ae4bcfb36b49f8e7d08a6ada`, and
  `cdb985f66e2ea9dfa9b42b610979c0bffb8b90bc`.
- Reviewed media-generation code checkpoint: the exact `CODE_CHECKPOINT`.
- Pinned public source: `d88d0ed41122664b9781015b517db64353e16f19`.
- Complete Win32 Debug result: CTest 13/13; transcript SHA-256
  `592559a41e77a0d51c378e678f8f17884be439c14954ba16ec4b5e90790be1f4`.
- Pre-fix RED transcript SHA-256:
  `7ec18fe16bc591980ea5eafc970abec25a894338f759fce3cb0e1f9cd9cd6b60`.
- Reproduced plotter generation transcript SHA-256: the exact
  `MEDIA_TRANSCRIPT_SHA256`.
- Review state: command/harness code checkpoint independently approved; final
  whole-branch publication review remains required before archive sync.
- Publication scope: source, evidence, docs, demo, one real SVG, and draft PRs;
  no binary package or claim of a complete engine port.
- Deferred: Host/DOS parity, overlap, negative strides, colour-key/FPU parity,
  widths above four, x64, `softrend`, `.mat/.pal/.pix`, FIXED, drivers,
  packaging, full viewer, and 3D Movie Maker.
```

Replace both explanatory value phrases with exact lowercase hashes before saving. Confirm `docs/superpowers/specs/2026-07-09-brender-evidence-consistency-design.md` and `docs/superpowers/plans/2026-07-09-brender-evidence-consistency.md` remain byte-identical to `37efdcb`.

In `docs/superpowers/specs/2026-07-09-brender-publication-design.md`, replace the two-line awaiting-review status with:

```markdown
Status: Approved by operator on 2026-07-10; implementation governed by
[the publication plan](../plans/2026-07-10-brender-publication.md)
```

- [ ] **Step 8: Run focused GREEN and all canonical publication checks**

```powershell
$python = 'C:\dev\worktrees\.venvs\engine-revival-brender-evidence\Scripts\python.exe'
$cli = 'C:\dev\worktrees\.venvs\engine-revival-brender-evidence\Scripts\engine-revival.exe'
& $python -m pytest tests/test_brender_publish_surface.py tests/test_brender_state_consistency.py tests/test_brender_harness_materializer.py tests/test_brender_memory_compat_evidence.py -q
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
& $python -m pytest -q
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
& $cli validate --root .
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
& $cli audit-public --root .
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
git diff --check
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
```

Expected: focused tests, full suite, validate, audit, and diff checks pass.

- [ ] **Step 9: Verify frozen code, raw media bytes, and historical docs before commit**

```powershell
$review = Get-Content -Raw -LiteralPath '.superpowers\sdd\brender-publication-code-review.md'
if ($review -notmatch '(?m)^Reviewed code checkpoint: ([0-9a-f]{40})\r?\nReview result: APPROVED\r?\n?\z') { throw 'Malformed code review' }
$codeCheckpoint = $Matches[1]
$frozenDiff = @(git diff --name-only "$codeCheckpoint...HEAD" -- `
  src/engine_revival/brender_harness.py `
  src/engine_revival/brender_harness_templates.py `
  'src/engine_revival/brender*_sources.py' `
  src/engine_revival/cli.py)
if ($frozenDiff) { throw "Generation code changed: $($frozenDiff -join ', ')" }
if ((Get-FileHash docs\media\brender\brender-core-plotter-smoke.svg -Algorithm SHA256).Hash.ToLowerInvariant() -ne 'e631561322d4ac3445d3b57e5d643bfd1bed6e59da57144c99738c00e0f61885') { throw 'SVG drift' }
if ((Get-FileHash docs\media\brender\LICENSE-BRENDER-MIT.txt -Algorithm SHA256).Hash.ToLowerInvariant() -ne 'f604769ea503e060b146607f1a8ebc1ce24daec3f75a2b1cbd9e2ab8ac3223fa') { throw 'MIT notice drift' }
if (@(git diff --name-only 37efdcbe8f9379de3e2a10b78f57e54076a57040 -- docs/superpowers/specs/2026-07-09-brender-evidence-consistency-design.md docs/superpowers/plans/2026-07-09-brender-evidence-consistency.md)) { throw 'Historical evidence-consistency docs changed' }
```

Expected: frozen generation paths, media bytes, license bytes, and historical 12/12 design documents are unchanged as required.

- [ ] **Step 10: Commit the public surface**

```powershell
git add .gitattributes README.md docs/BRENDER-DEMO.md docs/media/brender `
  tests/test_brender_publish_surface.py `
  docs/superpowers/specs/2026-07-09-brender-memory-compatibility-design.md `
  docs/superpowers/plans/2026-07-09-brender-memory-compatibility.md `
  docs/superpowers/specs/2026-07-09-brender-publication-design.md
git diff --cached --check
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
git commit -m "docs: publish BRender demo and provenance"
```

Expected: one docs/media/test commit; the SVG and MIT notice are exact raw blobs and no generation-relevant code changes after `CODE_CHECKPOINT`.

---

### Task 5: Prove and independently approve the complete canonical PR range

**Files:**

- Ignored review: `.superpowers/sdd/brender-publication-canonical-review.md`
- No tracked changes unless review corrections are required.

**Interfaces:**

- Consumes: Tasks 1-4 committed canonical HEAD, code checkpoint review, media evidence, exact transcript/media/license hashes, and deterministic records.
- Produces: exact-HEAD approval for the complete `6f2361d...canonical_HEAD` prospective PR range.
- Blocks: archive sync while any Critical/Important finding or stale gate remains.

- [ ] **Step 1: Re-run deterministic reports and require a clean tree**

```powershell
$cli = 'C:\dev\worktrees\.venvs\engine-revival-brender-evidence\Scripts\engine-revival.exe'
& $cli report --root . | Out-Null
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
$before = Get-ChildItem docs\generated -Recurse -File | ForEach-Object {
  [pscustomobject]@{Path=$_.FullName; Hash=(Get-FileHash $_.FullName -Algorithm SHA256).Hash}
}
& $cli report --root . | Out-Null
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
$after = Get-ChildItem docs\generated -Recurse -File | ForEach-Object {
  [pscustomobject]@{Path=$_.FullName; Hash=(Get-FileHash $_.FullName -Algorithm SHA256).Hash}
}
if (Compare-Object $before $after -Property Path,Hash) { throw 'Report generation drifted' }
if (@(git status --porcelain)) { throw 'Canonical tree changed during final report gate' }
```

Expected: both report runs are byte-identical and committed tree stays clean.

- [ ] **Step 2: Run full local and external evidence gates**

```powershell
$python = 'C:\dev\worktrees\.venvs\engine-revival-brender-evidence\Scripts\python.exe'
$cli = 'C:\dev\worktrees\.venvs\engine-revival-brender-evidence\Scripts\engine-revival.exe'
& $python -m pytest -q
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
& $cli validate --root .
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
& $cli audit-public --root .
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
git diff --check 6f2361d478c85a39b2cd146e3c94acd2127870f0...HEAD
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
$checks = @{
  'C:\dev\public\engine-revival-workspaces\brender-v132-portable-core-ctest-memory-compat-2026-07-09.txt' = '592559a41e77a0d51c378e678f8f17884be439c14954ba16ec4b5e90790be1f4'
  'C:\dev\public\engine-revival-workspaces\brender-v132-portable-core-ctest-memory-compat-red-2026-07-09.txt' = '7ec18fe16bc591980ea5eafc970abec25a894338f759fce3cb0e1f9cd9cd6b60'
  'docs\media\brender\brender-core-plotter-smoke.svg' = 'e631561322d4ac3445d3b57e5d643bfd1bed6e59da57144c99738c00e0f61885'
  'docs\media\brender\LICENSE-BRENDER-MIT.txt' = 'f604769ea503e060b146607f1a8ebc1ce24daec3f75a2b1cbd9e2ab8ac3223fa'
}
foreach ($path in $checks.Keys) {
  $actual = (Get-FileHash $path -Algorithm SHA256).Hash.ToLowerInvariant()
  if ($actual -ne $checks[$path]) { throw "Fixity failure: $path" }
}
$mediaReadme = Get-Content -Raw -LiteralPath 'docs\media\brender\README.md'
if ($mediaReadme -notmatch '(?m)^- Generation transcript SHA-256: `([0-9a-f]{64})`\.\r?$') {
  throw 'Tracked media provenance lacks one exact generation transcript hash'
}
$trackedTranscriptHash = $Matches[1]
$externalTranscript = 'C:\dev\public\engine-revival-workspaces\brender-v132-plotter-provenance-2026-07-10.txt'
$externalTranscriptHash = (Get-FileHash -LiteralPath $externalTranscript -Algorithm SHA256).Hash.ToLowerInvariant()
if ($trackedTranscriptHash -ne $externalTranscriptHash) {
  throw 'Tracked generation transcript hash does not bind the external transcript'
}
$mediaEvidence = Get-Content -Raw -LiteralPath '.superpowers\sdd\brender-publication-media-evidence.md'
if ($mediaEvidence -notmatch '(?m)^Media evidence result: VERIFIED\r?\n?\z') { throw 'Media evidence is unverified' }
```

Expected: full suite, validate, audit, diff, transcript, SVG, license, and ignored evidence gates pass.

- [ ] **Step 3: Prove frozen generation code and historical immutability**

```powershell
$codeReview = Get-Content -Raw -LiteralPath '.superpowers\sdd\brender-publication-code-review.md'
if ($codeReview -notmatch '(?m)^Reviewed code checkpoint: ([0-9a-f]{40})\r?\nReview result: APPROVED\r?\n?\z') { throw 'Malformed code review' }
$codeCheckpoint = $Matches[1]
$frozen = @(git diff --name-only "$codeCheckpoint...HEAD" -- `
  src/engine_revival/brender_harness.py `
  src/engine_revival/brender_harness_templates.py `
  'src/engine_revival/brender*_sources.py' `
  src/engine_revival/cli.py)
if ($frozen) { throw "Frozen generation path changed: $($frozen -join ', ')" }
$attempts = @(git diff --name-only fde1f9a4621ea37f1a3f6dd4aa03e6943238d17b...HEAD -- attempts)
if ($attempts.Count -ne 1 -or $attempts[0] -ne 'attempts/brender-v132-portable-core-memory-compat-smoke.json') { throw 'Historical attempt drift' }
if (@(git diff --name-only 37efdcbe8f9379de3e2a10b78f57e54076a57040...HEAD -- docs/superpowers/specs/2026-07-09-brender-evidence-consistency-design.md docs/superpowers/plans/2026-07-09-brender-evidence-consistency.md)) { throw 'Historical evidence-consistency docs drift' }
```

Expected: generation paths are identical to the reviewed checkpoint; only the new memory attempt differs; earlier design records are unchanged.

- [ ] **Step 4: Audit the complete prospective PR commit/path scope**

```powershell
$base = '6f2361d478c85a39b2cd146e3c94acd2127870f0'
git merge-base --is-ancestor $base HEAD
if ($LASTEXITCODE -ne 0) { throw 'Canonical PR base is not an ancestor' }
$commits = @(git log --format='%H %s' "$base..HEAD")
$paths = @(git diff --name-only "$base...HEAD")
if (-not $commits -or -not $paths) { throw 'Prospective canonical PR is empty' }
$forbidden = @($paths | Where-Object {
  $_ -like 'local-model/*' -or $_ -like 'public/index/*' -or
  $_ -like 'forum/*' -or $_ -like 'gather/*' -or
  $_ -like 'crucible/*' -or $_ -like 'telos/*' -or
  $_ -like 'mneme/*' -or $_ -like 'relay/*' -or
  $_ -like 'plexus/*' -or $_ -like 'telos-v2/*' -or
  $_ -like 'portfolio-site/*' -or $_ -like 'profile/*'
})
if ($forbidden) { throw "Forbidden path in PR: $($forbidden -join ', ')" }
$artifactExtensions = @('.exe','.dll','.lib','.pdb','.obj','.ppm','.zip','.7z')
$unsafeArtifacts = @($paths | Where-Object { $artifactExtensions -contains [IO.Path]::GetExtension($_).ToLowerInvariant() })
if ($unsafeArtifacts) { throw "Unexpected artifact in PR: $($unsafeArtifacts -join ', ')" }
git log --oneline --reverse "$base..HEAD"
git diff --stat "$base...HEAD"
```

Expected: base ancestry holds, commit/path scope is intentional, no Fable/excluded path or binary/build artifact appears.

- [ ] **Step 5: Obtain whole-range independent review**

An independent reviewer reads the approved design, this plan, complete commit list, and every changed canonical path from `6f2361d...HEAD`. The review covers correctness, exact claims/non-claims, raw media/license provenance, generated-file determinism, secret/public safety, frozen code, test strength, commit ordering, and unexpected paths. Write `.superpowers/sdd/brender-publication-canonical-review.md`; end with a line `Reviewed canonical HEAD: ` followed by the exact 40-character current HEAD, then `Reviewed canonical base: 6f2361d478c85a39b2cd146e3c94acd2127870f0`, then `Review result: APPROVED`.

Any Critical/Important finding requires correction, the proportional gates above, a new exact HEAD, and a fresh review. Archive work cannot begin until approval is exact-HEAD-bound.

---

### Task 6: Mirror the exact reviewed canonical shared delta

**Files:**

- Archive branch create: `feat/brender-memory-compat` from `500d9bc16281e966373f6cf87bc3fa569f55a32f`.
- Archive shared changes: every canonical path in `fde1f9a...reviewedCanonicalHEAD`, except `README.md`, `LICENSE`, `pyproject.toml`, and `gallery/**`.
- Protected unchanged: archive `README.md`, `LICENSE`, `pyproject.toml`, and existing `gallery/**` in this first commit.

**Interfaces:**

- Consumes: clean exact-HEAD-approved canonical branch and archive raw Git-blob baseline.
- Produces: one shared sync commit whose staged path set and every blob equal the reviewed canonical delta.
- Allows transiently: `tests/test_brender_publish_surface.py` fails only because archive-owned README/gallery provenance are intentionally Task 7.

- [ ] **Step 1: Verify exact review/base/branch and archive blob baseline**

Create helper `C:\dev\worktrees\engine-revival-brender-evidence\.superpowers\sdd\verify_archive_protected_blobs.py` in the already-ignored canonical SDD ledger with:

```python
import argparse
import hashlib
import subprocess


EXPECTED = {
    "README.md": "78a90dbaef31f2a1e5ea5c5af0e04d53b2a0b75c6597283ad46b821e93a074cc",
    "LICENSE": "0d96a4ff68ad6d4b6f1f30f713b18d5184912ba8dd389f86aa7710db079abcb0",
    "pyproject.toml": "0ea27ff98d7226dd2b165aa9a35a33b5cb63936d96a81a1889610e624bce0ced",
    "gallery/01-wireframe-cube.png": "7caa3fdffba2666ead882f0ad4aca5cd9e3c9d2a669c7867eebd170c5956f1ce",
    "gallery/02-scene-graph-cube.png": "e0d61cd295ce3115065ac1d2bef07acf1e4788c5bba05c4adb80977ea95c59a1",
    "gallery/03-solid-shaded.png": "99263cabcea1b273af0018b8ec94bcfd38c03155c2d04027dade846f4f5c1d66",
    "gallery/04-depth-buffer.png": "1417dde07f043fb94d8866a3c9a6aa77ece0d7707e787f0a9a887e404cea07a2",
    "gallery/05-texture-mapped.png": "d4ebf0d9db580e802ec01f2585375cbbac0772229182d0010eafe777f3930c92",
    "gallery/06-datafile-models.png": "6905626a2151ba7f0ff40eb6acfa16388e226d096b4f48cab63763cda0b97972",
    "gallery/07-uv-textured-globe.png": "550ac52aa751d759002b0fb4fb5054541049ff9165f8db327322ac1724e55522",
    "gallery/08-multipart-coupe.png": "d2be455e1110139068d449d3a083576130dfeffabc287f2bb70e1e49a8fb4745",
    "gallery/09-gouraud-sphere.png": "66fb2058d3d4e3f920c2ee11f4f30fec1bc6ce17f06222fa75067ea87e6bbac0",
    "gallery/10-teapot-plotter.png": "c7f0a0df1cab4c5e3ec42cf034bca1fd91934ce1f83318f53faff5891d5d095c",
    "gallery/10-teapot-plotter.svg": "e631561322d4ac3445d3b57e5d643bfd1bed6e59da57144c99738c00e0f61885",
}


parser = argparse.ArgumentParser()
parser.add_argument("--root", required=True)
parser.add_argument("--ref", required=True)
parser.add_argument("--exclude-readme", action="store_true")
args = parser.parse_args()
for path, wanted in EXPECTED.items():
    if args.exclude_readme and path == "README.md":
        continue
    data = subprocess.check_output(
        ["git", "-C", args.root, "show", f"{args.ref}:{path}"]
    )
    actual = hashlib.sha256(data).hexdigest()
    if actual != wanted:
        raise SystemExit(f"archive blob mismatch: {path}")
print("archive raw blob baseline verified")
```

```powershell
$canonical = 'C:\dev\worktrees\engine-revival-brender-evidence'
$mirror = 'C:\dev\worktrees\brender-archival-evidence'
$mirrorPython = 'C:\dev\worktrees\.venvs\brender-archival-evidence\Scripts\python.exe'
$review = Get-Content -Raw -LiteralPath (Join-Path $canonical '.superpowers\sdd\brender-publication-canonical-review.md')
if ($review -notmatch '(?m)^Reviewed canonical HEAD: ([0-9a-f]{40})\r?\nReviewed canonical base: 6f2361d478c85a39b2cd146e3c94acd2127870f0\r?\nReview result: APPROVED\r?\n?\z') { throw 'Canonical review malformed' }
$reviewedHead = $Matches[1]
if ((git -C $canonical rev-parse HEAD) -ne $reviewedHead) { throw 'Canonical review stale' }
if (@(git -C $canonical status --porcelain)) { throw 'Canonical worktree dirty' }
if ((git -C $mirror rev-parse HEAD) -ne '500d9bc16281e966373f6cf87bc3fa569f55a32f') { throw 'Archive base drifted' }
if (@(git -C $mirror status --porcelain)) { throw 'Archive worktree dirty' }
if (@(git -C $mirror branch --list feat/brender-memory-compat)) { throw 'Archive feature branch already exists' }
$helper = Join-Path $canonical '.superpowers\sdd\verify_archive_protected_blobs.py'
git -C $canonical check-ignore --quiet .superpowers/sdd/verify_archive_protected_blobs.py
if ($LASTEXITCODE -ne 0) { throw 'Protected-blob helper is not ignored by canonical worktree' }
& $mirrorPython $helper `
  --root $mirror --ref 500d9bc
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
```

Expected: exact review/base/clean state and all 14 raw Git-blob hashes pass.

- [ ] **Step 2: Create the archive feature branch and prove environment provenance**

```powershell
$mirror = 'C:\dev\worktrees\brender-archival-evidence'
git -C $mirror switch -c feat/brender-memory-compat 500d9bc16281e966373f6cf87bc3fa569f55a32f
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
& 'C:\dev\worktrees\.venvs\brender-archival-evidence\Scripts\python.exe' -c "import pathlib, engine_revival.cli; p=pathlib.Path(engine_revival.cli.__file__).resolve(); r=pathlib.Path(r'C:\dev\worktrees\brender-archival-evidence\src').resolve(); assert r in p.parents, p; print(p)"
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
```

Expected: new local archive feature branch and import path inside the archive worktree.

- [ ] **Step 3: Compute the no-deletion shared delta and copy it mechanically**

```powershell
$canonical = 'C:\dev\worktrees\engine-revival-brender-evidence'
$mirror = 'C:\dev\worktrees\brender-archival-evidence'
$review = Get-Content -Raw -LiteralPath (Join-Path $canonical '.superpowers\sdd\brender-publication-canonical-review.md')
$null = $review -match '(?m)^Reviewed canonical HEAD: ([0-9a-f]{40})$'
$reviewedHead = $Matches[1]
$range = "fde1f9a4621ea37f1a3f6dd4aa03e6943238d17b...$reviewedHead"
$nameStatus = @(git -C $canonical diff --name-status $range)
$deletions = @($nameStatus | Where-Object { $_ -match '^D\s' })
if ($deletions) { throw "Canonical shared delta contains deletion: $($deletions -join ', ')" }
$changed = @(git -C $canonical diff --name-only $range | Where-Object {
  $_ -notin @('README.md','LICENSE','pyproject.toml') -and $_ -notlike 'gallery/*'
} | Sort-Object -Unique)
foreach ($relative in $changed) {
  $sourcePath = Join-Path $canonical $relative
  $targetPath = Join-Path $mirror $relative
  $parent = Split-Path -Parent $targetPath
  if ($parent) { New-Item -ItemType Directory -Force -Path $parent | Out-Null }
  Copy-Item -LiteralPath $sourcePath -Destination $targetPath -Force
}
```

Expected: exact reviewed non-protected paths copied; no deletion or protected copy.

- [ ] **Step 4: Run shared-only gates and deterministic report check**

Run from the archive worktree:

```powershell
$python = 'C:\dev\worktrees\.venvs\brender-archival-evidence\Scripts\python.exe'
$cli = 'C:\dev\worktrees\.venvs\brender-archival-evidence\Scripts\engine-revival.exe'
& $python -m pytest -q --ignore=tests/test_brender_publish_surface.py
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
& $cli validate --root .
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
& $cli audit-public --root .
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
& $cli report --root . | Out-Null
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
$first = Get-ChildItem docs\generated -Recurse -File | ForEach-Object { [pscustomobject]@{Path=$_.FullName;Hash=(Get-FileHash $_.FullName -Algorithm SHA256).Hash} }
& $cli report --root . | Out-Null
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
$second = Get-ChildItem docs\generated -Recurse -File | ForEach-Object { [pscustomobject]@{Path=$_.FullName;Hash=(Get-FileHash $_.FullName -Algorithm SHA256).Hash} }
if (Compare-Object $first $second -Property Path,Hash) { throw 'Archive report drift' }
```

Expected: every test except the deliberately pending archive-owned presentation test passes; validation/audit and deterministic reports pass.

- [ ] **Step 5: Stage and require exact path/blob equality with canonical**

```powershell
$canonical = 'C:\dev\worktrees\engine-revival-brender-evidence'
$mirror = 'C:\dev\worktrees\brender-archival-evidence'
$review = Get-Content -Raw -LiteralPath (Join-Path $canonical '.superpowers\sdd\brender-publication-canonical-review.md')
$null = $review -match '(?m)^Reviewed canonical HEAD: ([0-9a-f]{40})$'
$reviewedHead = $Matches[1]
$range = "fde1f9a4621ea37f1a3f6dd4aa03e6943238d17b...$reviewedHead"
$expected = @(git -C $canonical diff --name-only $range | Where-Object {
  $_ -notin @('README.md','LICENSE','pyproject.toml') -and $_ -notlike 'gallery/*'
} | Sort-Object -Unique)
git -C $mirror add -A
git -C $mirror diff --cached --check
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
$staged = @(git -C $mirror diff --cached --name-only | Sort-Object -Unique)
if (Compare-Object $expected $staged) { throw 'Archive staged path set differs from canonical shared delta' }
$protected = @(git -C $mirror diff --cached --name-only -- README.md LICENSE pyproject.toml gallery)
if ($protected) { throw "Protected path staged in shared commit: $($protected -join ', ')" }

$blobEquality = @'
import hashlib, subprocess, sys
canonical, mirror, head, *paths = sys.argv[1:]
for path in paths:
    source = subprocess.check_output(["git", "-C", canonical, "show", f"{head}:{path}"])
    staged = subprocess.check_output(["git", "-C", mirror, "show", f":{path}"])
    if hashlib.sha256(source).digest() != hashlib.sha256(staged).digest():
        raise SystemExit(f"shared blob mismatch: {path}")
print(f"verified {len(paths)} shared SHA-256 values")
'@
& 'C:\dev\worktrees\.venvs\brender-archival-evidence\Scripts\python.exe' -c $blobEquality $canonical $mirror $reviewedHead @expected
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
```

Expected: staged path set equals the canonical shared delta, no protected path is staged, and every staged blob has equal SHA-256.

- [ ] **Step 6: Commit the exact shared sync**

```powershell
$canonicalHead = git -C 'C:\dev\worktrees\engine-revival-brender-evidence' rev-parse HEAD
git -C 'C:\dev\worktrees\brender-archival-evidence' commit -m "sync: BRender publication from engine-revival $canonicalHead"
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
$helper = 'C:\dev\worktrees\engine-revival-brender-evidence\.superpowers\sdd\verify_archive_protected_blobs.py'
& 'C:\dev\worktrees\.venvs\brender-archival-evidence\Scripts\python.exe' $helper `
  --root 'C:\dev\worktrees\brender-archival-evidence' --ref HEAD
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
```

Expected: first archive feature commit changes only canonical-shared paths, then
all 14 protected raw Git blobs—including the root README—still match the
`500d9bc` baseline.

---

### Task 7: Update archive-owned presentation and approve the complete archive PR range

**Files:**

- Modify: `C:\dev\worktrees\brender-archival-evidence\README.md`
- Create: `C:\dev\worktrees\brender-archival-evidence\gallery\README.md`
- Ignored review: `C:\dev\worktrees\brender-archival-evidence\.superpowers\sdd\brender-publication-archive-review.md`

**Interfaces:**

- Consumes: shared sync commit with canonical tests/media/docs and unchanged protected baseline.
- Produces: archive-specific 13-rung landing page, exact gallery provenance, all archive tests green, and exact-HEAD review of `4ef0025...archive_HEAD`.
- Preserves: archive AGPL identity, `LICENSE`, `pyproject.toml`, and all eleven existing gallery blobs.

- [ ] **Step 1: Update the archive landing-page identity and thirteen-rung result**

Replace `BRender is the completed flagship` with:

```text
BRender is the verified flagship revival, with the portable core proven through thirteen rungs; broader engine restoration remains in progress. The rest of the roster is also in progress.
```

Replace the ladder introduction with:

```markdown
From the public BRender v1.3.2 source at exact snapshot
`d88d0ed41122664b9781015b517db64353e16f19`, the materializer generates an
out-of-tree CMake harness that builds the FLOAT core through BRender's pure-C
memory-pixelmap path. Its thirteen-rung ladder passes CTest 13/13 on MSVC Win32;
readiness score remains 86 and packaging remains `not-started`.
```

Append this row to the rung table:

```markdown
| Memory compatibility | 1-4 byte pixel set/get, non-black RGB888 fill, positive-stride rectangular fill, and nonzero-start copy-bits through `_MemCopyBits_A` and `BrPixelmapCopyBits` |
```

After the existing gallery, embed the shared exact artifact and links:

```markdown
### Current verified plotter artifact

![BRender teapot hidden-line plotter output](docs/media/brender/brender-core-plotter-smoke.svg)

[Run the complete demo](docs/BRENDER-DEMO.md) ·
[Media provenance and file-specific MIT license](docs/media/brender/README.md) ·
[Gallery provenance](gallery/README.md)
```

Replace the abbreviated deferred sentence with:

```markdown
This is a verified portable-core milestone, not a 100% engine port. Host/DOS
fallback parity, overlap, negative strides, colour-key and FPU-wrapper parity,
widths above four, x64, period `softrend`, native `.mat/.pal/.pix`, FIXED
variants, drivers, packaging, a full viewer, and the separate 3D Movie Maker
build remain explicit follow-on work.
```

Update the reproduce section to build the full ladder rather than only `brender_core_model_smoke`, and link `docs/BRENDER-DEMO.md` for direct plotter/Gouraud/texture/memory commands.

- [ ] **Step 2: Add the exact gallery provenance inventory**

Create `gallery/README.md` with:

```markdown
# BRender Gallery Provenance

These eleven files are preserved release captures from the BRender portable
render-smoke ladder. Hashes below are SHA-256 over committed Git-blob bytes,
not checkout bytes transformed by `core.autocrlf`. All inputs were read from
public BRender snapshot `d88d0ed41122664b9781015b517db64353e16f19` or were
generated procedurally by the harness. No source model is vendored.

| File | Raw blob SHA-256 | Bytes / dimensions | Input or origin | Native/presentation provenance | Added |
|---|---|---|---|---|---|
| `01-wireframe-cube.png` | `7caa3fdffba2666ead882f0ad4aca5cd9e3c9d2a669c7867eebd170c5956f1ce` | 2,056 / 320x240 | Procedural projected cube; `brender-v132-portable-core-render-smoke` | Native PPM; PNG conversion command/tool unknown | `4e2fd6e` |
| `02-scene-graph-cube.png` | `e0d61cd295ce3115065ac1d2bef07acf1e4788c5bba05c4adb80977ea95c59a1` | 2,465 / 320x240 | Procedural v1db actor/model cube; `brender-v132-portable-core-scene-smoke` | Native PPM; PNG conversion command/tool unknown | `4e2fd6e` |
| `03-solid-shaded.png` | `99263cabcea1b273af0018b8ec94bcfd38c03155c2d04027dade846f4f5c1d66` | 1,831 / 320x240 | Procedural flat-shaded cube; `brender-v132-portable-core-fill-smoke` | Native PPM; PNG conversion command/tool unknown | `4e2fd6e` |
| `04-depth-buffer.png` | `1417dde07f043fb94d8866a3c9a6aa77ece0d7707e787f0a9a887e404cea07a2` | 1,083 / 320x240 | Procedural overlapping cubes; `brender-v132-portable-core-depth-smoke` | Native PPM; PNG conversion command/tool unknown | `4e2fd6e` |
| `05-texture-mapped.png` | `d4ebf0d9db580e802ec01f2585375cbbac0772229182d0010eafe777f3930c92` | 3,490 / 320x240 | Procedural cube/checker texture; `brender-v132-portable-core-texture-smoke` | Native PPM; PNG conversion command/tool unknown | `4e2fd6e` |
| `06-datafile-models.png` | `6905626a2151ba7f0ff40eb6acfa16388e226d096b4f48cab63763cda0b97972` | 15,568 / 640x480 | Public teapot, skull, car-panel, and torus `.dat` render composite; `brender-v132-portable-core-model-smoke` | Native PPM frames; composition and PNG conversion command/tool unknown | `4e2fd6e` |
| `07-uv-textured-globe.png` | `550ac52aa751d759002b0fb4fb5054541049ff9165f8db327322ac1724e55522` | 9,705 / 320x240 | Public `dat/sph32.dat` plus generated texture; `brender-v132-portable-core-material-smoke` | Native PPM; PNG conversion command/tool unknown | `4ef0025` |
| `08-multipart-coupe.png` | `d2be455e1110139068d449d3a083576130dfeffabc287f2bb70e1e49a8fb4745` | 2,853 / 320x240 | Twelve parts from public `dat/coupe.dat`; `brender-v132-portable-core-multimodel-smoke` | Native PPM; PNG conversion command/tool unknown; hierarchy transforms remain deferred | `4ef0025` |
| `09-gouraud-sphere.png` | `66fb2058d3d4e3f920c2ee11f4f30fec1bc6ce17f06222fa75067ea87e6bbac0` | 12,143 / 320x240 | Public `dat/sph32.dat`; `brender-v132-portable-core-gouraud-smoke` | Native PPM; PNG conversion command/tool unknown | `4ef0025` |
| `10-teapot-plotter.png` | `c7f0a0df1cab4c5e3ec42cf034bca1fd91934ce1f83318f53faff5891d5d095c` | 7,591 / 640x480 | Public `dat/teapot.dat`; `brender-v132-portable-core-plotter-smoke` | Native PPM preview; PNG conversion command/tool unknown | `4ef0025` |
| `10-teapot-plotter.svg` | `e631561322d4ac3445d3b57e5d643bfd1bed6e59da57144c99738c00e0f61885` | 56,287 / 640x480 | Public `dat/teapot.dat`; 1,002 native SVG lines; `brender-v132-portable-core-plotter-smoke` | Native SVG, no presentation conversion | `4ef0025` |

The archive repository is AGPL-3.0-or-later. BRender source/model input context
is upstream MIT. The byte-identical plotter SVG is distributed with its
[file-specific upstream MIT notice](../docs/media/brender/LICENSE-BRENDER-MIT.txt);
the archive project license does not replace that notice. Unknown historical
conversion details are intentionally labeled `unknown`, not reconstructed.
```

- [ ] **Step 3: Run the full archive publication suite and deterministic report gate**

```powershell
$python = 'C:\dev\worktrees\.venvs\brender-archival-evidence\Scripts\python.exe'
$cli = 'C:\dev\worktrees\.venvs\brender-archival-evidence\Scripts\engine-revival.exe'
& $python -m pytest -q
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
& $cli validate --root .
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
& $cli audit-public --root .
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
& $cli report --root . | Out-Null
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
$first = Get-ChildItem docs\generated -Recurse -File | ForEach-Object { [pscustomobject]@{Path=$_.FullName;Hash=(Get-FileHash $_.FullName -Algorithm SHA256).Hash} }
& $cli report --root . | Out-Null
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
$second = Get-ChildItem docs\generated -Recurse -File | ForEach-Object { [pscustomobject]@{Path=$_.FullName;Hash=(Get-FileHash $_.FullName -Algorithm SHA256).Hash} }
if (Compare-Object $first $second -Property Path,Hash) { throw 'Archive report drift' }
git diff --check
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
```

Expected: the previously transient publication-surface failure is green; full tests, validate, audit, deterministic report, and diff checks pass.

- [ ] **Step 4: Prove immutable protected blobs and commit presentation**

Before commit, run the defined ignored verifier against `HEAD` for `LICENSE`, `pyproject.toml`, and all eleven gallery paths. Root README is intentionally excluded now:

```powershell
$helper = 'C:\dev\worktrees\engine-revival-brender-evidence\.superpowers\sdd\verify_archive_protected_blobs.py'
& 'C:\dev\worktrees\.venvs\brender-archival-evidence\Scripts\python.exe' `
  $helper `
  --root . --ref HEAD --exclude-readme
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
git add README.md gallery/README.md
git diff --cached --check
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
$staged = @(git diff --cached --name-only | Sort-Object)
if (Compare-Object @('README.md','gallery/README.md') $staged) { throw 'Unexpected archive presentation path' }
git commit -m "docs: publish BRender archive presentation"
```

Expected: second archive commit changes only root README and adds gallery provenance.

- [ ] **Step 5: Verify cumulative archive ancestry, commits, blobs, and gates**

```powershell
$base = '4ef0025aff3d733a5433364b9e8b720de40e49dc'
git merge-base --is-ancestor $base HEAD
if ($LASTEXITCODE -ne 0) { throw 'Archive remote base is not ancestor' }
$commits = @(git rev-list --reverse "$base..HEAD")
if ($commits.Count -ne 3) { throw "Unexpected archive PR commit count: $($commits.Count)" }
if ($commits[0] -ne '500d9bc16281e966373f6cf87bc3fa569f55a32f') { throw 'Evidence-consistency commit is not first' }
if (@(git status --porcelain)) { throw 'Archive worktree dirty' }
git log --oneline --reverse "$base..HEAD"
git diff --stat "$base...HEAD"
```

Compute raw SHA-256 of final `README.md` from the committed blob:

```powershell
$readmeHashCode = @'
import hashlib, subprocess
data = subprocess.check_output(["git", "show", "HEAD:README.md"])
print(hashlib.sha256(data).hexdigest())
'@
$archiveReadmeSha256 = (& 'C:\dev\worktrees\.venvs\brender-archival-evidence\Scripts\python.exe' -c $readmeHashCode).Trim()
if ($archiveReadmeSha256 -notmatch '^[0-9a-f]{64}$') { throw 'Invalid archive README hash' }
"ARCHIVE_README_SHA256=$archiveReadmeSha256"
$helper = 'C:\dev\worktrees\engine-revival-brender-evidence\.superpowers\sdd\verify_archive_protected_blobs.py'
& 'C:\dev\worktrees\.venvs\brender-archival-evidence\Scripts\python.exe' `
  $helper `
  --root . --ref HEAD --exclude-readme
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
& 'C:\dev\worktrees\.venvs\brender-archival-evidence\Scripts\python.exe' -m pytest -q
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
& 'C:\dev\worktrees\.venvs\brender-archival-evidence\Scripts\engine-revival.exe' validate --root .
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
& 'C:\dev\worktrees\.venvs\brender-archival-evidence\Scripts\engine-revival.exe' audit-public --root .
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
```

Record the printed value as `ARCHIVE_README_SHA256` for the exact-HEAD review and Task 8.

- [ ] **Step 6: Obtain exact-HEAD archive review**

An independent reviewer inspects the entire `4ef0025...archive_HEAD` range, including `500d9bc`, shared sync, and presentation. Review commit ancestry/order, all paths, raw protected hashes, shared canonical blob equality, generated determinism, AGPL/MIT separation, gallery provenance, claims/non-claims, tests, secrets/public safety, and Fable exclusion. Write `.superpowers/sdd/brender-publication-archive-review.md`; end with `Reviewed archive HEAD: ` plus the exact 40-character current HEAD, `Reviewed archive base: 4ef0025aff3d733a5433364b9e8b720de40e49dc`, `Reviewed archive README SHA-256: ` plus exact `ARCHIVE_README_SHA256`, then `Review result: APPROVED`.

Any Critical/Important finding requires correction, all proportional gates, a new exact HEAD, and a fresh review. No remote mutation begins without approval.

---

### Task 8: Publish both reviewed branches and open two draft PRs

**Files/remote state:**

- Push canonical `feat/brender-memory-compat` only.
- Push archive `feat/brender-memory-compat` only.
- Create one draft PR in `HarperZ9/engine-revival` and one in `HarperZ9/brender-archival`, both targeting `main`.
- No local tracked changes.

**Interfaces:**

- Consumes: exact-HEAD approved clean branches, expected remote main heads, raw fixity, and complete PR evidence.
- Produces: two upstream-tracked feature branches and two verified draft PR URLs.
- Does not produce: direct main update, merge, tag, release, binary upload, force overwrite, or cleanup.

- [ ] **Step 1: Verify authentication, exact reviews, clean branches, and absence of existing PRs**

```powershell
gh auth status
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
$canonical = 'C:\dev\worktrees\engine-revival-brender-evidence'
$archive = 'C:\dev\worktrees\brender-archival-evidence'
if ((git -C $canonical branch --show-current) -ne 'feat/brender-memory-compat') { throw 'Wrong canonical branch' }
if ((git -C $archive branch --show-current) -ne 'feat/brender-memory-compat') { throw 'Wrong archive branch' }
if (@(git -C $canonical status --porcelain)) { throw 'Canonical dirty' }
if (@(git -C $archive status --porcelain)) { throw 'Archive dirty' }
$existingCanonical = gh pr list --repo HarperZ9/engine-revival --head feat/brender-memory-compat --state all --json number,url,state,isDraft
if (($existingCanonical | ConvertFrom-Json).Count -ne 0) { throw 'Canonical PR already exists; review instead of duplicating' }
$existingArchive = gh pr list --repo HarperZ9/brender-archival --head feat/brender-memory-compat --state all --json number,url,state,isDraft
if (($existingArchive | ConvertFrom-Json).Count -ne 0) { throw 'Archive PR already exists; review instead of duplicating' }
```

Expected: authenticated GitHub account, exact clean feature branches, and no existing PRs for either head.

- [ ] **Step 2: Just-in-time compare-and-swap publish the canonical feature ref**

```powershell
$repo = 'C:\dev\worktrees\engine-revival-brender-evidence'
$expectedMain = '6f2361d478c85a39b2cd146e3c94acd2127870f0'
$python = 'C:\dev\worktrees\.venvs\engine-revival-brender-evidence\Scripts\python.exe'
$cli = 'C:\dev\worktrees\.venvs\engine-revival-brender-evidence\Scripts\engine-revival.exe'
$head = git -C $repo rev-parse HEAD
$review = Get-Content -Raw -LiteralPath (Join-Path $repo '.superpowers\sdd\brender-publication-canonical-review.md')
if ($review -notmatch '(?m)^Reviewed canonical HEAD: ([0-9a-f]{40})\r?\nReviewed canonical base: 6f2361d478c85a39b2cd146e3c94acd2127870f0\r?\nReview result: APPROVED\r?\n?\z') {
  throw 'Canonical review malformed'
}
if ($Matches[1] -ne $head) { throw 'Canonical review is stale' }
if ((git -C $repo branch --show-current) -ne 'feat/brender-memory-compat') { throw 'Wrong canonical branch' }
if (@(git -C $repo status --porcelain)) { throw 'Canonical worktree dirty before publication' }

& $python -m pytest -q $repo
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
& $cli validate --root $repo
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
& $cli audit-public --root $repo
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
git -C $repo diff --check "$expectedMain...HEAD"
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

git -C $repo merge-base --is-ancestor $expectedMain HEAD
if ($LASTEXITCODE -ne 0) { throw 'Canonical PR base is not an ancestor' }
$commits = @(git -C $repo log --format='%H %s' "$expectedMain..HEAD")
$paths = @(git -C $repo diff --name-only "$expectedMain...HEAD")
if (-not $commits -or -not $paths) { throw 'Prospective canonical PR is empty' }
$forbidden = @($paths | Where-Object {
  $_ -like 'local-model/*' -or $_ -like 'public/index/*' -or
  $_ -like 'forum/*' -or $_ -like 'gather/*' -or
  $_ -like 'crucible/*' -or $_ -like 'telos/*' -or
  $_ -like 'mneme/*' -or $_ -like 'relay/*' -or
  $_ -like 'plexus/*' -or $_ -like 'telos-v2/*' -or
  $_ -like 'portfolio-site/*' -or $_ -like 'profile/*'
})
if ($forbidden) { throw "Forbidden path in canonical PR: $($forbidden -join ', ')" }
$artifactExtensions = @('.exe','.dll','.lib','.pdb','.obj','.ppm','.zip','.7z')
$unsafeArtifacts = @($paths | Where-Object {
  $artifactExtensions -contains [IO.Path]::GetExtension($_).ToLowerInvariant()
})
if ($unsafeArtifacts) { throw "Unexpected canonical artifact: $($unsafeArtifacts -join ', ')" }

$checks = @{
  'C:\dev\public\engine-revival-workspaces\brender-v132-portable-core-ctest-memory-compat-2026-07-09.txt' = '592559a41e77a0d51c378e678f8f17884be439c14954ba16ec4b5e90790be1f4'
  'C:\dev\public\engine-revival-workspaces\brender-v132-portable-core-ctest-memory-compat-red-2026-07-09.txt' = '7ec18fe16bc591980ea5eafc970abec25a894338f759fce3cb0e1f9cd9cd6b60'
  'C:\dev\worktrees\engine-revival-brender-evidence\docs\media\brender\brender-core-plotter-smoke.svg' = 'e631561322d4ac3445d3b57e5d643bfd1bed6e59da57144c99738c00e0f61885'
  'C:\dev\worktrees\engine-revival-brender-evidence\docs\media\brender\LICENSE-BRENDER-MIT.txt' = 'f604769ea503e060b146607f1a8ebc1ce24daec3f75a2b1cbd9e2ab8ac3223fa'
}
foreach ($path in $checks.Keys) {
  $actual = (Get-FileHash -LiteralPath $path -Algorithm SHA256).Hash.ToLowerInvariant()
  if ($actual -ne $checks[$path]) { throw "Canonical publication fixity failure: $path" }
}
$mediaReadme = Get-Content -Raw -LiteralPath (Join-Path $repo 'docs\media\brender\README.md')
if ($mediaReadme -notmatch '(?m)^- Generation transcript SHA-256: `([0-9a-f]{64})`\.\r?$') {
  throw 'Tracked media provenance lacks one exact generation transcript hash'
}
$trackedTranscriptHash = $Matches[1]
$externalTranscript = 'C:\dev\public\engine-revival-workspaces\brender-v132-plotter-provenance-2026-07-10.txt'
$externalTranscriptHash = (Get-FileHash -LiteralPath $externalTranscript -Algorithm SHA256).Hash.ToLowerInvariant()
if ($trackedTranscriptHash -ne $externalTranscriptHash) {
  throw 'Tracked generation transcript hash does not bind the external transcript'
}

$codeReview = Get-Content -Raw -LiteralPath (Join-Path $repo '.superpowers\sdd\brender-publication-code-review.md')
if ($codeReview -notmatch '(?m)^Reviewed code checkpoint: ([0-9a-f]{40})\r?\nReview result: APPROVED\r?\n?\z') {
  throw 'Code-checkpoint review malformed'
}
$codeCheckpoint = $Matches[1]
$frozen = @(git -C $repo diff --name-only "$codeCheckpoint...HEAD" -- `
  src/engine_revival/brender_harness.py `
  src/engine_revival/brender_harness_templates.py `
  'src/engine_revival/brender*_sources.py' `
  src/engine_revival/cli.py)
if ($frozen) { throw "Frozen generation path changed: $($frozen -join ', ')" }
$attempts = @(git -C $repo diff --name-only fde1f9a4621ea37f1a3f6dd4aa03e6943238d17b...HEAD -- attempts)
if ($attempts.Count -ne 1 -or $attempts[0] -ne 'attempts/brender-v132-portable-core-memory-compat-smoke.json') {
  throw 'Historical attempt drift before publication'
}
if (@(git -C $repo diff --name-only 37efdcbe8f9379de3e2a10b78f57e54076a57040...HEAD -- `
  docs/superpowers/specs/2026-07-09-brender-evidence-consistency-design.md `
  docs/superpowers/plans/2026-07-09-brender-evidence-consistency.md)) {
  throw 'Historical evidence-consistency docs drift before publication'
}

git -C $repo fetch --prune origin
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
$mainLine = @(git -C $repo ls-remote --heads origin refs/heads/main)
if ($LASTEXITCODE -ne 0 -or $mainLine.Count -ne 1 -or ($mainLine[0] -split '\s+')[0] -ne $expectedMain) { throw 'Canonical remote main drifted' }
$featureLine = @(git -C $repo ls-remote --heads origin refs/heads/feat/brender-memory-compat)
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
if ($featureLine.Count -eq 0) {
  git -C $repo push --set-upstream origin HEAD:refs/heads/feat/brender-memory-compat `
    --force-with-lease=refs/heads/feat/brender-memory-compat:
  if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
} elseif ($featureLine.Count -eq 1 -and ($featureLine[0] -split '\s+')[0] -eq $head) {
  git -C $repo branch --set-upstream-to=origin/feat/brender-memory-compat feat/brender-memory-compat
} else {
  throw 'Canonical remote feature ref is divergent'
}
$postMain = @(git -C $repo ls-remote --heads origin refs/heads/main)
$postFeature = @(git -C $repo ls-remote --heads origin refs/heads/feat/brender-memory-compat)
if (($postMain[0] -split '\s+')[0] -ne $expectedMain) { throw 'Canonical main moved after feature creation' }
if (($postFeature[0] -split '\s+')[0] -ne $head) { throw 'Canonical feature ref mismatch after push' }
```

Expected: remote canonical feature ref is created only if absent (or already exact), main stays fixed, upstream tracking is set, and no branch is overwritten.

- [ ] **Step 3: Create and verify the canonical draft PR through the GitHub connector**

Immediately re-run `ls-remote` for canonical main and feature; require the same two SHAs. Use `mcp__codex_apps__github_create_pull_request` with:

```text
repository_full_name: HarperZ9/engine-revival
base: main
head: feat/brender-memory-compat
draft: true
title: BRender: publish verified 13-rung memory compatibility release
```

Use this body, replacing the named runtime values with their exact hashes:

```markdown
## Summary

- Adds the thirteenth BRender portable-core smoke for direct memory semantics.
- Promotes five current-state carriers to the exact Win32 CTest 13/13 checkpoint.
- Publishes a reproducible demo, a real hidden-line plotter SVG, complete MIT provenance, and deterministic generated views.

## Verified evidence

- Base: `6f2361d478c85a39b2cd146e3c94acd2127870f0`.
- Head: exact reviewed canonical HEAD.
- Public source: `d88d0ed41122664b9781015b517db64353e16f19`.
- Media-generation code checkpoint: exact `CODE_CHECKPOINT`.
- Final CTest transcript SHA-256: `592559a41e77a0d51c378e678f8f17884be439c14954ba16ec4b5e90790be1f4`.
- Pre-fix RED transcript SHA-256: `7ec18fe16bc591980ea5eafc970abec25a894338f759fce3cb0e1f9cd9cd6b60`.
- SVG SHA-256: `e631561322d4ac3445d3b57e5d643bfd1bed6e59da57144c99738c00e0f61885`.
- Generation transcript SHA-256: exact `MEDIA_TRANSCRIPT_SHA256`.

## Checks

- Full isolated pytest passed.
- `engine-revival validate` passed.
- `engine-revival audit-public` passed.
- Report generation was byte-deterministic.
- Clean Win32 reproduction passed CTest 13/13 and reproduced the exact SVG.
- Complete `main...HEAD` range received exact-HEAD independent approval.

## Boundaries

This is a source/evidence publication, not a complete engine port or binary package. Host/DOS parity, overlap, negative strides, colour-key/FPU parity, widths above four, x64, period `softrend`, `.mat/.pal/.pix`, FIXED, drivers, packaging, full viewer, and 3D Movie Maker remain deferred. Score remains 86; packaging remains `not-started`.

Host note: `pwsh.exe` was absent from PATH; Windows PowerShell Desktop 5.1.26100.8764 executed the commands. This was not a BRender build or CTest failure.
```

Use the GitHub connector to read the created PR back. Require `isDraft=true`, base `main`, head `feat/brender-memory-compat`, and the exact head SHA. Re-run `ls-remote`; if main moved, leave the branch/PR draft and stop for re-review.

- [ ] **Step 4: Just-in-time compare-and-swap publish the archive feature ref**

Run the complete archive gate and push without relying on canonical-step state:

```powershell
$repo = 'C:\dev\worktrees\brender-archival-evidence'
$python = 'C:\dev\worktrees\.venvs\brender-archival-evidence\Scripts\python.exe'
$cli = 'C:\dev\worktrees\.venvs\brender-archival-evidence\Scripts\engine-revival.exe'
$expectedMain = '4ef0025aff3d733a5433364b9e8b720de40e49dc'
$head = git -C $repo rev-parse HEAD
if ((git -C $repo branch --show-current) -ne 'feat/brender-memory-compat') { throw 'Wrong archive branch' }
if (@(git -C $repo status --porcelain)) { throw 'Archive worktree dirty before publication' }
$review = Get-Content -Raw -LiteralPath (Join-Path $repo '.superpowers\sdd\brender-publication-archive-review.md')
if ($review -notmatch '(?m)^Reviewed archive HEAD: ([0-9a-f]{40})\r?\nReviewed archive base: 4ef0025aff3d733a5433364b9e8b720de40e49dc\r?\nReviewed archive README SHA-256: ([0-9a-f]{64})\r?\nReview result: APPROVED\r?\n?\z') { throw 'Archive review malformed' }
if ($Matches[1] -ne $head) { throw 'Archive review stale' }
$reviewedReadmeHash = $Matches[2]
& $python -m pytest -q $repo
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
& $cli validate --root $repo
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
& $cli audit-public --root $repo
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
$checks = @{
  'C:\dev\public\engine-revival-workspaces\brender-v132-portable-core-ctest-memory-compat-2026-07-09.txt' = '592559a41e77a0d51c378e678f8f17884be439c14954ba16ec4b5e90790be1f4'
  'C:\dev\public\engine-revival-workspaces\brender-v132-portable-core-ctest-memory-compat-red-2026-07-09.txt' = '7ec18fe16bc591980ea5eafc970abec25a894338f759fce3cb0e1f9cd9cd6b60'
  'C:\dev\worktrees\brender-archival-evidence\docs\media\brender\brender-core-plotter-smoke.svg' = 'e631561322d4ac3445d3b57e5d643bfd1bed6e59da57144c99738c00e0f61885'
  'C:\dev\worktrees\brender-archival-evidence\docs\media\brender\LICENSE-BRENDER-MIT.txt' = 'f604769ea503e060b146607f1a8ebc1ce24daec3f75a2b1cbd9e2ab8ac3223fa'
}
foreach ($path in $checks.Keys) {
  $actual = (Get-FileHash -LiteralPath $path -Algorithm SHA256).Hash.ToLowerInvariant()
  if ($actual -ne $checks[$path]) { throw "Archive publication fixity failure: $path" }
}
$mediaReadme = Get-Content -Raw -LiteralPath (Join-Path $repo 'docs\media\brender\README.md')
if ($mediaReadme -notmatch '(?m)^- Generation transcript SHA-256: `([0-9a-f]{64})`\.\r?$') {
  throw 'Archive media provenance lacks one exact generation transcript hash'
}
$trackedTranscriptHash = $Matches[1]
$externalTranscript = 'C:\dev\public\engine-revival-workspaces\brender-v132-plotter-provenance-2026-07-10.txt'
$externalTranscriptHash = (Get-FileHash -LiteralPath $externalTranscript -Algorithm SHA256).Hash.ToLowerInvariant()
if ($trackedTranscriptHash -ne $externalTranscriptHash) {
  throw 'Archive provenance does not bind the external generation transcript'
}
$helper = 'C:\dev\worktrees\engine-revival-brender-evidence\.superpowers\sdd\verify_archive_protected_blobs.py'
& $python $helper `
  --root $repo --ref HEAD --exclude-readme
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
$readmeCheck = @'
import hashlib, subprocess, sys
root, wanted = sys.argv[1:]
data = subprocess.check_output(["git", "-C", root, "show", "HEAD:README.md"])
if hashlib.sha256(data).hexdigest() != wanted:
    raise SystemExit("archive README blob drift")
'@
& $python -c $readmeCheck $repo $reviewedReadmeHash
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
git -C $repo merge-base --is-ancestor $expectedMain HEAD
if ($LASTEXITCODE -ne 0) { throw 'Archive base is not ancestor' }
$commits = @(git -C $repo rev-list --reverse "$expectedMain..HEAD")
if ($commits.Count -ne 3 -or $commits[0] -ne '500d9bc16281e966373f6cf87bc3fa569f55a32f') { throw 'Archive cumulative commit sequence drifted' }
git -C $repo fetch --prune origin
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
$mainLine = @(git -C $repo ls-remote --heads origin refs/heads/main)
if ($LASTEXITCODE -ne 0 -or $mainLine.Count -ne 1 -or ($mainLine[0] -split '\s+')[0] -ne $expectedMain) { throw 'Archive remote main drifted' }
$featureLine = @(git -C $repo ls-remote --heads origin refs/heads/feat/brender-memory-compat)
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
if ($featureLine.Count -eq 0) {
  git -C $repo push --set-upstream origin HEAD:refs/heads/feat/brender-memory-compat `
    --force-with-lease=refs/heads/feat/brender-memory-compat:
  if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
} elseif ($featureLine.Count -eq 1 -and ($featureLine[0] -split '\s+')[0] -eq $head) {
  git -C $repo branch --set-upstream-to=origin/feat/brender-memory-compat feat/brender-memory-compat
} else {
  throw 'Archive remote feature ref is divergent'
}
$postMain = @(git -C $repo ls-remote --heads origin refs/heads/main)
$postFeature = @(git -C $repo ls-remote --heads origin refs/heads/feat/brender-memory-compat)
if (($postMain[0] -split '\s+')[0] -ne $expectedMain) { throw 'Archive main moved after feature creation' }
if (($postFeature[0] -split '\s+')[0] -ne $head) { throw 'Archive feature ref mismatch after push' }
```

Expected: remote archive feature ref equals the exact reviewed archive HEAD; archive main is unchanged; upstream tracking is set.

- [ ] **Step 5: Create and verify the archive draft PR through the GitHub connector**

Use `mcp__codex_apps__github_create_pull_request` with:

```text
repository_full_name: HarperZ9/brender-archival
base: main
head: feat/brender-memory-compat
draft: true
title: BRender archival: publish 13-rung compatibility and provenance
```

Use this body, replacing runtime values with exact reviewed hashes:

```markdown
## Summary

- Cumulatively publishes the previously local evidence-consistency sync (`500d9bc`) and the reviewed BRender memory-compatibility publication.
- Updates the standalone archive to thirteen rungs and CTest 13/13.
- Preserves the archive's AGPL identity and every existing gallery blob while adding exact gallery provenance and the shared file-specific MIT media notice.

## Scope and ancestry

- Base: `4ef0025aff3d733a5433364b9e8b720de40e49dc`.
- First cumulative commit: `500d9bc16281e966373f6cf87bc3fa569f55a32f`.
- Head: exact reviewed archive HEAD.
- Shared canonical source: exact reviewed canonical HEAD.
- Final archive README raw SHA-256: exact `ARCHIVE_README_SHA256`.

## Evidence and checks

- Public source `d88d0ed41122664b9781015b517db64353e16f19`.
- Win32 CTest 13/13; final transcript SHA-256 `592559a41e77a0d51c378e678f8f17884be439c14954ba16ec4b5e90790be1f4`.
- Real SVG SHA-256 `e631561322d4ac3445d3b57e5d643bfd1bed6e59da57144c99738c00e0f61885`.
- Full archive pytest, validate, audit, deterministic report, shared-blob equality, protected-blob fixity, and exact-HEAD whole-range review passed.

## Boundaries

This remains a source/evidence archive, not a complete engine port or binary release. Host/DOS parity, overlap, negative strides, colour-key/FPU parity, widths above four, x64, period `softrend`, `.mat/.pal/.pix`, FIXED, drivers, packaging, full viewer, and 3D Movie Maker remain deferred. Score remains 86; packaging remains `not-started`.

Host note: `pwsh.exe` was absent from PATH; Windows PowerShell Desktop 5.1.26100.8764 executed the commands. This was not a BRender build or CTest failure.
```

Read the PR back with the connector; require draft/base/head/exact SHA. Re-run archive `ls-remote` for main and feature. If base moved, leave the draft intact and stop for re-review.

- [ ] **Step 6: Record publication outcome without further mutation**

Record both draft PR numbers/URLs, canonical/archive reviewed heads, remote base SHAs, and post-creation remote ref checks in ignored `.superpowers/sdd/brender-publication-remote-outcome.md`. Then run:

```powershell
$canonical = 'C:\dev\worktrees\engine-revival-brender-evidence'
$archive = 'C:\dev\worktrees\brender-archival-evidence'
if (@(git -C $canonical status --porcelain)) { throw 'Canonical dirty after publication' }
if (@(git -C $archive status --porcelain)) { throw 'Archive dirty after publication' }
git -C $canonical branch -vv
git -C $archive branch -vv
$canonicalMain = @(git -C $canonical ls-remote --heads origin refs/heads/main)
$archiveMain = @(git -C $archive ls-remote --heads origin refs/heads/main)
if (($canonicalMain[0] -split '\s+')[0] -ne '6f2361d478c85a39b2cd146e3c94acd2127870f0') { throw 'Canonical main changed' }
if (($archiveMain[0] -split '\s+')[0] -ne '4ef0025aff3d733a5433364b9e8b720de40e49dc') { throw 'Archive main changed' }
if (@(git -C $canonical tag --points-at HEAD)) { throw 'Canonical feature HEAD was tagged' }
if (@(git -C $archive tag --points-at HEAD)) { throw 'Archive feature HEAD was tagged' }
```

Expected: both worktrees are clean on upstream-tracked feature branches, both default branches remain at their original SHAs, neither feature HEAD is tagged, and only the two draft PRs/feature refs were created. Preserve worktrees and branches; do not merge, release, or clean up.

---

## Plan Self-Review

- [x] Each of the approved design's twelve components maps to a numbered task and exact gate.
- [x] Release-command hardening and harness README completeness precede the media-generation checkpoint.
- [x] Media provenance binds to an exact reviewed code checkpoint without a circular final-HEAD claim.
- [x] Fresh external Win32 build/CTest/direct-run must reproduce the exact SVG and transcript hashes.
- [x] Every claim-bearing carrier field is named, non-empty, positively complete, and checked for exact status/checkpoint plus delivered/deferred scope.
- [x] Five carriers share one exact 13/13 checkpoint; score 86, packaging `not-started`, and 3D Movie Maker pending remain explicit.
- [x] Generated views are record-first, exact-path-bounded, byte-deterministic, and historical attempt pages remain immutable.
- [x] README/demo/media/license/provenance/outcome requirements have concrete tests and content.
- [x] Raw Git-blob hashes, not EOL-filtered worktree hashes, control archive protection.
- [x] Shared archive sync has no deletion, exact staged-path equality, SHA-256 blob equality, and a protected-path exclusion.
- [x] Archive presentation is a separate commit; existing AGPL package identity and eleven media blobs are preserved.
- [x] Whole prospective PR ranges receive exact-HEAD independent reviews, including cumulative archive commit `500d9bc`.
- [x] Feature refs use an expected-absence compare-and-swap lease; main/ref drift stops PR publication.
- [x] PRs are draft; main pushes, merges, tags, releases, binaries, and cleanup are excluded.
- [x] Fable-owned and other excluded paths are absent from every task.
- [x] No undefined function/interface or unowned tracked output remains.

## Execution Handoff

Plan saved at `docs/superpowers/plans/2026-07-10-brender-publication.md`.

1. **Subagent-Driven (recommended):** dispatch a fresh implementer and reviewer per task, with exact-HEAD review checkpoints.
2. **Inline Execution:** execute in this session using the executing-plans workflow and batch checkpoints.

Both options preserve the same commit boundaries, evidence gates, archive protections, and remote publication rules.
