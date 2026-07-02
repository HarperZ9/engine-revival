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
        "artifacts/direct3d-directx2-release-record.json",
        "artifacts/directx-sdk-legacy-samples-archive.json",
        "artifacts/opengl-performer-getting-started-guide-30.json",
        "artifacts/opengl-performer-product-brochure.json",
        "artifacts/opengl-performer-programmers-guide-32.json",
        "artifacts/quesa-16d13-release-report.json",
        "artifacts/quesa-source-reimplementation.json",
        "artifacts/quickdraw-3d-documentation-qa.json",
        "artifacts/quickdraw-3d-manual-154-public-pdf.json",
        "artifacts/reality-lab-release-2-public-report.json",
        "artifacts/reality-lab-rendermorphics-acquisition-record.json",
        "artifacts/coin3d-open-inventor-api-compatibility.json",
        "artifacts/crystal-space-github-source-repository.json",
        "artifacts/crystal-space-sourceforge-project-record.json",
        "artifacts/mesa-license-and-api-summary.json",
        "artifacts/mesa-source-repository.json",
        "artifacts/ogre-licensing-record.json",
        "artifacts/ogre-source-repository.json",
        "artifacts/open-inventor-readme-first-build-notes.json",
        "artifacts/open-inventor-source-release.json",
        "artifacts/alias-autodesk-acquisition-record.json",
        "artifacts/iris-gl-opengl-history-record.json",
        "artifacts/japan-studio-restructure-public-report.json",
        "artifacts/opengl-registry-spec-anchor.json",
        "artifacts/openphigs-source-reimplementation.json",
        "artifacts/phigs-standard-public-metadata.json",
        "artifacts/psygnosis-studio-liverpool-closure-report.json",
        "artifacts/psygnosis-studio-liverpool-public-history.json",
        "artifacts/phigs-nist-validation-tests.json",
        "artifacts/softimage-autodesk-acquisition-record.json",
        "artifacts/softimage-last-release-transition-faq.json",
        "artifacts/softimage-microsoft-acquisition-record.json",
        "artifacts/team-ico-playstation-official-pages.json",
        "artifacts/team-ico-ueda-playstation-blog-interview.json",
    }
    missing = [path for path in expected if not (ROOT / path).exists()]
    assert missing == []


def test_first_accession_batch_is_recorded():
    expected = {
        "accessions/aqsis-renderer-source-planned.json",
        "accessions/brender-3dmm-source-planned.json",
        "accessions/brender-v132-source-planned.json",
        "accessions/crystal-space-github-source-repository-planned.json",
        "accessions/mesa-source-repository-planned.json",
        "accessions/ogre-source-repository-planned.json",
        "accessions/open-inventor-source-release-planned.json",
        "accessions/openphigs-source-reimplementation-planned.json",
        "accessions/pixie-renderer-source-mirror-planned.json",
        "accessions/quesa-source-reimplementation-planned.json",
    }
    missing = [path for path in expected if not (ROOT / path).exists()]
    assert missing == []


def test_rights_constrained_accession_batch_is_recorded():
    expected = {
        "accessions/net-yaroze-documentation-set-public-reference.json",
        "accessions/ps1-runtime-library-33-redump-entry-metadata-only.json",
        "accessions/ps1-sdevtc-development-environment-manual-public-reference.json",
        "accessions/ps2-linux-pal-installation-discs-archive-item-metadata-only.json",
        "accessions/ps2-linux-release-10-official-public-reference.json",
        "accessions/ps2-sn-systems-prodg-lineage-public-reference.json",
        "accessions/renderware-ps2-gold-release-notes-public-reference.json",
        "accessions/renderware-sdk-310-ps2-archive-item-metadata-only.json",
    }
    missing = [path for path in expected if not (ROOT / path).exists()]
    assert missing == []
