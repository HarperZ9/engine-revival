import json
import re
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
CTEST_PASS_RE = re.compile(r"\bpassed\s+(\d+)/(\d+)\s+CTest\b", re.IGNORECASE)
SUPPORTED_PENDING_TERMS = ("pending", "open", "not-started", "planned", "seeded")
R4_ASSET_PIPELINE_EVIDENCE_MARKERS = (
    "asset audit",
    "model geometry audit",
    "pixelmap audit",
    "pixelmap/palette decode",
    "material audit",
    "material-file audit",
    "pixelmap round trip",
    "BrPixelmapSave",
    "material-to-face",
    "material resolution",
    "file-texture",
    "loaded period .pix",
)
R5_GAME_SHELL_EVIDENCE_MARKERS = ("r5", "game-shell", "game shell")
NUMBER_WORDS = {
    6: "six",
    12: "twelve",
}


PUBLIC_CLAIM_SURFACES = [
    ROOT / "docs" / "BRENDER-ARCHIVAL.md",
    ROOT / "docs" / "REMASTER-LANE.md",
    ROOT / "tasks" / "brender-asset-pipeline.json",
    ROOT / "docs" / "generated" / "packets.md",
    ROOT / "docs" / "generated" / "tasks.md",
    ROOT / "docs" / "generated" / "packets" / "brender-asset-pipeline.md",
    ROOT / "docs" / "generated" / "targets" / "brender.md",
    ROOT / "docs" / "generated" / "database.json",
]


def _load_json(path: Path) -> dict[str, object]:
    return json.loads(path.read_text(encoding="utf-8"))


def _brender_readiness() -> dict[str, object]:
    return _load_json(ROOT / "readiness" / "brender-production-readiness.json")


def _brender_attempts(readiness: dict[str, object]) -> list[dict[str, object]]:
    attempts = []
    for attempt_id in readiness["attempt_ids"]:
        path = ROOT / "attempts" / f"{attempt_id}.json"
        assert path.exists(), f"missing readiness attempt record: {attempt_id}"
        attempts.append(_load_json(path))
    return attempts


def _brender_tasks() -> list[dict[str, object]]:
    tasks = []
    for path in (ROOT / "tasks").glob("*.json"):
        task = _load_json(path)
        if task.get("target_id") == "brender":
            tasks.append(task)
    return tasks


def _verified_ctest_count(attempts: list[dict[str, object]]) -> int:
    passing_counts = []
    for attempt in attempts:
        if attempt.get("status") != "completed":
            continue
        evidence_text = "\n".join(
            str(attempt.get(field, ""))
            for field in ("id", "attempt_type", "public_notes", "result_summary")
        )
        for passed, total in CTEST_PASS_RE.findall(evidence_text):
            if passed == total:
                passing_counts.append(int(total))

    assert passing_counts, "no completed CTest attempt evidence found"
    return max(passing_counts)


def _mentions_count(text: str, count: int, phrase: str) -> bool:
    count_forms = {str(count), NUMBER_WORDS.get(count, "")}
    return any(f"{form} {phrase}" in text for form in count_forms if form)


def _implemented_brender_rung_count() -> int:
    text = (ROOT / "docs" / "BRENDER-ARCHIVAL.md").read_text(encoding="utf-8")
    return len(re.findall(r"^\d+\. ", text, flags=re.MULTILINE))


def _completed_attempts_with_markers(
    attempts: list[dict[str, object]],
    markers: tuple[str, ...],
) -> list[str]:
    matching_attempt_ids = []
    for attempt in attempts:
        if attempt.get("status") != "completed":
            continue
        evidence_text = "\n".join(
            str(attempt.get(field, ""))
            for field in ("id", "attempt_type", "public_notes", "result_summary")
        )
        normalized = evidence_text.lower()
        if any(marker.lower() in normalized for marker in markers):
            matching_attempt_ids.append(str(attempt["id"]))
    return matching_attempt_ids


def _status_advances_r4_without_pending_boundary(status: str) -> bool:
    normalized = status.lower()
    return "r4" in normalized and not any(
        term in normalized for term in SUPPORTED_PENDING_TERMS
    )


def _status_advances_r5(status: str) -> bool:
    normalized = status.lower()
    return (
        "r5" in normalized
        or "game-shell" in normalized
        or "game shell" in normalized
    )


def test_brender_verified_boundary_is_derived_from_readiness_attempt_records():
    readiness = _brender_readiness()
    attempts = _brender_attempts(readiness)

    assert _verified_ctest_count(attempts) == 12
    assert readiness["flagship_score"] == 86
    assert readiness["readiness_stage"] == "portable-core-plotter-lane-passing"
    assert readiness["test_status"] == "plotter-lane-passing"


def test_brender_asset_pipeline_claims_match_structured_evidence_boundary():
    readiness = _brender_readiness()
    attempts = _brender_attempts(readiness)
    verified_count = _verified_ctest_count(attempts)
    implemented_count = _implemented_brender_rung_count()
    pending_count = implemented_count - verified_count
    task = _load_json(ROOT / "tasks" / "brender-asset-pipeline.json")
    notes = str(task["public_notes"])
    outputs = [str(output) for output in task["outputs"]]

    assert task["status"] == "r4-evidence-pending"
    assert verified_count == 12
    assert pending_count == 6
    assert _mentions_count(notes, verified_count, "verified CTest rungs")
    assert _mentions_count(notes, pending_count, "implemented asset-pipeline rungs")
    assert any(str(verified_count) in output for output in outputs)
    assert any(
        _mentions_count(output, pending_count, "implemented asset-pipeline rungs")
        and "pending" in output
        for output in outputs
    )
    assert "R4 is not closed" in notes
    assert "structured attempt records and command transcripts" in notes


def test_brender_archival_ladder_distinguishes_verified_and_pending_rungs():
    verified_count = _verified_ctest_count(_brender_attempts(_brender_readiness()))
    text = (ROOT / "docs" / "BRENDER-ARCHIVAL.md").read_text(encoding="utf-8")
    implemented_count = _implemented_brender_rung_count()
    pending_count = implemented_count - verified_count

    assert implemented_count == 18
    assert pending_count == 6
    assert _mentions_count(text, verified_count, "verified CTest rungs")
    assert _mentions_count(text, pending_count, "implemented asset-pipeline rungs")
    assert "R4 is not closed" in text


def test_brender_status_fields_do_not_advance_r4_or_r5_without_supporting_attempts():
    attempts = _brender_attempts(_brender_readiness())
    r4_supporting_attempt_ids = _completed_attempts_with_markers(
        attempts,
        R4_ASSET_PIPELINE_EVIDENCE_MARKERS,
    )
    r5_supporting_attempt_ids = _completed_attempts_with_markers(
        attempts,
        R5_GAME_SHELL_EVIDENCE_MARKERS,
    )

    assert len(r4_supporting_attempt_ids) < 6
    assert r5_supporting_attempt_ids == []

    for task in _brender_tasks():
        status = str(task["status"])
        if _status_advances_r4_without_pending_boundary(status):
            assert len(r4_supporting_attempt_ids) >= 6, (
                f"{task['id']} advances R4 via status {status!r} without six "
                f"completed R4 asset-pipeline attempt records"
            )
        if _status_advances_r5(status):
            assert r5_supporting_attempt_ids, (
                f"{task['id']} advances R5 via status {status!r} without a "
                "completed R5 game-shell attempt record"
            )


def test_brender_public_surfaces_do_not_reintroduce_closed_r4_overclaim():
    forbidden_fragments = [
        "R4 closed",
        "This closes R4",
        "eighteen CTest targets",
        "eighteen-target ladder",
        "sixteen-target ladder",
        "audit rungs with JSON receipts",
        "R5 opened",
        "nineteen CTest targets",
    ]

    for path in PUBLIC_CLAIM_SURFACES:
        text = path.read_text(encoding="utf-8")
        for fragment in forbidden_fragments:
            assert fragment not in text, f"{path} contains overclaim: {fragment}"
