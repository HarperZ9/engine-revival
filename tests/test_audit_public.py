from pathlib import Path

from engine_revival.audit import audit_public_workspace

ROOT = Path(__file__).resolve().parents[1]


def test_valid_fixture_passes_public_audit():
    assert audit_public_workspace(ROOT / "tests" / "fixtures" / "valid-mini") == []


def test_restricted_publishable_artifact_fails_public_audit():
    messages = audit_public_workspace(ROOT / "tests" / "fixtures" / "unsafe-records")
    assert any("restricted material cannot be publishable" in message for message in messages)
