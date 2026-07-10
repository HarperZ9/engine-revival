import json
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
ATTEMPT_PATH = ROOT / "attempts" / "brender-v132-portable-core-memory-compat-smoke.json"
TRANSCRIPT = (
    "external-workspace:C:\\dev\\public\\engine-revival-workspaces\\"
    "brender-v132-portable-core-ctest-memory-compat-2026-07-09.txt"
)
RED_TRANSCRIPT = (
    "external-workspace:C:\\dev\\public\\engine-revival-workspaces\\"
    "brender-v132-portable-core-ctest-memory-compat-red-2026-07-09.txt"
)
POSITIVE_CLAUSE = (
    "The targeted test then passed and the complete Win32 Debug ladder passed 13/13;"
)
NON_CLAIM = (
    "Host/DOS fallback policies, overlap, negative strides, colour-key parity, "
    "FPU wrappers, x64, drivers, softrend, and release packaging are not claimed."
)
RESULT_ENDING = (
    "targeted CTest passed and the complete portable-core ladder passed 13/13."
)
BUILD = (
    "C:/dev/public/engine-revival-workspaces/"
    "brender-v132-portable-core-build-memory-compat-2026-07-09"
)
FINAL_TRANSCRIPT = (
    "C:/dev/public/engine-revival-workspaces/"
    "brender-v132-portable-core-ctest-memory-compat-2026-07-09.txt"
)
FAIL_FAST = "if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }"
TARGETED_BUILD = (
    f"cmake --build {BUILD} --config Debug --target "
    "brender_core_memory_compat_smoke"
)
TARGETED_CTEST = (
    f"ctest --test-dir {BUILD} -C Debug -R ^brender_core_memory_compat_smoke$ "
    "--no-tests=error --output-on-failure"
)
DEFAULT_BUILD = f"cmake --build {BUILD} --config Debug"
FULL_CAPTURE = (
    f"$fullOutput = & ctest --test-dir {BUILD} -C Debug --output-on-failure 2>&1"
)
TEE_FINAL = f"$fullOutput | Tee-Object -FilePath {FINAL_TRANSCRIPT}"
FULL_GUARD = (
    "$joined = $fullOutput -join [Environment]::NewLine; "
    "if ($joined -notmatch '100% tests passed, 0 tests failed out of 13') { "
    "throw 'Full CTest output does not prove 13/13' }"
)
GUARDED_PREFIX = (
    f"{TARGETED_BUILD}; {FAIL_FAST}; {TARGETED_CTEST}; {FAIL_FAST}; "
    f"{DEFAULT_BUILD}; {FAIL_FAST}; "
)
EXPECTED_COMMAND = (
    f"{GUARDED_PREFIX}{FULL_CAPTURE}; $fullExit = $LASTEXITCODE; {TEE_FINAL}; "
    f"if ($fullExit -ne 0) {{ exit $fullExit }}; {FULL_GUARD}"
)


def test_memory_compat_attempt_records_verified_full_ladder():
    assert ATTEMPT_PATH.exists(), "verified memory-compat attempt record is missing"
    attempt = json.loads(ATTEMPT_PATH.read_text(encoding="utf-8"))
    assert attempt["id"] == "brender-v132-portable-core-memory-compat-smoke"
    assert attempt["target_id"] == "brender"
    assert attempt["status"] == "completed"
    assert attempt["exit_code"] == 0
    assert attempt["transcript_location"] == TRANSCRIPT
    assert RED_TRANSCRIPT in attempt["public_notes"]
    assert POSITIVE_CLAUSE in attempt["public_notes"]
    assert attempt["public_notes"].endswith(NON_CLAIM)
    assert attempt["result_summary"].endswith(RESULT_ENDING)

    command = attempt["command"]
    assert "--no-tests=error" in command
    assert command.startswith(GUARDED_PREFIX)
    assert command.count(FAIL_FAST) == 3
    assert FULL_CAPTURE in command
    assert "$fullExit = $LASTEXITCODE" in command
    assert TEE_FINAL in command
    assert "if ($fullExit -ne 0) { exit $fullExit }" in command
    assert command.endswith(FULL_GUARD)
    assert command == EXPECTED_COMMAND
