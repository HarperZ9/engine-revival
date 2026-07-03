import json

from engine_revival.report import write_reports
from engine_revival.seed import seed_workspace


def test_report_includes_production_readiness_index_and_database(tmp_path):
    seed_workspace(tmp_path)
    readiness = tmp_path / "docs" / "generated" / "production-readiness.md"
    database = tmp_path / "docs" / "generated" / "database.json"

    reports = write_reports(tmp_path)

    assert readiness in reports
    text = readiness.read_text(encoding="utf-8")
    assert "# Production Readiness" in text
    assert "| Target | Stage | Build | Runtime | Tests | Package | Modernization | Score | Next Actions |" in text
    assert "| brender | baseline-assessment | not-started | not-started | not-started | not-started | not-started | 0 | create repeatable build harness |" in text
    payload = json.loads(database.read_text(encoding="utf-8"))
    assert payload["counts"]["readiness"] == 22
    assert payload["readiness_by_target"]["brender"][0]["id"] == "brender-production-readiness"


def test_target_dossier_includes_production_readiness(tmp_path):
    seed_workspace(tmp_path)
    dossier = tmp_path / "docs" / "generated" / "targets" / "brender.md"

    assert dossier in write_reports(tmp_path)

    text = dossier.read_text(encoding="utf-8")
    assert "## Production Readiness" in text
    assert "| brender-production-readiness | baseline-assessment | not-started | 0 |" in text
    assert "create repeatable build harness" in text
