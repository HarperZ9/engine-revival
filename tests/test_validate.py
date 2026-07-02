import json
from pathlib import Path

from engine_revival.records import load_records
from engine_revival.validate import validate_workspace

ROOT = Path(__file__).resolve().parents[1]


def _write_json(path, payload):
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps(payload, indent=2) + "\n", encoding="utf-8")


def _write_accession_workspace(root):
    _write_json(root / "targets" / "brender.json", {
        "id": "brender",
        "name": "Argonaut BRender",
        "category": "engine",
        "era": "1990s",
        "platforms": [],
        "priority": 89,
        "revival_lane": "critical-edition",
        "rights_posture": "open",
        "summary": "BRender target.",
        "public_status": "curated-public-sources",
        "restricted_status": "none-known",
    })
    _write_json(root / "sources" / "brender-source.json", {
        "id": "brender-source",
        "title": "BRender source",
        "source_type": "source-repository",
        "confidence": "high",
        "claim_scope": "Public BRender source release.",
    })
    _write_json(root / "artifacts" / "brender-v132-source.json", {
        "id": "brender-v132-source",
        "target_id": "brender",
        "artifact_type": "source-release",
        "title": "BRender source",
        "origin": "public repository",
        "redistribution_status": "open",
        "access_level": "public",
        "evidence_quality": "public-source",
        "source_ids": ["brender-source"],
    })
    _write_json(root / "accessions" / "brender-v132-source-planned.json", {
        "id": "brender-v132-source-planned",
        "artifact_id": "brender-v132-source",
        "package_type": "source-snapshot",
        "capture_status": "planned",
        "storage_class": "external-url",
        "fixity_status": "not-started",
        "rights_review": "open-license",
        "public_notes": "Planned accession.",
        "source_ids": ["brender-source"],
    })


def test_valid_fixture_has_no_validation_errors():
    assert validate_workspace(ROOT / "tests" / "fixtures" / "valid-mini") == []


def test_valid_accession_fixture_passes_validation(tmp_path):
    _write_accession_workspace(tmp_path)
    assert validate_workspace(tmp_path) == []
    loaded = load_records(tmp_path, "accession")
    assert loaded[0].payload["artifact_id"] == "brender-v132-source"


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


def test_accession_artifact_id_must_reference_artifact(tmp_path):
    _write_accession_workspace(tmp_path)
    (tmp_path / "accessions" / "brender-v132-source-planned.json").unlink()
    _write_json(tmp_path / "accessions" / "bad.json", {
        "id": "bad",
        "artifact_id": "missing-artifact",
        "package_type": "metadata-only",
        "capture_status": "metadata-only",
        "storage_class": "not-held",
        "fixity_status": "not-applicable",
        "rights_review": "metadata-only",
        "public_notes": "Metadata only.",
    })
    messages = validate_workspace(tmp_path)
    assert any("unknown artifact_id: missing-artifact" in message for message in messages)


def test_accession_source_ids_must_reference_sources(tmp_path):
    _write_accession_workspace(tmp_path)
    _write_json(tmp_path / "accessions" / "bad-source.json", {
        "id": "bad-source",
        "artifact_id": "brender-v132-source",
        "package_type": "metadata-only",
        "capture_status": "metadata-only",
        "storage_class": "not-held",
        "fixity_status": "not-applicable",
        "rights_review": "metadata-only",
        "public_notes": "Metadata only.",
        "source_ids": ["missing-source"],
    })
    messages = validate_workspace(tmp_path)
    assert any("unknown source_id: missing-source" in message for message in messages)


def test_artifact_must_have_accession_record(tmp_path):
    _write_accession_workspace(tmp_path)
    (tmp_path / "accessions" / "brender-v132-source-planned.json").unlink()
    messages = validate_workspace(tmp_path)
    assert any("missing accession for artifact_id: brender-v132-source" in message for message in messages)


def test_duplicate_record_ids_fail_validation(tmp_path):
    _write_accession_workspace(tmp_path)
    _write_json(tmp_path / "sources" / "duplicate.json", {
        "id": "brender-source",
        "title": "Duplicate BRender source",
        "source_type": "source-repository",
        "confidence": "low",
        "claim_scope": "Duplicate fixture.",
    })
    messages = validate_workspace(tmp_path)
    assert any("duplicate source id: brender-source" in message for message in messages)
