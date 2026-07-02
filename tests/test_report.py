from engine_revival.report import write_reports
from engine_revival.seed import seed_workspace


def test_write_reports_creates_public_index(tmp_path):
    seed_workspace(tmp_path)
    index = tmp_path / "docs" / "generated" / "index.md"
    assert index in write_reports(tmp_path)
    assert "Argonaut BRender" in index.read_text(encoding="utf-8")


def test_write_reports_creates_artifact_summary(tmp_path):
    seed_workspace(tmp_path)
    (tmp_path / "artifacts").mkdir()
    (tmp_path / "artifacts" / "brender-source.json").write_text(
        """{
  "id": "brender-source",
  "target_id": "brender",
  "artifact_type": "source-release",
  "title": "BRender source release",
  "origin": "public repository",
  "redistribution_status": "open",
  "access_level": "public",
  "evidence_quality": "public-source"
}""",
        encoding="utf-8",
    )
    artifacts = tmp_path / "docs" / "generated" / "artifacts.md"
    assert artifacts in write_reports(tmp_path)
    assert "BRender source release" in artifacts.read_text(encoding="utf-8")


def test_write_reports_creates_accession_summary(tmp_path):
    seed_workspace(tmp_path)
    (tmp_path / "artifacts").mkdir()
    (tmp_path / "accessions").mkdir()
    (tmp_path / "artifacts" / "brender-source.json").write_text(
        """{
  "id": "brender-source",
  "target_id": "brender",
  "artifact_type": "source-release",
  "title": "BRender source",
  "origin": "public",
  "redistribution_status": "open",
  "access_level": "public",
  "evidence_quality": "public-source"
}""",
        encoding="utf-8",
    )
    (tmp_path / "accessions" / "brender-source-planned.json").write_text(
        """{
  "id": "brender-source-planned",
  "artifact_id": "brender-source",
  "package_type": "source-snapshot",
  "capture_status": "planned",
  "storage_class": "external-url",
  "fixity_status": "not-started",
  "rights_review": "open-license",
  "public_notes": "Planned public source accession."
}""",
        encoding="utf-8",
    )
    accessions = tmp_path / "docs" / "generated" / "accessions.md"
    assert accessions in write_reports(tmp_path)
    assert "brender-source" in accessions.read_text(encoding="utf-8")


def test_write_reports_creates_task_board(tmp_path):
    seed_workspace(tmp_path)
    tasks = tmp_path / "docs" / "generated" / "tasks.md"
    assert tasks in write_reports(tmp_path)
    task_board = tasks.read_text(encoding="utf-8")
    assert "brender-triage" in task_board
    assert "triage-public-record" in task_board


def test_write_reports_creates_coverage_summary(tmp_path):
    seed_workspace(tmp_path)
    (tmp_path / "artifacts").mkdir()
    (tmp_path / "accessions").mkdir()
    (tmp_path / "artifacts" / "brender-source.json").write_text(
        """{
  "id": "brender-source",
  "target_id": "brender",
  "artifact_type": "source-release",
  "title": "BRender source",
  "origin": "public",
  "redistribution_status": "open",
  "access_level": "public",
  "evidence_quality": "public-source"
}""",
        encoding="utf-8",
    )
    (tmp_path / "accessions" / "brender-source-planned.json").write_text(
        """{
  "id": "brender-source-planned",
  "artifact_id": "brender-source",
  "package_type": "source-snapshot",
  "capture_status": "planned",
  "storage_class": "external-url",
  "fixity_status": "not-started",
  "rights_review": "open-license",
  "public_notes": "Planned public source accession."
}""",
        encoding="utf-8",
    )
    coverage = tmp_path / "docs" / "generated" / "coverage.md"
    assert coverage in write_reports(tmp_path)
    text = coverage.read_text(encoding="utf-8")
    assert "| Artifact accession coverage | 1 | 1 |" in text
    assert "No missing artifact accessions." in text
