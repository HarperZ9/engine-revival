from engine_revival.report import write_reports
from engine_revival.seed import seed_workspace


def test_write_reports_creates_public_index(tmp_path):
    seed_workspace(tmp_path)
    index = tmp_path / "docs" / "generated" / "index.md"
    assert index in write_reports(tmp_path)
    assert "Argonaut BRender" in index.read_text(encoding="utf-8")
