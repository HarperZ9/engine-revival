<picture>
  <source media="(prefers-color-scheme: dark)" srcset="https://raw.githubusercontent.com/HarperZ9/engine-revival/main/docs/art/hero-dark.svg">
  <img src="https://raw.githubusercontent.com/HarperZ9/engine-revival/main/docs/art/hero-light.svg" alt="engine-revival: Triage lost game engines into evidence-backed revival records. A chain of small linked squares, each holding a few ruled lines, winds inward to a bright core." width="100%">
</picture>

# engine-revival

The catalogue of lost rendering and game engines: who made each one, what
survives today and under what licence, and which ones are being rebuilt.

**Read it as a site: [harperz9.github.io/engine-revival](https://harperz9.github.io/engine-revival/)**

[![version: 0.3.0](https://img.shields.io/badge/version-0.3.0-e6e1d6?style=flat-square&labelColor=1a1712)](https://github.com/HarperZ9/engine-revival/releases/latest)
[![CI](https://github.com/HarperZ9/engine-revival/actions/workflows/ci.yml/badge.svg)](https://github.com/HarperZ9/engine-revival/actions/workflows/ci.yml)
[![license](https://img.shields.io/badge/license-FSL--1.1--MIT-e6e1d6?style=flat-square&labelColor=1a1712)](https://github.com/HarperZ9/engine-revival/blob/main/LICENSE)
![python 3.11+](https://img.shields.io/badge/python-3.11%2B-e6e1d6?style=flat-square&labelColor=1a1712)

## Rebuilt engines

Each rebuilt engine lives in its own repository. This one is the hub that
catalogues all of them.

| Engine | What you get | Repository |
|---|---|---|
| Argonaut BRender v1.3.2 | The 1998 engine rebuilt from its public MIT source, with 21 tests that build it and check each step on every change | [HarperZ9/brender-archival](https://github.com/HarperZ9/brender-archival) |

## The catalogue

29 engines, libraries and APIs, each with what it was, what survives and where
to go. [Browse the catalogue](https://harperz9.github.io/engine-revival/#catalogue).

| Status | Engines | What it means |
|---|---|---|
| Rebuilt | 1 | Rebuilt from public source, with tests that build it and check every step. |
| Revival candidate | 6 | Public source or a clean-room project exists, so a revival could be built. |
| Alive upstream | 8 | A living project still maintains it. The catalogue points you there. |
| Documents only | 14 | Only documents and history survive in public, or the rights are unresolved. |

Coming next: the revival candidates, highest priority first, are listed on the
[site](https://harperz9.github.io/engine-revival/#next).

## Use it

```
pip install "engine-revival @ git+https://github.com/HarperZ9/engine-revival@v0.3.0"
git clone https://github.com/HarperZ9/engine-revival && cd engine-revival
engine-revival validate        # every record carries the fields its schema names
engine-revival audit-public    # nothing restricted is marked publishable
engine-revival site --out _site
```

The archive never hosts proprietary SDKs, leaked source, game assets, private
files or restricted media. A restricted engine is recorded as metadata only, and
`audit-public` refuses any record that marks restricted material as publishable.

## How a lead is triaged

![Eight stages taking a lost engine lead to a stated posture: lead, sources, liveness, rights, source, record, directory, posture. A lead starts as a name and a dead link. Sources are cited first, each carrying its own confidence rating, and eighty-six of them are cited across the corpus with sixty-nine rated high, sixteen moderate and one low. The archive then asks whether anybody still maintains the project, and eight of them are still maintained, so the directory links the maintainer instead of forking the code. Rights come next: a license, a named rightsholder, or an unresolved posture that blocks any revival. The source itself is either released, reconstructable clean-room, or genuinely lost. Each lead becomes one JSON file whose id matches its filename, and the directory sorts it into hosted restoration, maintained upstream, buildable candidate, or dossier. Twenty-nine engine targets are tracked across nineteen categories. Three outcomes: a revival candidate whose source exists and whose rights allow the work, a lead recorded as a dossier with nothing buildable claimed, and a project the archive does not re-host because its maintainer is active.](docs/art/directory-lane.svg)

A project somebody still maintains is linked, not forked. A lead whose rights
are unresolved stays a dossier. The posture is part of the record.

## See it work, step by step

The [animated explainer](https://harperz9.github.io/repo-explainers/engine-revival.html)
walks through a lead as a record, validation and its refusals, rights postures, the public-clean audit, the priority index, and claims by rung. Every value on it is output from this repository. Its
source is [docs/explainer/index.html](docs/explainer/index.html).

## Walkthrough

Install it, run it once, then use the main feature. Each command below is real, and so is its output.

1. **Install.** Install from a checkout. Python 3.11 or newer.

   ```text
   $ git clone https://github.com/HarperZ9/engine-revival && cd engine-revival
   $ python -m pip install -e ".[test]"
   ```

2. **First run: validate the archive.** Check every record and reference. A clean archive prints nothing.

   ```text
   $ engine-revival validate
   (no output)
   ```

3. **The public-clean guard.** Run the audit before publishing.

   ```text
   $ engine-revival audit-public
   redistribution do-not-redistribute, access metadata-only
   (no output)
   ```

4. **A record that breaks validation.** Rename one id and validation names every record it orphans.

   ```text
   $ engine-revival validate
   targets\brender.json: target id must match filename stem: brender-x != brender
   artifacts\brender-v132-source.json: unknown target_id: brender
   tasks\brender-triage.json: unknown target_id: brender
   ...
   ```

## The rung ladder

![Eight rungs taking a restored engine from dossier to a recovered title: dossier, source secured, build ladder, render parity, asset pipeline, game shell, remaster pass, lost-game recovery. The first rung records the lead and states its rights posture, claiming nothing buildable. The second pins authorized, license-verified source by commit or archive id. The third stands up a reproducible out-of-tree harness in which every rung self-verifies. The fourth matches reference frames to documented original output within a stated tolerance, and the fifth loads and renders original data formats from rights-clean assets. The sixth runs a title flow start to finish, the seventh reports measured gains such as resolution independence and float color, and the eighth makes a platform-lost title playable with provenance for every asset. Twenty-eight of the twenty-nine tracked targets sit at the first rung, twenty-one of them carrying a baseline readiness record and seven carrying none. The remaining engine carries imported evidence from a pinned external release, with a sanitized twenty-one target transcript and a readiness score of eighty-eight. Three outcomes: evidence imported for one engine, every other target still at the first rung, and neither a remaster pass nor a recovered title claimed anywhere.](docs/art/rung-lane.svg)

Each rung names the claim it earns and nothing above it. Twenty-eight of the
twenty-nine tracked targets sit at the first rung. Full rung definitions are in
the [remaster lane](docs/REMASTER-LANE.md).

## What the archive holds

![A table of twelve rows: what is in the archive, how many of it there are, and where each number is read from. Twelve record kinds are named in RECORD_DIRS, and three hundred and eighty-three JSON records sit across their directories, with sources leading at eighty-six and artifacts and accessions at sixty-six each. Twenty-nine engine targets span nineteen categories. Eighty-six sources are cited, sixty-nine of them rated high confidence, sixteen moderate and one low. Seven hundred and eighty-five references point from one record to another, and the validator resolves every one of them. Five artifacts are marked do-not-redistribute, and none of them carries a publishable access level. Twelve schemas name one hundred and fifteen required fields between them. The report command writes two hundred and thirty-five files and leaves the committed pages byte-identical. The portable materializer generates thirty-one files and twenty-one CTest targets, and CI builds and runs every one of them on Windows. Twenty-eight of the twenty-nine targets carry no rung claim above the first. One hundred and fifty-two Python tests cover the loaders, the validator, the reports, the audit, the materializer, and every number drawn here.](docs/art/corpus-table.svg)

Every count is read from the corpus or from the module that defines it. Rerun
the commands below and the numbers regenerate.

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

If the generated public docs look stale, run `engine-revival seed` before
`engine-revival report`. The seed command writes only the repository's synthetic
public fixtures. It does not fetch proprietary engines, source snapshots,
restricted media, or third-party assets.

`engine-revival audit-public` is the public-clean guard. Treat a failure there
as a release hold until the named record is corrected or removed from the public
surface. For image/media-specific checks, install the optional media extra only
when you need it:

```powershell
python -m pip install -e ".[media,test]"
```

## Licence

Code is FSL-1.1-MIT from v0.2.0 (earlier releases MIT): the Functional Source
License, Version 1.1, with MIT as the future licence, so each release becomes
MIT two years after it ships. The BRender C ports in
`src/engine_revival/brender_compat/` are MIT, with the Argonaut Software 1998
notice beside them. Upstream BRender source is never vendored here. See
[LICENSE](LICENSE) and [THIRD_PARTY_NOTICES.md](THIRD_PARTY_NOTICES.md).

## Public Docs

- [Revival mission](docs/REVIVAL-MISSION.md)
- [Lost engine directory](docs/DIRECTORY.md)
- [BRender archival packet](docs/BRENDER-ARCHIVAL.md)
- [Public boundary](docs/PUBLIC-BOUNDARY.md)
- [Recovery workflow](docs/RECOVERY-WORKFLOW.md)
- [Remaster lane](docs/REMASTER-LANE.md)
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

---

Built by **[Zain Dana Harper](https://harperz9.github.io)** in Seattle.
An independent lab building evidence-first tools that leave a re-checkable
artifact behind. Built by Zain Dana Harper in Seattle. The full workbench is at
[Project Telos](https://harperz9.github.io).
