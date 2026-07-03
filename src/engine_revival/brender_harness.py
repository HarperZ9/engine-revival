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
    source_lists = _load_core_float_source_lists(source)
    files = {
        "CMakeLists.txt": _cmake_project(),
        "README.md": _readme(),
        "cmake/brender-core-sources.cmake": _source_manifest_cmake(source_lists),
        "harness-manifest.json": _manifest_json(source_lists),
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
    for directory in CORE_FLOAT_DIRS:
        module_dir = source / "core" / directory
        required.extend([module_dir, module_dir / "makefile"])
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
    compile_definitions = "\n".join(f"  {definition}" for definition in CORE_FLOAT_DEFINES)
    return f"""cmake_minimum_required(VERSION 3.20)
project(brender_v132_portable_core C)

set(BRENDER_SOURCE_DIR "" CACHE PATH "Path to the public BRender v1.3.2 checkout")
if(NOT BRENDER_SOURCE_DIR)
  message(FATAL_ERROR "Set -DBRENDER_SOURCE_DIR=<public BRender checkout>")
endif()
get_filename_component(BRENDER_SOURCE_DIR "${{BRENDER_SOURCE_DIR}}" ABSOLUTE)

include(${{CMAKE_CURRENT_LIST_DIR}}/cmake/brender-core-sources.cmake)

add_library(brender_core_float STATIC ${{BRENDER_CORE_FLOAT_SOURCES}})
target_include_directories(brender_core_float PRIVATE
  "${{BRENDER_SOURCE_DIR}}/inc"
  "${{BRENDER_SOURCE_DIR}}/core/inc"
  "${{BRENDER_SOURCE_DIR}}/core/fw"
  "${{BRENDER_SOURCE_DIR}}/core/host"
  "${{BRENDER_SOURCE_DIR}}/core/std"
  "${{BRENDER_SOURCE_DIR}}/core/pixelmap"
  "${{BRENDER_SOURCE_DIR}}/core/dosio"
  "${{BRENDER_SOURCE_DIR}}/core/v1db"
  "${{BRENDER_SOURCE_DIR}}/core/math"
  "${{BRENDER_SOURCE_DIR}}/core/fmt"
)
target_compile_definitions(brender_core_float PRIVATE
{compile_definitions}
)
"""


def _load_core_float_source_lists(source: Path) -> dict[str, list[str]]:
    source_lists: dict[str, list[str]] = {}
    for directory in CORE_FLOAT_DIRS:
        module_dir = source / "core" / directory
        makefile = module_dir / "makefile"
        object_names = _parse_objs_c(makefile.read_text(encoding="utf-8"))
        if not object_names:
            raise HarnessMaterializationError(
                f"{makefile} does not define OBJS_C entries"
            )
        filenames = [f"{name}.c" for name in object_names]
        missing = [
            module_dir / filename
            for filename in filenames
            if not (module_dir / filename).exists()
        ]
        if missing:
            names = ", ".join(str(path) for path in missing)
            raise HarnessMaterializationError(
                f"BRender makefile references missing C source: {names}"
            )
        source_lists[directory] = filenames
    return source_lists


def _parse_objs_c(text: str) -> list[str]:
    object_names: list[str] = []
    in_block = False
    for raw_line in text.splitlines():
        line = raw_line.strip()
        if not in_block and not line.startswith("OBJS_C"):
            continue
        if not in_block:
            in_block = True
            line = line.split("=", 1)[1] if "=" in line else ""
        if not line or line.startswith("#"):
            if in_block and not line.endswith("\\"):
                break
            continue
        object_name = _extract_object_name(line)
        if object_name:
            object_names.append(object_name)
        if not raw_line.rstrip().endswith("\\"):
            break
    return object_names


def _extract_object_name(line: str) -> str | None:
    item = line.split("#", 1)[0].strip().rstrip("\\").strip()
    if not item:
        return None
    stem = item.rsplit("/", 1)[-1].split("$(OBJ_EXT)", 1)[0]
    if "$(" in stem or ")" in stem or not stem:
        return None
    return stem


def _source_manifest_cmake(source_lists: dict[str, list[str]]) -> str:
    lines = [
        "# Explicit source lists generated from the period OBJS_C makefile rules.",
    ]
    aggregate_vars: list[str] = []
    for directory in CORE_FLOAT_DIRS:
        variable = _module_source_var(directory)
        aggregate_vars.append(variable)
        lines.extend([
            "",
            f"set({variable}",
            *_indented(_cmake_source_paths(directory, source_lists[directory])),
            ")",
        ])
    lines.extend(["", "set(BRENDER_CORE_FLOAT_SOURCES"])
    lines.extend(f"  ${{{variable}}}" for variable in aggregate_vars)
    lines.extend([
        ")",
        "",
        "foreach(source_file IN LISTS BRENDER_CORE_FLOAT_SOURCES)",
        "  if(NOT EXISTS \"${source_file}\")",
        "    message(FATAL_ERROR \"Missing BRender core source: ${source_file}\")",
        "  endif()",
        "endforeach()",
        "",
    ])
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


def _manifest_json(source_lists: dict[str, list[str]]) -> str:
    payload = {
        "id": "brender-v132-portable-core-plan",
        "target_id": "brender",
        "harness_type": "portable-cmake-core-scaffold",
        "core_float_dirs": list(CORE_FLOAT_DIRS),
        "compile_definitions": list(CORE_FLOAT_DEFINES),
        "source_lists": source_lists,
        "source_policy": (
            "out-of-tree; explicit period OBJS_C source lists; "
            "no vendored source or generated binaries"
        ),
    }
    return json.dumps(payload, indent=2, sort_keys=True) + "\n"


def _module_source_var(directory: str) -> str:
    return f"BRENDER_CORE_FLOAT_{directory.upper()}_SOURCES"


def _cmake_source_paths(directory: str, filenames: list[str]) -> list[str]:
    return [
        f'"${{BRENDER_SOURCE_DIR}}/core/{directory}/{filename}"'
        for filename in filenames
    ]


def _indented(items: tuple[str, ...] | list[str]) -> list[str]:
    return [f"  {item}" for item in items]
