# Recovery Workflow

Use this workflow for each public archive addition.

1. Identify a public source or public-safe lead.
2. Create or update a `sources/*.json` record.
3. Create or update a `targets/*.json`, `artifacts/*.json`, or `tasks/*.json` record.
4. Run `engine-revival validate`.
5. Run `engine-revival audit-public`.
6. Run `engine-revival report`.
7. Review the generated docs before publishing.

When a lead points to restricted or unclear material, record only metadata and
use `restricted`, `unknown`, `rights-holder-needed`, `clean-room-only`, or
`do-not-redistribute`.
