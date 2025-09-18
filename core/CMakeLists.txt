cmake_minimum_required(VERSION 3.30)

project(Simply2D_core)

# set the path for conviniance
set(CORE_FOLDER_PATH ${CMAKE_CURRENT_SOURCE_DIR})
# grab all the files recursivly. No need to add them one by one
file(GLOB_RECURSE CORE_FILES CONFIGURE_DEPENDS "${CORE_FOLDER_PATH}/**.h" "${CORE_FOLDER_PATH}/**.cpp")

add_library(${PROJECT_NAME} STATIC)
add_library(Core::Source ALIAS ${PROJECT_NAME})

target_sources(${PROJECT_NAME}
    PRIVATE
        ${CORE_FILES}
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${CORE_FOLDER_PATH}
)

target_compile_features(${PROJECT_NAME}
    PUBLIC 
        cxx_std_20
)

source_group(
    TREE ${CORE_FOLDER_PATH}
    PREFIX "core"
    FILES ${CORE_FILES}
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        Core::Import
)