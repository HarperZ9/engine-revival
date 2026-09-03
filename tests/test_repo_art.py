"""The drawings in the README, held against the corpus and the code.

The art gate settles whether a drawing fits its columns and matches the spec it
was rendered from. Both sides of that check read the same JSON, so it cannot
settle whether a drawing is TRUE. That is what this file is for: every count the
three drawings put on the page is asserted here against the records or the
module that produces it, so a claim that stops holding fails the suite instead of
staying on the page.

One number is self-referential. The card says how many test functions the suite
carries, and that total includes the functions below, so the assertion reads the
same directory the claim describes.
"""

from __future__ import annotations

import ast
import json
import shutil
import sys
from collections import Counter
from pathlib import Path

import pytest

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))

import check_repo_art as GATE  # noqa: E402

from engine_revival.records import RECORD_DIRS, load_records  # noqa: E402

SPEC = json.loads(
    (ROOT / "docs" / "art" / "engine-revival.art.json").read_text(encoding="utf-8")
)
CARD = {field["key"]: field for field in SPEC["cards"][0]["fields"]}
RUNGS = next(flow for flow in SPEC["flows"] if flow["file"] == "rung-lane.svg")
TRIAGE = next(flow for flow in SPEC["flows"] if flow["file"] == "directory-lane.svg")

REFERENCE_FIELDS = (
    "target_id",
    "artifact_id",
    "reproduction_id",
    "source_ids",
    "artifact_ids",
    "task_ids",
    "reproduction_ids",
    "snapshot_ids",
    "build_ids",
    "harness_ids",
    "attempt_ids",
    "blocked_by",
)


@pytest.fixture(scope="module")
def records() -> dict[str, list]:
    return {kind: load_records(ROOT, kind) for kind in RECORD_DIRS}


def _corpus_copy(destination: Path) -> Path:
    for directory in RECORD_DIRS.values():
        shutil.copytree(ROOT / directory, destination / directory)
    return destination


def _count_references(records: dict[str, list]) -> int:
    total = 0
    for found in records.values():
        for record in found:
            for field in REFERENCE_FIELDS:
                value = record.payload.get(field)
                if isinstance(value, str):
                    total += 1
                elif isinstance(value, list):
                    total += len(value)
    return total


def test_the_art_gate_passes_every_check():
    """The gate runs under pytest too, so the front page is covered by CI."""
    result = GATE.receipt()
    assert [check for check in result["checks"] if not check["passed"]] == []
    assert result["passed"] is True


def test_the_corpus_holds_twelve_record_kinds():
    assert CARD["record kinds"]["value"] == "twelve of them"
    assert len(RECORD_DIRS) == 12
    named = CARD["record kinds"]["note"].split(":", 1)[1].rstrip(".")
    assert [word.strip() for word in named.split(",")] == list(RECORD_DIRS)


def test_three_hundred_and_eighty_two_records_sit_on_disk(records):
    assert CARD["records on disk"]["value"] == "382 files"
    counts = {kind: len(found) for kind, found in records.items()}
    assert sum(counts.values()) == 382
    assert counts["source"] == 85
    assert counts["artifact"] == 66
    assert counts["accession"] == 66
    assert counts["task"] == 38
    assert max(counts, key=counts.get) == "source"


def test_twenty_nine_engines_across_nineteen_categories(records):
    assert CARD["engines tracked"]["value"] == "29 targets"
    targets = records["target"]
    assert len(targets) == 29
    assert len({target.payload["category"] for target in targets}) == 19
    status = Counter(target.payload["public_status"] for target in targets)
    assert status["curated-public-metadata"] == 15
    assert status["curated-public-sources"] == 13


def test_every_source_carries_its_own_confidence(records):
    assert CARD["sources cited"]["value"] == "85 of them"
    confidence = Counter(source.payload["confidence"] for source in records["source"])
    assert confidence == {"high": 68, "moderate": 16, "low": 1}
    assert sum(confidence.values()) == 85


def test_seven_hundred_and_eighty_five_references_all_resolve(records):
    from engine_revival.validate import validate_workspace

    assert CARD["links that resolve"]["value"] == "785 edges"
    assert _count_references(records) == 785
    assert validate_workspace(ROOT) == []


def test_the_validator_reports_a_reference_that_goes_nowhere(tmp_path):
    """Without this, "links that resolve" would be a count of edges, not a claim."""
    from engine_revival.validate import validate_workspace

    root = _corpus_copy(tmp_path)
    path = sorted((root / "artifacts").glob("*.json"))[0]
    payload = json.loads(path.read_text(encoding="utf-8"))
    payload["target_id"] = "an-engine-nobody-recorded"
    path.write_text(json.dumps(payload), encoding="utf-8")
    messages = validate_workspace(root)
    assert [m for m in messages if "unknown target_id" in m]


def test_the_validator_reports_an_id_that_is_not_its_filename(tmp_path):
    from engine_revival.validate import validate_workspace

    root = _corpus_copy(tmp_path)
    path = sorted((root / "targets").glob("*.json"))[0]
    payload = json.loads(path.read_text(encoding="utf-8"))
    payload["id"] = payload["id"] + "-renamed"
    path.write_text(json.dumps(payload), encoding="utf-8")
    messages = validate_workspace(root)
    assert [m for m in messages if "must match filename stem" in m]
    stages = {stage["title"]: stage["note"] for stage in TRIAGE["stages"]}
    assert stages["Record"] == "One file, one id, one target."


def test_five_restricted_holds_and_none_of_them_publishable(records):
    from engine_revival.audit import PUBLISHABLE_LEVELS, audit_public_workspace

    assert CARD["restricted holds"]["value"] == "five artifacts"
    held = [
        artifact
        for artifact in records["artifact"]
        if artifact.payload["redistribution_status"] == "do-not-redistribute"
    ]
    assert len(held) == 5
    assert not [a for a in held if a.payload["access_level"] in PUBLISHABLE_LEVELS]
    assert audit_public_workspace(ROOT) == []


def test_twelve_schemas_name_one_hundred_and_fifteen_required_fields():
    from engine_revival.schema import load_schema

    assert CARD["schemas"]["value"] == "twelve files"
    assert CARD["required fields"]["value"] == "115 named"
    files = sorted((ROOT / "schemas").glob("*.schema.json"))
    assert len(files) == 12
    required = sum(len(load_schema(ROOT, kind).required) for kind in RECORD_DIRS)
    assert required == 115


def test_the_report_writes_two_hundred_and_thirty_five_files(tmp_path):
    """Written into a copy, so the assertion never rewrites the real tree."""
    from engine_revival.report import write_reports

    written = list(write_reports(_corpus_copy(tmp_path)))
    assert CARD["generated pages"]["value"] == "235 written"
    assert len(written) == 235
    assert len([path for path in written if path.suffix == ".md"]) == 234
    assert len([path for path in written if path.suffix == ".json"]) == 1


def test_the_report_leaves_the_committed_pages_byte_identical(tmp_path):
    from engine_revival.report import write_reports

    root = _corpus_copy(tmp_path)
    for path in write_reports(root):
        committed = ROOT / path.relative_to(root)
        assert committed.read_bytes() == path.read_bytes(), committed


def test_the_local_materializer_is_twelve_targets_not_twenty_one():
    from engine_revival import brender_harness as harness
    from engine_revival import brender_harness_templates as templates

    assert CARD["local ladder"]["value"] == "12 targets"
    assert len(harness.OUTPUT_FILES) == 18
    assert len(set(harness.OUTPUT_FILES)) == 18
    project = templates.cmake_project_source(harness.CORE_FLOAT_DEFINES)
    assert project.count("add_test(NAME") == 12
    assert len(harness.CORE_FLOAT_DIRS) == 8
    assert len(harness.CORE_FLOAT_DEFINES) == 9


def test_twenty_eight_of_twenty_nine_targets_claim_no_rung_above_the_first(records):
    assert CARD["still at rung zero"]["value"] == "28 of 29"
    readiness = records["readiness"]
    target_ids = {target.payload["id"] for target in records["target"]}
    scored = {record.payload["target_id"] for record in readiness}
    above = [
        record
        for record in readiness
        if record.payload["readiness_stage"] != "baseline-assessment"
    ]
    assert len(target_ids) == 29
    assert len(readiness) == 22
    assert len(above) == 1
    assert len(readiness) - len(above) == 21
    assert len(target_ids - scored) == 7
    assert 21 + 7 == 28


def test_the_one_engine_above_the_first_rung_names_its_evidence(records):
    above = [
        record
        for record in records["readiness"]
        if record.payload["readiness_stage"] != "baseline-assessment"
    ]
    assert above[0].payload["target_id"] == "brender"
    assert above[0].payload["flagship_score"] == 88
    transcript = (
        ROOT
        / "attempts"
        / "transcripts"
        / "brender-v132-ctest-twentyone-targets-2026-08-27.log"
    ).read_text(encoding="utf-8")
    assert "100% tests passed, 0 tests failed out of 21" in transcript
    assert "eighty-eight" in RUNGS["alt"]


def test_eight_maintained_projects_are_linked_rather_than_rehosted():
    text = (ROOT / "docs" / "DIRECTORY.md").read_text(encoding="utf-8")
    section = text.split("## Actively maintained")[1].split("\n## ")[0]
    rows = [
        line
        for line in section.splitlines()
        if line.startswith("|") and not set(line) <= set("|- ")
    ]
    assert len(rows) - 1 == 8
    assert "eight of them are still maintained" in TRIAGE["alt"]
    assert "does not fork or re-host it" in text


def test_the_suite_carries_the_number_of_tests_the_card_claims():
    """Self-referential on purpose: the count includes the functions here."""
    found = 0
    for path in sorted((ROOT / "tests").glob("test_*.py")):
        tree = ast.parse(path.read_text(encoding="utf-8"))
        found += len(
            [
                node
                for node in ast.walk(tree)
                if isinstance(node, (ast.FunctionDef, ast.AsyncFunctionDef))
                and node.name.startswith("test_")
            ]
        )
    assert CARD["python tests"]["value"] == f"{found} passing"
