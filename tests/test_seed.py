import json

from engine_revival.seed import INITIAL_TARGETS, seed_workspace


def test_initial_targets_include_required_research_targets():
    ids = {str(target["id"]) for target in INITIAL_TARGETS}
    assert {"brender", "renderware-ps2", "ps1-programmer-tool", "opengl-performer"} <= ids
    assert len(ids) == 22


def test_seed_workspace_writes_target_records(tmp_path):
    written = seed_workspace(tmp_path)
    target_path = tmp_path / "targets" / "brender.json"
    payload = json.loads(target_path.read_text(encoding="utf-8"))
    assert target_path in written
    assert payload["rights_posture"] == "open"
