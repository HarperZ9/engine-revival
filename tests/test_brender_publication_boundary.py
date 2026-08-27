import hashlib
import json
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
ATTEMPT_ID = "brender-v132-native-ctest-twentyone-targets-win32"
TRANSCRIPT = ROOT / "attempts" / "transcripts" / "brender-v132-ctest-twentyone-targets-2026-08-27.log"
MEDIA_MANIFEST = ROOT / "gallery" / "release-20260827" / "provenance-manifest.json"
BRENDER_RELEASE_SHA = "11b5a8d539e911a9c07991b751402a7d51bf1bde"
BRENDER_CANDIDATE_SHA = "bbf3ba2f26ee9ae265759e282dc1454b2234b6be"
BRENDER_SOURCE_SHA = "d88d0ed41122664b9781015b517db64353e16f19"


def _load_json(path: Path) -> dict[str, object]:
    return json.loads(path.read_text(encoding="utf-8"))


def test_brender_readiness_names_transcript_backed_21_target_attempt():
    readiness = _load_json(ROOT / "readiness" / "brender-production-readiness.json")
    attempt_path = ROOT / "attempts" / f"{ATTEMPT_ID}.json"

    assert ATTEMPT_ID in readiness["attempt_ids"]
    assert attempt_path.is_file()

    attempt = _load_json(attempt_path)
    assert attempt["status"] == "completed"
    assert attempt["exit_code"] == 0
    assert attempt["transcript_location"] == TRANSCRIPT.relative_to(ROOT).as_posix()
    assert "21/21" in attempt["result_summary"]
    assert "Test project <build>" in TRANSCRIPT.read_text(encoding="utf-8")


def test_brender_21_target_receipt_records_public_release_provenance():
    attempt = _load_json(ROOT / "attempts" / f"{ATTEMPT_ID}.json")
    provenance = attempt["release_provenance"]

    assert provenance["brender_archival_release_commit"] == BRENDER_RELEASE_SHA
    assert provenance["brender_archival_candidate_contents"] == BRENDER_CANDIDATE_SHA
    assert provenance["release_tag"] == "v0.1.1"
    assert provenance["pull_request"] == "HarperZ9/brender-archival#9"
    assert provenance["source_sha"] == BRENDER_SOURCE_SHA
    assert provenance["command_hashes"]
    assert provenance["platform_hashes"]
    assert provenance["output_hashes"]


def test_brender_21_target_transcript_is_sanitized_text():
    data = TRANSCRIPT.read_bytes()
    text = data.decode("utf-8")

    assert b"\x00" not in data
    assert text.startswith("Test project <build>")
    assert "<build>" in text
    assert "C:\\dev" not in text
    assert "1/21 Test  #1: brender_core_softrend_render" in text
    assert "21/21 Test #21: brender_core_host_semantic" in text
    assert "100% tests passed, 0 tests failed out of 21" in text


def test_release_media_manifest_hashes_match_committed_public_media():
    manifest = _load_json(MEDIA_MANIFEST)

    assert manifest["schema"] == "brender-archival.release-media-provenance/v1"
    assert manifest["source_sha"] == BRENDER_SOURCE_SHA
    assert manifest["brender_archival_release_commit"] == BRENDER_RELEASE_SHA
    assert manifest["source_attribution"].startswith("Argonaut Software BRender public MIT snapshot")
    assert manifest["metric"]["rung"] == "brender_core_softrend_render"
    assert manifest["metric"]["valid"] is True
    assert manifest["metric"]["final_frame_lit"] > 0

    output_paths = {entry["path"] for entry in manifest["outputs"]}
    assert "gallery/release-20260827/period-pipeline-still.png" in output_paths
    assert "gallery/release-20260827/period-pipeline-orbit-contact-sheet.png" in output_paths
    assert "gallery/release-20260827/social-card-1200x630.png" in output_paths

    for entry in manifest["outputs"]:
        media_path = ROOT / entry["path"]
        assert media_path.is_file(), f"missing media output: {entry['path']}"
        digest = hashlib.sha256(media_path.read_bytes()).hexdigest()
        assert digest == entry["sha256"], f"sha256 drift for {entry['path']}"


def test_public_docs_define_engine_boundaries_and_non_claims():
    public_text = "\n".join(
        path.read_text(encoding="utf-8")
        for path in (
            ROOT / "README.md",
            ROOT / "docs" / "BRENDER-ARCHIVAL.md",
            ROOT / "docs" / "PUBLIC-BOUNDARY.md",
            ROOT / "docs" / "REMASTER-LANE.md",
        )
    )
    lowered = public_text.lower()

    assert "retro engine" in lowered
    assert "engine revival" in lowered
    assert "brender archival" in lowered
    assert "generic retro output is never brender proof" in lowered
    assert "completed textured" in lowered and "not claimed" in lowered
    assert "x64" in lowered and "not claimed" in lowered
    assert "production" in lowered and "not claimed" in lowered
    assert "endorsement" in lowered and "not claimed" in lowered
    assert "12 ctest" not in lowered
    assert "20 executed ctest" not in lowered
    assert "twenty-rung" not in lowered


def test_public_records_do_not_reference_private_workspace_paths():
    offenders: list[str] = []
    scanned_suffixes = {".json", ".md", ".toml", ".log", ".txt"}
    forbidden = (
        "C:" + "\\dev\\public\\engine-revival-workspaces",
        "C:" + "/dev/public/engine-revival-workspaces",
        "C:" + "\\Users\\",
        "C:" + "/Users/",
        "black" + "-frame",
        "experimental/" + "black" + "-frame",
    )

    for path in ROOT.rglob("*"):
        if not path.is_file() or path.suffix.lower() not in scanned_suffixes:
            continue
        if ".git" in path.parts or "__pycache__" in path.parts:
            continue
        text = path.read_text(encoding="utf-8", errors="ignore")
        for term in forbidden:
            if term.lower() in text.lower():
                offenders.append(f"{path.relative_to(ROOT).as_posix()}: {term}")

    assert offenders == []
