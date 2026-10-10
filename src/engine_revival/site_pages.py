"""HTML for the Engine Revival documentation site: the shell and each page."""

from __future__ import annotations

from html import escape

STATUS_LABELS = {
    "rebuilt": "Rebuilt",
    "candidate": "Revival candidate",
    "maintained-elsewhere": "Alive upstream",
    "dossier": "Documents only",
}
STATUS_ORDER = ("rebuilt", "candidate", "maintained-elsewhere", "dossier")
STATUS_MEANING = {
    "rebuilt": "Rebuilt from public source here, with tests that build it and check every step.",
    "candidate": "Public source or a clean-room project exists, so a revival could be built.",
    "maintained-elsewhere": "A living project still maintains it. The archive points you there.",
    "dossier": "Only documents and history survive in public, or the rights are unresolved.",
}
FONTS = (
    '<link rel="preconnect" href="https://fonts.googleapis.com">'
    '<link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>'
    '<link rel="stylesheet" href="https://fonts.googleapis.com/css2?'
    'family=Hanken+Grotesk:wght@400;500;700;800&family=JetBrains+Mono:wght@500&display=swap">'
)
REPO = "https://github.com/HarperZ9/engine-revival"


def shell(title: str, body: str, *, prefix: str = "", description: str = "") -> str:
    desc = escape(description or "Lost rendering and game engines: what survives of each, and the ones being rebuilt.")
    return (
        "<!doctype html>\n<html lang=\"en\"><head><meta charset=\"utf-8\">"
        "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
        f"<title>{escape(title)}</title><meta name=\"description\" content=\"{desc}\">"
        f"<link rel=\"icon\" href=\"{prefix}assets/mark.svg\">{FONTS}"
        f"<link rel=\"stylesheet\" href=\"{prefix}assets/style.css\"></head><body>"
        "<header class=\"site\"><div class=\"wrap\">"
        f"<picture><source media=\"(prefers-color-scheme: dark)\" srcset=\"{prefix}assets/mark-dark.svg\">"
        f"<img src=\"{prefix}assets/mark.svg\" alt=\"\"></picture><a class=\"name\" href=\"{prefix}index.html\">Engine Revival</a>"
        f"<nav><a href=\"{prefix}index.html#catalogue\">Catalogue</a>"
        f"<a href=\"{prefix}index.html#next\">Coming next</a>"
        f"<a href=\"{prefix}about.html\">How it works</a>"
        f"<a class=\"hide-narrow\" href=\"{REPO}\">GitHub</a></nav></div></header>"
        f"<main><div class=\"wrap\">{body}</div></main>"
        "<footer class=\"site\"><div class=\"wrap\">Engine Revival. Records and tools are "
        f"<a href=\"{REPO}\">on GitHub</a>. Each page lists the sources behind it.</div></footer>"
        "</body></html>\n"
    )


def status_cell(status: str) -> str:
    return f"<span class=\"status {escape(status)}\">{escape(STATUS_LABELS[status])}</span>"


def catalogue_table(targets: list[dict], prefix: str = "") -> str:
    rows = []
    for target in targets:
        hub = target["hub"]
        rows.append(
            "<tr>"
            f"<td><a href=\"{prefix}engines/{escape(target['id'])}.html\">{escape(target['name'])}</a></td>"
            f"<td class=\"hide-narrow\">{escape(hub['maker'])}</td>"
            f"<td class=\"hide-narrow\">{escape(hub['years'])}</td>"
            f"<td class=\"hide-narrow\">{escape(category_label(target['category']))}</td>"
            f"<td>{status_cell(hub['status'])}</td></tr>"
        )
    head = "<tr><th>Engine</th><th class=\"hide-narrow\">Made by</th><th class=\"hide-narrow\">Years</th>" \
           "<th class=\"hide-narrow\">Kind</th><th>Status</th></tr>"
    return f"<div class=\"table-scroll\"><table><thead>{head}</thead><tbody>{''.join(rows)}</tbody></table></div>"


ACRONYMS = {"api": "API", "sdk": "SDK", "dcc": "DCC", "gl": "GL"}


def category_label(category: str) -> str:
    words = category.replace("-", " ").split()
    words = [ACRONYMS.get(word.lower(), word) for word in words]
    text = " ".join(words)
    return text[:1].upper() + text[1:]


def links_list(links: list[dict]) -> str:
    if not links:
        return "<p class=\"muted\">No public link is recorded yet.</p>"
    items = "".join(
        f"<li><a href=\"{escape(link['url'])}\">{escape(link['label'])}</a></li>" for link in links
    )
    return f"<ul class=\"plain\">{items}</ul>"


def sources_list(sources: list[dict]) -> str:
    if not sources:
        return "<p class=\"muted\">No source is cited for this entry yet.</p>"
    items = []
    for source in sources:
        publisher = f", {escape(source['publisher'])}" if source.get("publisher") else ""
        title = escape(source["title"])
        if source.get("url"):
            title = f"<a href=\"{escape(source['url'])}\">{title}</a>"
        items.append(
            f"<li>{title}{publisher}"
            f" <span class=\"muted\">(confidence: {escape(source.get('confidence', 'unknown'))})</span></li>"
        )
    return f"<ul class=\"plain sources\">{''.join(items)}</ul>"


def engine_page(target: dict, sources: list[dict]) -> str:
    hub = target["hub"]
    platforms = ", ".join(target.get("platforms", [])) or "unknown"
    body = (
        f"<p class=\"muted\"><a href=\"../index.html#catalogue\">Catalogue</a></p>"
        f"<h1>{escape(target['name'])}</h1><p class=\"lead\">{escape(hub['headline'])}</p>"
        "<dl class=\"facts\">"
        f"<dt>Status</dt><dd>{status_cell(hub['status'])} <span class=\"muted\">"
        f"{escape(STATUS_MEANING[hub['status']])}</span></dd>"
        f"<dt>Made by</dt><dd>{escape(hub['maker'])}</dd>"
        f"<dt>Years</dt><dd>{escape(hub['years'])}</dd>"
        f"<dt>Platforms</dt><dd>{escape(platforms)}</dd>"
        f"<dt>Kind</dt><dd>{escape(category_label(target['category']))}</dd></dl>"
        f"<h2>What it was</h2><p>{escape(hub['what_it_was'])}</p>"
        f"<h2>What survives</h2><p>{escape(hub['what_survives'])}</p>"
        f"<h2>What comes next</h2><p>{escape(hub['next'])}</p>"
        f"<h2>Where to go</h2>{links_list(hub.get('links', []))}"
        f"<h2>Sources</h2>{sources_list(sources)}"
    )
    return shell(f"{target['name']} | Engine Revival", body, prefix="../", description=hub["headline"])
