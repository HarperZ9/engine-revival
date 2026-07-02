from __future__ import annotations

import argparse
from collections.abc import Sequence


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(
        prog="engine-revival",
        description="Validate and publish public-safe engine revival records.",
    )
    subparsers = parser.add_subparsers(dest="command", required=True)
    for name in ("seed", "validate", "audit-public", "index", "report"):
        subparsers.add_parser(name)
    return parser


def main(argv: Sequence[str] | None = None) -> int:
    parser = build_parser()
    try:
        parser.parse_args(argv)
    except SystemExit as exc:
        return int(exc.code)
    return 0
