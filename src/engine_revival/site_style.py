"""The stylesheet for the Engine Revival documentation site.

Two typefaces (one grotesk, one mono), colour only for a verdict, ink on a
calm ground, light and dark.
"""

STYLE = """
:root {
  --ground: #f6f3ec; --panel: #fffdf7; --ink: #1a1712; --muted: #5d574c;
  --rule: #d9d2c3; --verified: #2f7a4b; --drift: #a3620f; --unverifiable: #6b6f76;
  --sans: "Hanken Grotesk", system-ui, -apple-system, "Segoe UI", sans-serif;
  --mono: "JetBrains Mono", ui-monospace, "Cascadia Mono", Consolas, monospace;
}
@media (prefers-color-scheme: dark) {
  :root {
    --ground: #12100d; --panel: #1a1712; --ink: #ece6d8; --muted: #a59d8c;
    --rule: #3a342a; --verified: #6fcf8f; --drift: #e0a24a; --unverifiable: #9aa0a8;
  }
}
* { box-sizing: border-box; }
html { -webkit-text-size-adjust: 100%; }
body {
  margin: 0; background: var(--ground); color: var(--ink);
  font: 400 17px/1.6 var(--sans);
}
a { color: inherit; text-decoration-thickness: 1px; text-underline-offset: 3px; }
a:hover { text-decoration-thickness: 2px; }
.wrap { max-width: 1040px; margin: 0 auto; padding: 0 16px; }
header.site { border-bottom: 1px solid var(--rule); }
header.site .wrap { display: flex; align-items: center; gap: 12px; padding-top: 14px; padding-bottom: 14px; }
header.site img { width: 28px; height: 28px; display: block; }
header.site nav { margin-left: auto; display: flex; gap: 18px; font-size: 15px; }
header.site .name { font-weight: 700; text-decoration: none; }
main { padding: 40px 0 64px; }
h1 { font-size: clamp(30px, 5vw, 46px); line-height: 1.1; font-weight: 800; margin: 0 0 14px; letter-spacing: -0.01em; }
h2 { font-size: 24px; font-weight: 700; margin: 48px 0 12px; padding-top: 18px; border-top: 1px solid var(--rule); }
h3 { font-size: 18px; font-weight: 700; margin: 24px 0 6px; }
p { margin: 0 0 14px; max-width: 70ch; }
.lead { font-size: 20px; color: var(--ink); max-width: 62ch; }
.muted { color: var(--muted); }
pre, code { font-family: var(--mono); font-size: 14px; }
pre { background: var(--panel); border: 1px solid var(--rule); padding: 14px 16px; overflow-x: auto; }
.feature { background: var(--panel); border: 1px solid var(--rule); border-left: 3px solid var(--verified); padding: 18px 20px; margin: 18px 0; }
.feature h3 { margin-top: 0; }
table { width: 100%; border-collapse: collapse; font-size: 15px; }
th, td { text-align: left; padding: 9px 10px 9px 0; border-bottom: 1px solid var(--rule); vertical-align: top; }
th { font: 500 12px/1.4 var(--mono); color: var(--muted); letter-spacing: 0.04em; }
.table-scroll { overflow-x: auto; }
.status { font: 500 13px/1.4 var(--mono); white-space: nowrap; }
.status.rebuilt { color: var(--verified); }
.status.dossier { color: var(--unverifiable); }
dl.facts { display: grid; grid-template-columns: max-content 1fr; gap: 6px 18px; margin: 18px 0; }
dl.facts dt { font: 500 13px/1.6 var(--mono); color: var(--muted); }
dl.facts dd { margin: 0; }
ul.plain { padding-left: 18px; }
ul.plain li { margin: 4px 0; }
.sources li { font-size: 15px; }
footer.site { border-top: 1px solid var(--rule); padding: 22px 0 40px; font-size: 14px; color: var(--muted); }
@media (max-width: 640px) {
  body { font-size: 16px; }
  header.site .wrap { flex-wrap: wrap; }
  header.site nav { margin-left: 0; width: 100%; gap: 14px; font-size: 14px; }
  dl.facts { grid-template-columns: 1fr; }
  .hide-narrow { display: none; }
}
"""
