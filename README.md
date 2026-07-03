# engine-revival

Public tooling spine for reviving historical game engines, SDKs, rendering
libraries, CGI toolkits, and studio technology lineages.

This repo publishes public-safe metadata, schemas, validation tools, target
matrices, and generated summaries. It does not publish proprietary SDKs, leaked
source, game assets, private donor files, private contact data, credentials, or
restricted media.

## First Workflow

```powershell
python -m pip install -e ".[test]"
engine-revival seed
engine-revival validate
engine-revival audit-public
engine-revival index
engine-revival report
python -m pytest
```

## BRender Harness

```powershell
engine-revival materialize-brender-harness `
  --source-root C:\path\to\BRender-v1.3.2 `
  --output-root C:\path\to\brender-v132-portable-core-harness
```

The command writes a CMake scaffold outside the source checkout. It does not
vendor BRender source or claim the scaffold compiles until a compiler transcript
is captured.

## Public Docs

- [Revival mission](docs/REVIVAL-MISSION.md)
- [BRender archival packet](docs/BRENDER-ARCHIVAL.md)
- [Public boundary](docs/PUBLIC-BOUNDARY.md)
- [Recovery workflow](docs/RECOVERY-WORKFLOW.md)
- [Generated public index](docs/generated/index.md)
- [Generated corpus database](docs/generated/database.json)
- [Generated targets](docs/generated/targets.md)
- [Generated sources](docs/generated/sources.md)
- [Generated artifacts](docs/generated/artifacts.md)
- [Generated accessions](docs/generated/accessions.md)
- [Generated tasks](docs/generated/tasks.md)
- [Generated milestones](docs/generated/milestones.md)
- [Generated reproductions](docs/generated/reproductions.md)
- [Generated snapshots](docs/generated/snapshots.md)
- [Generated production readiness](docs/generated/production-readiness.md)
- [Generated build environments](docs/generated/builds.md)
- [Generated harnesses](docs/generated/harnesses.md)
- [Generated attempts](docs/generated/attempts.md)
- [Generated coverage](docs/generated/coverage.md)
- [Generated rights summary](docs/generated/rights-summary.md)
- [Contributing](CONTRIBUTING.md)
