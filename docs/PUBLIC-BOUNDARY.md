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
