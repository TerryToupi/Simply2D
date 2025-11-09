cmake_minimum_required(VERSION 3.20)
project(Import)

include(ExternalProject)

# Installation directory - this is where everything goes
set(INSTALL_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}")

# SDL3
ExternalProject_Add(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG main
    CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=${INSTALL_PREFIX}
        -DCMAKE_BUILD_TYPE=Release
)

# SDL3 mixer
ExternalProject_Add(
    SDL3_mixer
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer.git
    GIT_TAG main
    CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=${INSTALL_PREFIX}
        -DCMAKE_BUILD_TYPE=Release
        -DSDLMIXER_VORBIS_VORBISFILE=OFF
        -DSDLMIXER_GME=OFF
        -DSDLMIXER_OPUS=OFF
)

# SDL3 image 
ExternalProject_Add(
    SDL3_image
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
    GIT_TAG main
    CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=${INSTALL_PREFIX}
        -DCMAKE_BUILD_TYPE=Release
        -DSDLIMAGE_AVIF=OFF
        -DSDLIMAGE_WEBP=OFF
)

#SDL3 ttf
ExternalProject_Add(
    SDL3_ttf 
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
    GIT_TAG main
    CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=${INSTALL_PREFIX}
        -DCMAKE_BUILD_TYPE=Release
)

# nlohmann_json
ExternalProject_Add(
    nlohmann_json 
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG develop 
    CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=${INSTALL_PREFIX}
        -DCMAKE_BUILD_TYPE=Release
        -DJSON_BuildTests=OFF
)