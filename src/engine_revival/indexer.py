from __future__ import annotations

from dataclasses import dataclass
from pathlib import Path

from engine_revival.records import load_records


@dataclass(frozen=True)
class TargetSummary:
    id: str
    name: str
    priority: int
    rights_posture: str
    revival_lane: str


def build_target_index(root: Path) -> list[TargetSummary]:
    summaries = [
        TargetSummary(
            id=str(record.payload["id"]),
            name=str(record.payload["name"]),
            priority=int(record.payload["priority"]),
            rights_posture=str(record.payload["rights_posture"]),
            revival_lane=str(record.payload["revival_lane"]),
        )
        for record in load_records(root, "target")
    ]
    return sorted(summaries, key=lambda item: (-item.priority, item.name.lower()))


def render_target_table(targets: list[TargetSummary]) -> str:
    lines = [
        "| Priority | Target | Rights | Revival lane |",
        "|---:|---|---|---|",
    ]
    for target in targets:
        lines.append(
            f"| {target.priority} | {target.name} | {target.rights_posture} | {target.revival_lane} |"
        )
    return "\n".join(lines) + "\n"
