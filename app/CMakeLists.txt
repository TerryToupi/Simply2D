cmake_minimum_required(VERSION 3.30)

project(Simply2D_app)

# set the path for conviniance
set(APP_FOLDER_PATH ${CMAKE_CURRENT_SOURCE_DIR})
# grab all the files recursivly. No need to add them one by one
file(GLOB_RECURSE APP_FILES CONFIGURE_DEPENDS "${APP_FOLDER_PATH}/**.h" "${APP_FOLDER_PATH}/**.cpp")

add_executable(${PROJECT_NAME}
    ${APP_FILES}
)

target_compile_definitions(${PROJECT_NAME}
    PUBLIC
        ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/Assets/"
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${APP_FOLDER_PATH}
)

target_compile_features(${PROJECT_NAME}
    PUBLIC 
        cxx_std_20
)

source_group(
    TREE ${APP_FOLDER_PATH}
    PREFIX "app"
    FILES ${APP_FILES}
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        Core::Source
)