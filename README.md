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
