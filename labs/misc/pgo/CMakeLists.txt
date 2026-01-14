cmake_minimum_required(VERSION 3.3)

project(lab)
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
include_directories(lua)
file(GLOB EXT_LAB_srcs lua/*.c)
file(GLOB EXT_VALIDATE_srcs lua/*.c)


option(PGO_BUILD "PGO build mode: OFF(normal), ON(generate), USE(optimize)" ON)

set(PGO_BUILD USE)

if(PGO_BUILD STREQUAL "ON")
    message(STATUS ">>> CMake PGO: 开启 插桩模式 (Generate Profile)")
    # 插桩参数 -fprofile-generate 必须加到 编译+链接 全局参数
    add_compile_options(-fprofile-generate)
    add_link_options(-fprofile-generate)
# ========== PGO 优化编译配置 (阶段3：使用Profile数据优化) ==========
elseif(PGO_BUILD STREQUAL "USE")
    message(STATUS ">>> CMake PGO: 开启 优化模式 (Use Profile)")
    add_compile_options(-fprofile-use=${CMAKE_SOURCE_DIR}/app.profdata)
    add_link_options(-fprofile-use=${CMAKE_SOURCE_DIR}/app.profdata)
endif()

set(VALIDATE_ARGS "${CMAKE_CURRENT_SOURCE_DIR}/reference_output.txt")

string(REGEX MATCH "^(.*)[\\/]labs[\\/].*$" repo "${CMAKE_CURRENT_SOURCE_DIR}")
include(${CMAKE_MATCH_1}/tools/labs.cmake)
