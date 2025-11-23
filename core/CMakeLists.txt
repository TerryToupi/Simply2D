cmake_minimum_required(VERSION 3.20)

project(Simply2D_core LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --------------------------------------------------
# Options mirroring rpmalloc + EASTL
# --------------------------------------------------
option(RPMALLOC_ENABLE_STATISTICS "Enable rpmalloc statistics" ON)
option(RPMALLOC_ENABLE_ASSERTS    "Enable rpmalloc asserts" OFF)

option(EASTL_STD_ITERATOR_CATEGORY_ENABLED      "Enable compatibility with std:: iterator categories" OFF)
option(EASTL_DISABLE_APRIL_2024_DEPRECATIONS    "Disable April 2024 deprecations (keep old API)" OFF)
option(EASTL_DISABLE_SEPT_2024_DEPRECATIONS     "Disable Sept 2024 deprecations (keep old API)" OFF)
option(EASTL_DISABLE_APRIL_2025_DEPRECATIONS    "Disable April 2025 deprecations (keep old API)" OFF)

# --------------------------------------------------
# Paths
# --------------------------------------------------
set(CORE_FOLDER       ${CMAKE_CURRENT_SOURCE_DIR})
set(THIRDPARTY_FOLDER ${CORE_FOLDER}/ThirdParty)

set(EABASE_ROOT   "${THIRDPARTY_FOLDER}/EABase")
set(EASTL_ROOT    "${THIRDPARTY_FOLDER}/EASTL")
set(RPMALLOC_ROOT "${THIRDPARTY_FOLDER}/rpmalloc")

# --------------------------------------------------
# EASTL compiler flags (CommonCppFlags.cmake)
# This is exactly what EASTL's own CMake does.
# --------------------------------------------------
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${EASTL_ROOT}/scripts/CMake")
include(CommonCppFlags)

# From here on, any targets we create in this directory will see those flags.

# --------------------------------------------------
# Core source files (exclude ThirdParty)
# --------------------------------------------------
file(GLOB_RECURSE CORE_FILES CONFIGURE_DEPENDS
    "${CORE_FOLDER}/*.h"
    "${CORE_FOLDER}/*.hpp"
    "${CORE_FOLDER}/*.cpp"
)

list(FILTER CORE_FILES EXCLUDE REGEX ".*/ThirdParty/.*")

# --------------------------------------------------
# EABase (headers only)
# --------------------------------------------------
file(GLOB_RECURSE EABASE_HEADERS CONFIGURE_DEPENDS
    "${EABASE_ROOT}/include/Common/*.h"
)

set(EABASE_INCLUDE_DIR "${EABASE_ROOT}/include/Common")

# --------------------------------------------------
# EASTL sources + headers
# --------------------------------------------------
file(GLOB EASTL_SOURCES CONFIGURE_DEPENDS
    "${EASTL_ROOT}/source/*.cpp"
)
file(GLOB_RECURSE EASTL_HEADERS CONFIGURE_DEPENDS
    "${EASTL_ROOT}/include/EASTL/*.h"
)

set(EASTL_INCLUDE_DIR "${EASTL_ROOT}/include")

# --------------------------------------------------
# rpmalloc (C sources + include)
# Mirrors their official CMake layout: rpmalloc/rpmalloc.c
# --------------------------------------------------
file(GLOB RPMALLOC_HEADERS CONFIGURE_DEPENDS
    "${RPMALLOC_ROOT}/rpmalloc/*.h"
)
file(GLOB RPMALLOC_SOURCES CONFIGURE_DEPENDS
    "${RPMALLOC_ROOT}/rpmalloc/*.c"
)

set(RPMALLOC_INCLUDE_DIR "${RPMALLOC_ROOT}/rpmalloc")

# --------------------------------------------------
# Single static library with EVERYTHING
# --------------------------------------------------
add_library(${PROJECT_NAME} 
    STATIC
        ${CORE_FILES}
        ${EABASE_HEADERS}
        ${EASTL_SOURCES}
        ${EASTL_HEADERS} 
        ${RPMALLOC_SOURCES}
        ${RPMALLOC_HEADERS}
)

add_library(Simply2D::Core ALIAS ${PROJECT_NAME})

target_compile_features(${PROJECT_NAME}
    PUBLIC
        cxx_std_20
        c_std_11 
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        "${CORE_FOLDER}/Include"
        "${CORE_FOLDER}/Public"

        # ThirdParty
        "${EABASE_INCLUDE_DIR}"
        "${EASTL_INCLUDE_DIR}"
        "${RPMALLOC_INCLUDE_DIR}"

    PRIVATE
        "${CORE_FOLDER}"
)

# ------------------------------------------------------------------------------------------
# Compiler flags
# ------------------------------------------------------------------------------------------
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/EASTL/scripts/CMake")
include(CommonCppFlags)

# --------------------------------------------------
# Core / EASTL / EABase defines
# (mirroring EASTL + EABase CMake)
# --------------------------------------------------
target_compile_definitions(${PROJECT_NAME}
    PUBLIC
        -D_CHAR16T
        -D_CRT_SECURE_NO_WARNINGS
        -D_SCL_SECURE_NO_WARNINGS
        -DEASTL_OPENSOURCE=1
        -DRPMALLOC_CONFIGURABLE=1
        -DEASTL_USER_DEFINED_ALLOCATOR=1
        -DEASTL_EXCEPTIONS_ENABLED=0
)

if(EASTL_STD_ITERATOR_CATEGORY_ENABLED)
    target_compile_definitions(${PROJECT_NAME} PUBLIC EASTL_STD_ITERATOR_CATEGORY_ENABLED=1)
endif()

if(EASTL_DISABLE_APRIL_2024_DEPRECATIONS)
    target_compile_definitions(${PROJECT_NAME} PUBLIC EA_DEPRECATIONS_FOR_2024_APRIL=EA_DISABLED)
endif()
if(EASTL_DISABLE_SEPT_2024_DEPRECATIONS)
    target_compile_definitions(${PROJECT_NAME} PUBLIC EA_DEPRECATIONS_FOR_2024_SEPT=EA_DISABLED)
endif()
if(EASTL_DISABLE_APRIL_2025_DEPRECATIONS)
    target_compile_definitions(${PROJECT_NAME} PUBLIC EA_DEPRECATIONS_FOR_2025_APRIL=EA_DISABLED)
endif()

if(RPMALLOC_ENABLE_STATISTICS)
    target_compile_definitions(${PROJECT_NAME} PUBLIC ENABLE_STATISTICS=1)
else()
    target_compile_definitions(${PROJECT_NAME} PUBLIC ENABLE_STATISTICS=0)
endif()

if(RPMALLOC_ENABLE_ASSERTS)
    target_compile_definitions(${PROJECT_NAME} PUBLIC ENABLE_ASSERTS=1)
endif()

if(MSVC)
    # same as rpmalloc CMake: disable half-baked C11 atomics
    target_compile_definitions(${PROJECT_NAME} PRIVATE RPMALLOC_NO_C11_ATOMICS=1)
endif()

# --------------------------------------------------
# EASTL NatVis (for MSVC)
# --------------------------------------------------
if(MSVC)
    set(EASTL_NATVIS_FILE "${EASTL_ROOT}/doc/EASTL.natvis")
    if(EXISTS "${EASTL_NATVIS_FILE}")
        target_sources(${PROJECT_NAME} PRIVATE "${EASTL_NATVIS_FILE}")
    endif()
endif()

# --------------------------------------------------
# External dependencies
# --------------------------------------------------
find_package(SDL3 REQUIRED CONFIG)
find_package(SDL3_image REQUIRED CONFIG)
find_package(SDL3_mixer REQUIRED CONFIG)
find_package(SDL3_ttf REQUIRED CONFIG)
find_package(nlohmann_json REQUIRED CONFIG)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        SDL3::SDL3
        SDL3_image::SDL3_image
        SDL3_mixer::SDL3_mixer
        SDL3_ttf::SDL3_ttf
        nlohmann_json
)

# --------------------------------------------------
# Group core files for IDE
# --------------------------------------------------
source_group(TREE ${CORE_FOLDER} PREFIX "Core" FILES 
    ${CORE_FILES}
    ${EABASE_HEADERS}
    ${EASTL_SOURCES}
    ${EASTL_HEADERS} 
    ${RPMALLOC_SOURCES}
    ${RPMALLOC_HEADERS}
)
