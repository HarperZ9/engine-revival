from __future__ import annotations

from pathlib import Path

from engine_revival.records import load_records


RESTRICTED_LABELS = {
    "restricted",
    "unknown",
    "rights-holder-needed",
    "do-not-redistribute",
}
PUBLISHABLE_LEVELS = {"public", "publishable", "downloadable"}
UNSAFE_WORDS = {"upload included", "download the sdk", "leaked source", "private contact"}
ACCESSION_RESTRICTED_REVIEWS = {
    "restricted",
    "unknown",
    "rights-holder-needed",
    "do-not-redistribute",
}


def audit_public_workspace(root: Path) -> list[str]:
    messages: list[str] = []
    if not (root / "artifacts").exists():
        return messages
    for record in load_records(root, "artifact"):
        redistribution = str(record.payload.get("redistribution_status", ""))
        access = str(record.payload.get("access_level", ""))
        text = " ".join(str(value).lower() for value in record.payload.values())
        if redistribution in RESTRICTED_LABELS and access in PUBLISHABLE_LEVELS:
            messages.append(f"{record.path}: restricted material cannot be publishable")
        for word in UNSAFE_WORDS:
            if word in text:
                messages.append(f"{record.path}: unsafe public wording: {word}")
    if (root / "accessions").exists():
        for record in load_records(root, "accession"):
            rights_review = str(record.payload.get("rights_review", ""))
            storage_class = str(record.payload.get("storage_class", ""))
            text = " ".join(str(value).lower() for value in record.payload.values())
            if rights_review in ACCESSION_RESTRICTED_REVIEWS and storage_class == "local-public":
                messages.append(f"{record.path}: restricted accession cannot use public storage")
            for word in UNSAFE_WORDS:
                if word in text:
                    messages.append(f"{record.path}: unsafe public wording: {word}")
    return messages
