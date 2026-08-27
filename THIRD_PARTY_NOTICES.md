# Third-party notices

This repository contains Engine Revival code and public evidence records. Keep
the license scopes distinct when reusing material.

## Engine Revival code

Engine Revival source code, schemas, tests, and first-party metadata tooling in
this repository are licensed under the repository MIT license unless a file
states otherwise.

## Upstream BRender source

The upstream BRender v1.3.2 source is referenced as a public MIT-licensed source
snapshot at commit `d88d0ed41122664b9781015b517db64353e16f19`.

Engine Revival does not vendor that upstream source, its data files, generated
build trees, binaries, or PPM frame outputs. In short: upstream BRender source
and assets are not vendored here. The upstream BRender MIT grant is therefore
recorded as source provenance, not as a grant for every imported release asset
in this repository.

## Imported BRender Archival release artifacts

The public BRender Archival v0.1.1 release repository at commit
`11b5a8d539e911a9c07991b751402a7d51bf1bde` contains an AGPL license file.
Engine Revival therefore treats imported or derived BRender Archival release
artifacts as AGPL-3.0-or-later covered third-party material unless verified
asset-specific evidence later grants a different license.

The canonical license text is committed at
[AGPL-3.0-or-later](LICENSES/AGPL-3.0-or-later.txt).

Asset-level treatment:

| Asset set | Source project | Treatment |
|---|---|---|
| `attempts/transcripts/brender-v132-ctest-twentyone-targets-2026-08-27.log` | `HarperZ9/brender-archival@v0.1.1` | Sanitized public transcript; AGPL-3.0-or-later treatment retained. |
| `gallery/release-20260827/*.png` | `HarperZ9/brender-archival@v0.1.1` release media or Engine Revival factual derivatives from that media | AGPL-3.0-or-later treatment retained unless a narrower asset-specific grant is verified. |
| `gallery/release-20260827/provenance-manifest.json` | Engine Revival public provenance record for imported BRender Archival release artifacts | Engine Revival metadata, with imported artifact rights tracked at `asset_rights`; do not treat this as relicensing the imported media. |

The release media does not claim completed textured rendering, x64 readiness,
production readiness, adoption, endorsement, or vendored upstream source/assets.
