#-------------------------------------------------------------------------------------------
# Copyright (C) Electronic Arts Inc.  All rights reserved.
#-------------------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.20)
project(EASTL LANGUAGES CXX)

# ------------------------------------------------------------------------------------------
# Options
# ------------------------------------------------------------------------------------------
option(EASTL_BUILD_BENCHMARK "Enable build files for benchmark" OFF)
option(EASTL_BUILD_TESTS "Enable build files for tests" OFF)
option(EASTL_STD_ITERATOR_CATEGORY_ENABLED "Enable compatibility with std:: iterator categories" OFF)

option(EASTL_DISABLE_APRIL_2024_DEPRECATIONS "Enable April 2024 deprecated API" OFF)
option(EASTL_DISABLE_SEPT_2024_DEPRECATIONS "Enable Sept 2024 deprecated API" OFF)
option(EASTL_DISABLE_APRIL_2025_DEPRECATIONS "Enable April 2025 deprecated API" OFF)

option(EASTL_INSTALL "Enable install" OFF)

# ------------------------------------------------------------------------------------------
# Compiler flags
# ------------------------------------------------------------------------------------------
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/scripts/CMake")
include(CommonCppFlags)

# ------------------------------------------------------------------------------------------
# Sources
# ------------------------------------------------------------------------------------------
file(GLOB EASTL_SOURCES "source/*.cpp")
file(GLOB_RECURSE EASTL_HEADERS "include/EASTL/*.h")

add_library(EASTL ${EASTL_SOURCES} ${EASTL_HEADERS})
add_library(EASTL::EASTL ALIAS EASTL)
target_compile_features(EASTL PUBLIC cxx_std_14)

# ------------------------------------------------------------------------------------------
# Visual Studio NatVis
# ------------------------------------------------------------------------------------------
if (MSVC)
    set(EASTL_NATVIS_DIR "doc")
    set(EASTL_NATVIS_FILE "${EASTL_NATVIS_DIR}/EASTL.natvis")
    target_sources(EASTL INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${EASTL_NATVIS_FILE}>
        $<INSTALL_INTERFACE:${EASTL_NATVIS_FILE}>
    )
endif()

# ------------------------------------------------------------------------------------------
# Benchmarks / Tests
# ------------------------------------------------------------------------------------------
if(EASTL_BUILD_BENCHMARK)
    add_subdirectory(benchmark)
endif()

if(EASTL_BUILD_TESTS)
    add_subdirectory(test)
endif()

# ------------------------------------------------------------------------------------------
# Defines
# ------------------------------------------------------------------------------------------
target_compile_definitions(EASTL
    PUBLIC
        -D_CHAR16T
        -D_CRT_SECURE_NO_WARNINGS
        -D_SCL_SECURE_NO_WARNINGS
        -DEASTL_OPENSOURCE=1
)

if(EASTL_STD_ITERATOR_CATEGORY_ENABLED)
    target_compile_definitions(EASTL PUBLIC EASTL_STD_ITERATOR_CATEGORY_ENABLED=1)
endif()

# ------------------------------------------------------------------------------------------
# Include directories
# ------------------------------------------------------------------------------------------
target_include_directories(EASTL PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# ------------------------------------------------------------------------------------------
# Dependencies (EABase)
# ------------------------------------------------------------------------------------------
add_subdirectory(ThirdParty/EABase)
target_link_libraries(EASTL PUBLIC EABase)

# ------------------------------------------------------------------------------------------
# Deprecations
# ------------------------------------------------------------------------------------------
if(EASTL_DISABLE_APRIL_2024_DEPRECATIONS)
    target_compile_definitions(EASTL PUBLIC EA_DEPRECATIONS_FOR_2024_APRIL=EA_DISABLED)
endif()
if(EASTL_DISABLE_SEPT_2024_DEPRECATIONS)
    target_compile_definitions(EASTL PUBLIC EA_DEPRECATIONS_FOR_2024_SEPT=EA_DISABLED)
endif()
if(EASTL_DISABLE_APRIL_2025_DEPRECATIONS)
    target_compile_definitions(EASTL PUBLIC EA_DEPRECATIONS_FOR_2025_APRIL=EA_DISABLED)
endif()

# ------------------------------------------------------------------------------------------
# Installation
# ------------------------------------------------------------------------------------------
if (EASTL_INSTALL)

    include(GNUInstallDirs)
    include(CMakePackageConfigHelpers)

    # Library
    install(
        TARGETS EASTL
        EXPORT EASTLTargets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )

    # Headers
    install(
        DIRECTORY include/EASTL
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )

    # NatVis (MSVC)
    if (MSVC)
        install(FILES ${EASTL_NATVIS_FILE} DESTINATION doc)
    endif()

    # ------------------ CMake package config (installed in <prefix>/cmake/) -------------
    set(EASTL_CONFIG_INSTALL_DIR cmake)

    write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/EASTLConfigVersion.cmake
        VERSION "3.00.00"
        COMPATIBILITY SameMajorVersion
    )

    configure_package_config_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/EASTLConfig.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/EASTLConfig.cmake
        INSTALL_DESTINATION ${EASTL_CONFIG_INSTALL_DIR}
    )

    install(
        FILES
            ${CMAKE_CURRENT_BINARY_DIR}/EASTLConfig.cmake
            ${CMAKE_CURRENT_BINARY_DIR}/EASTLConfigVersion.cmake
        DESTINATION ${EASTL_CONFIG_INSTALL_DIR}
    )

    install(
        EXPORT EASTLTargets
        FILE EASTLTargets.cmake
        NAMESPACE EASTL::
        DESTINATION ${EASTL_CONFIG_INSTALL_DIR}
    )

endif()
