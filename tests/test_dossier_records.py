from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]


def test_first_dossier_artifacts_are_recorded():
    expected = {
        "artifacts/brender-v132-source.json",
        "artifacts/brender-3dmm-source.json",
        "artifacts/renderware-ps2-gold-release-notes.json",
        "artifacts/renderware-sdk-310-ps2-archive-item.json",
    }
    missing = [path for path in expected if not (ROOT / path).exists()]
    assert missing == []
