import json

import pytest

from engine_revival.validate import validate_workspace


CHECKPOINT = {
    "id": "brender-v132-portable-core-plotter-2026-07-03",
    "stage": "portable-core-plotter-lane-passing",
    "passed": 12,
    "total": 12,
    "source_snapshot": "d88d0ed41122664b9781015b517db64353e16f19",
}
BOUNDS_INVALID_CHECKPOINT = {**CHECKPOINT, "passed": 13}


def _write_json(path, payload):
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps(payload, indent=2) + "\n", encoding="utf-8")


def _with_checkpoint(payload, checkpoint):
    if checkpoint is not None:
        payload["evidence_checkpoint"] = checkpoint
    return payload


def _write_workspace(root, first_checkpoint, second_checkpoint):
    _write_json(root / "targets" / "brender.json", {
        "id": "brender",
        "name": "Argonaut BRender",
        "category": "engine",
        "era": "1990s",
        "platforms": ["Windows"],
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
    _write_json(root / "tasks" / "brender-first.json", _with_checkpoint({
        "id": "brender-first",
        "target_id": "brender",
        "task_type": "verification",
        "status": "verified",
        "public_notes": "First checkpoint carrier.",
    }, first_checkpoint))
    _write_json(root / "tasks" / "brender-second.json", _with_checkpoint({
        "id": "brender-second",
        "target_id": "brender",
        "task_type": "verification",
        "status": "verified",
        "public_notes": "Second checkpoint carrier.",
    }, second_checkpoint))
    _write_json(root / "milestones" / "brender-baseline.json", {
        "id": "brender-baseline",
        "target_id": "brender",
        "milestone_type": "baseline-public-record",
        "status": "curated",
        "evidence": "Initial public record fixture.",
        "source_ids": ["brender-source"],
    })


def _write_checkpoint_carrier(root, kind, checkpoint):
    carriers = {
        "reproduction": ("reproductions/brender-reproduction.json", {
            "id": "brender-reproduction",
            "target_id": "brender",
            "reproduction_type": "source-build",
            "status": "verified",
            "environment": [],
            "steps": [],
            "expected_outputs": [],
            "public_notes": "Reproduction checkpoint carrier.",
            "source_ids": ["brender-source"],
        }),
        "build": ("builds/brender-build.json", {
            "id": "brender-build",
            "target_id": "brender",
            "reproduction_id": "brender-reproduction",
            "status": "verified",
            "host_platform": "Windows",
            "source_checkout": "public source",
            "snapshot_ids": [],
            "toolchain_probe": {},
            "build_system": "CMake",
            "required_variables": [],
            "observed_layout": {},
            "blockers": [],
            "next_actions": [],
            "public_notes": "Build checkpoint carrier.",
            "source_ids": ["brender-source"],
        }),
        "harness": ("harnesses/brender-harness.json", {
            "id": "brender-harness",
            "target_id": "brender",
            "build_id": "brender-build",
            "reproduction_id": "brender-reproduction",
            "status": "verified",
            "harness_type": "smoke-test",
            "entrypoint": "run-smoke",
            "source_policy": "public-only",
            "implementation_units": [],
            "steps": [],
            "expected_outputs": [],
            "blockers": [],
            "next_actions": [],
            "public_notes": "Harness checkpoint carrier.",
            "source_ids": ["brender-source"],
        }),
        "readiness": ("readiness/brender-readiness.json", {
            "id": "brender-readiness",
            "target_id": "brender",
            "readiness_stage": "verified",
            "build_status": "verified",
            "runtime_status": "verified",
            "test_status": "verified",
            "packaging_status": "pending",
            "modernization_status": "pending",
            "flagship_score": 1,
            "blockers": [],
            "next_actions": [],
            "public_notes": "Readiness checkpoint carrier.",
            "source_ids": ["brender-source"],
        }),
    }
    relative, payload = carriers[kind]
    _write_json(root / relative, _with_checkpoint(payload, checkpoint))


def _checkpoint_messages(root):
    return [message for message in validate_workspace(root) if "evidence_checkpoint" in message]


def test_matching_evidence_checkpoints_validate(tmp_path):
    _write_workspace(tmp_path, CHECKPOINT, CHECKPOINT.copy())
    assert _checkpoint_messages(tmp_path) == []


def test_evidence_checkpoint_omission_remains_valid(tmp_path):
    _write_workspace(tmp_path, None, None)
    assert validate_workspace(tmp_path) == []


def test_matching_evidence_checkpoints_validate_across_all_carrier_kinds(tmp_path):
    _write_workspace(tmp_path, CHECKPOINT, CHECKPOINT.copy())
    for kind in ("reproduction", "build", "harness", "readiness"):
        _write_checkpoint_carrier(tmp_path, kind, CHECKPOINT.copy())
    assert validate_workspace(tmp_path) == []


def test_mismatched_evidence_checkpoint_field_fails(tmp_path):
    mismatched = {**CHECKPOINT, "passed": 11, "total": 12}
    _write_workspace(tmp_path, CHECKPOINT, mismatched)
    messages = _checkpoint_messages(tmp_path)
    assert any("field passed differs" in message for message in messages)
    assert any(CHECKPOINT["id"] in message for message in messages)


def test_cross_kind_valid_checkpoint_mismatch_fails(tmp_path):
    _write_workspace(tmp_path, CHECKPOINT, CHECKPOINT.copy())
    _write_checkpoint_carrier(
        tmp_path,
        "readiness",
        {**CHECKPOINT, "passed": 11, "total": 12},
    )
    messages = _checkpoint_messages(tmp_path)
    assert any(
        "brender-readiness.json" in message and "field passed differs" in message
        for message in messages
    )


@pytest.mark.parametrize(
    ("field", "value"),
    [
        pytest.param("id", "", id="empty-id"),
        pytest.param("id", 12, id="non-string-id"),
        pytest.param("stage", "", id="empty-stage"),
        pytest.param("stage", 12, id="non-string-stage"),
        pytest.param("source_snapshot", "", id="empty-source-snapshot"),
        pytest.param("source_snapshot", 12, id="non-string-source-snapshot"),
    ],
)
def test_evidence_checkpoint_rejects_invalid_string_scalars(tmp_path, field, value):
    invalid = {**CHECKPOINT, field: value}
    _write_workspace(tmp_path, CHECKPOINT, invalid)
    messages = _checkpoint_messages(tmp_path)
    assert any(f"{field} must be non-empty string" in message for message in messages)
    assert all("differs from" not in message for message in messages)


@pytest.mark.parametrize(
    ("field", "value"),
    [
        pytest.param("passed", True, id="boolean-passed"),
        pytest.param("passed", 12.5, id="non-integer-passed"),
        pytest.param("total", False, id="boolean-total"),
        pytest.param("total", 12.5, id="non-integer-total"),
    ],
)
def test_evidence_checkpoint_rejects_invalid_integer_scalars(tmp_path, field, value):
    invalid = {**CHECKPOINT, field: value}
    _write_workspace(tmp_path, CHECKPOINT, invalid)
    messages = _checkpoint_messages(tmp_path)
    assert any(f"{field} must be integer" in message for message in messages)
    assert all("differs from" not in message for message in messages)


@pytest.mark.parametrize(
    ("changes", "expected"),
    [
        pytest.param(
            {"passed": 0, "total": 0},
            ("total must be > 0",),
            id="non-positive-total",
        ),
        pytest.param(
            {"passed": -1},
            ("passed must be >= 0",),
            id="negative-passed",
        ),
        pytest.param(
            {"passed": 13},
            ("passed must be <= total",),
            id="passed-exceeds-total",
        ),
        pytest.param(
            {"passed": 0, "total": -1},
            ("total must be > 0", "passed must be <= total"),
            id="all-applicable-bounds-diagnostics",
        ),
    ],
)
def test_invalid_evidence_checkpoint_bounds_fail_without_mismatch(
    tmp_path,
    changes,
    expected,
):
    invalid = {**CHECKPOINT, **changes}
    _write_workspace(tmp_path, CHECKPOINT, invalid)
    messages = _checkpoint_messages(tmp_path)
    assert all(any(text in message for message in messages) for text in expected)
    assert all("differs from" not in message for message in messages)


@pytest.mark.parametrize(
    ("first_checkpoint", "second_checkpoint"),
    [
        pytest.param(CHECKPOINT, BOUNDS_INVALID_CHECKPOINT, id="invalid-second"),
        pytest.param(BOUNDS_INVALID_CHECKPOINT, CHECKPOINT, id="invalid-first"),
    ],
)
def test_bounds_invalid_checkpoint_does_not_emit_agreement_mismatch(
    tmp_path,
    first_checkpoint,
    second_checkpoint,
):
    _write_workspace(tmp_path, first_checkpoint, second_checkpoint)
    messages = _checkpoint_messages(tmp_path)
    assert any("passed must be <= total" in message for message in messages)
    assert all("differs from" not in message for message in messages)


def test_evidence_checkpoint_rejects_missing_and_unexpected_fields(tmp_path):
    malformed = {key: value for key, value in CHECKPOINT.items() if key != "stage"}
    malformed["note"] = "not part of the contract"
    _write_workspace(tmp_path, CHECKPOINT, malformed)
    messages = _checkpoint_messages(tmp_path)
    assert any("missing field: stage" in message for message in messages)
    assert any("unexpected field: note" in message for message in messages)
    assert all("differs from" not in message for message in messages)
