from __future__ import annotations


def cmake_project_source(core_float_defines: tuple[str, ...]) -> str:
    compile_definitions = "\n".join(f"  {definition}" for definition in core_float_defines)
    return f"""cmake_minimum_required(VERSION 3.20)
project(brender_v132_portable_core C)
enable_testing()

if(NOT CMAKE_SIZEOF_VOID_P EQUAL 4)
  message(FATAL_ERROR
    "BRender v1.3.2 harness currently requires a 32-bit C target; "
    "use -A Win32 with Visual Studio.")
endif()

set(BRENDER_SOURCE_DIR "" CACHE PATH "Path to the public BRender v1.3.2 checkout")
if(NOT BRENDER_SOURCE_DIR)
  message(FATAL_ERROR "Set -DBRENDER_SOURCE_DIR=<public BRender checkout>")
endif()
get_filename_component(BRENDER_SOURCE_DIR "${{BRENDER_SOURCE_DIR}}" ABSOLUTE)

include(${{CMAKE_CURRENT_LIST_DIR}}/cmake/brender-core-sources.cmake)

set(BRENDER_PORTABLE_COMPAT_SOURCES
  "${{CMAKE_CURRENT_LIST_DIR}}/compat/brender-portable-core-stubs.c"
  "${{CMAKE_CURRENT_LIST_DIR}}/compat/brender-portable-host-stubs.c"
)

set(BRENDER_CORE_INCLUDE_DIRS
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

set(BRENDER_CORE_V1DB_DISABLED_SOURCES
  ${{BRENDER_CORE_FLOAT_FW_SOURCES}}
  ${{BRENDER_CORE_FLOAT_HOST_SOURCES}}
  ${{BRENDER_CORE_FLOAT_STD_SOURCES}}
  ${{BRENDER_CORE_FLOAT_PIXELMAP_SOURCES}}
  ${{BRENDER_CORE_FLOAT_DOSIO_SOURCES}}
  ${{BRENDER_CORE_FLOAT_MATH_SOURCES}}
)

foreach(source_file IN LISTS BRENDER_CORE_V1DB_DISABLED_SOURCES)
  set_property(SOURCE "${{source_file}}" APPEND PROPERTY COMPILE_DEFINITIONS __BR_V1DB__=0)
endforeach()

foreach(source_file IN LISTS BRENDER_CORE_FLOAT_HOST_SOURCES)
  set_property(SOURCE "${{source_file}}" APPEND PROPERTY COMPILE_DEFINITIONS __WIN_32__=1)
endforeach()

add_library(brender_core_float STATIC
  ${{BRENDER_CORE_FLOAT_SOURCES}}
  ${{BRENDER_PORTABLE_COMPAT_SOURCES}}
)
target_include_directories(brender_core_float PRIVATE ${{BRENDER_CORE_INCLUDE_DIRS}})
target_compile_definitions(brender_core_float PRIVATE
{compile_definitions}
)

add_executable(brender_core_smoke smoke/brender-core-smoke.c)
target_include_directories(brender_core_smoke PRIVATE ${{BRENDER_CORE_INCLUDE_DIRS}})
target_compile_definitions(brender_core_smoke PRIVATE
{compile_definitions}
)
target_link_libraries(brender_core_smoke PRIVATE brender_core_float)
add_test(NAME brender_core_smoke COMMAND brender_core_smoke)

add_executable(brender_core_startup_smoke smoke/brender-core-startup-smoke.c)
target_include_directories(brender_core_startup_smoke PRIVATE ${{BRENDER_CORE_INCLUDE_DIRS}})
target_compile_definitions(brender_core_startup_smoke PRIVATE
{compile_definitions}
)
target_link_libraries(brender_core_startup_smoke PRIVATE brender_core_float)
add_test(NAME brender_core_startup_smoke COMMAND brender_core_startup_smoke)

add_executable(brender_core_render_smoke smoke/brender-core-render-smoke.c)
target_include_directories(brender_core_render_smoke PRIVATE ${{BRENDER_CORE_INCLUDE_DIRS}})
target_compile_definitions(brender_core_render_smoke PRIVATE
{compile_definitions}
)
target_link_libraries(brender_core_render_smoke PRIVATE brender_core_float)
add_test(NAME brender_core_render_smoke
  COMMAND brender_core_render_smoke brender-core-render-smoke.ppm)
"""


def readme_source() -> str:
    return """# BRender v1.3.2 Portable Core Harness

This harness materializes an out-of-tree CMake scaffold for the public BRender
v1.3.2 source checkout. It does not vendor BRender source, generated binaries,
private assets, or restricted SDK material.

```powershell
cmake -S . -B build -A Win32 "-DBRENDER_SOURCE_DIR=<path-to-public-brender-checkout>"
cmake --build build --config Debug --target brender_core_float
cmake --build build --config Debug --target brender_core_smoke
cmake --build build --config Debug --target brender_core_startup_smoke
cmake --build build --config Debug --target brender_core_render_smoke
ctest --test-dir build -C Debug --output-on-failure
```

The `brender_core_render_smoke` target renders a projected 3D wireframe cube
into an in-memory pixelmap through the pure-C memory dispatch path (no assembly
software driver) and writes a PPM image next to the executable. Keep the
generated image out of git unless it is intentionally reviewed as a public
release artifact.

The first compiler run is expected to produce portability findings. Record the
transcript before advancing production-readiness status.
"""
