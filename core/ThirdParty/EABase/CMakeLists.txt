#-------------------------------------------------------------------------------------------
# Copyright (C) Electronic Arts Inc.  All rights reserved.
#-------------------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.20)
project(EABase LANGUAGES CXX)

#-------------------------------------------------------------------------------------------
# Options
#-------------------------------------------------------------------------------------------
option(EABASE_BUILD_TESTS "Enable generation of build files for tests" OFF)
option(EABASE_INSTALL      "Enable installation of EABase" ON)

#-------------------------------------------------------------------------------------------
# Tests
#-------------------------------------------------------------------------------------------
if(EABASE_BUILD_TESTS)
    add_subdirectory(test)
endif()

#-------------------------------------------------------------------------------------------
# Defines
#-------------------------------------------------------------------------------------------
add_definitions(-D_CHAR16T)

#-------------------------------------------------------------------------------------------
# Header-only library
#-------------------------------------------------------------------------------------------
add_library(EABase INTERFACE)
add_library(EABase::EABase ALIAS EABase)

target_include_directories(EABase INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/Common>
    $<INSTALL_INTERFACE:include>
)

#-------------------------------------------------------------------------------------------
# Installation
#-------------------------------------------------------------------------------------------
if(EABASE_INSTALL)

    include(GNUInstallDirs)
    include(CMakePackageConfigHelpers)

    # Install headers into include/EABase
    install(
        DIRECTORY include/Common/EABase
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )

    # ----------------------------- CMake Package Config (prefix/cmake/) ---------------------
    set(EABASE_CONFIG_INSTALL_DIR cmake)

    write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/EABaseConfigVersion.cmake
        VERSION "2.00.00"
        COMPATIBILITY SameMajorVersion
    )

    configure_package_config_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/EABaseConfig.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/EABaseConfig.cmake
        INSTALL_DESTINATION ${EABASE_CONFIG_INSTALL_DIR}
    )

    install(
        FILES
            ${CMAKE_CURRENT_BINARY_DIR}/EABaseConfig.cmake
            ${CMAKE_CURRENT_BINARY_DIR}/EABaseConfigVersion.cmake
        DESTINATION ${EABASE_CONFIG_INSTALL_DIR}
    )

    install(
        EXPORT EABaseTargets
        FILE EABaseTargets.cmake
        NAMESPACE EABase::
        DESTINATION ${EABASE_CONFIG_INSTALL_DIR}
    )

    # Export the interface target
    install(
        TARGETS EABase
        EXPORT EABaseTargets
    )

endif()
