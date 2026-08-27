from __future__ import annotations

import hashlib
import json
from copy import deepcopy
from pathlib import Path
from textwrap import wrap

from PIL import Image, ImageDraw, ImageFont


ROOT = Path(__file__).resolve().parents[1]
GALLERY = ROOT / "gallery" / "release-20260827"

BRENDER_RELEASE_SHA = "11b5a8d539e911a9c07991b751402a7d51bf1bde"
BRENDER_CANDIDATE_SHA = "bbf3ba2f26ee9ae265759e282dc1454b2234b6be"
BRENDER_SOURCE_SHA = "d88d0ed41122664b9781015b517db64353e16f19"
LOCAL_BOUNDARY_ID = "engine-revival-local-12-target-materializer"
EXTERNAL_BOUNDARY_ID = "harperz9-brender-archival-v0.1.1-21-target-release"
TRANSCRIPT_REF = "attempts/transcripts/brender-v132-ctest-twentyone-targets-2026-08-27.log"

BG = (14, 22, 30)
PANEL = (24, 35, 47)
TEXT = (244, 241, 234)
MUTED = (225, 211, 167)
GREEN = (111, 224, 136)
LINE = (224, 205, 147)


def sha256_bytes(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def sha256_text(text: str) -> str:
    return sha256_bytes(text.encode("utf-8"))


def load_json(path: Path) -> dict[str, object]:
    return json.loads(path.read_text(encoding="utf-8"))


def write_json(path: Path, payload: dict[str, object]) -> None:
    path.write_text(json.dumps(payload, indent=2) + "\n", encoding="utf-8")


def font(size: int, *, bold: bool = False) -> ImageFont.FreeTypeFont | ImageFont.ImageFont:
    candidates = (
        ("C:/Windows/Fonts/segoeuib.ttf" if bold else "C:/Windows/Fonts/segoeui.ttf"),
        ("C:/Windows/Fonts/arialbd.ttf" if bold else "C:/Windows/Fonts/arial.ttf"),
        ("DejaVuSans-Bold.ttf" if bold else "DejaVuSans.ttf"),
        ("Arialbd.ttf" if bold else "Arial.ttf"),
    )
    for candidate in candidates:
        try:
            return ImageFont.truetype(candidate, size)
        except OSError:
            pass
    return ImageFont.load_default()


def draw_wrapped(
    draw: ImageDraw.ImageDraw,
    text: str,
    xy: tuple[int, int],
    width: int,
    fill: tuple[int, int, int],
    image_font: ImageFont.FreeTypeFont | ImageFont.ImageFont,
    *,
    spacing: int = 4,
) -> int:
    x, y = xy
    lines: list[str] = []
    for paragraph in text.split("\n"):
        if not paragraph:
            lines.append("")
            continue
        line = ""
        for word in paragraph.split():
            trial = word if not line else f"{line} {word}"
            if draw.textlength(trial, font=image_font) <= width:
                line = trial
            else:
                if line:
                    lines.append(line)
                line = word
        if line:
            lines.append(line)
    _, _, _, line_height = draw.textbbox((0, 0), "Ag", font=image_font)
    for line in lines:
        draw.text((x, y), line, fill=fill, font=image_font)
        y += line_height + spacing
    return y


def boundary_records() -> list[dict[str, object]]:
    local = {
        "id": LOCAL_BOUNDARY_ID,
        "owner": "Engine Revival",
        "scope": "local portable materializer metadata and scaffold",
        "target_count": 12,
        "claim": "local portable scaffold evidence only; not the external BRender Archival release receipt",
        "recipe": [
            "engine-revival materialize-brender-harness --source-root <public BRender v1.3.2 checkout> --output-root <out-of-tree-harness-dir>",
            "cmake -S <out-of-tree-harness-dir> -B <build> -A Win32 -DBRENDER_SOURCE_DIR=<public BRender v1.3.2 checkout>",
            "ctest --test-dir <build> -C Debug --output-on-failure for the local 12-target materializer ladder",
        ],
    }
    external = {
        "id": EXTERNAL_BOUNDARY_ID,
        "owner": "HarperZ9/brender-archival",
        "scope": "external pinned public release evidence imported as transcript, media, and hashes",
        "target_count": 21,
        "claim": "21-target release evidence comes from the pinned external BRender Archival v0.1.1 checkout; Engine Revival does not port or vendor that implementation in this patch",
        "repository": "https://github.com/HarperZ9/brender-archival",
        "release_tag": "v0.1.1",
        "release_commit": BRENDER_RELEASE_SHA,
        "candidate_contents": BRENDER_CANDIDATE_SHA,
        "checkout_recipe": [
            "git clone https://github.com/HarperZ9/brender-archival.git <brender-archival-v0.1.1>",
            "git -C <brender-archival-v0.1.1> fetch --tags origin",
            f"git -C <brender-archival-v0.1.1> checkout {BRENDER_RELEASE_SHA}",
            f"git -C <brender-archival-v0.1.1> rev-parse HEAD # must equal {BRENDER_RELEASE_SHA}",
        ],
    }
    return [local, external]


def recipes() -> dict[str, list[str]]:
    boundaries = {entry["id"]: entry for entry in boundary_records()}
    return {
        "engine_revival_local_12_target_materializer": list(
            boundaries[LOCAL_BOUNDARY_ID]["recipe"]
        ),
        "external_brender_archival_v0.1.1_21_target_checkout": list(
            boundaries[EXTERNAL_BOUNDARY_ID]["checkout_recipe"]
        ),
        "engine_revival_import_receipt": [
            f"read sanitized transcript and release media from pinned external commit {BRENDER_RELEASE_SHA}",
            "copy public-safe release media and transcript into Engine Revival",
            "verify transcript, manifest, media hashes, and public-boundary scans",
        ],
    }


def put_boundaries(payload: dict[str, object]) -> None:
    payload["evidence_boundaries"] = deepcopy(boundary_records())


def update_structured_records() -> None:
    paths = [
        ROOT / "harnesses" / "brender-v132-portable-core-plan.json",
        ROOT / "reproductions" / "brender-critical-edition-source-build.json",
        ROOT / "builds" / "brender-v132-build-environment.json",
        ROOT / "readiness" / "brender-production-readiness.json",
        ROOT / "tasks" / "brender-asset-pipeline.json",
        ROOT / "tasks" / "brender-critical-edition-packet.json",
    ]
    for path in paths:
        payload = load_json(path)
        put_boundaries(payload)
        write_json(path, payload)

    harness_path = ROOT / "harnesses" / "brender-v132-portable-core-plan.json"
    harness = load_json(harness_path)
    harness["harness_type"] = "local-12-target-materializer-with-external-21-target-release-receipt"
    harness["expected_outputs"] = [
        "Engine Revival local 12-target portable materializer scaffold outside this repo",
        "external BRender Archival v0.1.1 pinned checkout receipt for the 21-target release",
        "sanitized 21-target CTest transcript imported into Engine Revival",
        "provenance-pinned eight-frame period-pipeline orbit media under gallery/release-20260827",
        "release-media provenance manifest with source, command, input, output, and rights hashes",
    ]
    harness["implementation_units"] = [
        "local vector math target: brender_core_smoke",
        "local framework startup target: brender_core_startup_smoke",
        "local wireframe render target: brender_core_render_smoke",
        "local scene graph target: brender_core_scene_smoke",
        "local solid fill target: brender_core_fill_smoke",
        "local depth target: brender_core_depth_smoke",
        "local texture target: brender_core_texture_smoke",
        "local model target: brender_core_model_smoke",
        "local material target: brender_core_material_smoke",
        "local multimodel target: brender_core_multimodel_smoke",
        "local Gouraud target: brender_core_gouraud_smoke",
        "local plotter target: brender_core_plotter_smoke",
    ]
    harness["external_release_summary"] = (
        "The 21-target native CTest receipt is not produced by the local Engine Revival "
        "materializer in this patch. It is imported from the external pinned "
        "HarperZ9/brender-archival v0.1.1 release checkout."
    )
    harness["public_notes"] = (
        "This record distinguishes two boundaries. The Engine Revival local 12-target "
        "portable materializer remains metadata/scaffold for the public BRender v1.3.2 "
        "snapshot. The external pinned BRender Archival v0.1.1 21-target release "
        f"checkout at {BRENDER_RELEASE_SHA} is the source of the imported Win32 CTest "
        "receipt and release media. Engine Revival preserves the receipt and media "
        "provenance; BRender Archival owns the specific restoration; Retro Engine play "
        "output is not BRender proof."
    )
    harness["steps"] = [
        "for local scaffold work, run the Engine Revival 12-target portable materializer outside this repo",
        "for the 21-target release receipt, clone the external BRender Archival repository",
        f"checkout the external release commit {BRENDER_RELEASE_SHA}",
        "verify the external checkout commit before importing public-safe receipts",
        "import only sanitized transcript, media, provenance, and hashes into Engine Revival",
    ]
    write_json(harness_path, harness)

    reproduction_path = ROOT / "reproductions" / "brender-critical-edition-source-build.json"
    reproduction = load_json(reproduction_path)
    reproduction["public_notes"] = (
        "The BRender source-build reproduction is represented as imported public "
        "BRender Archival v0.1.1 evidence. Engine Revival's local 12-target portable "
        "materializer is separate scaffold metadata; the external pinned BRender "
        f"Archival v0.1.1 21-target release checkout at {BRENDER_RELEASE_SHA} is the "
        "source of the 21/21 native CTest receipt and media. Engine Revival does not "
        "vendor source/assets and does not claim completed textured TIA rendering, x64 "
        "readiness, production readiness, adoption, or endorsement."
    )
    reproduction["steps"] = [
        "clone or locate the public BRender v1.3.2 snapshot at d88d0ed41122664b9781015b517db64353e16f19",
        "optionally run the Engine Revival local 12-target portable materializer for scaffold metadata",
        "clone https://github.com/HarperZ9/brender-archival.git into an external checkout",
        f"checkout external BRender Archival commit {BRENDER_RELEASE_SHA}",
        "verify the checkout with git rev-parse HEAD before trusting the release receipt",
        "import the public-safe transcript, media, provenance manifest, and receipt hashes into Engine Revival",
    ]
    reproduction["expected_outputs"] = [
        "Engine Revival local 12-target scaffold metadata",
        "external BRender Archival v0.1.1 pinned checkout receipt",
        "CTest transcript for 21 external release targets",
        "public-safe PNG media and provenance manifest",
        "Engine Revival imported evidence record and sanitized transcript",
    ]
    write_json(reproduction_path, reproduction)

    build_path = ROOT / "builds" / "brender-v132-build-environment.json"
    build = load_json(build_path)
    build["build_system"] = (
        "Engine Revival local 12-target portable materializer metadata plus external "
        "BRender Archival v0.1.1 21-target release receipt"
    )
    build["public_notes"] = (
        "Engine Revival records two build boundaries. The local Engine Revival "
        "portable materializer is a 12-target scaffold path. The 21/21 Win32 CTest "
        "receipt is imported from the external pinned BRender Archival v0.1.1 release "
        f"commit {BRENDER_RELEASE_SHA}. The committed Engine Revival evidence is "
        "metadata, a sanitized transcript, media, and hashes only; no upstream source, "
        "assets, build tree, executables, or PPM source frames are vendored."
    )
    write_json(build_path, build)

    readiness_path = ROOT / "readiness" / "brender-production-readiness.json"
    readiness = load_json(readiness_path)
    readiness["evidence"][1] = (
        "2026-08-27 public BRender Archival release boundary: external pinned "
        f"HarperZ9/brender-archival v0.1.1 checkout at {BRENDER_RELEASE_SHA} is the "
        "source of the 21/21 CTest receipt. Engine Revival's local portable "
        "materializer remains a separate 12-target scaffold path."
    )
    readiness["public_notes"] = (
        "Current Engine Revival boundary, verified on 2026-08-27: Engine Revival "
        "preserves the external pinned BRender Archival v0.1.1 evidence boundary. "
        f"The BRender Archival release commit {BRENDER_RELEASE_SHA} merged PR #9 from "
        f"candidate contents {BRENDER_CANDIDATE_SHA}. Engine Revival's local "
        "12-target portable materializer is separate from the external BRender "
        "Archival 21-target release checkout. Engine Revival equals preservation, "
        "research, and evidence. BRender Archival equals the verified specific "
        "restoration. Retro Engine equals play, and generic Retro output is never "
        "BRender proof. Completed textured TIA output, x64 readiness, production "
        "readiness, adoption, endorsement, and vendored upstream source/assets are not claimed."
    )
    write_json(readiness_path, readiness)

    for task_name in ("brender-asset-pipeline", "brender-critical-edition-packet"):
        task_path = ROOT / "tasks" / f"{task_name}.json"
        task = load_json(task_path)
        if task_name == "brender-asset-pipeline":
            task["public_notes"] = (
                "The BRender asset-pipeline evidence is represented by imported "
                "external BRender Archival v0.1.1 public release receipts. Engine "
                "Revival's local 12-target portable materializer is separate scaffold "
                "metadata; the external pinned BRender Archival v0.1.1 21-target "
                "release supplies the asset/material/pixelmap audit, material "
                "resolution, file texture sampling, game shell, host semantic, and "
                "period-pipeline evidence. Release media is pinned by "
                "gallery/release-20260827/provenance-manifest.json and reports "
                "final_frame_lit=19284 valid=true. Completed textured TIA output, x64 "
                "readiness, production readiness, adoption, endorsement, and generic "
                "Retro output as BRender proof are not claimed."
            )
        else:
            task["public_notes"] = (
                "The BRender critical-edition packet is publishable as an Engine "
                "Revival evidence packet once the operator approves publication. It "
                "separates the Engine Revival local 12-target portable materializer "
                "from the external pinned BRender Archival v0.1.1 21-target release "
                f"checkout at {BRENDER_RELEASE_SHA}. The imported period-pipeline "
                "media source run renders sph32.dat as an eight-frame nonblack orbit "
                "and reports final_frame_lit=19284 valid=true. The packet does not "
                "vendor upstream source/assets and does not claim completed textured "
                "TIA rendering, x64 readiness, production readiness, adoption, or "
                "endorsement. Engine Revival equals preservation, research, and "
                "evidence; BRender Archival equals the verified specific restoration; "
                "Retro Engine equals play; generic Retro output is never BRender proof."
            )
        write_json(task_path, task)

    source_path = ROOT / "sources" / "harperz9-brender-archival-v011.json"
    source = load_json(source_path)
    source["license_treatment"] = (
        "AGPL-3.0-or-later for the public BRender Archival release repository and "
        "imported release artifacts unless verified asset-specific evidence grants otherwise"
    )
    source["notes"] = (
        "Verified by git remote on 2026-08-27: origin/main and tag v0.1.1 both "
        f"resolve to {BRENDER_RELEASE_SHA}. The candidate contents commit is "
        f"{BRENDER_CANDIDATE_SHA}; PR #9 merged the public release branch. Engine "
        "Revival imports public-safe receipts and media only, not the implementation."
    )
    put_boundaries(source)
    write_json(source_path, source)


def card(
    draw: ImageDraw.ImageDraw,
    xy: tuple[int, int],
    size: tuple[int, int],
    title: str,
    lines: list[str],
) -> None:
    x, y = xy
    w, h = size
    draw.rounded_rectangle((x, y, x + w, y + h), radius=18, fill=PANEL, outline=LINE, width=2)
    draw.text((x + 24, y + 24), title, fill=TEXT, font=font(23, bold=True))
    yy = y + 63
    for line in lines:
        draw.text((x + 24, yy), line, fill=MUTED, font=font(19))
        yy += 30


def arrow(draw: ImageDraw.ImageDraw, start: tuple[int, int], end: tuple[int, int]) -> None:
    draw.line((*start, *end), fill=GREEN, width=4)
    x, y = end
    draw.polygon([(x, y), (x - 14, y - 8), (x - 14, y + 8)], fill=GREEN)


def generate_pipeline_diagram() -> None:
    image = Image.new("RGB", (1280, 720), BG)
    draw = ImageDraw.Draw(image)
    draw.text((56, 48), "BRender evidence boundaries", fill=TEXT, font=font(38, bold=True))
    draw.text(
        (58, 98),
        "local 12-target materializer is separate from the external 21-target release receipt",
        fill=MUTED,
        font=font(20),
    )

    card(draw, (80, 205), (260, 126), "Engine Revival local", ["12-target portable", "materializer scaffold"])
    card(draw, (410, 205), (260, 126), "BRender source", ["MIT snapshot", "d88d0ed4..."])
    card(draw, (740, 205), (260, 126), "BRender Archival", ["external v0.1.1", "21-target receipt"])
    card(draw, (410, 430), (510, 126), "Engine Revival import", ["transcript, media, manifest", "hash receipts only; no source/assets/build tree"])

    arrow(draw, (340, 268), (410, 268))
    arrow(draw, (670, 268), (740, 268))
    arrow(draw, (870, 331), (870, 430))
    arrow(draw, (740, 493), (920, 493))

    draw.text((946, 453), "Public packet", fill=TEXT, font=font(28, bold=True))
    draw_wrapped(
        draw,
        "Engine Revival preserves evidence. BRender Archival owns the specific restoration. Generic Retro output is never BRender proof.",
        (946, 492),
        260,
        MUTED,
        font(18),
    )

    image.save(GALLERY / "pipeline-diagram.png", compress_level=9)


def generate_orbit_sequence() -> list[dict[str, object]]:
    image = Image.new("RGB", (1412, 760), BG)
    draw = ImageDraw.Draw(image)
    draw.text((390, 28), "BRender verified orbit-frame sequence", fill=TEXT, font=font(30, bold=True))
    draw.text(
        (358, 70),
        "all panels are ordered frames from brender_core_softrend_render; hashes are manifest-pinned",
        fill=MUTED,
        font=font(17),
    )

    panels: list[dict[str, object]] = []
    thumb_w, thumb_h = 300, 225
    x_positions = [36, 376, 716, 1056]
    y_positions = [145, 470]
    inputs = load_json(GALLERY / "provenance-manifest.json")["inputs"]
    by_label = {entry["label"]: entry for entry in inputs}
    for index in range(8):
        source_label = f"period-pipeline-frame-{index:02d}"
        source = by_label[source_label]
        frame_path = GALLERY / f"period-pipeline-orbit-{index:02d}.png"
        frame = Image.open(frame_path).convert("RGB").resize(
            (thumb_w, thumb_h), Image.Resampling.LANCZOS
        )
        x = x_positions[index % 4]
        y = y_positions[index // 4]
        draw.text(
            (x + 4, y - 45),
            f"orbit frame {index:02d}",
            fill=MUTED,
            font=font(18, bold=True),
        )
        draw.text(
            (x + 4, y - 22),
            f"{source_label} · sha256 {str(source['sha256'])[:8]}",
            fill=MUTED,
            font=font(13),
        )
        draw.rectangle((x - 1, y - 1, x + thumb_w + 1, y + thumb_h + 1), outline=LINE, width=1)
        image.paste(frame, (x, y))
        panels.append(
            {
                "label": f"orbit frame {index:02d}",
                "source_input_label": source_label,
                "source_sha256": source["sha256"],
                "source_rung": "brender_core_softrend_render",
                "source_path": f"gallery/release-20260827/period-pipeline-orbit-{index:02d}.png",
            }
        )

    image.save(GALLERY / "orbit-frame-sequence.png", compress_level=9)
    stale = GALLERY / "progress-sequence.png"
    if stale.exists():
        stale.unlink()
    return panels


def dimensions(path: Path) -> tuple[int, int]:
    with Image.open(path) as image:
        return image.size


def file_entry(path: str, extra: dict[str, object] | None = None) -> dict[str, object]:
    full_path = ROOT / path
    width, height = dimensions(full_path)
    payload: dict[str, object] = {
        "bytes": full_path.stat().st_size,
        "height": height,
        "path": path,
        "sha256": sha256_bytes(full_path.read_bytes()),
        "width": width,
    }
    if extra:
        payload.update(extra)
    return payload


def update_manifest(sequence_panels: list[dict[str, object]]) -> None:
    manifest_path = GALLERY / "provenance-manifest.json"
    manifest = load_json(manifest_path)
    put_boundaries(manifest)
    manifest.pop("commands", None)
    manifest["recipes"] = recipes()
    manifest["rights"] = {
        "engine_revival_code_license": "MIT",
        "upstream_brender_source_license": "MIT",
        "upstream_brender_source_scope": "public source checkout referenced by hash; not vendored in Engine Revival",
        "imported_release_artifact_license_treatment": "AGPL-3.0-or-later unless verified asset-specific evidence grants otherwise",
        "third_party_notice": "THIRD_PARTY_NOTICES.md",
    }
    manifest["limitations"] = [
        "Experimental textured TIA executes but black output remains blocked by a measured vertex-layout/state mismatch.",
        "Native repeat executions on the Win32 Debug port can vary by a few edge pixels; this manifest pins the exact verified PPM source hashes used for public media.",
        "The release media does not claim completed textured rendering, x64 readiness, production readiness, endorsement, or vendored upstream source/assets.",
        "All raster media derive from verified render outputs or factual diagrams/cards; black diagnostic frames are excluded.",
        "The Engine Revival local 12-target portable materializer is not the source of the external BRender Archival v0.1.1 21-target release receipt.",
    ]

    existing = {entry["path"]: entry for entry in manifest["outputs"]}
    existing.pop("gallery/release-20260827/progress-sequence.png", None)
    existing["gallery/release-20260827/pipeline-diagram.png"] = file_entry(
        "gallery/release-20260827/pipeline-diagram.png",
        {
            "label": "boundary-diagram",
            "diagram_boundaries": [LOCAL_BOUNDARY_ID, EXTERNAL_BOUNDARY_ID],
            "provenance": "factual Engine Revival diagram distinguishing local scaffold from external release receipt",
        },
    )
    existing["gallery/release-20260827/orbit-frame-sequence.png"] = file_entry(
        "gallery/release-20260827/orbit-frame-sequence.png",
        {
            "label": "orbit-frame-sequence",
            "provenance": "eight ordered frames from brender_core_softrend_render",
            "panels": sequence_panels,
        },
    )
    ordered_paths = [
        "gallery/release-20260827/evidence-card.png",
        *[f"gallery/release-20260827/period-pipeline-orbit-{index:02d}.png" for index in range(8)],
        "gallery/release-20260827/period-pipeline-orbit-contact-sheet.png",
        "gallery/release-20260827/period-pipeline-poster.png",
        "gallery/release-20260827/period-pipeline-still.png",
        "gallery/release-20260827/pipeline-diagram.png",
        "gallery/release-20260827/orbit-frame-sequence.png",
        "gallery/release-20260827/social-card-1200x630.png",
    ]
    manifest["outputs"] = [existing[path] for path in ordered_paths]

    asset_rights = []
    for output in manifest["outputs"]:
        asset_rights.append(
            {
                "path": output["path"],
                "source_project": "HarperZ9/brender-archival",
                "source_commit": BRENDER_RELEASE_SHA,
                "license_treatment": "AGPL-3.0-or-later unless verified asset-specific evidence grants otherwise",
                "copyright": "BRender Archival release contributors and applicable upstream rightsholders; Engine Revival treats the imported/derived release asset as third-party material.",
            }
        )
    asset_rights.append(
        {
            "path": TRANSCRIPT_REF,
            "source_project": "HarperZ9/brender-archival",
            "source_commit": BRENDER_RELEASE_SHA,
            "license_treatment": "AGPL-3.0-or-later unless verified asset-specific evidence grants otherwise",
            "copyright": "BRender Archival release contributors and applicable upstream rightsholders; sanitized by Engine Revival for public-safe publication.",
        }
    )
    manifest["asset_rights"] = asset_rights
    write_json(manifest_path, manifest)


def update_attempt_hashes() -> None:
    attempt_path = ROOT / "attempts" / "brender-v132-native-ctest-twentyone-targets-win32.json"
    attempt = load_json(attempt_path)
    attempt["artifacts_policy"] = (
        "The BRender source checkout, external BRender Archival checkout, generated harness, "
        "build tree, executables, PPM source frames, and upstream datafiles stayed outside this "
        "metadata repo. Engine Revival commits only the sanitized CTest transcript, public-safe "
        "release media, and provenance hashes imported or derived from the public BRender "
        "Archival v0.1.1 release."
    )
    attempt["command"] = "; ".join(
        recipes()["external_brender_archival_v0.1.1_21_target_checkout"]
        + recipes()["engine_revival_import_receipt"]
    )
    attempt["public_notes"] = (
        "Imported public BRender Archival v0.1.1 release evidence. The external pinned "
        f"release commit {BRENDER_RELEASE_SHA} merged PR #9 from candidate contents "
        f"{BRENDER_CANDIDATE_SHA}. The sanitized CTest transcript records 21/21 passing "
        "native targets with `Test project <build>`. Engine Revival's local 12-target "
        "portable materializer is a separate scaffold boundary and is not the source of "
        "the 21-target receipt. Release media derives from verified nonblack "
        "brender_core_softrend_render orbit frames and reports final_frame_lit=19284 "
        "valid=true. Engine Revival preserves this evidence boundary; BRender Archival "
        "owns the specific restoration; Retro Engine is for play and generic Retro output "
        "is never BRender proof."
    )
    provenance = attempt["release_provenance"]
    put_boundaries(provenance)
    command_entries = []
    for command in (
        recipes()["external_brender_archival_v0.1.1_21_target_checkout"]
        + recipes()["engine_revival_import_receipt"]
    ):
        command_entries.append({"command": command, "sha256": sha256_text(command)})
    provenance["command_hashes"] = command_entries
    provenance["media_manifest"] = "gallery/release-20260827/provenance-manifest.json"
    output_paths = [
        TRANSCRIPT_REF,
        "gallery/release-20260827/provenance-manifest.json",
        "gallery/release-20260827/period-pipeline-still.png",
        "gallery/release-20260827/period-pipeline-orbit-contact-sheet.png",
        "gallery/release-20260827/orbit-frame-sequence.png",
        "gallery/release-20260827/pipeline-diagram.png",
        "gallery/release-20260827/social-card-1200x630.png",
    ]
    provenance["output_hashes"] = [
        {"path": path, "sha256": sha256_bytes((ROOT / path).read_bytes())}
        for path in output_paths
    ]
    provenance["asset_rights_notice"] = "THIRD_PARTY_NOTICES.md"
    attempt["release_provenance"] = provenance
    write_json(attempt_path, attempt)


def main() -> None:
    update_structured_records()
    generate_pipeline_diagram()
    sequence_panels = generate_orbit_sequence()
    update_manifest(sequence_panels)
    update_attempt_hashes()


if __name__ == "__main__":
    main()
