import hashlib
import json
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
ATTEMPT_ID = "brender-v132-native-ctest-twentyone-targets-win32"
TRANSCRIPT = ROOT / "attempts" / "transcripts" / "brender-v132-ctest-twentyone-targets-2026-08-27.log"
MEDIA_MANIFEST = ROOT / "gallery" / "release-20260827" / "provenance-manifest.json"
THIRD_PARTY_NOTICES = ROOT / "THIRD_PARTY_NOTICES.md"
AGPL_LICENSE = ROOT / "LICENSES" / "AGPL-3.0-or-later.txt"
BRENDER_RELEASE_SHA = "11b5a8d539e911a9c07991b751402a7d51bf1bde"
BRENDER_CANDIDATE_SHA = "bbf3ba2f26ee9ae265759e282dc1454b2234b6be"
BRENDER_SOURCE_SHA = "d88d0ed41122664b9781015b517db64353e16f19"
AGPL_LICENSE_SHA256 = "0d96a4ff68ad6d4b6f1f30f713b18d5184912ba8dd389f86aa7710db079abcb0"
LOCAL_MATERIALIZER_BOUNDARY = "engine-revival-local-12-target-materializer"
EXTERNAL_RELEASE_BOUNDARY = "harperz9-brender-archival-v0.1.1-21-target-release"


def _load_json(path: Path) -> dict[str, object]:
    return json.loads(path.read_text(encoding="utf-8"))


def _boundary_map(record: dict[str, object]) -> dict[str, dict[str, object]]:
    boundaries = record.get("evidence_boundaries")
    assert isinstance(boundaries, list), "missing evidence_boundaries list"
    return {str(boundary["id"]): boundary for boundary in boundaries}


def _assert_local_and_external_boundaries(record: dict[str, object]) -> None:
    boundaries = _boundary_map(record)

    assert set(boundaries) >= {LOCAL_MATERIALIZER_BOUNDARY, EXTERNAL_RELEASE_BOUNDARY}

    local = boundaries[LOCAL_MATERIALIZER_BOUNDARY]
    assert local["owner"] == "Engine Revival"
    assert local["target_count"] == 12
    assert "portable materializer" in str(local["scope"]).lower()
    assert "21" not in str(local["claim"]).lower()
    _assert_local_recipe_orders_configure_build_test(local["recipe"])

    external = boundaries[EXTERNAL_RELEASE_BOUNDARY]
    assert external["owner"] == "HarperZ9/brender-archival"
    assert external["target_count"] == 21
    assert external["release_tag"] == "v0.1.1"
    assert external["release_commit"] == BRENDER_RELEASE_SHA
    assert external["candidate_contents"] == BRENDER_CANDIDATE_SHA

    checkout_recipe = "\n".join(str(step) for step in external["checkout_recipe"])
    assert "https://github.com/HarperZ9/brender-archival.git" in checkout_recipe
    assert BRENDER_RELEASE_SHA in checkout_recipe
    assert "materialize-brender-harness" not in checkout_recipe


def _assert_local_recipe_orders_configure_build_test(recipe: list[str]) -> None:
    assert recipe == [
        "engine-revival materialize-brender-harness --source-root <public BRender v1.3.2 checkout> --output-root <out-of-tree-harness-dir>",
        "cmake -S <out-of-tree-harness-dir> -B <build> -A Win32 -DBRENDER_SOURCE_DIR=<public BRender v1.3.2 checkout>",
        "cmake --build <build> --config Debug",
        "ctest --test-dir <build> -C Debug --output-on-failure for the local 12-target materializer ladder",
    ]


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


def test_brender_records_separate_local_12_target_materializer_from_external_21_target_release():
    structured_records = [
        _load_json(MEDIA_MANIFEST),
        _load_json(ROOT / "attempts" / f"{ATTEMPT_ID}.json")["release_provenance"],
        _load_json(ROOT / "harnesses" / "brender-v132-portable-core-plan.json"),
        _load_json(ROOT / "reproductions" / "brender-critical-edition-source-build.json"),
        _load_json(ROOT / "builds" / "brender-v132-build-environment.json"),
        _load_json(ROOT / "readiness" / "brender-production-readiness.json"),
        _load_json(ROOT / "tasks" / "brender-asset-pipeline.json"),
        _load_json(ROOT / "tasks" / "brender-critical-edition-packet.json"),
    ]

    for record in structured_records:
        _assert_local_and_external_boundaries(record)


def test_manifest_canonical_local_recipe_orders_configure_build_test():
    manifest = _load_json(MEDIA_MANIFEST)
    local_recipe = manifest["recipes"]["engine_revival_local_12_target_materializer"]
    _assert_local_recipe_orders_configure_build_test(local_recipe)


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


def test_orbit_frame_sequence_replaces_progress_stage_labels():
    manifest = _load_json(MEDIA_MANIFEST)
    inputs = {entry["label"]: entry for entry in manifest["inputs"]}
    outputs = {entry["path"]: entry for entry in manifest["outputs"]}

    assert "gallery/release-20260827/progress-sequence.png" not in outputs
    sequence = outputs["gallery/release-20260827/orbit-frame-sequence.png"]
    assert sequence["label"] == "orbit-frame-sequence"
    assert sequence["provenance"] == "eight ordered frames from brender_core_softrend_render"

    panels = sequence["panels"]
    assert len(panels) == 8
    for index, panel in enumerate(panels):
        input_label = f"period-pipeline-frame-{index:02d}"
        assert panel["label"] == f"orbit frame {index:02d}"
        assert panel["source_input_label"] == input_label
        assert panel["source_sha256"] == inputs[input_label]["sha256"]
        assert panel["source_rung"] == "brender_core_softrend_render"

    panel_text = json.dumps(panels).lower()
    forbidden_stage_labels = (
        "wireframe",
        "flat fill",
        "depth",
        "texture file",
        "material resolve",
        "game shell",
        "restoration stage",
        "progress stage",
    )
    assert not any(term in panel_text for term in forbidden_stage_labels)


def test_imported_brender_release_assets_have_third_party_notice_and_asset_level_rights():
    assert THIRD_PARTY_NOTICES.is_file()
    notice = THIRD_PARTY_NOTICES.read_text(encoding="utf-8")
    notice_lower = notice.lower()

    assert "engine revival code" in notice_lower
    assert "mit" in notice_lower
    assert "upstream brender source" in notice_lower
    assert "not vendored" in notice_lower
    assert "imported brender archival release artifacts" in notice_lower
    assert "agpl" in notice_lower
    assert "[AGPL-3.0-or-later](LICENSES/AGPL-3.0-or-later.txt)" in notice
    assert BRENDER_RELEASE_SHA in notice
    assert AGPL_LICENSE.is_file()
    assert hashlib.sha256(AGPL_LICENSE.read_bytes()).hexdigest() == AGPL_LICENSE_SHA256

    manifest = _load_json(MEDIA_MANIFEST)
    rights = manifest["rights"]
    assert rights["engine_revival_code_license"] == "MIT"
    assert rights["upstream_brender_source_license"] == "MIT"
    assert rights["imported_release_artifact_license_treatment"] == (
        "AGPL-3.0-or-later unless verified asset-specific evidence grants otherwise"
    )

    asset_rights = {entry["path"]: entry for entry in manifest["asset_rights"]}
    expected_assets = {entry["path"] for entry in manifest["outputs"]} | {
        TRANSCRIPT.relative_to(ROOT).as_posix()
    }
    assert set(asset_rights) == expected_assets
    for asset in asset_rights.values():
        assert asset["source_project"] == "HarperZ9/brender-archival"
        assert asset["license_treatment"].startswith("AGPL-3.0")
        assert "copyright" in asset


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
    assert "engine revival local 12-target portable materializer" in lowered
    assert "external pinned brender archival v0.1.1 21-target release" in lowered
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
