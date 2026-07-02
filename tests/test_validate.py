from pathlib import Path

from engine_revival.validate import validate_workspace

ROOT = Path(__file__).resolve().parents[1]


def test_valid_fixture_has_no_validation_errors():
    assert validate_workspace(ROOT / "tests" / "fixtures" / "valid-mini") == []


def test_repo_without_records_reports_missing_directories(tmp_path):
    assert "missing record directory: targets" in validate_workspace(tmp_path)


def test_artifact_source_ids_must_reference_sources(tmp_path):
    (tmp_path / "targets").mkdir()
    (tmp_path / "artifacts").mkdir()
    (tmp_path / "sources").mkdir()
    (tmp_path / "targets" / "brender.json").write_text(
        """{
  "id": "brender",
  "name": "Argonaut BRender",
  "category": "engine",
  "era": "1990s",
  "platforms": [],
  "priority": 89,
  "revival_lane": "critical-edition",
  "rights_posture": "open",
  "summary": "BRender target.",
  "public_status": "seeded",
  "restricted_status": "none-known"
}""",
        encoding="utf-8",
    )
    (tmp_path / "artifacts" / "bad-source.json").write_text(
        """{
  "id": "bad-source",
  "target_id": "brender",
  "artifact_type": "source",
  "title": "Bad source link",
  "origin": "fixture",
  "redistribution_status": "open",
  "access_level": "public",
  "evidence_quality": "fixture",
  "source_ids": ["missing-source"]
}""",
        encoding="utf-8",
    )
    messages = validate_workspace(tmp_path)
    assert any("unknown source_id: missing-source" in message for message in messages)
