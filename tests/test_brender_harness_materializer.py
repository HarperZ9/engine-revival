import json
from pathlib import Path

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
    softrend_dir = root / "drivers" / "softrend"
    softrend_dir.mkdir(parents=True)
    (softrend_dir / "alpha.c").write_text("void sr_alpha(void) {}\n", encoding="utf-8")
    (softrend_dir / "clip.c").write_text("void sr_clip(void) {}\n", encoding="utf-8")
    (softrend_dir / "makefile").write_text(
        "\n".join([
            "OBJS_C=\\",
            "    $(BLD_DIR)/alpha$(OBJ_EXT)\\",
            "    $(BLD_DIR)/clip$(OBJ_EXT)\\",
            "",
            "OBJS_ASM=\\",
            "    $(BLD_DIR)/cull$(OBJ_EXT)\\",
            ""],
        ),
        encoding="utf-8",
    )

    pentprim_dir = root / "drivers" / "pentprim"
    pentprim_dir.mkdir(parents=True)
    (pentprim_dir / "driver.c").write_text("void pp_driver(void) {}\n", encoding="utf-8")
    (pentprim_dir / "l_pi.c").write_text("void pp_lpi(void) {}\n", encoding="utf-8")

    (pentprim_dir / "awtmz.c").write_text("void pp_awtmz(void) {}\n", encoding="utf-8")
    (pentprim_dir / "makefile").write_text(
        "\n".join([
            "OBJS_C=\\",
            "    $(BLD_DIR)/driver$(OBJ_EXT)\\",
            "",
            "XOBJS_C=\\",
            "    $(BLD_DIR)/l_pi$(OBJ_EXT)\\",
            "",
            "XOBJS_ASM=\\",
            "    $(BLD_DIR)/zb8$(OBJ_EXT)\\",
            ""],
        ),
        encoding="utf-8",
    )
    for name in CORE_DIRS:
        directory = root / "core" / name
        directory.mkdir(parents=True)
        (directory / f"{name}_listed.c").write_text("void fixture(void) {}\n", encoding="utf-8")
        (directory / f"{name}_unlisted.c").write_text("void skip_me(void) {}\n", encoding="utf-8")
        (directory / f"{name}_commented.c").write_text("void skip_comment(void) {}\n", encoding="utf-8")
        (directory / "makefile").write_text(
            "\n".join([
                "OBJS_C=\\",
                f"    $(BLD_DIR)/{name}_listed$(OBJ_EXT)\\",
                f"#   $(BLD_DIR)/{name}_commented$(OBJ_EXT)\\",
                "",
                "OBJS_ASM=\\",
                "",
            ]),
        encoding="utf-8",
    )


def _materialized_smoke_sources(tmp_path):
    source = tmp_path / "source"
    output = tmp_path / "harness"
    _write_source_fixture(source)
    materialize_brender_core_harness(source, output)
    return output / "smoke"


def _materialized_harness(tmp_path):
    source = tmp_path / "source"
    output = tmp_path / "harness"
    _write_source_fixture(source)
    materialize_brender_core_harness(source, output)
    return output


def test_materialize_brender_core_harness_writes_out_of_tree_files(tmp_path):
    source = tmp_path / "source"
    output = tmp_path / "harness"
    _write_source_fixture(source)

    written = materialize_brender_core_harness(source, output)

    assert written == [
        output / "CMakeLists.txt",
        output / "README.md",
        output / "cmake/brender-core-sources.cmake",
        output / "cmake/brender-softrend.cmake",
        output / "cmake/brender-pentprim.cmake",
        output / "compat/brender-softrend-subdiv-port.c",
        output / "compat/brender-pentprim-c-port.c",
        output / "compat/brender-pentprim-fpsetup-port.h",
        output / "compat/brender-pentprim-fpsetup-port.c",
        output / "compat/brender-pentprim-i8-port.c",
        output / "compat/brender-pentprim-pfpsetup-port.h",
        output / "compat/brender-pentprim-pfpsetup-port.c",
        output / "compat/brender-pentprim-i8p-port.c",
        output / "compat/brender-pentprim-i8pow2-port.c",
        output / "compat/brender-pentprim-tt-port.c",
        output / "compat/brender-pentprim-awsl-port.c",
        output / "compat/brender-pentprim-zb16-port.c",
        output / "compat/brender-pentprim-tip-port.c",
        output / "compat/brender-pentprim-mmx-port.h",
        output / "compat/brender-pentprim-mmx-tables-port.c",
        output / "compat/brender-pentprim-mmx-common-port.c",
        output / "compat/brender-pentprim-mmx-uv-port.c",
        output / "compat/brender-pentprim-mmx-setup-port.c",
        output / "compat/brender-pentprim-mmx-zrgb-port.c",
        output / "compat/brender-pentprim-mmx-zuv-port.c",
        output / "compat/brender-pentprim-mmx-zuvc-port.c",
        output / "compat/brender-pentprim-mmx-zuvrgb-port.c",
        output / "compat/brender-portable-core-stubs.c",
        output / "compat/brender-portable-host-stubs.c",
        output / "smoke/brender-core-smoke.c",
        output / "smoke/brender-core-startup-smoke.c",
        output / "smoke/brender-core-render-smoke.c",
        output / "smoke/brender-core-scene-smoke.c",
        output / "smoke/brender-core-fill-smoke.c",
        output / "smoke/brender-core-depth-smoke.c",
        output / "smoke/brender-core-texture-smoke.c",
        output / "smoke/brender-core-model-smoke.c",
        output / "smoke/brender-core-material-smoke.c",
        output / "smoke/brender-core-multimodel-smoke.c",
        output / "smoke/brender-core-gouraud-smoke.c",
        output / "smoke/brender-core-plotter-smoke.c",
        output / "smoke/brender-core-asset-audit.c",
        output / "smoke/brender-core-material-audit.c",
        output / "smoke/brender-core-material-file-audit.c",
        output / "smoke/brender-core-pixelmap-roundtrip.c",
        output / "smoke/brender-core-material-resolve.c",
        output / "smoke/brender-core-texture-file-sample.c",
        output / "smoke/brender-core-game-shell.c",
        output / "smoke/brender-core-host-semantic.c",
        output / "smoke/brender-core-softrend-render.c",
        output / "harness-manifest.json",
    ]
    cmake = (output / "CMakeLists.txt").read_text(encoding="utf-8")
    assert "project(brender_v132_portable_core C)" in cmake
    assert "add_library(brender_core_float STATIC" in cmake
    assert "add_executable(brender_core_smoke" in cmake
    assert "add_executable(brender_core_startup_smoke" in cmake
    assert "target_link_libraries(brender_core_smoke PRIVATE brender_core_float)" in cmake
    assert "target_link_libraries(brender_core_startup_smoke PRIVATE brender_core_float)" in cmake
    assert "add_test(NAME brender_core_smoke COMMAND brender_core_smoke)" in cmake
    assert "add_test(NAME brender_core_startup_smoke COMMAND brender_core_startup_smoke)" in cmake
    assert "add_executable(brender_core_render_smoke" in cmake
    assert "target_link_libraries(brender_core_render_smoke PRIVATE brender_core_float)" in cmake
    assert "add_test(NAME brender_core_render_smoke" in cmake
    assert "add_executable(brender_core_scene_smoke" in cmake
    assert "target_link_libraries(brender_core_scene_smoke PRIVATE brender_core_float)" in cmake
    assert "add_test(NAME brender_core_scene_smoke" in cmake
    assert "add_executable(brender_core_fill_smoke" in cmake
    assert "target_link_libraries(brender_core_fill_smoke PRIVATE brender_core_float)" in cmake
    assert "add_test(NAME brender_core_fill_smoke" in cmake
    assert "add_executable(brender_core_depth_smoke" in cmake
    assert "target_link_libraries(brender_core_depth_smoke PRIVATE brender_core_float)" in cmake
    assert "add_test(NAME brender_core_depth_smoke" in cmake
    assert "add_executable(brender_core_texture_smoke" in cmake
    assert "target_link_libraries(brender_core_texture_smoke PRIVATE brender_core_float)" in cmake
    assert "add_test(NAME brender_core_texture_smoke" in cmake
    assert "add_executable(brender_core_model_smoke" in cmake
    assert "target_link_libraries(brender_core_model_smoke PRIVATE brender_core_float)" in cmake
    assert "add_test(NAME brender_core_model_smoke" in cmake
    assert "${BRENDER_SOURCE_DIR}/dat/duck.dat" in cmake
    assert "add_executable(brender_core_material_smoke" in cmake
    assert "add_test(NAME brender_core_material_smoke" in cmake
    assert "${BRENDER_SOURCE_DIR}/dat/sph32.dat" in cmake
    assert "add_executable(brender_core_multimodel_smoke" in cmake
    assert "${BRENDER_SOURCE_DIR}/dat/coupe.dat" in cmake
    assert "add_executable(brender_core_gouraud_smoke" in cmake
    assert "add_executable(brender_core_plotter_smoke" in cmake
    assert "${BRENDER_SOURCE_DIR}/dat/teapot.dat" in cmake
    assert "add_executable(brender_core_asset_audit" in cmake
    assert "target_link_libraries(brender_core_asset_audit PRIVATE brender_core_float)" in cmake
    assert "add_test(NAME brender_core_asset_audit" in cmake
    assert "add_executable(brender_core_material_audit" in cmake
    assert "target_link_libraries(brender_core_material_audit PRIVATE brender_core_float)" in cmake
    assert "add_test(NAME brender_core_material_audit" in cmake
    assert "add_executable(brender_core_material_file_audit" in cmake
    assert "add_test(NAME brender_core_material_file_audit" in cmake
    assert "${BRENDER_SOURCE_DIR}/dat/std.pal" in cmake
    assert "add_executable(brender_core_pixelmap_roundtrip" in cmake
    assert "add_test(NAME brender_core_pixelmap_roundtrip" in cmake
    assert "add_executable(brender_core_material_resolve" in cmake
    assert "add_test(NAME brender_core_material_resolve" in cmake
    assert "add_executable(brender_core_texture_file_sample" in cmake
    assert "${BRENDER_SOURCE_DIR}/dat/std.pal" in cmake
    assert "add_test(NAME brender_core_texture_file_sample" in cmake
    assert "add_executable(brender_core_game_shell" in cmake
    assert "add_test(NAME brender_core_game_shell" in cmake
    assert "add_executable(brender_core_host_semantic" in cmake
    assert "target_link_libraries(brender_core_host_semantic PRIVATE brender_core_float)" in cmake
    assert "add_test(NAME brender_core_host_semantic" in cmake
    assert "compat/brender-portable-core-stubs.c" in cmake
    assert "compat/brender-portable-host-stubs.c" in cmake
    assert "CMAKE_SIZEOF_VOID_P" in cmake
    assert "use -A Win32 with Visual Studio" in cmake
    assert "__BR_V1DB__=0" in cmake
    assert "__WIN_32__=1" in cmake
    assert "target_compile_definitions(brender_core_float PRIVATE" in cmake
    for definition in [
        "BASED_FLOAT=1",
        "BASED_FIXED=0",
        "INLINE_FIXED=0",
        "__386__=1",
        "DEBUG=0",
        "PARANOID=0",
        "EVAL=0",
        "STATIC=static",
        "ADD_RCS_ID=0",
    ]:
        assert f"  {definition}" in cmake
    assert "BRENDER_SOURCE_DIR" in cmake
    source_manifest = (output / "cmake" / "brender-core-sources.cmake").read_text(
        encoding="utf-8"
    )
    assert "file(GLOB" not in source_manifest
    assert '"${BRENDER_SOURCE_DIR}/core/fw/fw_listed.c"' in source_manifest
    assert "fw_unlisted.c" not in source_manifest
    assert "fw_commented.c" not in source_manifest
    smoke = (output / "smoke" / "brender-core-smoke.c").read_text(encoding="utf-8")
    assert '#include "brender.h"' in smoke
    assert "#define _NO_VECTOR_MACROS 1" in smoke
    assert "BrVector3SetFloat(&vector, 1.0f, 2.0f, 3.0f)" in smoke
    assert "BrScalarToFloat(vector.v[2])" in smoke
    assert "BrBegin()" not in smoke
    startup_smoke = (output / "smoke" / "brender-core-startup-smoke.c").read_text(
        encoding="utf-8"
    )
    assert "#define __BR_V1DB__ 0" in startup_smoke
    assert "BrBegin()" in startup_smoke
    assert "BrEnd()" in startup_smoke
    render_smoke = (output / "smoke" / "brender-core-render-smoke.c").read_text(
        encoding="utf-8"
    )
    assert "BrPixelmapAllocate(BR_PMT_RGB_888" in render_smoke
    assert "BrMatrix4Perspective(" in render_smoke
    assert "BrMatrix4ApplyP(" in render_smoke
    assert "BrPixelmapLine(" in render_smoke
    assert "BrPixelmapPixelGet(" in render_smoke
    assert 'fprintf(f, "P6\\n%d %d\\n255\\n"' in render_smoke
    scene_smoke = (output / "smoke" / "brender-core-scene-smoke.c").read_text(
        encoding="utf-8"
    )
    assert "BrActorAllocate(BR_ACTOR_CAMERA" in scene_smoke
    assert "BrModelAllocate(" in scene_smoke
    assert "BrModelUpdate(model, BR_MODU_ALL)" in scene_smoke
    assert "BrActorToScreenMatrix4(" in scene_smoke
    assert "BrPixelmapLine(" in scene_smoke
    fill_smoke = (output / "smoke" / "brender-core-fill-smoke.c").read_text(
        encoding="utf-8"
    )
    assert "fill_triangle(" in fill_smoke
    assert "BrActorToScreenMatrix4(" in fill_smoke
    assert "BrMatrix34ApplyP(" in fill_smoke
    depth_smoke = (output / "smoke" / "brender-core-depth-smoke.c").read_text(
        encoding="utf-8"
    )
    assert "fill_triangle_z(" in depth_smoke
    assert "g_zbuf" in depth_smoke
    assert "g_rejections" in depth_smoke
    texture_smoke = (output / "smoke" / "brender-core-texture-smoke.c").read_text(
        encoding="utf-8"
    )
    assert "fill_triangle_tex(" in texture_smoke
    assert "BrPixelmapPixelGet(tex" in texture_smoke
    assert "uow" in texture_smoke and "vow" in texture_smoke
    model_smoke = (output / "smoke" / "brender-core-model-smoke.c").read_text(
        encoding="utf-8"
    )
    assert "BrModelLoad(" in model_smoke
    assert "BrActorToScreenMatrix4(" in model_smoke
    assert "fill_triangle_z(" in model_smoke
    material_smoke = (output / "smoke" / "brender-core-material-smoke.c").read_text(
        encoding="utf-8"
    )
    assert "BrModelLoad(" in material_smoke
    assert "fill_triangle_tex(" in material_smoke
    assert "model->vertices[i].map" in material_smoke
    multimodel_smoke = (output / "smoke" / "brender-core-multimodel-smoke.c").read_text(encoding="utf-8")
    assert "BrModelLoadMany(" in multimodel_smoke
    gouraud_smoke = (output / "smoke" / "brender-core-gouraud-smoke.c").read_text(encoding="utf-8")
    assert "fill_triangle_gouraud(" in gouraud_smoke
    plotter_smoke = (output / "smoke" / "brender-core-plotter-smoke.c").read_text(encoding="utf-8")
    assert "raster_depth(" in plotter_smoke
    assert "<svg xmlns=" in plotter_smoke
    asset_audit = (output / "smoke" / "brender-core-asset-audit.c").read_text(
        encoding="utf-8"
    )
    assert "BrModelLoad(" in asset_audit
    assert "nonfinite_vertices" in asset_audit
    assert "out_of_range_faces" in asset_audit
    assert "degenerate_faces" in asset_audit
    assert "BrModelFree(model)" in asset_audit
    material_audit = (output / "smoke" / "brender-core-material-audit.c").read_text(
        encoding="utf-8"
    )
    assert "BrPixelmapLoad(" in material_audit
    assert "pixels_decoded" in material_audit
    assert "BrPixelmapFree(pm)" in material_audit
    material_file = (output / "smoke" / "brender-core-material-file-audit.c").read_text(
        encoding="utf-8"
    )
    assert "BrMaterialLoad(" in material_file
    assert "has_colour_map" in material_file
    assert "BrMaterialFree(mat)" in material_file
    assert "faces_with_material" in asset_audit
    roundtrip = (output / "smoke" / "brender-core-pixelmap-roundtrip.c").read_text(
        encoding="utf-8"
    )
    assert "BrPixelmapSave(" in roundtrip
    assert 'remove(work_path)' in roundtrip
    assert '"match":%s' in roundtrip or 'match' in roundtrip
    resolve = (output / "smoke" / "brender-core-material-resolve.c").read_text(
        encoding="utf-8"
    )
    assert "BrMaterialLoad(" in resolve
    assert ".material = mat;" in resolve
    assert "faces_attached" in resolve
    texfile = (output / "smoke" / "brender-core-texture-file-sample.c").read_text(
        encoding="utf-8"
    )
    assert "BrPixelmapLoad(" in texfile
    assert "resolve_texel_colour(tex, tu, tv)" in texfile
    assert ".map.v[0]" in texfile
    assert "distinct_colours" in texfile
    shell = (output / "smoke" / "brender-core-game-shell.c").read_text(
        encoding="utf-8"
    )
    assert "SHELL_INIT" in shell
    assert "SHELL_LOAD" in shell
    assert "SHELL_RUN" in shell
    assert "SHELL_TEARDOWN" in shell
    assert "shell-frame-%02d.ppm" in shell
    compat = (output / "compat" / "brender-portable-core-stubs.c").read_text(
        encoding="utf-8"
    )
    assert "void BR_RESIDENT_ENTRY _PRO(void)" in compat
    assert "br_uint_16 BR_ASM_CALL _GetSysQual(void)" in compat
    assert "struct br_font BR_ASM_DATA _FontFixed3x5" in compat
    assert "static void copy_source_colour_key0(" in compat
    host_compat = (output / "compat" / "brender-portable-host-stubs.c").read_text(
        encoding="utf-8"
    )
    assert "br_uint_16 _RealSelector = 0;" in host_compat
    assert "br_error BR_RESIDENT_ENTRY HostRealAllocate(" in host_compat
    assert "void BR_ASM_CALL CPUInfo(" in host_compat
    assert "br_error BR_RESIDENT_ENTRY HostInterruptGet(" in host_compat
    assert "void BR_RESIDENT_ENTRY HostFarBlockWrite(" in host_compat
    readme = (output / "README.md").read_text(encoding="utf-8")
    assert "does not vendor BRender source" in readme
    assert "cmake -S . -B build -A Win32" in readme
    assert '"-DBRENDER_SOURCE_DIR=<path-to-public-brender-checkout>"' in readme
    assert "ctest --test-dir build -C Debug --output-on-failure" in readme
    manifest = json.loads((output / "harness-manifest.json").read_text(encoding="utf-8"))
    assert manifest["target_id"] == "brender"
    assert manifest["cmake_platform"] == "Win32"
    assert manifest["core_float_dirs"] == list(CORE_DIRS)
    assert manifest["smoke_target"] == "brender_core_smoke"
    assert manifest["smoke_targets"] == [
        "brender_core_smoke",
        "brender_core_startup_smoke",
        "brender_core_render_smoke",
        "brender_core_scene_smoke",
        "brender_core_fill_smoke",
        "brender_core_depth_smoke",
        "brender_core_texture_smoke",
        "brender_core_model_smoke",
        "brender_core_material_smoke",
        "brender_core_multimodel_smoke",
        "brender_core_gouraud_smoke",
        "brender_core_plotter_smoke",
        "brender_core_asset_audit",
        "brender_core_material_audit",
        "brender_core_material_file_audit",
        "brender_core_pixelmap_roundtrip",
        "brender_core_material_resolve",
        "brender_core_texture_file_sample",
        "brender_core_game_shell",
        "brender_core_host_semantic",
        "brender_core_softrend_render",
    ]
    assert manifest["portable_compat_source"] == "compat/brender-portable-core-stubs.c"
    assert manifest["portable_compat_sources"] == [
        "compat/brender-portable-core-stubs.c",
        "compat/brender-portable-host-stubs.c",
    ]
    assert manifest["source_lists"]["fw"] == ["fw_listed.c"]
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


def test_material_resolve_scanline_edge_initializes_third_edge_w2(tmp_path):
    smoke = _materialized_smoke_sources(tmp_path)

    resolve = (smoke / "brender-core-material-resolve.c").read_text(encoding="utf-8")

    assert "ex[2][0]=x2; ey[2][0]=y2; ew[2][0]=w2;" in resolve
    assert "ex[2][0]=x2; ey[2][0]=y2; ew[2][1]=w2;" not in resolve


def test_pixelmap_roundtrip_rejects_existing_workfile_and_cleans_only_owned_path(tmp_path):
    smoke = _materialized_smoke_sources(tmp_path)

    roundtrip = (smoke / "brender-core-pixelmap-roundtrip.c").read_text(
        encoding="utf-8"
    )

    assert "path_exists(work_path)" in roundtrip
    assert '"workfile-exists"' in roundtrip
    assert "created_workfile = 1;" in roundtrip
    assert "if (created_workfile) remove(work_path);" in roundtrip
    assert (
        roundtrip.replace("if (created_workfile) remove(work_path);", "").find(
            "remove(work_path);"
        )
        == -1
    )


def test_material_file_audit_cleans_only_owned_default_workfile(tmp_path):
    smoke = _materialized_smoke_sources(tmp_path)

    material_file = (smoke / "brender-core-material-file-audit.c").read_text(
        encoding="utf-8"
    )

    assert "path_exists(work_path)" in material_file
    assert '\\"workfile_exists\\":true' in material_file
    assert "created_workfile = 1;" in material_file
    assert "if (created_workfile) remove(work_path);" in material_file
    assert (
        material_file.replace("if (created_workfile) remove(work_path);", "").find(
            "remove(work_path);"
        )
        == -1
    )


def test_generated_receipts_escape_json_string_values(tmp_path):
    smoke = _materialized_smoke_sources(tmp_path)
    generated_sources = [
        smoke / "brender-core-asset-audit.c",
        smoke / "brender-core-material-audit.c",
        smoke / "brender-core-material-file-audit.c",
        smoke / "brender-core-pixelmap-roundtrip.c",
        smoke / "brender-core-material-resolve.c",
        smoke / "brender-core-texture-file-sample.c",
        smoke / "brender-core-game-shell.c",
        smoke / "brender-core-softrend-render.c",
    ]
    unsafe_receipt_formats = [
        '{"model":"%s"',
        '{"file":"%s"',
        '{"asset":"%s"',
        ',"id":"%s"',
        ',"roundtrip":"%s"',
        ',"material":"%s"',
        ',"material_id":"%s"',
        ',"texture":"%s"',
        ',"palette":"%s"',
    ]

    for path in generated_sources:
        source = path.read_text(encoding="utf-8")
        assert "static void json_write_string" in source
        assert "case '\"':" in source
        assert "case '\\\\':" in source
        assert "ch < 0x20" in source
        assert "\\\\u%04x" in source
        for unsafe_format in unsafe_receipt_formats:
            assert unsafe_format not in source


def test_texture_rungs_resolve_indexed_texels_through_loaded_palette(tmp_path):
    smoke = _materialized_smoke_sources(tmp_path)
    texture_sources = [
        smoke / "brender-core-texture-file-sample.c",
        smoke / "brender-core-game-shell.c",
    ]

    for path in texture_sources:
        source = path.read_text(encoding="utf-8")
        assert "resolve_texel_colour(tex, tu, tv)" in source
        assert "case BR_PMT_INDEX_8:" in source
        assert "if (tex->map != NULL)" in source
        assert "BrPixelmapPixelGet(tex->map, 0, texel)" in source
        assert (
            "texel = BrPixelmapPixelGet(tex, tu, tv);\n"
            "                r  = (int)(((texel >> 16) & 0xff) * shade);"
        ) not in source


def test_host_semantic_rejects_existing_workfile_and_cleans_only_owned_path(tmp_path):
    smoke = _materialized_smoke_sources(tmp_path)

    host = (smoke / "brender-core-host-semantic.c").read_text(encoding="utf-8")

    assert "path_exists(work_path)" in host
    assert '\\"workfile_exists\\":true' in host
    assert "created_workfile = 1;" in host
    assert "if (created_workfile) remove(work_path);" in host
    assert (
        host.replace("if (created_workfile) remove(work_path);", "").find(
            "remove(work_path);"
        )
        == -1
    )


def test_softrend_and_tia_sources_omit_investigation_diagnostics(tmp_path):
    harness = _materialized_harness(tmp_path)

    softrend = (harness / "smoke" / "brender-core-softrend-render.c").read_text(
        encoding="utf-8"
    )
    pentprim = (harness / "compat" / "brender-pentprim-c-port.c").read_text(
        encoding="utf-8"
    )

    for diagnostic in ["PLIB[", "MARK af-precomputed", "STATECHK"]:
        assert diagnostic not in softrend
    for diagnostic in ["TIA tw=", "pal[0]=", "TIADBG"]:
        assert diagnostic not in pentprim


def test_softrend_period_pipeline_emits_eight_orbit_frames(tmp_path):
    smoke = _materialized_smoke_sources(tmp_path)
    softrend = (smoke / "brender-core-softrend-render.c").read_text(
        encoding="utf-8"
    )

    assert "for (frame = 0; frame < 8; frame++)" in softrend
    assert '\\"frames\\":8' in softrend
    assert 'frame * 45' in softrend


def test_softrend_period_pipeline_clears_colour_and_depth_each_orbit_frame(tmp_path):
    smoke = _materialized_smoke_sources(tmp_path)
    softrend = (smoke / "brender-core-softrend-render.c").read_text(
        encoding="utf-8"
    )
    render_loop = softrend.split("for (frame = 0; frame < 8; frame++)", 1)[1]

    assert "static void clear_pixelmap_storage" in softrend
    assert "clear_pixelmap_storage(pm);" in render_loop
    # far is 0xFFFF: tt24_piz.asm draws where z < *zbuf (corrected 2026-10-16)
    assert "BrPixelmapFill(depth, 0xFFFFFFFF);" in render_loop
    assert "BrPixelmapFill(depth, 0);" not in softrend
    assert render_loop.index("clear_pixelmap_storage(pm);") < render_loop.index(
        "BrZbSceneRender(world, camera_actor, pm, depth);"
    )
    assert render_loop.index("BrPixelmapFill(depth, 0xFFFFFFFF);") < render_loop.index(
        "BrZbSceneRender(world, camera_actor, pm, depth);"
    )
    assert "BrZbSceneRenderContinue(" not in render_loop


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
    assert (output / "smoke" / "brender-core-smoke.c").exists()
    assert (output / "smoke" / "brender-core-startup-smoke.c").exists()
    assert (output / "smoke" / "brender-core-render-smoke.c").exists()
    assert (output / "smoke" / "brender-core-scene-smoke.c").exists()
    assert (output / "smoke" / "brender-core-fill-smoke.c").exists()


def test_softrend_rung_fails_ctest_when_its_render_is_invalid(tmp_path):
    """Before 2026-10-16 main fell off its end and returned 0 whatever the
    receipt said, so CTest passed an empty frame."""
    smoke = _materialized_smoke_sources(tmp_path)
    softrend = (smoke / "brender-core-softrend-render.c").read_text(encoding="utf-8")
    assert "return ok ? 0 : 13;" in softrend


def test_pentprim_port_uses_the_real_component_slots():
    """The port once redefined C_SX and friends as 0..8 with #ifndef, which
    shadows priminfo.h's enum (C_SX is 5) and read the wrong slots."""
    for name in ("brender-pentprim-c-port.c", "brender-pentprim-tt-port.c"):
        port = _compat_port(name)
        assert "#define C_SX" not in port and "#ifndef C_SX" not in port, name
    port = _compat_port("brender-pentprim-tt-port.c")
    assert "static void tt_triangle(" in port
    for name in ("TriangleRenderPIZ2_RGB_888", "TriangleRenderPIZ2I_RGB_888"):
        assert port.count(f"TT_KERNEL({name},") == 1, name
    # the depth test of tt24_piz.asm: draw where the new z is below the stored one
    assert "if (!(z16 < zbuf)) return;" in port


def test_awtmz_setup_is_compiled_and_its_trapezoids_are_ported(tmp_path):
    """Argonaut's C triangle setup for the textured mappers (awtmz.c) builds
    with the harness; the port supplies the t_piza.asm trapezoid loops."""
    port = _compat_port("brender-pentprim-awsl-port.c")
    for name in ("TrapezoidRenderPIZ2TA24", "TrapezoidRenderPIZ2TIA_RGB_888"):
        assert port.count(f"void BR_ASM_CALL {name}(void)") == 1, name
    # awtmz.c defines these triangle wrappers now; no port may
    for source in (port, _compat_port("brender-pentprim-c-port.c")):
        for name in ("TriangleRenderPIZ2TA24(", "TriangleRenderPIZ2TIA_RGB_888(", "TriangleRenderPIZ2TA15("):
            assert name not in source, name
    # the forward and backward depth tests of t_piza.asm share one check: draw where z <= *zbuf
    assert port.count("return zbuf >= z16;") == 1 and port.count("awsl_depth_passes(k, ebp)") == 2


def test_safe_fixed_mac2_div_is_the_safediv_asm_sum():
    port = (Path(__file__).resolve().parents[1] / "src" / "engine_revival" / "brender_compat"
            / "brender-pentprim-c-port.c").read_text(encoding="utf-8")
    body = port[port.index("br_int_32 SafeFixedMac2Div("):port.index("/* RasteriseBufferDisable and the rest of rastbuff.asm")]
    assert "(long long)a * b + (long long)c * d" in body
    assert "<< 16" not in body


def test_softrend_rung_requires_a_textured_frame(tmp_path):
    smoke = _materialized_smoke_sources(tmp_path)
    softrend = (smoke / "brender-core-softrend-render.c").read_text(encoding="utf-8")
    assert "int ok = (lit > 500) && (chroma > 500);" in softrend
    assert softrend.index("BrMapUpdate(tex, BR_MAPU_ALL);") < softrend.index("BrMaterialUpdate(material, BR_MATU_ALL);")
    assert "expand_texture(tex," in softrend


def _compat_port(name: str) -> str:
    return (Path(__file__).resolve().parents[1] / "src" / "engine_revival" / "brender_compat" / name).read_text(
        encoding="utf-8")


def test_index8_kernels_are_ported_once_and_not_stubbed():
    """The 8-bit kernels of zb8.asm and fti8_piz.asm live in the i8 port; the
    diagnostic stubs for them are gone from the main port."""
    i8, main = _compat_port("brender-pentprim-i8-port.c"), _compat_port("brender-pentprim-c-port.c")
    for name in ("TriangleRender_Z_I8_D16", "TriangleRender_ZI_I8_D16", "TriangleRender_ZT_I8_D16_256",
                 "TriangleRender_ZTI_I8_D16_256"):
        assert i8.count(f"void BR_ASM_CALL {name}(") == 1, name
        assert f"void BR_ASM_CALL {name}(" not in main, name
    # zb8.asm compares the z step with the decimal 80000000; fti8_piz.asm with 80000000h
    assert "if (dzx >= 80000000u) dzx -= 1u;" in i8
    assert "if (g >= 0x80000000u) g -= 1u;" in i8
    # a zero texel is transparent in both textured kernels
    assert i8.count("texel != 0") == 2


def test_perspective_kernels_are_ported_once_and_not_stubbed():
    """fti8pizp.asm's perspective kernels live in the i8p port (256 here; the
    other sizes in the next test); their diagnostic stubs are gone and the
    cheat case jumps into the linear ports."""
    i8p, i8 = _compat_port("brender-pentprim-i8p-port.c"), _compat_port("brender-pentprim-i8-port.c")
    main = _compat_port("brender-pentprim-c-port.c")
    assert i8p.count("P8_KERNELS(256, 8)") == 1
    for name in ("TriangleRender_ZPT_I8_D16_256", "TriangleRender_ZPTI_I8_D16_256"):
        assert f"void BR_ASM_CALL {name}(" not in main, name
    # the flat perspective kernel (ZPTI_256_FLAT) is ported too
    assert "void BR_ASM_CALL TriangleRender_ZPTI_I8_D16_256_FLAT(" not in main
    assert "P8_KERNEL(TriangleRender_ZPTI_I8_D16_##n##_FLAT, 2, pow2)" in i8p
    # cheat case: jc TriangleRasterise_ZT_I8_D16_256 / TriangleRasterise_ZTI_I8_D16_256
    assert "if (pow2 == 8) { if (lit) i8_rasterise_zti_256(); else i8_rasterise_zt_256(); }" in i8p
    assert i8.count("void i8_rasterise_zt_256(void)") == 1
    assert i8.count("void i8_rasterise_zti_256(void)") == 1
    # inc al / inc ah (256), or the shifted forms: u and v wrap at the map size
    assert "uint32_t m = (1u << p8.pow2) - 1u, u = *src & m, v = (*src >> p8.pow2) & m" in i8p
    # five scanline loops: udirn and vdirn of d, i or b
    assert "enum { P8_D, P8_I, P8_B };" in i8p
    # the major edge keeps its integer in the low word and adds the carry back
    assert "p8.main_i += p8.main_d_i + carry;" in i8p


def test_hicolor_kernels_are_ported_once_and_not_stubbed():
    """The 15 and 16-bit kernels (tt15_piz.asm, tt16_piz.asm, zb16p2ul.asm and
    t_piza.asm's DBPP 2 instances) live in their own ports, beside the 24-bit
    ones they share code with; no diagnostic stub for them is left."""
    tt, awsl = _compat_port("brender-pentprim-tt-port.c"), _compat_port("brender-pentprim-awsl-port.c")
    zb16, main = _compat_port("brender-pentprim-zb16-port.c"), _compat_port("brender-pentprim-c-port.c")
    for fmt in ("888", "555", "565"):
        for name in (f"TriangleRenderPIZ2_RGB_{fmt}", f"TriangleRenderPIZ2I_RGB_{fmt}"):
            assert f"TT_KERNEL({name}," in tt, name
            assert f"void BR_ASM_CALL {name}(" not in main, name
    for size in (32, 64, 128, 256, 1024):
        assert f"ZB16_KERNEL({size}," in zb16, size
        assert f"TriangleRender_ZT_RGB565_D16_{size}(" not in main, size
    for name in ("TrapezoidRenderPIZ2TA24", "TrapezoidRenderPIZ2TIA_RGB_888", "TrapezoidRenderPIZ2TA15",
                 "TrapezoidRenderPIZ2TIA_RGB_555"):
        assert awsl.count(f"void BR_ASM_CALL {name}(void)") == 1, name
        assert f"void BR_ASM_CALL {name}(" not in main, name
    # tt15/tt16 GDIVIDE gives 0 on overflow; tt24 keeps the low dword
    assert "return zero_on_overflow ? 0 : (int32_t)(uint32_t)m;" in tt
    # the colour words: 555 and 565 masks as the assembly builds them
    assert "((g & 0xfc0000u) >> 13)" in tt and "((g & 0xf80000u) >> 14)" in tt
    # the DBPP 2 shade table entry is two bytes; the DBPP 3 one is read at four
    assert "index * (k->dbpp == 2 ? 2u : 4u)" in awsl
    # zb16p2ul.asm: the decimal 80000000, z drawn where z <= *zptr, zero texels transparent
    assert "if (fps.d_z_x >= 80000000u) fps.d_z_x -= 1u;" in zb16
    assert "if (z16 > zbuf) return;" in zb16 and "if (texel == 0) return;" in zb16


def test_pfpsetup_port_keeps_the_assembly_formats():
    setup = _compat_port("brender-pentprim-pfpsetup-port.c")
    assert "#define FP_CONV_D 6755399441055744.0" in setup  # 1.5 * 2^52
    assert 6755399441055744.0 == 1.5 * 2 ** 52
    # 2^28 / maxuv from the float's bits, after rounding maxuv up to a power of two
    assert "e += (uint32_t)(127 * 2 + 28) << 23;" in setup
    # q0, q1, q2 and maxuv are stored as singles
    assert "d->q0 = (float)q;" in setup and "maxuv = (float)mx;" in setup
    # cheat when w_range * s_range < 4 * w_min, compared as single bits
    assert "return pfps_bits(wr_sr) < pfps_bits(wmin_4);" in setup
    assert "#pragma fp_contract(off)" in setup


def test_fpsetup_port_keeps_the_assembly_conversions():
    setup = _compat_port("brender-pentprim-fpsetup-port.c")
    assert "#define FP_CONV_D16 103079215104.0" in setup  # 1.5 * 2^36
    assert 103079215104.0 == 1.5 * 2 ** 36
    # the overlapping sort tables, read as one array, and the parity flip table
    assert "{1, 2, 0, 0, 0, 0, 2, 1, 0, 1, 2, 0, 0, 0}" in setup
    assert "#pragma fp_contract(off)" in setup


def test_index8_ports_are_in_the_pentprim_library(tmp_path):
    harness = _materialized_harness(tmp_path)
    cmake = (harness / "cmake" / "brender-pentprim.cmake").read_text(encoding="utf-8")
    for name in ("brender-pentprim-fpsetup-port.c", "brender-pentprim-i8-port.c",
                 "brender-pentprim-pfpsetup-port.c", "brender-pentprim-i8p-port.c",
                 "brender-pentprim-tt-port.c", "brender-pentprim-awsl-port.c", "brender-pentprim-zb16-port.c"):
        assert f"compat/{name}" in cmake, name
    assert (harness / "compat" / "brender-pentprim-fpsetup-port.h").is_file()
    assert (harness / "compat" / "brender-pentprim-pfpsetup-port.h").is_file()


def test_mmx_family_is_ported_and_behind_an_explicit_option(tmp_path):
    """pentprim's MMX 15/16-bit table (gsetuptf.asm, rastbuff.asm and the
    xz*.inc kernels) has a C port in the pentprim library; the harness only
    reaches it with BRENDER_PENTPRIM_MMX, which is off by default."""
    harness = _materialized_harness(tmp_path)
    cmake = (harness / "cmake" / "brender-pentprim.cmake").read_text(encoding="utf-8")
    for name in ("brender-pentprim-mmx-tables-port.c", "brender-pentprim-mmx-common-port.c",
                 "brender-pentprim-mmx-uv-port.c", "brender-pentprim-mmx-setup-port.c",
                 "brender-pentprim-mmx-zrgb-port.c", "brender-pentprim-mmx-zuv-port.c",
                 "brender-pentprim-mmx-zuvc-port.c", "brender-pentprim-mmx-zuvrgb-port.c"):
        assert f"compat/{name}" in cmake, name
    assert (harness / "compat" / "brender-pentprim-mmx-port.h").is_file()
    assert 'option(BRENDER_PENTPRIM_MMX "' in cmake and '" OFF)' in cmake
    option = cmake[cmake.index("if(BRENDER_PENTPRIM_MMX)"):]
    assert "target_compile_definitions(brender_pentprim_float PRIVATE USE_MMX=1)" in option
    assert "BRENDER_HOST_MMX=1" in option
    host = (harness / "compat" / "brender-portable-host-stubs.c").read_text(encoding="utf-8")
    assert "#if BRENDER_HOST_MMX" in host and "*features = HOST_CAPS_MMX;" in host
    # every MMX rasteriser entry point, 555 and 565, plain, D, S and SD, LR and RL
    ports = {k: _compat_port(f"brender-pentprim-mmx-{k}-port.c") for k in ("zrgb", "zuv", "zuvc", "zuvrgb")}
    for kernel, family in (("zrgb", "Z_RGB"), ("zuv", "Z_UV"), ("zuvc", "Z_UV_C"), ("zuvrgb", "Z_UV_RGB")):
        for fmt in ("555", "565"):
            for variant in ("", "_D", "_S", "_SD"):
                assert f"_PAIR(RasteriseMMX{fmt}_{family}{variant}," in ports[kernel], (fmt, family, variant)
    setup = _compat_port("brender-pentprim-mmx-setup-port.c")
    for name in ("ZRGBUVS", "ZRGBUV", "ZRGB", "Z", "ZRGBS", "ZUVC", "ZUV", "ZC"):
        assert f"void BR_ASM_CALL GenericSetupFloat{name}_A(void)" in setup, name
    assert "void BR_ASM_CALL GenericSetupTriangleFloat_A(" in setup
    # rastbuff.asm: 40 qwords of leeway under 200 qwords of stack, drawn top first
    assert "#define RB_LEEWAY (40 * 8)" in setup and "#define RB_OFF (RB_LEEWAY + 200 * 8)" in setup
    assert "void BR_ASM_CALL RasteriseBufferDisable(void)" in setup
    assert "void RasteriseBufferDisable" not in _compat_port("brender-pentprim-c-port.c")


def test_mmx_port_keeps_the_assembly_details():
    """Details kept as found in the 1998 assembly."""
    common = _compat_port("brender-pentprim-mmx-common-port.c") + _compat_port("brender-pentprim-mmx-tables-port.c")
    header = _compat_port("brender-pentprim-mmx-port.h")
    zuv = _compat_port("brender-pentprim-mmx-zuv-port.c")
    zuvc = _compat_port("brender-pentprim-mmx-zuvc-port.c")
    # the saturating instructions saturate
    assert "mmx_usat16" in header and "mmx_ssat16" in header and "mmx_ssat8" in header
    # rastrise.asm's dither squares and right_masks/left_masks
    assert "0x07ff04ff037f007full" in common and "0xffff000000000000ull" in common
    # the 16-bit dithered many-word span leaves green unmasked (xzuv1.inc tests COLOUR_TYPE_15 twice)
    assert "if (k->c15) g = mmx_pand(mmx_pand(g, mmx_mask_5), mmx_mask_6);" in zuv
    # the constant-colour modulate dithers with paddw (wrapping), the plain map with paddusw
    assert "b = mmx_paddw(b, mmx_dither_table_11[row]);" in zuvc
    assert "r = mmx_paddusw(r, mmx_dither_table_11[row & 3]);" in zuv
    # the carry from bit 18 of xm picks the dy1 step
    assert "w->carry = (edx >> 18) & 1u ? -1 : 0;" in common


def test_zsorted_and_tip_kernels_are_ported_once_and_not_stubbed(tmp_path):
    """The z-sorted kernels (tt15/tt16/tt24_pi.asm, t_pia.asm's true colour
    instances) share their ports with the z-buffered ones; the TIP256
    perspective scanlines (t15_pizp.asm, t15_pip.asm) have their own; none of
    them is left a stub, and the harness compiles the TIP port."""
    tt, awsl, tip = (_compat_port("brender-pentprim-tt-port.c"), _compat_port("brender-pentprim-awsl-port.c"),
                     _compat_port("brender-pentprim-tip-port.c"))
    main = _compat_port("brender-pentprim-c-port.c")
    for fmt in ("888", "555", "565"):
        for name in (f"TriangleRenderPI_RGB_{fmt}", f"TriangleRenderPII_RGB_{fmt}"):
            assert f"TT_KERNEL({name}," in tt, name
            assert f"void BR_ASM_CALL {name}(" not in main, name
    for name in ("TrapezoidRenderPITA24", "TrapezoidRenderPITIA_RGB_888", "TrapezoidRenderPITA15",
                 "TrapezoidRenderPITIA_RGB_555"):
        assert awsl.count(f"void BR_ASM_CALL {name}(void)") == 1, name
        assert f"void BR_ASM_CALL {name}(" not in main, name
    for name in ("ScanLinePIZ2TIP256_RGB_555", "ScanLinePIZ2TIPB256_RGB_555", "ScanLinePIZ2TIPB256_RGB_565",
                 "ScanLinePITIP256_RGB_555", "ScanLinePITIPB256_RGB_555", "ScanLinePITIPB256_RGB_565"):
        assert tip.count(f"void BR_ASM_CALL {name}(void)") == 1, name
        assert f"void BR_ASM_CALL {name}(" not in main, name
    # the z-buffered blended form tests depth but does not store it; the halving masks
    assert "} else if (k->zbuffered) {" in tip and "0x3defu" in tip and "0xf7deu" in tip
    # the unlit z-sorted trapezoids still write work.pi.current, as t_pia.asm does
    assert "else if (!k->zbuffered) /* t_pia.asm's unconditional add ebx,ecx ; mov work.pi.current,ebx */" in awsl
    harness = _materialized_harness(tmp_path)
    assert "compat/brender-pentprim-tip-port.c" in (harness / "cmake" / "brender-pentprim.cmake").read_text(encoding="utf-8")


def test_softrend_subdivision_is_ported_not_stubbed(tmp_path):
    """subdiv.asm's three FLOAT routines: softrend subdivides blocks that ask
    for it (the MMX table's perspective rows), so they must do the work."""
    sub = _compat_port("brender-softrend-subdiv-port.c")
    for name in ("br_boolean BR_ASM_CALL subdivideCheck(", "void BR_ASM_CALL averageVerticesOnScreen(",
                 "void BR_ASM_CALL averageVertices("):
        assert sub.count(name) == 1, name
    assert "return BR_FALSE;\n}" not in sub.split("subdivideCheck(")[1][:400], "subdivideCheck is no stub"
    # seven parameters, as subdiv.asm's FLOAT procs and faceops.c's FLOAT declarations have
    assert "brp_vertex *m0, brp_vertex *m1, brp_vertex *m2,\n                                 brp_vertex *v0, brp_vertex *v1, brp_vertex *v2)" in sub
    assert "const uint32_t table[16] = {G, G, 0, 0, G, G, GL, 0, 0, L, 0, 0, GL, L, GL, 0};" in sub
    harness = _materialized_harness(tmp_path)
    cmake = (harness / "cmake" / "brender-softrend.cmake").read_text(encoding="utf-8")
    assert "compat/brender-softrend-subdiv-port.c" in cmake and "float-fallbacks" not in cmake


def test_eight_bit_kernels_for_the_other_map_sizes_are_ported_not_stubbed(tmp_path):
    """fti8pizp.asm's 32, 64, 128 and 1024 instances share the 256 port;
    zb8p2unl.asm and zb8p2lit.asm (8 to 1024, not 256) have their own, which
    the perspective kernels' cheat case jumps into. None is left a stub."""
    i8p, pow2 = _compat_port("brender-pentprim-i8p-port.c"), _compat_port("brender-pentprim-i8pow2-port.c")
    main = _compat_port("brender-pentprim-c-port.c")
    for n, k in ((32, 5), (64, 6), (128, 7), (256, 8), (1024, 10)):
        assert f"P8_KERNELS({n}, {k})" in i8p, n
    for n, k in ((8, 3), (16, 4), (32, 5), (64, 6), (128, 7), (1024, 10)):
        assert f"P2_KERNELS({n}, {k})" in pow2, n
    for name in [f"ZPT{i}_I8_D16_{n}" for n in (32, 64, 128, 1024) for i in ("", "I")] + \
                [f"ZT{i}_I8_D16_{n}" for n in (8, 16, 32, 64, 128, 1024) for i in ("", "I")]:
        assert f"void BR_ASM_CALL TriangleRender_{name}(" not in main, name
    # the FLAT kernels are ported (engine-revival 0.12.0), not stubs
    for name in [f"ZT_I8_D16_{n}_FLAT" for n in (8, 16, 32, 64, 128, 256, 1024)] + \
                [f"ZPTI_I8_D16_{n}_FLAT" for n in (32, 64, 128, 256, 1024)]:
        assert f"void BR_ASM_CALL TriangleRender_{name}(" not in main, name
    assert "P2_KERNEL(TriangleRender_ZT_I8_D16_256_FLAT, P2_FLAT, 8)" in pow2
    # the cheat case of the other sizes goes to the power-of-two rasterisers
    assert "else if (lit) i8_rasterise_zti_pow2(pow2);" in i8p
    # kept as found: the decimal z gradient constant, and the lit kernel's whole-eax shade lookup
    assert "edx = edx < 80000000u ? edx : edx - 1u;" in pow2
    assert "uint32_t row = (at & 0xffff0000u) | ((i >> 16) & 0xffu) << 8 | texel;" in pow2
    harness = _materialized_harness(tmp_path)
    assert "compat/brender-pentprim-i8pow2-port.c" in (harness / "cmake" / "brender-pentprim.cmake").read_text(encoding="utf-8")


def test_the_flat_lit_kernels_are_ported_not_stubbed():
    """zb8p2fla.asm's linear ZT_*_FLAT (8 to 1024) and fti8pizp.asm's
    perspective ZPTI_*_FLAT (32 to 1024): flat lighting is a constant shade
    row for the face. The linear kernel masks the texel to a byte, so it does
    not read past the table the way the smooth 1024 kernel does."""
    pow2, i8p = _compat_port("brender-pentprim-i8pow2-port.c"), _compat_port("brender-pentprim-i8p-port.c")
    main = _compat_port("brender-pentprim-c-port.c")
    for name in [f"ZT_I8_D16_{n}_FLAT" for n in (8, 16, 32, 64, 128, 256, 1024)]:
        assert f"void BR_ASM_CALL TriangleRender_{name}(" not in main, name
    for name in [f"ZPTI_I8_D16_{n}_FLAT" for n in (32, 64, 128, 256, 1024)]:
        assert f"void BR_ASM_CALL TriangleRender_{name}(" not in main, name
    # the flat shade row is fixed for the face, from v0's intensity
    assert "void fps_flat_shade(union brp_vertex *v0)" in _compat_port("brender-pentprim-fpsetup-port.c")
    # the flat kernel masks the texel to a byte, so no overread
    assert "else if (mode == P2_FLAT) { /* and eax,0ffh ; mov al,[shade_row+eax] */" in pow2
    assert "c = t->shade_row[texel];" in pow2
    # the perspective flat intensity is a constant from v0 (SETUP_FLOAT leaves ebx = v0)
    assert "else if (lit == 2) { fps_const_i(v0); }" in _compat_port("brender-pentprim-pfpsetup-port.c")
