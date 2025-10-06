cmake_minimum_required(VERSION 3.30)

project(ImportLibraries)

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
        "SDLIMAGE_WEBP OFF"
)

CPMAddPackage(
    NAME SDL_ttf 
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
    GIT_TAG main
) 

CPMAddPackage(
    NAME nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG develop 
)

# Libraries needed only by the engine
add_library(CoreImport INTERFACE)
add_library(Core::Import ALIAS CoreImport) 

target_link_libraries(CoreImport
    INTERFACE
        SDL3::SDL3
        SDL3_mixer::SDL3_mixer
        SDL3_image::SDL3_image
        SDL3_ttf::SDL3_ttf
        nlohmann_json
)

# Libraries needed by the engine and the application
add_library(GlobalImport INTERFACE)
add_library(Global::Import ALIAS GlobalImport) 

target_link_libraries(GlobalImport
    INTERFACE
        nlohmann_json
)

# Organizing all of the imported targets
# to appear under the "import" filter view
# in any IDE making the code cleaner
organize_target(SDL3::SDL3 "Import")
organize_target(SDL3_mixer::SDL3_mixer "Import")
organize_target(SDL3_image::SDL3_image "Import")
organize_target(SDL3_ttf::SDL3_ttf "Import")
organize_target(freetype "Import")
organize_target(harfbuzz "Import")
organize_target(plutosvg "Import")
organize_target(plutovg "Import")
organize_target(SDL_uclibc "Import")
organize_target(SDL3_test "Import")
organize_target(tiff "Import")
organize_target(xmp_shared "Import")
organize_target(nlohmann_json "Import")