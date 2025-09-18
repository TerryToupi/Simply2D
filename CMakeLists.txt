cmake_minimum_required(VERSION 3.30)

# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

# prevent installing to system directories. 
set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}" CACHE INTERNAL "")

project(Simply2D)

# configuring CPM to have separate cache directory from the build direcory
# This way we can delete safely the build directory without worry of 
# re-downloading the external packages
set(CPM_SOURCE_CACHE "${CMAKE_CURRENT_SOURCE_DIR}/import" CACHE PATH "")
set(CPM_USE_NAMED_CACHE_DIRECTORIES ON)
# including CPM to the main project
include(cmake/CPM.cmake)
include(cmake/utils.cmake)

add_subdirectory(Import)
add_subdirectory(Core) 
add_subdirectory(App)