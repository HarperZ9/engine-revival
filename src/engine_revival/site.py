"""Build the Engine Revival documentation site from the records.

    engine-revival site --root . --out _site

Every page is a pure function of targets/*.json (with their ``hub`` block) and
sources/*.json, so the site regenerates from the archive and cannot drift from it.
"""

from __future__ import annotations

import json
import shutil
from html import escape
from pathlib import Path

from engine_revival.site_pages import (
    REPO,
    STATUS_LABELS,
    STATUS_MEANING,
    STATUS_ORDER,
    catalogue_table,
    engine_page,
    shell,
)
from engine_revival.site_style import STYLE

HUB_FIELDS = ("headline", "maker", "years", "what_it_was", "what_survives", "status", "next")


class SiteError(ValueError):
    """A record cannot be published on the site as it stands."""


def load_targets(root: Path) -> list[dict]:
    targets = []
    for path in sorted((root / "targets").glob("*.json")):
        target = json.loads(path.read_text(encoding="utf-8"))
        hub = target.get("hub")
        if not isinstance(hub, dict):
            raise SiteError(f"{path}: missing hub block")
        missing = [field for field in HUB_FIELDS if not hub.get(field)]
        if missing:
            raise SiteError(f"{path}: hub is missing {', '.join(missing)}")
        if hub["status"] not in STATUS_LABELS:
            raise SiteError(f"{path}: unknown hub status {hub['status']!r}")
        targets.append(target)
    order = {status: index for index, status in enumerate(STATUS_ORDER)}
    return sorted(targets, key=lambda t: (order[t["hub"]["status"]], -t.get("priority", 0), t["name"]))


def load_sources(root: Path) -> dict[str, dict]:
    sources = {}
    for path in sorted((root / "sources").glob("*.json")):
        record = json.loads(path.read_text(encoding="utf-8"))
        sources[record["id"]] = record
    return sources


def _featured(targets: list[dict]) -> str:
    cards = []
    for target in (t for t in targets if t["hub"]["status"] == "rebuilt"):
        hub = target["hub"]
        repo_links = "".join(
            f" <a href=\"{escape(link['url'])}\">{escape(link['label'])}</a>." for link in hub.get("links", [])[:1]
        )
        cards.append(
            f"<div class=\"feature\"><h3><a href=\"engines/{escape(target['id'])}.html\">"
            f"{escape(target['name'])}</a></h3><p>{escape(hub['headline'])}</p>"
            f"<p>{escape(hub['next'])}{repo_links}</p></div>"
        )
    return "".join(cards)


def _status_key(targets: list[dict]) -> str:
    rows = []
    for status in STATUS_ORDER:
        count = sum(1 for t in targets if t["hub"]["status"] == status)
        rows.append(
            f"<tr><td><span class=\"status {status}\">{STATUS_LABELS[status]}</span></td>"
            f"<td>{count}</td><td>{escape(STATUS_MEANING[status])}</td></tr>"
        )
    head = "<tr><th>Status</th><th>Engines</th><th>What it means</th></tr>"
    return f"<div class=\"table-scroll\"><table><thead>{head}</thead><tbody>{''.join(rows)}</tbody></table></div>"


def _coming_next(targets: list[dict]) -> str:
    candidates = [t for t in targets if t["hub"]["status"] == "candidate"]
    items = "".join(
        f"<li><a href=\"engines/{escape(t['id'])}.html\">{escape(t['name'])}</a>: {escape(t['hub']['next'])}</li>"
        for t in candidates
    )
    return f"<ul class=\"plain\">{items}</ul>"


def index_page(targets: list[dict]) -> str:
    body = (
        "<h1>Lost engines, and the ones coming back</h1>"
        "<p class=\"lead\">A catalogue of historical rendering and game engines: who made each one, "
        "what survives today and under what licence, and which ones are being rebuilt so you can run "
        "them again. Each rebuilt engine lives in its own repository. This site is where you read "
        "about all of them.</p>"
        f"<h2>Rebuilt</h2>{_featured(targets)}"
        f"<h2>Where each engine stands</h2>{_status_key(targets)}"
        f"<h2 id=\"catalogue\">Catalogue</h2><p>{len(targets)} engines, libraries and APIs. "
        "Select one to read what it was, what survives and where to go.</p>"
        f"{catalogue_table(targets)}"
        "<h2 id=\"next\">Coming next</h2><p>Engines with public source or a clean-room project, "
        f"so a revival could be built. Highest priority first.</p>{_coming_next(targets)}"
        "<h2>Use the archive yourself</h2><p>The records behind these pages are plain JSON, and the "
        "tools that check and publish them install with pip.</p>"
        f"<pre><code>pip install \"engine-revival @ git+{REPO}@v0.3.0\"\n"
        "git clone https://github.com/HarperZ9/engine-revival\ncd engine-revival\n"
        "engine-revival validate      # every record has the fields its schema names\n"
        "engine-revival audit-public  # nothing restricted is marked publishable\n"
        "engine-revival site --out _site</code></pre>"
    )
    return shell("Engine Revival", body)


def about_page() -> str:
    body = (
        "<h1>How the archive works</h1>"
        "<p class=\"lead\">Every fact here is a JSON record with a cited source, and the pages are "
        "generated from those records.</p>"
        "<h2>Records and sources</h2><p>Each engine has a target record: what it is, its era and "
        "platforms, and a plain summary. Sources are records too, each with a title, a publisher, a "
        "link and a confidence rating. Schemas name the fields every record must carry, and "
        "<code>engine-revival validate</code> rejects a record that leaves one out.</p>"
        "<h2>What the archive never hosts</h2><p>No proprietary SDKs, leaked source, game assets, "
        "private files or restricted media. When something is restricted, the archive records that "
        "it exists and where it is documented, and holds nothing else. "
        "<code>engine-revival audit-public</code> refuses any record that marks restricted material "
        "as publishable, and it runs on every change.</p>"
        "<h2>Rebuilding an engine</h2><p>A revival starts only from source that is public and openly "
        "licensed, or from a clean-room project. The rebuild lives in its own repository, keeps the "
        "licence the source was released under, and carries tests that build it and check each step. "
        "BRender is the first: its build harness ships in this package, and CI builds it and runs "
        "all 21 steps on Windows.</p>"
        "<pre><code>engine-revival materialize-brender-harness --source-root BRender-v1.3.2 --output-root harness\n"
        "cmake -S harness -B build -A Win32 -DBRENDER_SOURCE_DIR=BRender-v1.3.2\n"
        "cmake --build build --config Debug\nctest --test-dir build -C Debug</code></pre>"
        f"<h2>Help</h2><p>Know an engine that belongs here, or a source we missed? Read "
        f"<a href=\"{REPO}/blob/main/CONTRIBUTING.md\">CONTRIBUTING.md</a> and open an issue.</p>"
    )
    return shell("How it works | Engine Revival", body)


def build_site(root: Path, out: Path) -> list[Path]:
    targets = load_targets(root)
    sources = load_sources(root)
    if out.exists():
        shutil.rmtree(out)
    (out / "engines").mkdir(parents=True)
    (out / "assets").mkdir()
    written = [_write(out / "index.html", index_page(targets)), _write(out / "about.html", about_page())]
    for target in targets:
        cited = [sources[i] for i in target["hub"].get("source_ids", []) if i in sources]
        written.append(_write(out / "engines" / f"{target['id']}.html", engine_page(target, cited)))
    written.append(_write(out / "assets" / "style.css", STYLE.lstrip()))
    for name, source in (("mark.svg", "docs/brand/mark-light.svg"), ("mark-dark.svg", "docs/brand/mark-dark.svg")):
        shutil.copyfile(root / source, out / "assets" / name)
        written.append(out / "assets" / name)
    _write(out / ".nojekyll", "")
    return written


def _write(path: Path, text: str) -> Path:
    path.write_text(text, encoding="utf-8", newline="\n")
    return path
