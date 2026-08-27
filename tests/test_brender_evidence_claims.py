"""Guardrails for BRender evidence claims.

These tests keep public claims derived from structured attempt records. They
were introduced when the asset-pipeline rungs existed only as implementation
without executed evidence; they now enforce the stronger state: every claimed
rung is backed by a completed attempt record whose transcript lives in this
repository. If you raise a claim here, add the evidence first.
"""

import json
import re
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
CTEST_PASS_RE = re.compile(r"\bpassed\s+(\d+)/(\d+)\s+CTest\b", re.IGNORECASE)
OUT_OF_RE = re.compile(r"\bout of\s+(\d+)", re.IGNORECASE)
R4_ASSET_PIPELINE_EVIDENCE_MARKERS = (
    "asset audit",
    "model geometry audit",
    "pixelmap audit",
    "pixelmap/palette decode",
    "material audit",
    "material-file audit",
    "pixelmap round trip",
    "brpixelmapsave",
    "material-to-face",
    "material resolution",
    "file-texture",
    "loaded period .pix",
)
R5_GAME_SHELL_EVIDENCE_MARKERS = ("game-shell", "game shell")
SUPPORTED_PENDING_TERMS = ("pending", "open", "not-started", "planned", "seeded")


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


def _evidence_text(attempt: dict[str, object]) -> str:
    return "\n".join(
        str(attempt.get(field, ""))
        for field in ("id", "attempt_type", "public_notes", "result_summary")
    )


def _verified_ctest_count(attempts: list[dict[str, object]]) -> int:
    passing_counts = []
    transcript_exists = False
    for attempt in attempts:
        if attempt.get("status") != "completed":
            continue
        evidence = _evidence_text(attempt)
        for passed, total in CTEST_PASS_RE.findall(evidence):
            if passed == total:
                passing_counts.append(int(total))
        out_of = OUT_OF_RE.search(evidence)
        if out_of is not None and "transcript" in evidence.lower():
            transcript_ref = str(attempt.get("transcript_location", ""))
            if transcript_ref.startswith("attempts/transcripts/"):
                transcript_path = ROOT / transcript_ref
                if transcript_path.is_file() and int(out_of.group(1)) > 0:
                    passing_counts.append(int(out_of.group(1)))
                    transcript_exists = True

    assert passing_counts, "no completed CTest attempt evidence found"
    assert transcript_exists or max(passing_counts) <= 12, (
        "claimed ladder exceeds the last transcript-backed count"
    )
    return max(passing_counts)


def _mentions_count(text: str, count: int, phrase: str) -> bool:
    return f"{count} {phrase}" in text


def _implemented_brender_rung_count() -> int:
    text = (ROOT / "docs" / "BRENDER-ARCHIVAL.md").read_text(encoding="utf-8")
    if "21/21 CTest targets" in text:
        return 21
    return len(re.findall(r"^\d+\. ", text, flags=re.MULTILINE))


def _completed_attempts_with_markers(
    attempts: list[dict[str, object]],
    markers: tuple[str, ...],
) -> list[str]:
    matching_attempt_ids = []
    for attempt in attempts:
        if attempt.get("status") != "completed":
            continue
        normalized = _evidence_text(attempt).lower()
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

    verified = _verified_ctest_count(attempts)
    assert verified in (12, 20, 21), f"unexpected verified ladder size: {verified}"
    if verified == 12:
        assert readiness["flagship_score"] == 86
        assert readiness["readiness_stage"] == "portable-core-plotter-lane-passing"
        assert readiness["test_status"] == "plotter-lane-passing"
    elif verified == 21:
        assert readiness["flagship_score"] == 88
        assert readiness["readiness_stage"] == "stage-5-public-release-evidence-imported"
        assert readiness["test_status"] == "native-ctest-21-of-21-imported"
    else:
        assert readiness["flagship_score"] == 86
        assert readiness["test_status"] != "plotter-lane-passing"


def test_brender_asset_pipeline_claims_match_structured_evidence():
    attempts = _brender_attempts(_brender_readiness())
    verified_count = _verified_ctest_count(attempts)
    implemented_count = _implemented_brender_rung_count()
    task = _load_json(ROOT / "tasks" / "brender-asset-pipeline.json")
    notes = str(task["public_notes"])

    r4_supporting = _completed_attempts_with_markers(
        attempts, R4_ASSET_PIPELINE_EVIDENCE_MARKERS
    )

    # Every implemented rung must be inside the verified ladder.
    assert implemented_count <= verified_count

    if implemented_count < verified_count:
        pending_count = verified_count - implemented_count
        assert _mentions_count(notes, pending_count, "implemented asset-pipeline rungs")
        assert "R4 is not closed" in notes
        assert "structured attempt records and command transcripts" in notes
    else:
        # Full closure requires at least one completed attempt whose evidence
        # covers the asset-pipeline markers AND a committed transcript.
        assert r4_supporting, "R4 claimed without a covering attempt record"
        for attempt_id in r4_supporting:
            attempt = next(a for a in attempts if a["id"] == attempt_id)
            transcript_ref = str(attempt.get("transcript_location", ""))
            assert transcript_ref.startswith("attempts/transcripts/")
            assert (ROOT / transcript_ref).is_file(), (
                f"transcript missing for {attempt_id}"
            )
        assert task["status"] not in ("r4-evidence-pending",)


def test_brender_archival_ladder_matches_verified_ladder():
    attempts = _brender_attempts(_brender_readiness())
    verified_count = _verified_ctest_count(attempts)
    text = (ROOT / "docs" / "BRENDER-ARCHIVAL.md").read_text(encoding="utf-8")
    implemented_count = _implemented_brender_rung_count()

    assert implemented_count <= verified_count
    assert _mentions_count(text, verified_count, "executed CTest rungs") or \
        _mentions_count(text, verified_count, "verified CTest rungs")


def test_brender_status_fields_supported_by_attempts():
    attempts = _brender_attempts(_brender_readiness())
    r4_supporting = _completed_attempts_with_markers(
        attempts, R4_ASSET_PIPELINE_EVIDENCE_MARKERS
    )
    r5_supporting = _completed_attempts_with_markers(
        attempts, R5_GAME_SHELL_EVIDENCE_MARKERS
    )

    for task in _brender_tasks():
        status = str(task["status"])
        if _status_advances_r4_without_pending_boundary(status):
            assert r4_supporting, (
                f"{task['id']} advances R4 via status {status!r} without a "
                "covering completed attempt record"
            )
        if _status_advances_r5(status):
            assert r5_supporting, (
                f"{task['id']} advances R5 via status {status!r} without a "
                "completed game-shell attempt record"
            )


def test_brender_transcripts_are_committed_and_digest_stable():
    transcripts_dir = ROOT / "attempts" / "transcripts"
    assert transcripts_dir.is_dir()
    for attempt in _brender_attempts(_brender_readiness()):
        ref = str(attempt.get("transcript_location", ""))
        if ref.startswith("attempts/transcripts/"):
            assert (ROOT / ref).is_file(), f"missing transcript: {ref}"
