cmake_minimum_required(VERSION 3.15)
project(rpmalloc LANGUAGES C)

# ----------------------------------------
# Options
# ----------------------------------------
option(RPMALLOC_BUILD_STATIC "Build static library" ON)
option(RPMALLOC_BUILD_SHARED "Build shared library" OFF)

option(RPMALLOC_ENABLE_STATISTICS "Enable rpmalloc statistics" ON)
option(RPMALLOC_ENABLE_ASSERTS "Enable rpmalloc asserts" OFF)

option(RPMALLOC_INSTALL "Enable install targets" OFF)

# ----------------------------------------
# Sources
# ----------------------------------------
set(RPMALLOC_SOURCES
    rpmalloc/rpmalloc.c
)

set(RPMALLOC_PUBLIC_HEADERS
    rpmalloc/rpmalloc.h
    rpmalloc/rpnew.h
)

# ----------------------------------------
# Library type
# ----------------------------------------
set(_lib_type STATIC)
if (RPMALLOC_BUILD_SHARED AND NOT RPMALLOC_BUILD_STATIC)
    set(_lib_type SHARED)
endif()

# ----------------------------------------
# Target
# ----------------------------------------
add_library(rpmalloc ${_lib_type}
    ${RPMALLOC_SOURCES}
    ${RPMALLOC_PUBLIC_HEADERS}
)

add_library(rpmalloc::rpmalloc ALIAS rpmalloc)

# ----------------------------------------
# Fix MSVC C11 atomics issue
# ----------------------------------------
if (MSVC)
    # Disable MSVC's half-implemented C11 atomics
    target_compile_definitions(rpmalloc PRIVATE RPMALLOC_NO_C11_ATOMICS=1)
endif()

# ----------------------------------------
# Include directories
# ----------------------------------------
target_include_directories(rpmalloc
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/rpmalloc>
        $<INSTALL_INTERFACE:include>
)

# ----------------------------------------
# Standard Features
# ----------------------------------------
target_compile_features(rpmalloc
    PUBLIC
        c_std_11
)

# ----------------------------------------
# Compiler definitions
# ----------------------------------------
if (RPMALLOC_ENABLE_STATISTICS)
    target_compile_definitions(rpmalloc PUBLIC ENABLE_STATISTICS=1)
else()
    target_compile_definitions(rpmalloc PUBLIC ENABLE_STATISTICS=0)
endif()

if (RPMALLOC_ENABLE_ASSERTS)
    target_compile_definitions(rpmalloc PUBLIC ENABLE_ASSERTS=1)
endif()

# ----------------------------------------
# Installation
# ----------------------------------------
if (RPMALLOC_INSTALL)

    include(GNUInstallDirs)
    include(CMakePackageConfigHelpers)

    # Install the library
    install(
        TARGETS rpmalloc
        EXPORT rpmallocTargets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )

    # Install public headers only
    install(
        FILES ${RPMALLOC_PUBLIC_HEADERS}
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/rpmalloc
    )

    # ----------------------------------------
    # CMake package config files (installed into <prefix>/cmake)
    # ----------------------------------------
    set(RPMALLOC_CONFIG_INSTALL_DIR "cmake")

    write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/rpmallocConfigVersion.cmake
        VERSION "1.4.5"
        COMPATIBILITY SameMajorVersion
    )

    configure_package_config_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/rpmallocConfig.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/rpmallocConfig.cmake
        INSTALL_DESTINATION ${RPMALLOC_CONFIG_INSTALL_DIR}
    )

    install(
        FILES
            ${CMAKE_CURRENT_BINARY_DIR}/rpmallocConfig.cmake
            ${CMAKE_CURRENT_BINARY_DIR}/rpmallocConfigVersion.cmake
        DESTINATION ${RPMALLOC_CONFIG_INSTALL_DIR}
    )

    install(
        EXPORT rpmallocTargets
        FILE rpmallocTargets.cmake
        NAMESPACE rpmalloc::
        DESTINATION ${RPMALLOC_CONFIG_INSTALL_DIR}
    )

endif()
