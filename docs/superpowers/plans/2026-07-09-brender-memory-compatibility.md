# BRender Portable Memory Compatibility Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Add and verify a thirteenth Win32 BRender CTest that proves four portable memory contracts, reproduces and narrowly fixes the copy-bits offset defect, records honest 13/13 evidence, and mirrors the reviewed result without changing archive-owned files.

**Architecture:** Generate one dedicated C semantic executable through the existing out-of-tree harness materializer. Prove wiring with Python TDD, prove semantics with a real Visual Studio Win32 CTest RED/GREEN cycle, promote evidence only after the full ladder passes, then copy the reviewed canonical delta into the downstream archival mirror with blob and protected-file hash gates.

**Tech Stack:** Python 3.12, pytest, generated C, public BRender v1.3.2 headers/source, CMake, Visual Studio 18 2026, MSVC 19.50, Win32 Debug, CTest, JSON evidence records, Git worktrees, PowerShell.

## Global Constraints

- Canonical worktree: `C:\dev\worktrees\engine-revival-brender-evidence`.
- At execution start, create branch `feat/brender-memory-compat` from the clean plan commit; preserve `fix/evidence-checkpoint-hardening` at that commit.
- Use only public BRender snapshot `d88d0ed41122664b9781015b517db64353e16f19` at `C:\dev\public\engine-revival-workspaces\brender-v132`.
- Use the isolated editable environment at `C:\dev\worktrees\.venvs\engine-revival-brender-evidence`; verify its imported module path before CLI gates.
- Configure only Visual Studio `-A Win32`; do not claim x64.
- Write generated harnesses, build products, and transcripts only under `C:\dev\public\engine-revival-workspaces`.
- Do not vendor BRender source, models, palettes, textures, binaries, or generated harness output into either Git repository.
- Preserve all existing `attempts/brender-v132-*.json` records and referenced historical transcripts byte-for-byte.
- The approved semantic scope is pixel set/get widths 1-4, non-black RGB888 fill, positive-stride rectangular fill, and nonzero-start copy-bits through both raw and public dispatch.
- Do not claim host/DOS fallback parity, overlap safety, negative strides, colour-key parity, FPU wrapper parity, widths above four, x64 safety, period `softrend`, native `.mat/.pal/.pix` resolution, FIXED variants, drivers, packaged binaries, or a full viewer.
- The final shared checkpoint is exactly `brender-v132-portable-core-memory-compat-2026-07-09`, stage `portable-core-memory-compat-lane-passing`, source snapshot `d88d0ed41122664b9781015b517db64353e16f19`, and 13/13.
- The flagship score remains `86`; score recalibration is not part of this plan.
- New task status is `portable-memory-compat-lane-verified`, not `published`.
- Do not modify `C:\dev\local-model`, `E:\local-model-run`, `public/index`, `forum`, `gather`, `crucible`, `telos`, `mneme`, `relay`, `plexus`, `telos-v2`, `portfolio-site`, `profile`, or either live Fable session tree.
- Never print live child-process command lines or the exposed Hugging Face credential.
- No push, merge, PR, release, publication, or worktree cleanup.

---

## Execution Preflight

- [ ] **Create the feature branch and verify environment provenance once before Task 1**

From the clean committed plan in the canonical worktree, the controller runs:

```powershell
$dirty = @(git status --porcelain)
if ($dirty) { throw "Canonical worktree is not clean: $($dirty -join ', ')" }
$existingBranch = @(git branch --list feat/brender-memory-compat)
if ($existingBranch) { throw 'Canonical feature branch already exists unexpectedly' }
$source = 'C:\dev\public\engine-revival-workspaces\brender-v132'
$sourceHead = git -C $source rev-parse HEAD
if ($sourceHead -ne 'd88d0ed41122664b9781015b517db64353e16f19') {
  throw "Unexpected BRender source snapshot: $sourceHead"
}
$sourceDirty = @(git -C $source status --porcelain)
if ($sourceDirty) { throw "Pinned BRender source is dirty: $($sourceDirty -join ', ')" }
git switch -c feat/brender-memory-compat
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
& 'C:\dev\worktrees\.venvs\engine-revival-brender-evidence\Scripts\python.exe' -c `
  "import pathlib, engine_revival.cli; p=pathlib.Path(engine_revival.cli.__file__).resolve(); root=pathlib.Path(r'C:\dev\worktrees\engine-revival-brender-evidence\src').resolve(); assert root in p.parents, p; print(p)"
```

Expected import path begins with
`C:\dev\worktrees\engine-revival-brender-evidence\src`. Stop before Task 1 if
the branch already exists unexpectedly, the working tree is not clean, or the
module resolves outside the isolated canonical worktree.

---

### Task 1: Generate the thirteenth semantic smoke target

**Files:**

- Create: `src/engine_revival/brender_memory_compat_sources.py`
- Modify: `src/engine_revival/brender_harness.py:7-88,38-57,207-235`
- Modify: `src/engine_revival/brender_harness_templates.py:169-208`
- Modify: `tests/test_brender_harness_materializer.py:38-247`

**Interfaces:**

- Consumes: existing `materialize_brender_core_harness(source_root, output_root)`, `CORE_FLOAT_DEFINES`, `BRENDER_CORE_INCLUDE_DIRS`, and `brender_core_float`.
- Produces: `memory_compat_smoke_source() -> str`, output `smoke/brender-core-memory-compat-smoke.c`, CMake/CTest target `brender_core_memory_compat_smoke`, and a final manifest entry with the same target name.
- Does not change: `portable_core_stubs_source()` semantics; the runtime defect must remain present for Task 2 RED.

- [ ] **Step 1: Add failing materializer expectations**

In `tests/test_brender_harness_materializer.py`, append the new source immediately after the plotter source in the exact `written` list:

```python
        output / "smoke" / "brender-core-plotter-smoke.c",
        output / "smoke" / "brender-core-memory-compat-smoke.c",
        output / "harness-manifest.json",
```

Add these CMake assertions after the plotter assertions:

```python
    assert "add_executable(brender_core_memory_compat_smoke" in cmake
    assert (
        "target_link_libraries(brender_core_memory_compat_smoke "
        "PRIVATE brender_core_float)"
    ) in cmake
    assert "add_test(NAME brender_core_memory_compat_smoke" in cmake
```

Read the generated source and assert all approved interfaces and constants:

```python
    memory_compat_smoke = (
        output / "smoke" / "brender-core-memory-compat-smoke.c"
    ).read_text(encoding="utf-8")
    for token in (
        "_MemPixelSet",
        "_MemPixelGet",
        "_MemFill_A",
        "_MemRectFill_A",
        "_MemCopyBits_A",
        "BrPixelmapCopyBits",
        "0xA1B2C3D4u",
        "0x00112233u",
        "0x2A",
        "COPY_START_BIT 2",
        "COPY_END_BIT 6",
        "pixel-prefix-canary",
        "rgb888-fill-canary",
        "rect-fill-padding",
        "copy-bits-raw-offset",
        "copy-bits-public-offset",
    ):
        assert token in memory_compat_smoke
```

Append the manifest target after the plotter target:

```python
        "brender_core_gouraud_smoke",
        "brender_core_plotter_smoke",
        "brender_core_memory_compat_smoke",
    ]
```

Require the generated README command and scope statement:

```python
    assert (
        "--target brender_core_memory_compat_smoke" in readme
    )
    assert "Host/DOS compatibility is not claimed" in readme
```

- [ ] **Step 2: Run the focused test to verify RED**

Run:

```powershell
& 'C:\dev\worktrees\.venvs\engine-revival-brender-evidence\Scripts\python.exe' `
  -m pytest tests/test_brender_harness_materializer.py::test_materialize_brender_core_harness_writes_out_of_tree_files -q
```

Expected: FAIL because the returned `written` list lacks
`brender-core-memory-compat-smoke.c`. A fixture/import error is not an acceptable RED.

- [ ] **Step 3: Create the dedicated source generator**

Create `src/engine_revival/brender_memory_compat_sources.py` with exactly:

```python
from __future__ import annotations


def memory_compat_smoke_source() -> str:
    """C source for direct and public-dispatch BRender memory semantics."""
    return r"""/*
 * BRender v1.3.2 portable memory compatibility smoke.
 *
 * Proves only pixel widths 1-4, RGB888 fill, positive-stride rectangular
 * fill, and nonzero-start copy-bits. Host/DOS and wider compatibility are
 * intentionally outside this executable.
 */
#define __BR_V1DB__ 0
#include "brender.h"
#include "pm.h"
#include "pm_ip.h"

#include <stdio.h>
#include <string.h>

#define SENTINEL ((br_uint_8)0x5A)
#define PIXEL_COLOUR 0xA1B2C3D4u
#define FILL_COLOUR 0x00112233u
#define COPY_SOURCE ((br_uint_8)0x2A)
#define COPY_COLOUR ((br_uint_32)0xE7)
#define COPY_START_BIT 2
#define COPY_END_BIT 6

static int fail(int code, const char *label)
{
    fprintf(stderr, "memory-compat:%s\n", label);
    return code;
}

static int check_pixel_widths(void)
{
    static const br_uint_32 expected[] = {
        0x000000D4u,
        0x0000C3D4u,
        0x00B2C3D4u,
        0xA1B2C3D4u
    };
    br_uint_8 buffer[6];
    br_uint_32 bytes;
    br_uint_32 i;

    for (bytes = 1; bytes <= 4; bytes++) {
        memset(buffer, SENTINEL, sizeof(buffer));
        _MemPixelSet((char *)(buffer + 1), 0, bytes, PIXEL_COLOUR);
        if (_MemPixelGet((char *)(buffer + 1), 0, bytes) != expected[bytes - 1]) {
            return fail(10 + (int)bytes, "pixel-value");
        }
        if (buffer[0] != SENTINEL) {
            return fail(15, "pixel-prefix-canary");
        }
        for (i = 1 + bytes; i < (br_uint_32)sizeof(buffer); i++) {
            if (buffer[i] != SENTINEL) {
                return fail(16, "pixel-suffix-canary");
            }
        }
    }
    return 0;
}

static int check_rgb888_fill(void)
{
    static const br_uint_8 expected[] = {
        0x33, 0x22, 0x11, 0x33, 0x22, 0x11
    };
    br_uint_8 buffer[8];

    memset(buffer, SENTINEL, sizeof(buffer));
    _MemFill_A((char *)(buffer + 1), 0, 2, 3, FILL_COLOUR);
    if (memcmp(buffer + 1, expected, sizeof(expected)) != 0) {
        return fail(20, "rgb888-fill-value");
    }
    if (buffer[0] != SENTINEL || buffer[7] != SENTINEL) {
        return fail(21, "rgb888-fill-canary");
    }
    return 0;
}

static int check_positive_stride_rect_fill(void)
{
    static const br_uint_8 expected_row[] = {
        0x33, 0x22, 0x11, 0x33, 0x22, 0x11
    };
    br_uint_8 buffer[16];
    br_uint_8 *row_start;
    br_uint_32 row;

    memset(buffer, SENTINEL, sizeof(buffer));
    _MemRectFill_A((char *)buffer, 0, 2, 2, 8, 3, FILL_COLOUR);
    for (row = 0; row < 2; row++) {
        row_start = buffer + row * 8;
        if (memcmp(row_start, expected_row, sizeof(expected_row)) != 0) {
            return fail(30 + (int)row, "rect-fill-value");
        }
        if (row_start[6] != SENTINEL || row_start[7] != SENTINEL) {
            return fail(32 + (int)row, "rect-fill-padding");
        }
    }
    return 0;
}

static int check_copy_bits_raw(void)
{
    br_uint_8 source[1] = {COPY_SOURCE};
    br_uint_8 destination[8];
    br_uint_8 expected;
    br_uint_32 i;

    memset(destination, SENTINEL, sizeof(destination));
    _MemCopyBits_A(
        (char *)destination, 0, 8, source, 1,
        COPY_START_BIT, COPY_END_BIT, 1, 1, COPY_COLOUR);

    for (i = 0; i < (br_uint_32)sizeof(destination); i++) {
        expected = (i == 2 || i == 4)
            ? (br_uint_8)COPY_COLOUR
            : SENTINEL;
        if (destination[i] != expected) {
            return fail(40, "copy-bits-raw-offset");
        }
    }
    return 0;
}

static int check_copy_bits_public(void)
{
    br_pixelmap *pixelmap = NULL;
    br_uint_8 source[1] = {COPY_SOURCE};
    br_uint_32 expected;
    br_uint_32 x;
    int code = 0;

    if (BrBegin() != BRE_OK) {
        return fail(50, "copy-bits-public-begin");
    }

    pixelmap = BrPixelmapAllocate(
        BR_PMT_INDEX_8, 10, 1, NULL, BR_PMAF_NORMAL);
    if (pixelmap == NULL) {
        code = fail(51, "copy-bits-public-allocate");
    } else {
        BrPixelmapFill(pixelmap, SENTINEL);
        BrPixelmapCopyBits(
            pixelmap, 3, 0, source, 1,
            COPY_START_BIT, COPY_END_BIT, 1, COPY_COLOUR);

        for (x = 0; x < 10; x++) {
            expected = (x == 3 || x == 5)
                ? COPY_COLOUR
                : (br_uint_32)SENTINEL;
            if (BrPixelmapPixelGet(pixelmap, (br_int_32)x, 0) != expected) {
                code = fail(52, "copy-bits-public-offset");
                break;
            }
        }
    }

    if (pixelmap != NULL) {
        BrPixelmapFree(pixelmap);
    }
    if (BrEnd() != BRE_OK && code == 0) {
        return fail(53, "copy-bits-public-end");
    }
    return code;
}

int main(void)
{
    int code;

    code = check_pixel_widths();
    if (code != 0) return code;

    code = check_rgb888_fill();
    if (code != 0) return code;

    code = check_positive_stride_rect_fill();
    if (code != 0) return code;

    code = check_copy_bits_raw();
    if (code != 0) return code;

    return check_copy_bits_public();
}
"""
```

- [ ] **Step 4: Wire the new source into the materializer**

In `src/engine_revival/brender_harness.py`, import:

```python
from engine_revival.brender_memory_compat_sources import memory_compat_smoke_source
```

Append the output path immediately after the plotter source:

```python
    "smoke/brender-core-plotter-smoke.c",
    "smoke/brender-core-memory-compat-smoke.c",
    "harness-manifest.json",
```

Add the file payload immediately after the plotter payload:

```python
        "smoke/brender-core-plotter-smoke.c": plotter_smoke_source(),
        "smoke/brender-core-memory-compat-smoke.c": memory_compat_smoke_source(),
        "harness-manifest.json": _manifest_json(source_lists),
```

Append the manifest target:

```python
            "brender_core_plotter_smoke",
            "brender_core_memory_compat_smoke",
```

- [ ] **Step 5: Wire CMake and generated README**

In `src/engine_revival/brender_harness_templates.py`, append after the plotter target:

```cmake

add_executable(brender_core_memory_compat_smoke
  smoke/brender-core-memory-compat-smoke.c)
target_include_directories(brender_core_memory_compat_smoke PRIVATE
  ${{BRENDER_CORE_INCLUDE_DIRS}})
target_compile_definitions(brender_core_memory_compat_smoke PRIVATE
{compile_definitions}
)
target_link_libraries(brender_core_memory_compat_smoke PRIVATE brender_core_float)
add_test(NAME brender_core_memory_compat_smoke
  COMMAND brender_core_memory_compat_smoke)
```

In `readme_source()`, add this command after the model-smoke command:

```powershell
cmake --build build --config Debug --target brender_core_memory_compat_smoke
```

Add this paragraph before the first-compiler-run paragraph:

```markdown
The `brender_core_memory_compat_smoke` target directly verifies 1-4-byte pixel
set/get, RGB888 fill, positive-stride rectangular fill, and nonzero-start
copy-bits through both the raw compatibility routine and public pixelmap
dispatch. Host/DOS compatibility is not claimed by this target.
```

- [ ] **Step 6: Run focused and full Python GREEN gates**

Run:

```powershell
& 'C:\dev\worktrees\.venvs\engine-revival-brender-evidence\Scripts\python.exe' `
  -m pytest tests/test_brender_harness_materializer.py -q
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
& 'C:\dev\worktrees\.venvs\engine-revival-brender-evidence\Scripts\python.exe' `
  -m pytest -q
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
git diff --check
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
```

Expected: all tests pass; no warnings or whitespace errors. Confirm changed paths are exactly the four Task 1 paths.

- [ ] **Step 7: Commit Task 1**

```powershell
git add src/engine_revival/brender_memory_compat_sources.py `
  src/engine_revival/brender_harness.py `
  src/engine_revival/brender_harness_templates.py `
  tests/test_brender_harness_materializer.py
git diff --cached --check
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
git commit -m "feat: add BRender memory compatibility smoke"
```

---

### Task 2: Reproduce and fix the copy-bits offset defect

**Files:**

- Modify only after runtime RED: `src/engine_revival/brender_compat_sources.py:118-139`
- External output: `C:\dev\public\engine-revival-workspaces\brender-v132-portable-core-harness-memory-compat-2026-07-09`
- External build: `C:\dev\public\engine-revival-workspaces\brender-v132-portable-core-build-memory-compat-2026-07-09`
- External RED transcript: `C:\dev\public\engine-revival-workspaces\brender-v132-portable-core-ctest-memory-compat-red-2026-07-09.txt`

**Interfaces:**

- Consumes: Task 1 materializer and `brender_core_memory_compat_smoke` target.
- Produces: captured executable RED, the minimal `_MemCopyBits_A` destination-index correction if the RED matches the prediction, and a targeted GREEN CTest.
- Does not produce: 13/13 repository evidence; that requires Task 3 full-ladder proof.

- [ ] **Step 1: Assert fresh external paths**

```powershell
$paths = @(
  'C:\dev\public\engine-revival-workspaces\brender-v132-portable-core-harness-memory-compat-2026-07-09',
  'C:\dev\public\engine-revival-workspaces\brender-v132-portable-core-build-memory-compat-2026-07-09',
  'C:\dev\public\engine-revival-workspaces\brender-v132-portable-core-ctest-memory-compat-red-2026-07-09.txt',
  'C:\dev\public\engine-revival-workspaces\brender-v132-portable-core-ctest-memory-compat-2026-07-09.txt'
)
$existing = @($paths | Where-Object { Test-Path -LiteralPath $_ })
if ($existing) { throw "Memory-compat evidence paths already exist: $($existing -join ', ')" }
```

Expected: no existing paths. Do not delete or reuse unexpected evidence paths.

- [ ] **Step 2: Materialize and configure the pre-fix harness**

```powershell
$cli = 'C:\dev\worktrees\.venvs\engine-revival-brender-evidence\Scripts\engine-revival.exe'
$source = 'C:\dev\public\engine-revival-workspaces\brender-v132'
$harness = 'C:\dev\public\engine-revival-workspaces\brender-v132-portable-core-harness-memory-compat-2026-07-09'
$build = 'C:\dev\public\engine-revival-workspaces\brender-v132-portable-core-build-memory-compat-2026-07-09'
& $cli materialize-brender-harness --source-root $source --output-root $harness
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
cmake -S $harness -B $build -A Win32 "-DBRENDER_SOURCE_DIR=$source"
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
cmake --build $build --config Debug --target brender_core_memory_compat_smoke
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
```

Expected: configure and targeted build exit 0. A compile failure is a separate diagnosis; do not edit semantics.

- [ ] **Step 3: Run targeted CTest and prove runtime RED**

```powershell
$build = 'C:\dev\public\engine-revival-workspaces\brender-v132-portable-core-build-memory-compat-2026-07-09'
$redTranscript = 'C:\dev\public\engine-revival-workspaces\brender-v132-portable-core-ctest-memory-compat-red-2026-07-09.txt'
$redOutput = & ctest --test-dir $build -C Debug `
  -R '^brender_core_memory_compat_smoke$' --no-tests=error `
  --output-on-failure 2>&1
$redExit = $LASTEXITCODE
$redOutput | Tee-Object -FilePath $redTranscript
if ($redExit -eq 0) { throw 'Copy-bits hypothesis disproved: targeted CTest passed' }
if (($redOutput -join "`n") -notmatch 'memory-compat:copy-bits-raw-offset') {
  throw 'Targeted RED did not match the predicted raw copy-bits offset failure'
}
```

Expected: one failed CTest with `memory-compat:copy-bits-raw-offset`. Preserve the
transcript. If the test passes, stop Task 2, record that the hypothesis was
disproved, do not edit `brender_compat_sources.py`, and revise the diagnosis
before proceeding. If it fails for another reason, diagnose that failure before
changing semantics.

- [ ] **Step 4: Apply the minimal production fix**

In `_MemCopyBits_A` inside `portable_core_stubs_source()`, replace only:

```c
                write_pixel(dest + pixel_bytes(bit - start_bit, bpp), bpp, colour);
```

with:

```c
                write_pixel(dest + pixel_bytes(bit, bpp), bpp, colour);
```

Do not refactor adjacent helpers or change host stubs.

- [ ] **Step 5: Re-materialize, rebuild, and prove targeted GREEN**

```powershell
$cli = 'C:\dev\worktrees\.venvs\engine-revival-brender-evidence\Scripts\engine-revival.exe'
$source = 'C:\dev\public\engine-revival-workspaces\brender-v132'
$harness = 'C:\dev\public\engine-revival-workspaces\brender-v132-portable-core-harness-memory-compat-2026-07-09'
$build = 'C:\dev\public\engine-revival-workspaces\brender-v132-portable-core-build-memory-compat-2026-07-09'
& $cli materialize-brender-harness --source-root $source --output-root $harness
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
cmake --build $build --config Debug --target brender_core_memory_compat_smoke
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
ctest --test-dir $build -C Debug `
  -R '^brender_core_memory_compat_smoke$' --no-tests=error `
  --output-on-failure
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
```

Expected: targeted CTest passes, proving the raw and public-dispatch checks.

- [ ] **Step 6: Run repository regressions and commit Task 2**

```powershell
& 'C:\dev\worktrees\.venvs\engine-revival-brender-evidence\Scripts\python.exe' `
  -m pytest tests/test_brender_harness_materializer.py -q
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
& 'C:\dev\worktrees\.venvs\engine-revival-brender-evidence\Scripts\python.exe' `
  -m pytest -q
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
git diff --check
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
git add src/engine_revival/brender_compat_sources.py
git diff --cached --check
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
git commit -m "fix: preserve BRender copy-bits destination offset"
```

Expected: all Python tests pass; Task 2 commit changes exactly one tracked file.

---

### Task 3: Capture immutable 13/13 attempt evidence

**Files:**

- Create: `tests/test_brender_memory_compat_evidence.py`
- Create: `attempts/brender-v132-portable-core-memory-compat-smoke.json`
- External transcript: `C:\dev\public\engine-revival-workspaces\brender-v132-portable-core-ctest-memory-compat-2026-07-09.txt`

**Interfaces:**

- Consumes: targeted GREEN from Task 2 and the same external build tree.
- Produces: a full 13/13 transcript and one immutable public-safe attempt record.
- Does not yet change: the five current checkpoint carriers; state promotion occurs atomically in Task 4.

- [ ] **Step 1: Build and run the complete ladder**

```powershell
$build = 'C:\dev\public\engine-revival-workspaces\brender-v132-portable-core-build-memory-compat-2026-07-09'
$finalTranscript = 'C:\dev\public\engine-revival-workspaces\brender-v132-portable-core-ctest-memory-compat-2026-07-09.txt'
cmake --build $build --config Debug
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
$fullOutput = & ctest --test-dir $build -C Debug --output-on-failure 2>&1
$fullExit = $LASTEXITCODE
$fullOutput | Tee-Object -FilePath $finalTranscript
if ($fullExit -ne 0) { exit $fullExit }
$joined = $fullOutput -join "`n"
if ($joined -notmatch '100% tests passed, 0 tests failed out of 13') {
  throw 'Full CTest output does not prove 13/13'
}
```

Expected: full build exits 0 and CTest proves 13/13.

- [ ] **Step 2: Add the failing attempt-record test**

Create `tests/test_brender_memory_compat_evidence.py`:

```python
import json
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
ATTEMPT_PATH = ROOT / "attempts" / "brender-v132-portable-core-memory-compat-smoke.json"
TRANSCRIPT = (
    "external-workspace:C:\\dev\\public\\engine-revival-workspaces\\"
    "brender-v132-portable-core-ctest-memory-compat-2026-07-09.txt"
)


def test_memory_compat_attempt_records_verified_full_ladder():
    assert ATTEMPT_PATH.exists(), "verified memory-compat attempt record is missing"
    attempt = json.loads(ATTEMPT_PATH.read_text(encoding="utf-8"))
    assert attempt["id"] == "brender-v132-portable-core-memory-compat-smoke"
    assert attempt["target_id"] == "brender"
    assert attempt["status"] == "completed"
    assert attempt["exit_code"] == 0
    assert attempt["transcript_location"] == TRANSCRIPT
    assert "13/13" in attempt["result_summary"]
    assert "memory-compat-red-2026-07-09.txt" in attempt["public_notes"]
    assert "Host/DOS" in attempt["public_notes"]
```

- [ ] **Step 3: Run the evidence test to verify RED**

```powershell
& 'C:\dev\worktrees\.venvs\engine-revival-brender-evidence\Scripts\python.exe' `
  -m pytest tests/test_brender_memory_compat_evidence.py -q
```

Expected: FAIL at the explicit missing-record assertion.

- [ ] **Step 4: Add the exact attempt record**

Create `attempts/brender-v132-portable-core-memory-compat-smoke.json`:

```json
{
  "artifacts_policy": "Harness, build tree, executable, library, and transcripts remain outside this metadata repo; public BRender source is read in place. No binaries, vendored source or assets, private material, or restricted SDK content committed.",
  "attempt_type": "cmake-memory-compat-semantic-smoke",
  "blockers": [
    "still requires a 32-bit C target",
    "host/DOS fallback policies are not covered by this memory semantic test",
    "overlap, negative-stride, colour-key, FPU-wrapper, and widths-above-four contracts remain separately scoped"
  ],
  "build_id": "brender-v132-build-environment",
  "command": "cmake --build C:/dev/public/engine-revival-workspaces/brender-v132-portable-core-build-memory-compat-2026-07-09 --config Debug --target brender_core_memory_compat_smoke; ctest --test-dir C:/dev/public/engine-revival-workspaces/brender-v132-portable-core-build-memory-compat-2026-07-09 -C Debug -R ^brender_core_memory_compat_smoke$ --no-tests=error --output-on-failure; cmake --build C:/dev/public/engine-revival-workspaces/brender-v132-portable-core-build-memory-compat-2026-07-09 --config Debug; ctest --test-dir C:/dev/public/engine-revival-workspaces/brender-v132-portable-core-build-memory-compat-2026-07-09 -C Debug --output-on-failure",
  "exit_code": 0,
  "harness_id": "brender-v132-portable-core-plan",
  "host_platform": "Windows, Visual Studio 18 2026, MSVC 19.50, Win32 Debug",
  "id": "brender-v132-portable-core-memory-compat-smoke",
  "next_actions": [
    "add a separately named host/DOS fail-closed contract smoke",
    "resolve the period three-byte colour-key discrepancy before claiming colour-key parity",
    "add negative-stride rectangle semantics as a separate test"
  ],
  "public_notes": "This attempt directly verifies the portable memory compatibility layer for 1-, 2-, 3-, and 4-byte pixel set/get with canaries, non-black RGB888 fill ordering, positive-stride rectangular fill with preserved padding, and nonzero-start MSB-first copy-bits. Copy-bits passes through both the raw _MemCopyBits_A entry point and BrPixelmapCopyBits public dispatch. The initial targeted CTest reproduced the raw destination-offset defect before the narrow index fix; its preserved transcript is external-workspace:C:\\dev\\public\\engine-revival-workspaces\\brender-v132-portable-core-ctest-memory-compat-red-2026-07-09.txt. The targeted test then passed and the complete Win32 Debug ladder passed 13/13; transcript_location points to that final full-ladder transcript. Host/DOS fallback policies, overlap, negative strides, colour-key parity, FPU wrappers, x64, drivers, softrend, and release packaging are not claimed.",
  "reproduction_id": "brender-critical-edition-source-build",
  "result_summary": "Built and ran brender_core_memory_compat_smoke with exit 0 after reproducing and narrowly fixing the copy-bits destination offset; targeted CTest passed and the complete portable-core ladder passed 13/13.",
  "source_ids": [
    "foone-brender-v132",
    "blazingrender-index"
  ],
  "status": "completed",
  "target_id": "brender",
  "transcript_location": "external-workspace:C:\\dev\\public\\engine-revival-workspaces\\brender-v132-portable-core-ctest-memory-compat-2026-07-09.txt"
}
```

- [ ] **Step 5: Verify GREEN, schema/public safety, and commit Task 3**

```powershell
& 'C:\dev\worktrees\.venvs\engine-revival-brender-evidence\Scripts\python.exe' `
  -m pytest tests/test_brender_memory_compat_evidence.py -q
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
& 'C:\dev\worktrees\.venvs\engine-revival-brender-evidence\Scripts\engine-revival.exe' validate --root .
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
& 'C:\dev\worktrees\.venvs\engine-revival-brender-evidence\Scripts\engine-revival.exe' audit-public --root .
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
git diff --check
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
git add tests/test_brender_memory_compat_evidence.py `
  attempts/brender-v132-portable-core-memory-compat-smoke.json
git diff --cached --check
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
git commit -m "test: record BRender memory compatibility attempt"
```

Expected: focused test and both CLI gates pass; exactly two files are committed.

---

### Task 4: Promote the verified 13/13 checkpoint and regenerate public views

**Files:**

- Modify: `tests/test_brender_state_consistency.py`
- Modify: `tasks/brender-critical-edition-packet.json`
- Modify: `reproductions/brender-critical-edition-source-build.json`
- Modify: `builds/brender-v132-build-environment.json`
- Modify: `harnesses/brender-v132-portable-core-plan.json`
- Modify: `readiness/brender-production-readiness.json`
- Modify: `docs/BRENDER-ARCHIVAL.md`
- Regenerate: BRender-related paths under `docs/generated/`

**Interfaces:**

- Consumes: immutable successful attempt from Task 3.
- Produces: one identical 13/13 `evidence_checkpoint` across five carriers, honest verified statuses, a 13-rung archival narrative, deterministic generated views, and unchanged score 86.
- Preserves: 3D Movie Maker pending step, packaging `not-started`, all unsupported boundaries, and every historical attempt/transcript.

- [ ] **Step 1: Update state tests first**

In `tests/test_brender_state_consistency.py`, replace `EXPECTED_CHECKPOINT` with:

```python
EXPECTED_CHECKPOINT = {
    "id": "brender-v132-portable-core-memory-compat-2026-07-09",
    "stage": "portable-core-memory-compat-lane-passing",
    "passed": 13,
    "total": 13,
    "source_snapshot": "d88d0ed41122664b9781015b517db64353e16f19",
}
MEMORY_ATTEMPT_ID = "brender-v132-portable-core-memory-compat-smoke"
```

Rename
`test_brender_state_records_share_verified_plotter_checkpoint` to
`test_brender_state_records_share_verified_memory_compat_checkpoint`, and
rename `test_brender_public_packet_describes_twelve_rungs_without_stale_deferral`
to `test_brender_public_packet_describes_thirteen_rungs_without_stale_deferral`.

Replace the status expectations with:

```python
    assert _load(STATE_PATHS[0])["status"] == "portable-memory-compat-lane-verified"
    assert (
        _load(STATE_PATHS[1])["status"]
        == "v132-portable-memory-compat-verified-3dmm-pending"
    )
    assert _load(STATE_PATHS[2])["status"] == "portable-core-memory-compat-lane-passing"
    assert _load(STATE_PATHS[3])["status"] == "portable-core-memory-compat-lane-passing"
    readiness = _load(STATE_PATHS[4])
    assert readiness["readiness_stage"] == "portable-core-memory-compat-lane-passing"
    assert readiness["build_status"] == "portable-core-memory-compat-lane-built"
    assert readiness["runtime_status"] == "memory-compat-lane-passing"
    assert readiness["test_status"] == "memory-compat-lane-passing"
    assert (
        readiness["modernization_status"]
        == "portable-memory-compat-verified-x64-port-scoped"
    )
    assert readiness["flagship_score"] == 86
    assert readiness["packaging_status"] == "not-started"
```

Replace twelve-rung packet assertions with:

```python
    assert "thirteenth CTest rung" in task["public_notes"]
    assert "CTest 13/13" in task["public_notes"]
    assert "score remains 86" in task["public_notes"]
    assert "eight verifying" not in task["public_notes"]
    assert "twelve verifying" not in task["public_notes"]
    assert any("thirteen verifying" in output for output in task["outputs"])
    assert all("eight verifying" not in output for output in task["outputs"])
    assert all("twelve verifying" not in output for output in task["outputs"])

    packet = (ROOT / "docs/BRENDER-ARCHIVAL.md").read_text(encoding="utf-8")
    assert "thirteen self-verifying portable-core smokes" in packet
    assert "13. Memory compatibility semantics" in packet
    assert "BrPixelmapCopyBits" in packet
    deferred = packet.split("## Honestly deferred", 1)[1].split("## Records", 1)[0]
    assert "Host/DOS fallback policies" in deferred
```

Add promotion coverage:

```python
def test_memory_compat_attempt_is_promoted_into_readiness():
    readiness = _load(STATE_PATHS[4])
    assert MEMORY_ATTEMPT_ID in readiness["attempt_ids"]
    assert any("CTest 13/13" in item for item in readiness["evidence"])
    assert any("host/DOS" in item for item in readiness["blockers"])
```

- [ ] **Step 2: Run the state test to verify RED**

```powershell
& 'C:\dev\worktrees\.venvs\engine-revival-brender-evidence\Scripts\python.exe' `
  -m pytest tests/test_brender_state_consistency.py -q
```

Expected: FAIL on the old 12/12 checkpoint/status/narrative, not an import error.

- [ ] **Step 3: Apply the exact checkpoint to all five records**

Use this identical object in task, reproduction, build, harness, and readiness:

```json
"evidence_checkpoint": {
  "id": "brender-v132-portable-core-memory-compat-2026-07-09",
  "stage": "portable-core-memory-compat-lane-passing",
  "passed": 13,
  "total": 13,
  "source_snapshot": "d88d0ed41122664b9781015b517db64353e16f19"
}
```

Set statuses exactly as asserted in Step 1. Leave readiness score `86` and packaging `not-started`.

- [ ] **Step 4: Reconcile task and reproduction narratives**

Set task `public_notes` to:

```text
The BRender critical-edition portable pure-C render lane remains published as the standalone brender-archival repository. The current feature branch verifies a thirteenth CTest rung at the pinned public v1.3.2 snapshot: direct memory compatibility semantics for 1-4 byte pixel set/get, non-black RGB888 fill, positive-stride rectangular fill, and nonzero-start copy-bits through both _MemCopyBits_A and BrPixelmapCopyBits. The complete Win32 Debug ladder passed CTest 13/13 and readiness score remains 86. Deferred work remains explicit: host/DOS fallback policy contracts, x64 pointer-width portability, period softrend translation, native .mat/.pal/.pix resolution, overlap, negative-stride and colour-key semantics, FPU wrappers, FIXED variants, drivers, packaged release binaries, and a full viewer.
```

Append `"memory-compatibility CTest transcript"` to task inputs. Replace the twelve-smoke output with:

```json
"thirteen verifying portable-core smokes (vector math through memory compatibility semantics)"
```

Set reproduction `public_notes` to:

```text
The public BRender v1.3.2 portable-core lane is verified through CTest 13/13 at the pinned source snapshot, including direct memory semantics through raw and public pixelmap dispatch. The separate 3D Movie Maker branch build attempt remains pending, so this reproduction recipe is not complete and its unexecuted 3D Movie Maker build step remains in the sequence.
```

- [ ] **Step 5: Reconcile build and harness records**

In the build record:

- replace the semantic-coverage blocker with:

```text
runtime coverage reaches the thirteen-rung portable-core ladder through direct memory compatibility semantics; host/DOS fallback policies, drivers, period softrend, and full renderer parity are not covered
```

- replace the memory/host semantic next action with:

```text
add separately named semantic contracts for portable host/DOS fallback policies; memory primitives now have direct coverage
```

- set `public_notes` to:

```text
BRender v1.3.2 public source was cloned outside the metadata repo and matched the recorded snapshot commit. The out-of-tree CMake/MSVC Win32 harness builds brender_core_float.lib and passes CTest 13/13 across the prior render/plotter ladder plus direct memory compatibility semantics for pixel widths 1-4, RGB888 fill, positive-stride rectangular fill, and nonzero-start copy-bits through raw and public dispatch. It does not claim x64 pointer-width portability, host/DOS fallback parity, overlap, negative strides, colour-key or FPU-wrapper parity, drivers, period softrend translation, native .mat/.pal/.pix resolution, FIXED variants, packaged release binaries, or a full viewer.
```

In the harness record:

- replace the compatibility blocker with:

```text
portable memory primitives have direct semantic coverage; host/DOS fallback stubs remain startup/link scaffolding, not full semantic replacements for DOS, driver, or rendering behavior
```

- append expected output:

```text
memory compatibility smoke executable and CTest transcript covering raw/public pixel operations
```

- append implementation unit:

```text
core memory compatibility target: brender_core_memory_compat_smoke directly verifies 1-4 byte pixel set/get, RGB888 fill, positive-stride rectangle fill, and nonzero-start copy-bits through _MemCopyBits_A and BrPixelmapCopyBits
```

- replace the semantic next action with:

```text
add a separately named host/DOS fail-closed contract smoke; memory primitives now have direct coverage
```

- change the build step to:

```text
build all thirteen brender_core_* smoke targets and run CTest with the selected multi-config build configuration
```

- set `public_notes` to:

```text
This public BRender harness design record converts the inspected period make topology into an out-of-tree CMake/MSVC Win32 build for the FLOAT core library. Its thirteen-rung CTest ladder covers the published render/plotter capabilities plus direct memory compatibility semantics for 1-4 byte pixel access, RGB888 fill, positive-stride rectangle fill, and nonzero-start copy-bits through raw and public dispatch. It does not claim x64 pointer-width portability, host/DOS fallback parity, warning cleanup, overlap, negative strides, colour-key or FPU-wrapper parity, period softrend translation, native .mat/.pal/.pix resolution, drivers, FIXED variants, packaged release binaries, or a full viewer.
```

- [ ] **Step 6: Reconcile readiness evidence and boundaries**

In readiness:

- replace the compatibility blocker with:

```text
portable memory primitives now have direct semantic coverage; host/DOS fallback policies still need a separately named semantic test before production use
```

- append evidence:

```text
2026-07-09 brender_core_memory_compat_smoke reproduced the nonzero-start _MemCopyBits_A destination-offset defect, passed after the narrow index fix through both raw and BrPixelmapCopyBits dispatch, and the complete Win32 Debug ladder passed CTest 13/13
```

- append attempt ID `brender-v132-portable-core-memory-compat-smoke`;
- replace the material/semantic next action with two actions:

```json
"add a separately named host/DOS fail-closed contract smoke",
"resolve original materials/textures from period .mat/.pal/.pix files"
```

- set `modernization_status` to `portable-memory-compat-verified-x64-port-scoped`;
- set `public_notes` to:

```text
BRender's pinned public v1.3.2 snapshot now has a successful out-of-tree CMake/MSVC Win32 FLOAT core-library build and a thirteen-rung self-verifying CTest ladder. The published render/plotter path remains intact, and the new feature-branch rung directly verifies 1-4 byte pixel set/get, non-black RGB888 fill, positive-stride rectangular fill, and nonzero-start copy-bits through both _MemCopyBits_A and BrPixelmapCopyBits. CTest 13/13 passed and flagship score remains 86. The pure-C portable path does not claim x64 pointer-width portability, host/DOS fallback parity, overlap, negative strides, colour-key or FPU-wrapper parity, period softrend translation, native .mat/.pal/.pix resolution, drivers, FIXED variants, packaged release binaries, or a full viewer; packaging remains not started.
```

- [ ] **Step 7: Update the archival narrative**

In `docs/BRENDER-ARCHIVAL.md`:

- change `twelve self-verifying render smokes` to `thirteen self-verifying portable-core smokes`;
- append rung 13:

```markdown
13. Memory compatibility semantics: direct 1-4-byte pixel set/get, non-black
    RGB888 fill, positive-stride rectangular fill, and nonzero-start copy-bits,
    with copy-bits also verified through `BrPixelmapCopyBits` public dispatch.
```

- add capability bullet:

```markdown
- Use directly verified portable memory primitives for 1-4-byte pixel access,
  RGB888 fill, positive-stride rectangle fill, and copy-bits dispatch.
```

- add deferred bullet:

```markdown
- Host/DOS fallback policies and the separately scoped overlap,
  negative-stride, colour-key, and FPU-wrapper contracts.
```

- [ ] **Step 8: Run focused GREEN and promote deterministic reports**

```powershell
& 'C:\dev\worktrees\.venvs\engine-revival-brender-evidence\Scripts\python.exe' `
  -m pytest tests/test_brender_state_consistency.py `
  tests/test_brender_memory_compat_evidence.py `
  tests/test_evidence_checkpoint_validate.py -q
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
$cli = 'C:\dev\worktrees\.venvs\engine-revival-brender-evidence\Scripts\engine-revival.exe'
& $cli validate --root .
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
& $cli audit-public --root .
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
& $cli report --root . | Out-Null
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
$before = Get-ChildItem docs\generated -Recurse -File | ForEach-Object {
  [pscustomobject]@{ Path = $_.FullName; Hash = (Get-FileHash $_.FullName -Algorithm SHA256).Hash }
}
& $cli report --root . | Out-Null
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
$after = Get-ChildItem docs\generated -Recurse -File | ForEach-Object {
  [pscustomobject]@{ Path = $_.FullName; Hash = (Get-FileHash $_.FullName -Algorithm SHA256).Hash }
}
$drift = Compare-Object $before $after -Property Path,Hash
if ($drift) { $drift; throw 'Second report generation changed output' }
```

Expected: focused tests and CLI gates pass; second report is byte-stable.

- [ ] **Step 9: Run full canonical gates and commit Task 4**

```powershell
& 'C:\dev\worktrees\.venvs\engine-revival-brender-evidence\Scripts\python.exe' `
  -m pytest -q
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
$cli = 'C:\dev\worktrees\.venvs\engine-revival-brender-evidence\Scripts\engine-revival.exe'
& $cli validate --root .
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
& $cli audit-public --root .
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
git diff --check
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
$attemptDiff = @(git diff --name-only `
  fde1f9a4621ea37f1a3f6dd4aa03e6943238d17b...HEAD -- attempts)
$unexpectedAttempts = @($attemptDiff | Where-Object {
  $_ -ne 'attempts/brender-v132-portable-core-memory-compat-smoke.json'
})
if ($unexpectedAttempts) {
  throw "Historical attempt changed: $($unexpectedAttempts -join ', ')"
}
if ($attemptDiff.Count -ne 1) {
  throw 'Expected exactly the new memory-compat attempt across the feature branch'
}
git add tasks reproductions builds harnesses readiness docs tests/test_brender_state_consistency.py
git diff --cached --check
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
git commit -m "docs: record BRender memory compatibility checkpoint"
```

Expected: all tests pass; the only attempt diff across the feature branch is the new Task 3 record; historical attempts remain untouched.

- [ ] **Step 10: Obtain an independent canonical review before mirroring**

Review the complete canonical delta from
`fix/evidence-checkpoint-hardening...feat/brender-memory-compat` against the
approved design, this implementation plan, generated C semantics, evidence
claims, and exclusion boundaries. Record the result at
`.superpowers/sdd/brender-memory-compat-final-review.md`. End the report with
two machine-checkable lines: first `Reviewed HEAD: ` followed by the exact
40-character hash returned by `git rev-parse HEAD`, then the exact line
`Review result: APPROVED`. Write the approval line only when no Critical or
Important finding remains.

Do not begin Task 5 while any Critical or Important finding remains. For any
runtime-affecting correction, repeat the targeted build, complete 13/13 CTest,
transcript, Python, validation, audit, report-determinism, and evidence gates
before re-review. For documentation-only corrections, repeat the full Python,
validation, audit, and deterministic-report gates before re-review.

---

### Task 5: Mirror the independently reviewed canonical capability into brender-archival

**Files:**

- Canonical source: reviewed delta beginning at `fde1f9a4621ea37f1a3f6dd4aa03e6943238d17b` and ending at the exact canonical HEAD captured after Task 4 gates.
- Mirror worktree: `C:\dev\worktrees\brender-archival-evidence`.
- Mirror base: `500d9bc16281e966373f6cf87bc3fa569f55a32f`.
- Protected: mirror `README.md`, `LICENSE`, `pyproject.toml`, and `gallery/**`.

**Interfaces:**

- Consumes: clean, reviewed canonical branch with Python gates and Win32 CTest 13/13.
- Produces: local mirror branch `feat/brender-memory-compat` whose shared blobs equal canonical and whose archive identity/gallery hashes are unchanged.
- Does not produce: push, merge, PR, release, publication, or cleanup.

- [ ] **Step 1: Verify canonical and mirror baselines**

```powershell
$canonical = 'C:\dev\worktrees\engine-revival-brender-evidence'
$mirror = 'C:\dev\worktrees\brender-archival-evidence'
$canonicalDirty = @(git -C $canonical status --porcelain)
$mirrorDirty = @(git -C $mirror status --porcelain)
if ($canonicalDirty) { throw "Canonical worktree is dirty: $($canonicalDirty -join ', ')" }
if ($mirrorDirty) { throw "Mirror worktree is dirty: $($mirrorDirty -join ', ')" }
$reviewPath = Join-Path $canonical '.superpowers\sdd\brender-memory-compat-final-review.md'
if (-not (Test-Path -LiteralPath $reviewPath)) {
  throw "Canonical review report is missing: $reviewPath"
}
$reviewText = Get-Content -Raw -Encoding utf8 $reviewPath
if ($reviewText -notmatch '(?m)^Reviewed HEAD: ([0-9a-f]{40})\r?\nReview result: APPROVED\r?\n?\z') {
  throw 'Canonical review lacks a HEAD-bound final approval block'
}
$reviewedHead = $Matches[1]
$canonicalHead = git -C $canonical rev-parse HEAD
if ($reviewedHead -ne $canonicalHead) {
  throw "Canonical review is stale: reviewed $reviewedHead, current $canonicalHead"
}
$canonicalBranch = git -C $canonical branch --show-current
if ($canonicalBranch -ne 'feat/brender-memory-compat') {
  throw "Unexpected canonical branch: $canonicalBranch"
}
$mirrorHead = git -C $mirror rev-parse HEAD
if ($mirrorHead -ne '500d9bc16281e966373f6cf87bc3fa569f55a32f') {
  throw "Unexpected mirror base: $mirrorHead"
}
git -C $canonical status --short --branch
git -C $mirror status --short --branch
git -C $canonical rev-parse HEAD
git -C $mirror rev-parse HEAD
```

Expected: both clean; mirror is exactly `500d9bc16281e966373f6cf87bc3fa569f55a32f` before branching.

- [ ] **Step 2: Create the mirror feature branch and verify its CLI provenance**

```powershell
$mirror = 'C:\dev\worktrees\brender-archival-evidence'
$existingMirrorBranch = @(git -C $mirror branch --list feat/brender-memory-compat)
if ($existingMirrorBranch) {
  throw 'Mirror feature branch already exists unexpectedly'
}
git -C $mirror switch -c feat/brender-memory-compat 500d9bc16281e966373f6cf87bc3fa569f55a32f
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
$mirrorBranch = git -C $mirror branch --show-current
if ($mirrorBranch -ne 'feat/brender-memory-compat') {
  throw "Unexpected mirror branch after switch: $mirrorBranch"
}
& 'C:\dev\worktrees\.venvs\brender-archival-evidence\Scripts\python.exe' -c `
  "import pathlib, engine_revival.cli; p=pathlib.Path(engine_revival.cli.__file__).resolve(); root=pathlib.Path(r'C:\dev\worktrees\brender-archival-evidence\src').resolve(); assert root in p.parents, p; print(p)"
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
& 'C:\dev\worktrees\.venvs\brender-archival-evidence\Scripts\python.exe' `
  -m pytest -q $mirror
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
& 'C:\dev\worktrees\.venvs\brender-archival-evidence\Scripts\engine-revival.exe' validate --root $mirror
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
& 'C:\dev\worktrees\.venvs\brender-archival-evidence\Scripts\engine-revival.exe' audit-public --root $mirror
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
```

Run the test command with working directory `$mirror`. Expected import path is under the mirror worktree; all baseline gates pass.

- [ ] **Step 3: Hash-protect archive-owned files and copy only the canonical delta**

Run the snapshot and copy in one PowerShell process:

```powershell
$canonical = 'C:\dev\worktrees\engine-revival-brender-evidence'
$mirror = 'C:\dev\worktrees\brender-archival-evidence'
$reviewText = Get-Content -Raw -Encoding utf8 `
  (Join-Path $canonical '.superpowers\sdd\brender-memory-compat-final-review.md')
if ($reviewText -notmatch '(?m)^Reviewed HEAD: ([0-9a-f]{40})\r?\nReview result: APPROVED\r?\n?\z') {
  throw 'Canonical review lacks a HEAD-bound final approval block'
}
$reviewedHead = $Matches[1]
$canonicalHead = git -C $canonical rev-parse HEAD
if ($reviewedHead -ne $canonicalHead) {
  throw "Canonical review is stale: reviewed $reviewedHead, current $canonicalHead"
}
$protected = @('README.md', 'LICENSE', 'pyproject.toml') + @(
  Get-ChildItem (Join-Path $mirror 'gallery') -Recurse -File |
    ForEach-Object { $_.FullName.Substring($mirror.Length + 1) }
)
$before = @{}
foreach ($relative in $protected) {
  $before[$relative] = (Get-FileHash (Join-Path $mirror $relative) -Algorithm SHA256).Hash
}
$canonicalRange = "fde1f9a4621ea37f1a3f6dd4aa03e6943238d17b...$reviewedHead"
$changed = git -C $canonical diff --name-only $canonicalRange
$excluded = @('README.md', 'LICENSE', 'pyproject.toml')
foreach ($relative in $changed) {
  if ($excluded -contains $relative -or $relative -like 'gallery/*') { continue }
  $sourcePath = Join-Path $canonical $relative
  $targetPath = Join-Path $mirror $relative
  if (-not (Test-Path -LiteralPath $sourcePath)) {
    throw "Canonical delta path is missing: $relative"
  }
  $parent = Split-Path -Parent $targetPath
  if ($parent) { New-Item -ItemType Directory -Force -Path $parent | Out-Null }
  Copy-Item -LiteralPath $sourcePath -Destination $targetPath -Force
}
$changedProtected = @($protected | Where-Object {
  (Get-FileHash (Join-Path $mirror $_) -Algorithm SHA256).Hash -ne $before[$_]
})
if ($changedProtected) {
  throw "Protected mirror files changed: $($changedProtected -join ', ')"
}
```

Expected: zero protected hash changes.

- [ ] **Step 4: Verify mirror semantics and deterministic reports**

From the mirror worktree:

```powershell
$canonical = 'C:\dev\worktrees\engine-revival-brender-evidence'
$mirror = 'C:\dev\worktrees\brender-archival-evidence'
$reviewText = Get-Content -Raw -Encoding utf8 `
  (Join-Path $canonical '.superpowers\sdd\brender-memory-compat-final-review.md')
if ($reviewText -notmatch '(?m)^Reviewed HEAD: ([0-9a-f]{40})\r?\nReview result: APPROVED\r?\n?\z') {
  throw 'Canonical review lacks a HEAD-bound final approval block'
}
$reviewedHead = $Matches[1]
$canonicalHead = git -C $canonical rev-parse HEAD
if ($reviewedHead -ne $canonicalHead) {
  throw "Canonical review is stale: reviewed $reviewedHead, current $canonicalHead"
}
$cli = 'C:\dev\worktrees\.venvs\brender-archival-evidence\Scripts\engine-revival.exe'
& 'C:\dev\worktrees\.venvs\brender-archival-evidence\Scripts\python.exe' `
  -m pytest -q $mirror
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
& $cli validate --root $mirror
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
& $cli audit-public --root $mirror
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
& $cli report --root $mirror | Out-Null
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
$reportBefore = Get-ChildItem (Join-Path $mirror 'docs\generated') -Recurse -File | ForEach-Object {
  [pscustomobject]@{
    Path = $_.FullName
    Hash = (Get-FileHash $_.FullName -Algorithm SHA256).Hash
  }
}
& $cli report --root $mirror | Out-Null
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
$reportAfter = Get-ChildItem (Join-Path $mirror 'docs\generated') -Recurse -File | ForEach-Object {
  [pscustomobject]@{
    Path = $_.FullName
    Hash = (Get-FileHash $_.FullName -Algorithm SHA256).Hash
  }
}
$reportDrift = Compare-Object $reportBefore $reportAfter -Property Path,Hash
if ($reportDrift) { $reportDrift; throw 'Second mirror report changed output' }
$canonicalRange = "fde1f9a4621ea37f1a3f6dd4aa03e6943238d17b...$reviewedHead"
$copied = @(git -C $canonical diff --name-only $canonicalRange | Where-Object {
    $_ -notin @('README.md', 'LICENSE', 'pyproject.toml') -and
    $_ -notlike 'gallery/*'
  })
$blobMismatch = @($copied | Where-Object {
  (Get-FileHash (Join-Path $canonical $_) -Algorithm SHA256).Hash -ne
  (Get-FileHash (Join-Path $mirror $_) -Algorithm SHA256).Hash
})
if ($blobMismatch) {
  throw "Canonical/mirror blob mismatch: $($blobMismatch -join ', ')"
}
git -C $mirror diff --check
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
$protectedDiff = @(git -C $mirror diff --name-only -- `
  README.md LICENSE pyproject.toml gallery)
if ($protectedDiff) {
  throw "Protected mirror path changed after report: $($protectedDiff -join ', ')"
}
```

Expected: the second report is byte-stable, every copied path matches canonical
by SHA-256, and protected paths have no diff.

- [ ] **Step 5: Commit exact canonical provenance**

```powershell
$canonical = 'C:\dev\worktrees\engine-revival-brender-evidence'
$mirror = 'C:\dev\worktrees\brender-archival-evidence'
$reviewText = Get-Content -Raw -Encoding utf8 `
  (Join-Path $canonical '.superpowers\sdd\brender-memory-compat-final-review.md')
if ($reviewText -notmatch '(?m)^Reviewed HEAD: ([0-9a-f]{40})\r?\nReview result: APPROVED\r?\n?\z') {
  throw 'Canonical review lacks a HEAD-bound final approval block'
}
$reviewedHead = $Matches[1]
$canonicalHead = git -C $canonical rev-parse HEAD
if ($reviewedHead -ne $canonicalHead) {
  throw "Canonical review is stale: reviewed $reviewedHead, current $canonicalHead"
}
git -C $mirror add -A
git -C $mirror diff --cached --check
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
$canonicalRange = "fde1f9a4621ea37f1a3f6dd4aa03e6943238d17b...$reviewedHead"
$expectedShared = @(git -C $canonical diff --name-only $canonicalRange |
  Where-Object {
    $_ -notin @('README.md', 'LICENSE', 'pyproject.toml') -and
    $_ -notlike 'gallery/*'
  } | Sort-Object -Unique)
$stagedPaths = @(git -C $mirror diff --cached --name-only |
  Sort-Object -Unique)
$pathDrift = Compare-Object $expectedShared $stagedPaths
if ($pathDrift) {
  $pathDrift
  throw 'Staged mirror path set differs from reviewed canonical delta'
}
$stagedProtected = @(git -C $mirror diff --cached --name-only -- `
  README.md LICENSE pyproject.toml gallery)
if ($stagedProtected) {
  throw "Protected mirror path is staged: $($stagedProtected -join ', ')"
}
git -C $mirror commit -m "sync: BRender memory compatibility from engine-revival $canonicalHead"
```

Expected: commit subject contains the exact full canonical hash and no protected path.

- [ ] **Step 6: Run final canonical and mirror gates**

In each worktree, run:

```powershell
$canonical = 'C:\dev\worktrees\engine-revival-brender-evidence'
$mirror = 'C:\dev\worktrees\brender-archival-evidence'
$canonicalPython = 'C:\dev\worktrees\.venvs\engine-revival-brender-evidence\Scripts\python.exe'
$canonicalCli = 'C:\dev\worktrees\.venvs\engine-revival-brender-evidence\Scripts\engine-revival.exe'
$mirrorPython = 'C:\dev\worktrees\.venvs\brender-archival-evidence\Scripts\python.exe'
$mirrorCli = 'C:\dev\worktrees\.venvs\brender-archival-evidence\Scripts\engine-revival.exe'

& $canonicalPython -m pytest -q $canonical
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
& $canonicalCli validate --root $canonical
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
& $canonicalCli audit-public --root $canonical
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
git -C $canonical diff --exit-code
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
$canonicalDirty = @(git -C $canonical status --porcelain)
if ($canonicalDirty) { throw "Canonical worktree is dirty: $($canonicalDirty -join ', ')" }
git -C $canonical status --short --branch

& $mirrorPython -m pytest -q $mirror
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
& $mirrorCli validate --root $mirror
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
& $mirrorCli audit-public --root $mirror
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
git -C $mirror diff --exit-code
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
$mirrorDirty = @(git -C $mirror status --porcelain)
if ($mirrorDirty) { throw "Mirror worktree is dirty: $($mirrorDirty -join ', ')" }
git -C $mirror status --short --branch
```

Expected: all tests and CLI gates pass; both worktrees are clean on their
feature branches. Preserve both branches and worktrees locally.

---

## Plan Self-Review Checklist

- [x] Every approved semantic contract maps to both generated C checks and materializer assertions.
- [x] Raw and public-dispatch copy-bits paths are both covered.
- [x] Runtime RED precedes the single production fix.
- [x] Full 13/13 proof precedes attempt creation and five-record promotion.
- [x] The checkpoint ID, stage, source snapshot, and pass/total values are identical everywhere.
- [x] Task status is `verified`, not `published`; score remains 86 and packaging remains `not-started`.
- [x] 3D Movie Maker remains pending.
- [x] Host/DOS and all other non-goals remain explicit.
- [x] Historical attempts/transcripts are immutable.
- [x] CLI commands use worktree-bound isolated environments.
- [x] Mirror-specific identity and gallery files are hash-protected.
- [x] The final canonical review is bound to the exact reviewed HEAD.
- [x] The staged mirror path set must equal the reviewed canonical shared delta.
- [x] No command touches a live Fable exclusion path.
- [x] No push, merge, PR, release, publication, or cleanup is authorized.
- [x] No placeholders, undefined interfaces, or cross-task naming mismatches remain.
