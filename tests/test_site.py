"""The documentation site: built from the records, complete, and in plain language."""

import json
import re
from html.parser import HTMLParser
from pathlib import Path

import pytest

from engine_revival.site import SiteError, build_site, load_targets

ROOT = Path(__file__).resolve().parents[1]
# Words that belong in internal records, never on the public site.
INTERNAL_WORDS = re.compile(
    r"\b(lane|gate|operator|receipt|rung|scaffold|accession|critical-edition|posture|tranche|public-safe)s?\b",
    re.IGNORECASE,
)
DASHES = ("—", "–")


class _Collector(HTMLParser):
    def __init__(self):
        super().__init__()
        self.hrefs, self.srcs, self.text = [], [], []

    def handle_starttag(self, tag, attrs):
        attrs = dict(attrs)
        if tag == "a" and "href" in attrs:
            self.hrefs.append(attrs["href"])
        if tag == "img" and "src" in attrs:
            self.srcs.append(attrs["src"])

    def handle_data(self, data):
        self.text.append(data)


def _parse(path):
    collector = _Collector()
    collector.feed(path.read_text(encoding="utf-8"))
    return collector


@pytest.fixture(scope="module")
def site(tmp_path_factory):
    out = tmp_path_factory.mktemp("site")
    build_site(ROOT, out)
    return out


def test_every_target_has_a_page_linked_from_the_catalogue(site):
    targets = sorted(p.stem for p in (ROOT / "targets").glob("*.json"))
    assert len(targets) == 29
    pages = sorted(p.stem for p in (site / "engines").glob("*.html"))
    assert pages == targets
    index_links = _parse(site / "index.html").hrefs
    for target in targets:
        assert f"engines/{target}.html" in index_links


def test_every_internal_link_and_image_resolves(site):
    for page in site.rglob("*.html"):
        parsed = _parse(page)
        for ref in parsed.hrefs + parsed.srcs:
            if ref.startswith(("http://", "https://", "mailto:")):
                continue
            target = (page.parent / ref.split("#")[0]).resolve()
            assert target.is_file(), f"{page.name}: broken link {ref}"


def test_pages_use_plain_language_and_no_dashes(site):
    for page in site.rglob("*.html"):
        text = " ".join(_parse(page).text)
        for dash in DASHES:
            assert dash not in text, f"{page.name} contains a long dash"
        match = INTERNAL_WORDS.search(text)
        assert match is None, f"{page.name}: internal word {match.group(0)!r}"


def test_every_cited_source_exists_and_links_out(site):
    sources = {json.loads(p.read_text(encoding="utf-8"))["id"] for p in (ROOT / "sources").glob("*.json")}
    for target in load_targets(ROOT):
        for source_id in target["hub"].get("source_ids", []):
            assert source_id in sources, f"{target['id']} cites unknown source {source_id}"


def test_brender_is_rebuilt_and_points_to_its_own_repository(site):
    brender = {t["id"]: t for t in load_targets(ROOT)}["brender"]
    assert brender["hub"]["status"] == "rebuilt"
    links = _parse(site / "engines" / "brender.html").hrefs
    assert "https://github.com/HarperZ9/brender-archival" in links


def test_a_target_without_a_hub_block_fails_the_build(tmp_path):
    (tmp_path / "targets").mkdir()
    (tmp_path / "sources").mkdir()
    (tmp_path / "targets" / "x.json").write_text(json.dumps({"id": "x", "name": "X"}), encoding="utf-8")
    with pytest.raises(SiteError, match="missing hub"):
        build_site(tmp_path, tmp_path / "out")


def test_readme_status_counts_match_the_records():
    from collections import Counter

    from engine_revival.site_pages import STATUS_LABELS

    counts = Counter(t["hub"]["status"] for t in load_targets(ROOT))
    readme = (ROOT / "README.md").read_text(encoding="utf-8")
    for status, label in STATUS_LABELS.items():
        assert f"| {label} | {counts[status]} |" in readme, label
    assert f"{sum(counts.values())} engines, libraries and APIs" in readme
