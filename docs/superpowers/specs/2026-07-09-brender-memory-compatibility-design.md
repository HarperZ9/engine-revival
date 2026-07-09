# BRender Portable Memory Compatibility Semantic CTest Design

Date: 2026-07-09

Status: Approved design direction; written specification awaiting operator review

## Objective

Add a thirteenth Win32 CTest to the materialized BRender v1.3.2 portable-core
harness that directly proves four narrow memory-compatibility contracts. The
new test must exercise the generated compatibility implementation as compiled
with the pinned public BRender snapshot, expose any real semantic defect before
production code changes, and promote repository evidence only after the
targeted test and the complete 13-test ladder pass.

This is the next executable BRender revival capability after the reviewed
12/12 plotter checkpoint and evidence-consistency hardening. It is not a host
or DOS compatibility claim.

## Approved Decision

Use one dedicated semantic executable:

`brender_core_memory_compat_smoke`

It covers only:

1. 1-, 2-, 3-, and 4-byte pixel set/get semantics.
2. Non-black RGB888 fill byte order and repetition.
3. Positive-stride rectangular fill and padding preservation.
4. MSB-first copy-bits behavior with a nonzero start bit.

Host/DOS fallback policies remain a separately named follow-on slice. Negative
strides, overlapping copies, colour-key edge cases, FPU wrappers, widths above
four bytes, native material resolution, period `softrend`, x64, drivers, FIXED
variants, packaging, and a viewer are outside this design.

## Current Evidence

- Canonical source snapshot:
  `d88d0ed41122664b9781015b517db64353e16f19`.
- The existing materializer emits a Win32-only CMake harness with twelve CTest
  targets and two generated compatibility sources.
- The reviewed checkpoint records CTest 12/12 and keeps semantic compatibility
  stubs explicitly deferred.
- Existing render coverage indirectly exercises black fill and one RGB888
  pixel round trip, but it does not isolate the compatibility contracts below.
- `core/pixelmap/pm_ip.h` declares the period memory entry points.
- `core/pixelmap/memloops.asm` treats source bit zero as `0x80`, writes pixels at
  their original bit positions, and accepts only pixel widths one through four
  for direct pixel set/get.
- `core/pixelmap/pmmem.c` biases the destination pointer backward before calling
  `_MemCopyBits_A`, confirming that the raw routine retains the source bit
  offset in its destination indexing.
- The generated C fallback currently writes copy-bits destinations at
  `bit - start_bit`. Static comparison predicts an offset defect, but no code
  change is authorized until the executable test reproduces it.

## Architecture

### Dedicated smoke-source module

Create:

`src/engine_revival/brender_memory_compat_sources.py`

It exports one function:

`memory_compat_smoke_source() -> str`

The module owns only the generated semantic-test program. It does not own the
compatibility implementation. This follows the existing one-module-per-render
smoke pattern and avoids making `brender_compat_sources.py` responsible for
both implementation and its direct semantic test.

### Existing compatibility implementation

`src/engine_revival/brender_compat_sources.py` remains the source of
`compat/brender-portable-core-stubs.c`.

The only permitted production change in this slice is a narrow
`_MemCopyBits_A` destination-index correction if, and only if, the new Win32
CTest fails in the predicted way. No adjacent memory helpers are refactored.

### Harness materializer

Update `src/engine_revival/brender_harness.py` to:

- import `memory_compat_smoke_source`;
- emit `smoke/brender-core-memory-compat-smoke.c`;
- append that path immediately before `harness-manifest.json` in
  `OUTPUT_FILES`; and
- append `brender_core_memory_compat_smoke` after the plotter target in the
  manifest's `smoke_targets` list.

Appending preserves every historical target's order and keeps the plotter as
test 12.

### CMake and generated README

Update `src/engine_revival/brender_harness_templates.py` to append:

- executable target `brender_core_memory_compat_smoke`;
- the existing BRender include directories and compile definitions;
- linkage to `brender_core_float`; and
- CTest `brender_core_memory_compat_smoke` with no file arguments.

The generated harness README must list the new build target and describe the
four proven contracts without implying host/DOS, driver, renderer, x64, or
release-package parity.

### Python integration coverage

Extend `tests/test_brender_harness_materializer.py` to require:

- the new output path in exact order;
- the executable, linkage, and CTest declarations;
- the target as the final manifest smoke target; and
- source tokens proving that each of the four approved contracts is emitted.

The actual Win32 executable remains the semantic authority. Python string
assertions protect materializer wiring, not runtime correctness.

## Generated C Program

The program includes the public and internal period declarations:

```c
#define __BR_V1DB__ 0
#include "brender.h"
#include "pm.h"
#include "pm_ip.h"
```

It uses explicit return codes and deterministic `stderr` labels rather than
`assert()`, so release-style compile definitions cannot remove checks. Buffers
are initialized to nonzero sentinel bytes and checked completely after each
operation.

### Contract 1: pixel set/get widths 1–4

For each width from one through four:

- reset a guarded buffer to a sentinel value;
- call `_MemPixelSet` with colour `0xA1B2C3D4` at an interior pointer;
- call `_MemPixelGet` for the same width;
- require the low 8, 16, 24, or 32 bits respectively; and
- require prefix and suffix canaries to remain unchanged.

No behavior is claimed for width zero or widths above four.

### Contract 2: non-black RGB888 fill

Call `_MemFill_A` for two three-byte pixels with colour `0x00112233`.

Require each pixel to contain bytes `33 22 11` in hexadecimal order, matching
BRender's `0x00RRGGBB` colour representation and period little-endian RGB888
storage. Guard bytes immediately before and after the six-byte destination
must remain unchanged.

### Contract 3: positive-stride rectangular fill

Call `_MemRectFill_A` for a two-by-two RGB888 rectangle with an eight-byte row
stride and the same non-black colour.

Require six pixel bytes on each row to change to `33 22 11 33 22 11`; require
both padding bytes on each row to remain at the sentinel value. This design
does not establish negative-stride behavior.

### Contract 4: nonzero-start copy-bits

Use one source byte `0x2A`, one row, one-byte destination pixels,
`start_bit = 2`, `end_bit = 6`, and colour `0xE7`.

Bit numbering is MSB-first. Source positions 2 and 4 are set and must write
destination positions 2 and 4. Position 6 is excluded by the half-open end
bound. Every other destination byte must retain its sentinel value.

The current C fallback is expected to write positions 0 and 2. If the targeted
CTest demonstrates exactly that failure, replace `bit - start_bit` with `bit`
in the destination index. If the test fails differently or passes, stop and
investigate; do not apply the predicted fix by inspection alone.

## TDD and Execution Flow

### Phase 1: materializer RED

Add the Python expectations before generator or CMake changes and run:

```powershell
python -m pytest tests/test_brender_harness_materializer.py -q
```

The failure must be missing output/wiring for the thirteenth target, not a
fixture or import error.

### Phase 2: materializer GREEN

Add the dedicated source generator and the minimal harness/template wiring.
Re-run the focused Python test, then the full Python suite.

### Phase 3: fresh external harness

Materialize into:

`C:\dev\public\engine-revival-workspaces\brender-v132-portable-core-harness-memory-compat-2026-07-09`

Configure/build in:

`C:\dev\public\engine-revival-workspaces\brender-v132-portable-core-build-memory-compat-2026-07-09`

Use the isolated editable environment bound to the active canonical worktree:

```powershell
engine-revival materialize-brender-harness `
  --source-root C:\dev\public\engine-revival-workspaces\brender-v132 `
  --output-root C:\dev\public\engine-revival-workspaces\brender-v132-portable-core-harness-memory-compat-2026-07-09

cmake -S C:\dev\public\engine-revival-workspaces\brender-v132-portable-core-harness-memory-compat-2026-07-09 `
  -B C:\dev\public\engine-revival-workspaces\brender-v132-portable-core-build-memory-compat-2026-07-09 `
  -A Win32 `
  "-DBRENDER_SOURCE_DIR=C:\dev\public\engine-revival-workspaces\brender-v132"

cmake --build C:\dev\public\engine-revival-workspaces\brender-v132-portable-core-build-memory-compat-2026-07-09 `
  --config Debug --target brender_core_memory_compat_smoke

ctest --test-dir C:\dev\public\engine-revival-workspaces\brender-v132-portable-core-build-memory-compat-2026-07-09 `
  -C Debug -R "^brender_core_memory_compat_smoke$" --output-on-failure
```

Capture the targeted RED transcript before any compatibility implementation
change.

### Phase 4: minimal runtime fix and GREEN

Only after a reproduced offset failure, apply the single copy-bits index fix.
Re-materialize into the same disposable external harness, rebuild the targeted
executable, and require the targeted CTest to pass.

Then build the default target set and run the complete ladder:

```powershell
cmake --build C:\dev\public\engine-revival-workspaces\brender-v132-portable-core-build-memory-compat-2026-07-09 --config Debug
ctest --test-dir C:\dev\public\engine-revival-workspaces\brender-v132-portable-core-build-memory-compat-2026-07-09 -C Debug --output-on-failure
```

The final transcript path is:

`C:\dev\public\engine-revival-workspaces\brender-v132-portable-core-ctest-memory-compat-2026-07-09.txt`

No 13/13 claim is permitted until this complete run succeeds.

## Evidence Promotion

After a real 13/13 run, add attempt record:

`attempts/brender-v132-portable-core-memory-compat-smoke.json`

Use this exact shared checkpoint across task, reproduction, build, harness, and
readiness records:

```json
{
  "id": "brender-v132-portable-core-memory-compat-2026-07-09",
  "stage": "portable-core-memory-compat-lane-passing",
  "passed": 13,
  "total": 13,
  "source_snapshot": "d88d0ed41122664b9781015b517db64353e16f19"
}
```

Record states become:

- task: `portable-memory-compat-lane-verified`;
- reproduction: `v132-portable-memory-compat-verified-3dmm-pending`;
- build: `portable-core-memory-compat-lane-passing`;
- harness: `portable-core-memory-compat-lane-passing`;
- readiness stage: `portable-core-memory-compat-lane-passing`;
- readiness build: `portable-core-memory-compat-lane-built`;
- readiness runtime/test: `memory-compat-lane-passing`;
- packaging: `not-started`; and
- flagship score: `86`; score recalibration is a separate reviewed change.

The task status says `verified`, not `published`, because this plan does not
authorize a push or public release. Public notes retain that the earlier
plotter lane is already published while the new memory-semantic result is
verified on the feature branch.

Update the compatibility blocker to state that the four approved memory
contracts now have direct semantic coverage while host/DOS fallback policies
remain unverified. Preserve every historical attempt and transcript
byte-for-byte.

Update `tests/test_brender_state_consistency.py`, the archival narrative, and
deterministically generated reports. Run the checkpoint validator before and
after report generation.

## Failure Handling

- Materializer-test failure outside the expected missing target/wiring means
  stop and repair the test fixture.
- Compile failure means capture the full compiler transcript and diagnose the
  declaration or ABI boundary before changing semantics.
- A targeted runtime failure other than the predicted copy-bits offset means
  preserve the output and use systematic debugging; do not bundle fixes.
- If the targeted test passes before a production fix, record that the static
  hypothesis was disproved and leave `_MemCopyBits_A` unchanged.
- A full-ladder regression blocks evidence promotion even if the targeted test
  passes.
- Any mismatch among the five checkpoint carriers blocks report generation and
  mirroring.

## Public-Safety and Scope Boundaries

- Read the pinned public BRender checkout in place; do not vendor source or
  assets into either repository.
- Write harness, build products, and transcripts only under
  `engine-revival-workspaces`.
- Do not touch either active Fable workflow or its repositories.
- Do not claim DOS interrupts/selectors, host fallback parity, overlap safety,
  negative strides, colour-key parity, x64 safety, period `softrend`, native
  `.mat/.pal/.pix` resolution, FIXED variants, drivers, packaged binaries, or a
  full viewer.
- Do not push, merge, create a PR, publish, or release in this slice.

## Canonical and Mirror Flow

Implement and review the capability in the canonical `engine-revival`
worktree. After all Python, Win32 CTest, evidence, validation, audit, report,
and review gates pass, copy only the reviewed canonical delta into a new
isolated `brender-archival` branch.

Hash-protect the mirror's `README.md`, `LICENSE`, `pyproject.toml`, and gallery;
require every copied shared blob to match canonical. The mirror receives no
push, merge, release, or cleanup without separate authorization.

## Acceptance Criteria

- Checkpoint diagnostic hardening is present and green before feature work.
- The materializer deterministically emits the thirteenth source and target.
- Python materializer tests demonstrate RED then GREEN.
- The generated C program implements exactly the four approved contracts with
  canaries and deterministic failure codes.
- A fresh Win32 build compiles and links the new executable.
- The predicted copy-bits defect is either reproduced before its narrow fix or
  explicitly disproved with captured evidence.
- Targeted CTest passes.
- Complete CTest passes 13/13 and its transcript is retained externally.
- All Python tests pass.
- Worktree-bound `engine-revival validate` and `audit-public` pass.
- Report generation is deterministic.
- The five canonical records share the exact 13/13 checkpoint.
- Historical attempts and transcripts remain unchanged.
- Public text states precisely what memory semantics are proven and what
  remains deferred.
- The reviewed mirror delta preserves all archive-owned files.

## Ordered Follow-On Slices

After this memory-semantic checkpoint:

1. Host/DOS neutral and `BRE_UNSUPPORTED` policy contracts in a separately
   named CTest.
2. Two-byte source-colour-key semantics after resolving the period three-byte
   discrepancy.
3. Negative-stride rectangle behavior.
4. Faithful public `.pix`/`.pal` material resolution.
5. Wider viewer and packaging work.
6. Debugger-led x64 pointer-width port and period `softrend` translation as
   independent systemic projects.
