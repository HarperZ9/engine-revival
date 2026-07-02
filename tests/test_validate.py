from pathlib import Path

from engine_revival.validate import validate_workspace

ROOT = Path(__file__).resolve().parents[1]


def test_valid_fixture_has_no_validation_errors():
    assert validate_workspace(ROOT / "tests" / "fixtures" / "valid-mini") == []


def test_repo_without_records_reports_missing_directories(tmp_path):
    assert "missing record directory: targets" in validate_workspace(tmp_path)
