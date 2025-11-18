cmake_minimum_required(VERSION 3.30)

project(Simply2D_core)

# set the path for conviniance
set(CORE_FOLDER_PATH ${CMAKE_CURRENT_SOURCE_DIR})
# grab all the files recursivly. No need to add them one by one
file(
    GLOB_RECURSE 
        CORE_FILES 
    CONFIGURE_DEPENDS 
            "${CORE_FOLDER_PATH}/**.h" 
            "${CORE_FOLDER_PATH}/**.cpp"
)
# Exclude ThridParty folder
list(FILTER CORE_FILES EXCLUDE REGEX ".*/ThridParty/.*")

add_library(${PROJECT_NAME} STATIC)
add_library(Simply2D::Core ALIAS ${PROJECT_NAME})

target_sources(${PROJECT_NAME}
    PRIVATE
        ${CORE_FILES}
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        "${CORE_FOLDER_PATH}/Include"
        "${CORE_FOLDER_PATH}/Public"
    PRIVATE
        "${CORE_FOLDER_PATH}"
)

target_precompile_headers(${PROJECT_NAME}
    PRIVATE 
        ${CORE_FOLDER_PATH}/pch.h
)

target_compile_features(${PROJECT_NAME}
    PUBLIC 
        cxx_std_20
)

source_group(
    TREE ${CORE_FOLDER_PATH}
    PREFIX "Core"
    FILES ${CORE_FILES}
)

find_package(SDL3 REQUIRED CONFIG)
find_package(SDL3_image REQUIRED CONFIG)
find_package(SDL3_mixer REQUIRED CONFIG)
find_package(SDL3_ttf REQUIRED CONFIG)
find_package(nlohmann_json REQUIRED CONFIG)

add_subdirectory(ThridParty/rpmalloc)
set_property(TARGET rpmalloc PROPERTY FOLDER "ThirdParty")

add_subdirectory(ThridParty/EASTL)
set_property(TARGET EASTL PROPERTY FOLDER "ThirdParty")
set_property(TARGET EABase PROPERTY FOLDER "ThirdParty")

target_link_libraries(${PROJECT_NAME}
    PUBLIC 
        SDL3::SDL3
        SDL3_mixer::SDL3_mixer
        SDL3_image::SDL3_image
        SDL3_ttf::SDL3_ttf
        nlohmann_json
    
    PUBLIC
        rpmalloc::rpmalloc
        EASTL::EASTL
)