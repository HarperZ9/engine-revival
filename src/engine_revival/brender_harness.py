from __future__ import annotations

import json
from pathlib import Path


CORE_FLOAT_DIRS = ("fw", "host", "std", "pixelmap", "dosio", "v1db", "math", "fmt")
CORE_FLOAT_DEFINES = (
    "BASED_FLOAT=1",
    "BASED_FIXED=0",
    "INLINE_FIXED=0",
    "__386__=1",
    "DEBUG=0",
    "PARANOID=0",
    "EVAL=0",
    "STATIC=static",
    "ADD_RCS_ID=0",
)
OUTPUT_FILES = (
    "CMakeLists.txt",
    "README.md",
    "cmake/brender-core-sources.cmake",
    "harness-manifest.json",
)


class HarnessMaterializationError(ValueError):
    pass


def materialize_brender_core_harness(source_root: Path, output_root: Path) -> list[Path]:
    source = source_root.resolve()
    output = output_root.resolve()
    _validate_source_tree(source)
    _validate_output_location(source, output)
    files = {
        "CMakeLists.txt": _cmake_project(),
        "README.md": _readme(),
        "cmake/brender-core-sources.cmake": _source_manifest_cmake(),
        "harness-manifest.json": _manifest_json(),
    }
    written: list[Path] = []
    for relative_name in OUTPUT_FILES:
        path = output / relative_name
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text(files[relative_name], encoding="utf-8")
        written.append(path)
    return written


def _validate_source_tree(source: Path) -> None:
    required = [source / "inc", source / "core" / "inc"]
    required.extend(source / "core" / directory for directory in CORE_FLOAT_DIRS)
    missing = [path for path in required if not path.exists()]
    if missing:
        names = ", ".join(str(path) for path in missing)
        raise HarnessMaterializationError(f"BRender source checkout is missing: {names}")


def _validate_output_location(source: Path, output: Path) -> None:
    if output == source or source in output.parents:
        raise HarnessMaterializationError(
            "BRender harness output must be outside the source checkout"
        )


def _cmake_project() -> str:
    return """cmake_minimum_required(VERSION 3.20)
project(brender_v132_portable_core C)

set(BRENDER_SOURCE_DIR "" CACHE PATH "Path to the public BRender v1.3.2 checkout")
if(NOT BRENDER_SOURCE_DIR)
  message(FATAL_ERROR "Set -DBRENDER_SOURCE_DIR=<public BRender checkout>")
endif()
get_filename_component(BRENDER_SOURCE_DIR "${BRENDER_SOURCE_DIR}" ABSOLUTE)

include(${CMAKE_CURRENT_LIST_DIR}/cmake/brender-core-sources.cmake)

add_library(brender_core_float STATIC ${BRENDER_CORE_FLOAT_SOURCES})
target_include_directories(brender_core_float PRIVATE
  "${BRENDER_SOURCE_DIR}/inc"
  "${BRENDER_SOURCE_DIR}/core/inc"
  "${BRENDER_SOURCE_DIR}/core/fw"
  "${BRENDER_SOURCE_DIR}/core/host"
  "${BRENDER_SOURCE_DIR}/core/std"
  "${BRENDER_SOURCE_DIR}/core/pixelmap"
  "${BRENDER_SOURCE_DIR}/core/dosio"
  "${BRENDER_SOURCE_DIR}/core/v1db"
  "${BRENDER_SOURCE_DIR}/core/math"
  "${BRENDER_SOURCE_DIR}/core/fmt"
)
target_compile_definitions(brender_core_float PRIVATE BASED_FLOAT=1 BASED_FIXED=0 INLINE_FIXED=0 __386__=1 DEBUG=0 PARANOID=0 EVAL=0 STATIC=static ADD_RCS_ID=0)
"""


def _source_manifest_cmake() -> str:
    lines = [
        "set(BRENDER_CORE_FLOAT_DIRS",
        *_indented(CORE_FLOAT_DIRS),
        ")",
        "",
        "set(BRENDER_CORE_FLOAT_SOURCES)",
        "foreach(directory IN LISTS BRENDER_CORE_FLOAT_DIRS)",
        "  set(module_dir \"${BRENDER_SOURCE_DIR}/core/${directory}\")",
        "  if(NOT IS_DIRECTORY \"${module_dir}\")",
        "    message(FATAL_ERROR \"Missing BRender core directory: ${module_dir}\")",
        "  endif()",
        "  file(GLOB module_sources CONFIGURE_DEPENDS \"${module_dir}/*.c\")",
        "  list(APPEND BRENDER_CORE_FLOAT_SOURCES ${module_sources})",
        "endforeach()",
        "",
    ]
    return "\n".join(lines)


def _readme() -> str:
    return """# BRender v1.3.2 Portable Core Harness

This harness materializes an out-of-tree CMake scaffold for the public BRender
v1.3.2 source checkout. It does not vendor BRender source, generated binaries,
private assets, or restricted SDK material.

```powershell
cmake -S . -B build "-DBRENDER_SOURCE_DIR=<path-to-public-brender-checkout>"
cmake --build build --target brender_core_float
```

The first compiler run is expected to produce portability findings. Record the
transcript before advancing production-readiness status.
"""


def _manifest_json() -> str:
    payload = {
        "id": "brender-v132-portable-core-plan",
        "target_id": "brender",
        "harness_type": "portable-cmake-core-scaffold",
        "core_float_dirs": list(CORE_FLOAT_DIRS),
        "compile_definitions": list(CORE_FLOAT_DEFINES),
        "source_policy": "out-of-tree; no vendored source or generated binaries",
    }
    return json.dumps(payload, indent=2, sort_keys=True) + "\n"


def _indented(items: tuple[str, ...]) -> list[str]:
    return [f"  {item}" for item in items]
