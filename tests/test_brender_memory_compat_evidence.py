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
