cmake_minimum_required(VERSION 3.30)

project(CoreLibraries)

add_library(${PROJECT_NAME} INTERFACE)
add_library(Core::Import ALIAS ${PROJECT_NAME})

CPMAddPackage(
    NAME SDL 
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git 
    GIT_TAG main 
)

CPMAddPackage(
    NAME SDL_mixer
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer.git
    GIT_TAG main
    OPTIONS 
        "SDLMIXER_VENDORED ON"
        "SDLMIXER_MIDI_NATIVE OFF"
        "SDLMIXER_GME OFF"
        "SDLMIXER_WAVPACK OFF"
        "SDLMIXER_MOD OFF"
        "SDLMIXER_OPUS OFF"
        "SDLMIXER_MP3_MPG123 OFF"
        "SDLMIXER_VORBIS_VORBISFILE OFF"
        "SDLMIXER_FLAC_LIBFLAC OFF"
)

CPMAddPackage(
    NAME SDL_image
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
    GIT_TAG main
    OPTIONS
        "SDLIMAGE_VENDORED ON"
        "SDLIMAGE_AVIF OFF"
        "SDLIMAGE_BMP OFF"
        "SDLIMAGE_JPEG OFF"
        "SDLIMAGE_WEBP OFF"
)

CPMAddPackage(
    NAME SDL_ttf 
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
    GIT_TAG main
)

target_link_libraries(${PROJECT_NAME}
    INTERFACE
        SDL3::SDL3
        SDL3_mixer::SDL3_mixer
        SDL3_image::SDL3_image
        SDL3_ttf::SDL3_ttf
)

# Organizing all of the imported targets
# to appear under the "import" filter view
# in any IDE making the code cleaner
organize_target(SDL3::SDL3 "import")
organize_target(SDL3_mixer::SDL3_mixer "import")
organize_target(SDL3_image::SDL3_image "import")
organize_target(SDL3_ttf::SDL3_ttf "import")
organize_target(freetype "import")
organize_target(harfbuzz "import")
organize_target(plutosvg "import")
organize_target(plutovg "import")
organize_target(SDL_uclibc "import")
organize_target(SDL3_test "import")
organize_target(tiff "import")
organize_target(xmp_shared "import")