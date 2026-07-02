# Engine Revival Public Archive Design

Date: 2026-07-02
Status: Approved for implementation planning
Workspace: `C:\dev\public\engine-revival`

## Purpose

`engine-revival` is a public-facing tooling-first archive for reviving,
documenting, and curating historical game engines, rendering libraries, CGI
toolkits, platform SDKs, and studio technology lineages.

The project starts with tooling and structured evidence rather than a visual
exhibit. Its first responsibility is to keep public records accurate,
reviewable, and safe to publish. A website or exhibit can later consume the
same records.

## Context

The seed research comes from two local reports:

- `C:\Users\Zain\Downloads\deep-research-report2.md`
- `C:\Users\Zain\Downloads\deep-research-report3.md`

Those reports identify two simultaneous preservation lanes:

- Stabilize public/open recoverable material, such as BRender, Open Inventor,
  Quesa, Mesa, Crystal Space, OGRE, Aqsis, and Pixie.
- Track fragmented or rights-constrained material through public-safe metadata,
  clean-room notes, and recovery tasks, such as RenderWare PS2, Reality Lab,
  OpenGL Performer, QuickDraw 3D, PS1/PS2 developer toolchains, GOOL/GOAL,
  tri-Ace rendering, Psygnosis, and Japan Studio/Team Ico.

## Goals

- Create a public repo at `C:\dev\public\engine-revival`.
- Define schemas for targets, artifacts, sources, tasks, and milestones.
- Provide a Python CLI that validates records, builds indexes, audits public
  safety, and generates Markdown reports.
- Seed the archive with the full initial target set from the research reports.
- Make public/restricted boundaries explicit and mechanically checked.
- Enable later public website generation without changing the record model.

## Non-Goals

- Do not publish proprietary SDK binaries, leaked source, game assets, private
  donor material, private contact data, credentials, or restricted media.
- Do not build the public website in the first implementation slice.
- Do not attempt exact engine restoration for restricted targets in this repo.
  This repo records public metadata, clean-room lanes, and recovery state.
- Do not make unverified legal claims. Rights labels describe archive posture,
  not legal advice.

## Architecture

The repo has four layers.

### `schemas/`

JSON Schemas for the public record model:

- target records
- artifact records
- source records
- revival task records
- milestone records

Schemas define required identifiers, status labels, confidence fields, and
public-safety fields.

### `engine_revival/`

A small Python package and CLI. It validates records, builds generated indexes,
emits provenance summaries, and catches unsafe or ambiguous publication states.

The implementation should avoid unnecessary dependencies in the first slice.
If validation needs JSON Schema support, use one well-scoped dependency and
document it in `pyproject.toml`.

### Record Directories

Structured public records live in separate directories:

- `targets/`
- `artifacts/`
- `sources/`
- `tasks/`

The preferred first record format is JSON for straightforward schema
validation. YAML can be considered later if contributor ergonomics outweigh the
extra parser dependency.

### `docs/`

Generated and hand-written public docs:

- target matrix
- priority table
- rights/status summary
- recovery workflow
- contribution guide
- archival ethics
- how to submit leads
- per-target public summaries

Generated docs should clearly identify themselves so hand-written docs are not
silently overwritten.

## Data Model

### Target

A recoverable engine, SDK, toolchain, renderer, studio lineage, middleware
family, or upstream graphics system.

Required fields:

- `id`
- `name`
- `category`
- `era`
- `platforms`
- `priority`
- `revival_lane`
- `rights_posture`
- `summary`
- `public_status`
- `restricted_status`

Optional fields:

- `related_targets`
- `notes`
- `tags`

### Artifact

A specific source, repo, manual, disc, ISO, paper, release note, SDK package,
talk, press disc, or other recoverable item.

Required fields:

- `id`
- `target_id`
- `artifact_type`
- `title`
- `origin`
- `redistribution_status`
- `access_level`
- `evidence_quality`

Optional fields:

- `version`
- `date`
- `location`
- `hashes`
- `source_ids`
- `notes`

### Source

A citation or provenance pointer supporting a claim.

Required fields:

- `id`
- `title`
- `source_type`
- `confidence`
- `claim_scope`

Optional fields:

- `url`
- `local_pointer`
- `publisher`
- `retrieved_at`
- `notes`

### Revival Task

A concrete recovery or reconstruction task.

Required fields:

- `id`
- `target_id`
- `task_type`
- `status`
- `public_notes`

Optional fields:

- `owner`
- `inputs`
- `outputs`
- `blocked_by`
- `source_ids`

### Milestone

A target state checkpoint.

Required fields:

- `id`
- `target_id`
- `milestone_type`
- `status`
- `evidence`

## Controlled Status Labels

Rights and access labels should be conservative and machine-checkable.

Initial labels:

- `open`
- `public-reference-only`
- `restricted`
- `unknown`
- `clean-room-only`
- `rights-holder-needed`
- `do-not-redistribute`

The CLI should fail records that imply publication or redistribution while also
using `restricted`, `unknown`, `rights-holder-needed`, or
`do-not-redistribute`.

## CLI Surface

Initial commands:

### `engine-revival seed`

Creates starter public-safe records for the initial target set. It should not
overwrite edited records unless an explicit force flag is added later.

### `engine-revival validate`

Validates all records against schemas and reports exact file paths for failures.

### `engine-revival audit-public`

Checks publication safety. It fails when records:

- omit rights posture
- omit access level
- use ambiguous redistribution language
- attach restricted material as a publishable artifact
- describe private or proprietary files as public deliverables

### `engine-revival index`

Builds generated index artifacts:

- target matrix
- priority table
- rights/status rollup
- task board

### `engine-revival report`

Generates Markdown summaries under `docs/generated/`, including a public index
and per-target summaries.

## First Successful Workflow

```powershell
python -m pip install -e ".[test]"
engine-revival seed
engine-revival validate
engine-revival audit-public
engine-revival index
engine-revival report
python -m pytest
```

## Initial Target Set

Seed these targets first:

- BRender
- PS1 Programmer's Tool / SDevTC / Net Yaroze
- RenderWare PS2
- GOOL and GOAL
- PS2 ProDG / EB / Linux stack
- tri-Ace PS2 renderer
- Psygnosis / Studio Liverpool
- Japan Studio / Team Ico
- Open Inventor
- Quesa
- Mesa
- Crystal Space
- OGRE
- Aqsis
- Pixie
- RenderMorphics Reality Lab
- OpenGL Performer
- QuickDraw 3D
- RenderMan Interface
- PHIGS
- IRIS GL / OpenGL bridge
- Softimage / Alias bridge

## Public Boundary

Allowed in the public repo:

- public metadata
- public citations
- checksums of public artifacts
- public-safe summaries
- clean-room notes
- rights/status labels
- task records
- generated public docs
- schemas and validation tools

Not allowed in the public repo:

- proprietary SDK binaries
- leaked source
- private donor files
- private contact data
- credentials
- real game assets
- restricted media images
- access instructions for restricted material

Restricted leads are represented as metadata with access status, not as files.

## Verification

Tests must assert meaningful behavior:

- schema validation accepts good records and rejects malformed records
- generated docs are stable for the seeded records
- `audit-public` fails unsafe publication states
- status labels are controlled and consistent
- target IDs referenced by artifacts, tasks, and milestones exist
- CLI commands exit nonzero on invalid input

The success criterion is not "the CLI runs." The success criterion is that the
repo catches unsafe or incoherent public archive states before publication.

## Implementation Slices

The implementation plan should split work into small reviewable slices:

1. Repo bootstrap: package metadata, AGENTS, README, tests, minimal CLI.
2. Schemas: target, artifact, source, task, milestone.
3. Validation engine: record discovery, schema validation, cross-reference
   checks.
4. Public audit: rights/access enforcement and unsafe-language checks.
5. Seed data: initial target records from the reports.
6. Index/report generation: generated docs and summary tables.
7. Verification hardening: targeted tests and sample invalid fixtures.

## Approval Record

User-approved decisions:

- Use `C:\dev` as the canonical workspace.
- Make the archive public-facing from the start.
- Create a new public repo/workspace at `C:\dev\public\engine-revival`.
- Choose a tooling-first archive approach.
- Use the architecture, data model, CLI workflow, public boundary, and
  verification model described in this spec.
