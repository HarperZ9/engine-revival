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

## Public Docs

- [Public boundary](docs/PUBLIC-BOUNDARY.md)
- [Recovery workflow](docs/RECOVERY-WORKFLOW.md)
- [Generated public index](docs/generated/index.md)
- [Generated targets](docs/generated/targets.md)
- [Generated artifacts](docs/generated/artifacts.md)
- [Generated accessions](docs/generated/accessions.md)
- [Generated tasks](docs/generated/tasks.md)
- [Generated coverage](docs/generated/coverage.md)
- [Generated rights summary](docs/generated/rights-summary.md)
- [Contributing](CONTRIBUTING.md)
