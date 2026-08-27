# Public Boundary

This repo is public-facing from the start. It publishes metadata and tooling,
not restricted artifacts.

## Allowed

- public metadata
- public citations
- checksums of public artifacts
- public-safe summaries
- clean-room notes
- rights and status labels
- recovery task records
- generated public docs
- schemas and validation tools
- public-safe first-party media with explicit provenance, source attribution,
  dimensions, and hashes
- sanitized transcripts that replace local build paths with placeholders

## Not Allowed

- proprietary SDK binaries
- leaked source
- private donor files
- private contact data
- credentials
- real game assets
- restricted media images
- access instructions for restricted material
- local experimental branch output as public proof
- generic Retro output as BRender proof

Restricted leads are represented as metadata with access status, not as files.

## Relationship Boundary

- Retro Engine equals play.
- Engine Revival equals preservation, research, metadata, and evidence.
- BRender Archival equals the verified specific BRender restoration.
- Generic Retro output is never BRender proof.

The BRender Archival v0.1.1 media imported here is allowed because
`gallery/release-20260827/provenance-manifest.json` records public source
attribution, release commit, commands, dimensions, input hashes, output hashes,
and limitations. It does not include upstream BRender source, upstream assets,
private build trees, or restricted media.

The BRender packet uses two explicit evidence boundaries:

- Engine Revival local 12-target portable materializer: public scaffold metadata
  for the local portable harness.
- External pinned BRender Archival v0.1.1 21-target release: public transcript,
  media, and receipt evidence from `HarperZ9/brender-archival` at commit
  `11b5a8d539e911a9c07991b751402a7d51bf1bde`.

Imported or derived BRender Archival release media and the sanitized release
transcript are treated as AGPL-3.0-or-later covered third-party artifacts unless
verified asset-specific evidence grants otherwise. Engine Revival code remains
MIT licensed, and the upstream BRender source MIT license is recorded separately
as non-vendored source provenance. See
[third-party notices](../THIRD_PARTY_NOTICES.md).
