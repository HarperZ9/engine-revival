import json

import pytest

from engine_revival.brender_harness import (
    HarnessMaterializationError,
    materialize_brender_core_harness,
)
from engine_revival.cli import main


CORE_DIRS = ("fw", "host", "std", "pixelmap", "dosio", "v1db", "math", "fmt")


def _write_source_fixture(root):
    (root / "inc").mkdir(parents=True)
    (root / "inc" / "brender.h").write_text("/* public header fixture */\n", encoding="utf-8")
    (root / "core" / "inc").mkdir(parents=True)
    for name in CORE_DIRS:
        directory = root / "core" / name
        directory.mkdir(parents=True)
        (directory / f"{name}.c").write_text("void fixture(void) {}\n", encoding="utf-8")


def test_materialize_brender_core_harness_writes_out_of_tree_files(tmp_path):
    source = tmp_path / "source"
    output = tmp_path / "harness"
    _write_source_fixture(source)

    written = materialize_brender_core_harness(source, output)

    assert written == [
        output / "CMakeLists.txt",
        output / "README.md",
        output / "cmake" / "brender-core-sources.cmake",
        output / "harness-manifest.json",
    ]
    cmake = (output / "CMakeLists.txt").read_text(encoding="utf-8")
    assert "project(brender_v132_portable_core C)" in cmake
    assert "add_library(brender_core_float STATIC" in cmake
    assert (
        "target_compile_definitions(brender_core_float PRIVATE "
        "BASED_FLOAT=1 BASED_FIXED=0 INLINE_FIXED=0 __386__=1 "
        "DEBUG=0 PARANOID=0 EVAL=0 STATIC=static ADD_RCS_ID=0)"
    ) in cmake
    assert "BRENDER_SOURCE_DIR" in cmake
    readme = (output / "README.md").read_text(encoding="utf-8")
    assert "does not vendor BRender source" in readme
    assert '"-DBRENDER_SOURCE_DIR=<path-to-public-brender-checkout>"' in readme
    manifest = json.loads((output / "harness-manifest.json").read_text(encoding="utf-8"))
    assert manifest["target_id"] == "brender"
    assert manifest["core_float_dirs"] == list(CORE_DIRS)
    assert manifest["compile_definitions"] == [
        "BASED_FLOAT=1",
        "BASED_FIXED=0",
        "INLINE_FIXED=0",
        "__386__=1",
        "DEBUG=0",
        "PARANOID=0",
        "EVAL=0",
        "STATIC=static",
        "ADD_RCS_ID=0",
    ]


def test_materializer_refuses_output_inside_source_checkout(tmp_path):
    source = tmp_path / "source"
    _write_source_fixture(source)

    with pytest.raises(HarnessMaterializationError, match="outside the source checkout"):
        materialize_brender_core_harness(source, source / "generated-harness")


def test_cli_materializes_brender_harness(tmp_path, capsys):
    source = tmp_path / "source"
    output = tmp_path / "harness"
    _write_source_fixture(source)

    exit_code = main([
        "materialize-brender-harness",
        "--source-root",
        str(source),
        "--output-root",
        str(output),
    ])

    captured = capsys.readouterr()
    assert exit_code == 0
    assert str(output / "CMakeLists.txt") in captured.out
    assert (output / "cmake" / "brender-core-sources.cmake").exists()
