# Simply2D

Simply2D is a C++20 2D application framework and sample game project built on SDL3. The repository contains a reusable engine-style core library, a sample application, local third-party support code, and an `import` project that fetches and installs external runtime dependencies for the main build.

## Features

- SDL3-based windowing, rendering, audio, image loading, and font support.
- Scene-oriented application flow with sprites, tile layers, cameras, bounds, gravity, and motion helpers.
- Asset loading for images, audio, and JSON scene data.
- Core utility types and memory support backed by bundled EASTL, EABase, and rpmalloc sources.
- CMake-based build setup for the dependency import stage and the root project.

## Repository Layout

```text
Simply2D/
  app/                  Sample application, scenes, and assets
  core/                 Simply2D core library source and public headers
  core/ThirdParty/      Bundled EASTL, EABase, and rpmalloc sources
  cmake/                Project CMake helper scripts
  import/               External dependency fetch/build/install project
  CMakeLists.txt        Root project build
```

## Prerequisites

- CMake 3.30 or newer for the root project.
- A C++20-capable compiler.
- Git, required by the `import` project to fetch external dependencies.
- A supported native build tool, such as Visual Studio, Ninja, or Make.

## Build

Build Simply2D in two stages. First build the `import` project so the external dependencies are installed into the `import` folder. Then configure and build the root project.

### 1. Build External Dependencies

```powershell
cd import
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

The import step fetches and installs:

- SDL3
- SDL3_image
- SDL3_mixer
- SDL3_ttf
- nlohmann_json

### 2. Build Simply2D

```powershell
cd ..
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

The root build uses `import` as its CMake package prefix and builds:

- `Simply2D_core`: the static core library.
- `Simply2D_app`: the sample executable.

## Run

After a successful build, run the sample application from the generated build output. The exact path depends on the CMake generator.

Common Windows paths are:

```powershell
.\build\App\Release\Simply2D_app.exe
```

or, with single-configuration generators such as Ninja:

```powershell
.\build\App\Simply2D_app.exe
```

The application loads its assets from the `app` directory through the `ASSETS_PATH` compile definition.

## Development Notes

- Public engine headers are exposed from `core/Public` and `core/Include`.
- Include the umbrella header with:

```cpp
#include <Simply2D.h>
```

- Add application-specific code under `app`.
- Add reusable engine code under `core`.
- If new external dependencies are required, add them to `import/CMakeLists.txt` and ensure the root project can find them through `CMAKE_PREFIX_PATH`.

## License

This project is licensed under the Apache License 2.0. See `LICENSE` for details.
