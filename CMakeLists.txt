cmake_minimum_required(VERSION 3.30)
project(Simply2D)

include(cmake/copy_dlls.cmake)

# Point CMake to where you installed the dependencies
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Import")

add_subdirectory(Core) 
add_subdirectory(App)
