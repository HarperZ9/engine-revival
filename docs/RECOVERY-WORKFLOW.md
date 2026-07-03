# Recovery Workflow

Use this workflow for each public archive addition.

1. Identify a public source or public-safe lead.
2. Create or update a `sources/*.json` record.
3. Create or update a `targets/*.json`, `artifacts/*.json`, or `tasks/*.json` record.
4. Create or update an `accessions/*.json` record when the lead has a planned,
   external, local, or metadata-only custody action.
5. Create or update a `readiness/*.json` record when the target's rebuild,
   runtime, tests, packaging, or modernization status changes.
6. Run `engine-revival validate`.
7. Run `engine-revival audit-public`.
8. Run `engine-revival report`.
9. Review the generated docs before publishing.

When a lead points to restricted or unclear material, record only metadata and
use `restricted`, `unknown`, `rights-holder-needed`, `clean-room-only`, or
`do-not-redistribute`.
