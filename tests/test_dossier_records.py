from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]


def test_first_dossier_artifacts_are_recorded():
    expected = {
        "artifacts/brender-v132-source.json",
        "artifacts/brender-3dmm-source.json",
        "artifacts/renderware-ps2-gold-release-notes.json",
        "artifacts/renderware-sdk-310-ps2-archive-item.json",
        "artifacts/net-yaroze-documentation-set.json",
        "artifacts/ps1-psnoobsdk-open-source-sdk.json",
        "artifacts/ps1-runtime-library-33-redump-entry.json",
        "artifacts/ps1-sdevtc-development-environment-manual.json",
        "artifacts/ps2-linux-pal-installation-discs-archive-item.json",
        "artifacts/ps2-linux-release-10-official.json",
        "artifacts/ps2-ps2sdk-open-source-sdk.json",
        "artifacts/ps2-sn-systems-prodg-lineage.json",
        "artifacts/gool-andy-gavin-developer-article.json",
        "artifacts/opengoal-jak-project-source.json",
        "artifacts/opengoal-project-overview-docs.json",
        "artifacts/tri-ace-pbr-implementation-course-notes.json",
        "artifacts/tri-ace-ps2-sh-lighting-hdr-gdc2005.json",
        "artifacts/aqsis-renderer-source.json",
        "artifacts/aqsis-renderer-tooling-metadata.json",
        "artifacts/pixie-renderer-source-mirror.json",
        "artifacts/pixie-sourceforge-project-record.json",
        "artifacts/renderman-interface-spec-31-public-pdf.json",
    }
    missing = [path for path in expected if not (ROOT / path).exists()]
    assert missing == []
