from __future__ import annotations

from pathlib import Path

from engine_revival.records import load_records


def _records_if_present(root: Path, kind: str) -> list[dict[str, object]]:
    directory = root / f"{kind}s"
    if kind == "accession":
        directory = root / "accessions"
    if not directory.exists():
        return []
    return [record.payload for record in load_records(root, kind)]


def packet_tasks(root: Path) -> list[dict[str, object]]:
    return [
        record.payload for record in load_records(root, "task")
        if record.payload["task_type"] == "build-archive-packet"
    ]


def _as_bullets(value: object) -> list[str]:
    if not isinstance(value, list) or not value:
        return ["- none recorded"]
    return [f"- {item}" for item in value]


def packet_index(root: Path) -> str:
    packets = sorted(packet_tasks(root), key=lambda item: str(item["id"]))
    lines = [
        "# Archive Packets",
        "",
        "| Target | Packet | Status | Notes |",
        "|---|---|---|---|",
    ]
    for packet in packets:
        packet_id = str(packet["id"])
        lines.append(
            f"| {packet['target_id']} | [{packet_id}](packets/{packet_id}.md) | "
            f"{packet['status']} | {packet['public_notes']} |"
        )
    if not packets:
        lines.append("| none | none | none | No archive packet tasks recorded yet. |")
    return "\n".join(lines) + "\n"


def packet_page(packet: dict[str, object]) -> str:
    lines = [
        f"# {packet['id']}",
        "",
        "| Field | Value |",
        "|---|---|",
        f"| Target | {packet['target_id']} |",
        f"| Status | {packet['status']} |",
        f"| Task type | {packet['task_type']} |",
        "",
        "## Public Notes",
        "",
        str(packet["public_notes"]),
        "",
        "## Inputs",
        "",
        *_as_bullets(packet.get("inputs")),
        "",
        "## Outputs",
        "",
        *_as_bullets(packet.get("outputs")),
        "",
        "## Blocked By",
        "",
        *_as_bullets(packet.get("blocked_by")),
        "",
        "## Evidence Sources",
        "",
        *_as_bullets(packet.get("source_ids")),
    ]
    return "\n".join(lines) + "\n"


def _records_by_id(records: list[dict[str, object]]) -> dict[str, dict[str, object]]:
    return {str(record["id"]): record for record in records}


def _records_by_target(records: list[dict[str, object]]) -> dict[str, list[dict[str, object]]]:
    grouped: dict[str, list[dict[str, object]]] = {}
    for record in records:
        target_id = str(record["target_id"])
        grouped.setdefault(target_id, []).append(record)
    return grouped


def _accessions_by_target(
    accessions: list[dict[str, object]],
    artifacts: list[dict[str, object]],
) -> dict[str, list[dict[str, object]]]:
    artifact_targets = {
        str(artifact["id"]): str(artifact["target_id"]) for artifact in artifacts
    }
    grouped: dict[str, list[dict[str, object]]] = {}
    for accession in accessions:
        target_id = artifact_targets.get(str(accession["artifact_id"]))
        if target_id:
            grouped.setdefault(target_id, []).append(accession)
    return grouped


def corpus_database(root: Path) -> dict[str, object]:
    targets = _records_if_present(root, "target")
    sources = _records_if_present(root, "source")
    artifacts = _records_if_present(root, "artifact")
    accessions = _records_if_present(root, "accession")
    tasks = _records_if_present(root, "task")
    milestones = _records_if_present(root, "milestone")
    return {
        "schema": "engine-revival-corpus-v1",
        "counts": {
            "targets": len(targets),
            "sources": len(sources),
            "artifacts": len(artifacts),
            "accessions": len(accessions),
            "tasks": len(tasks),
            "milestones": len(milestones),
        },
        "targets": targets,
        "sources": sources,
        "artifacts": artifacts,
        "accessions": accessions,
        "tasks": tasks,
        "milestones": milestones,
        "targets_by_id": _records_by_id(targets),
        "sources_by_id": _records_by_id(sources),
        "artifacts_by_target": _records_by_target(artifacts),
        "accessions_by_target": _accessions_by_target(accessions, artifacts),
        "tasks_by_target": _records_by_target(tasks),
        "milestones_by_target": _records_by_target(milestones),
    }
