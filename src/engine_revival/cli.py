from __future__ import annotations

import argparse
from collections.abc import Sequence
from pathlib import Path


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(
        prog="engine-revival",
        description="Validate and publish public-safe engine revival records.",
    )
    subparsers = parser.add_subparsers(dest="command", required=True)
    for name in ("seed", "validate", "audit-public", "index", "report"):
        command = subparsers.add_parser(name)
        command.add_argument("--root", default=".", help="workspace root")
    return parser


def main(argv: Sequence[str] | None = None) -> int:
    parser = build_parser()
    try:
        args = parser.parse_args(argv)
    except SystemExit as exc:
        return int(exc.code)
    if args.command == "validate":
        from engine_revival.validate import validate_workspace

        messages = validate_workspace(Path(args.root))
        for message in messages:
            print(message)
        return 1 if messages else 0
    if args.command == "seed":
        from engine_revival.seed import seed_workspace

        for path in seed_workspace(Path(args.root)):
            print(path)
        return 0
    if args.command == "audit-public":
        from engine_revival.audit import audit_public_workspace

        messages = audit_public_workspace(Path(args.root))
        for message in messages:
            print(message)
        return 1 if messages else 0
    return 0
