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
