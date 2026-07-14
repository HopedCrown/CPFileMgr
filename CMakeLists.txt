
# Configure with [fxsdk build-fx] or [fxsdk build-cg], which provide the
# toolchain file and module path of the fxSDK

cmake_minimum_required(VERSION 3.15)
project(CPFileMgr LANGUAGES C CXX)

include(GenerateG1A)
include(GenerateG3A)
include(GenerateHH3)
include(Fxconv)
find_package(Gint 2.9.0 REQUIRED)
find_package(JustUI 1.4.0 REQUIRED)

file(GLOB_RECURSE SOURCES
  src/*.c
  src/*.cpp
)

# Shared assets, fx-9860G-only assets and fx-CG-50-only assets
set(ASSETS
  # ...
)
set(ASSETS_fx
  assets-fx/example.png
  # ...
)
set(ASSETS_cg
  assets-cg/example.png
  # ...
)

fxconv_declare_assets(${ASSETS} ${ASSETS_fx} ${ASSETS_cg} WITH_METADATA)

add_executable(CPFileMgr ${SOURCES} ${ASSETS} ${ASSETS_${FXSDK_PLATFORM}})
target_compile_options(CPFileMgr PRIVATE -Wall -Wextra -Os -g -flto=auto )
target_link_libraries(CPFileMgr JustUI::JustUI Gint::Gint)
target_include_directories(CPFileMgr PRIVATE include)

if("${FXSDK_PLATFORM_LONG}" STREQUAL fx9860G)
  generate_g1a(TARGET CPFileMgr OUTPUT "CPFileMgr.g1a"
    NAME "CPFileMgr" ICON assets-fx/icon.png)
elseif("${FXSDK_PLATFORM_LONG}" STREQUAL fxCG50)
  generate_g3a(TARGET CPFileMgr OUTPUT "CPFileMgr.g3a"
    NAME "CPFileMgr" ICONS assets-cg/icon-uns.png assets-cg/icon-sel.png)
elseif("${FXSDK_PLATFORM_LONG}" STREQUAL fx9860G_G3A)
  generate_g3a(TARGET CPFileMgr OUTPUT "CPFileMgr-fx.g3a"
    NAME "CPFileMgr-fx" ICONS assets-cg/icon-uns.png assets-cg/icon-sel.png)
elseif("${FXSDK_PLATFORM_LONG}" STREQUAL fxCP)
  generate_hh3(TARGET CPFileMgr OUTPUT "CPFileMgr.hh3")
endif()
