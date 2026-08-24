import json
import re
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]


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


def test_brender_asset_pipeline_claims_stop_at_verified_evidence_boundary():
    task = json.loads((ROOT / "tasks" / "brender-asset-pipeline.json").read_text(encoding="utf-8"))
    notes = task["public_notes"]

    assert task["status"] == "r4-evidence-pending"
    assert "12 verified CTest rungs" in notes
    assert "six implemented asset-pipeline rungs" in notes
    assert "R4 is not closed" in notes
    assert "structured attempt records and command transcripts" in notes


def test_brender_archival_ladder_distinguishes_verified_and_pending_rungs():
    text = (ROOT / "docs" / "BRENDER-ARCHIVAL.md").read_text(encoding="utf-8")
    numbered_rungs = re.findall(r"^\d+\. ", text, flags=re.MULTILINE)

    assert len(numbered_rungs) == 18
    assert "12 verified CTest rungs" in text
    assert "six implemented asset-pipeline rungs" in text
    assert "R4 is not closed" in text


def test_brender_public_surfaces_do_not_reintroduce_closed_r4_overclaim():
    forbidden_fragments = [
        "R4 closed",
        "This closes R4",
        "eighteen CTest targets",
        "eighteen-target ladder",
        "sixteen-target ladder",
        "audit rungs with JSON receipts",
    ]

    for path in PUBLIC_CLAIM_SURFACES:
        text = path.read_text(encoding="utf-8")
        for fragment in forbidden_fragments:
            assert fragment not in text, f"{path} contains overclaim: {fragment}"
