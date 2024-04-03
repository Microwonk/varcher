# VARCHER

(Voxel Ray Marcher)

## Building

- vcpkg to manage dependencies.
- CMake + Ninja
- GCC / MINGW / MSVC (Should work on Windows and Linux)

### Linux
You need to set a variable called VCPKG_ROOT (root of your vcpkg) for the build to work on linux with the default cmake preset.

```bash
cmake --preset=default
mkdir build
cmake --build build
```

### Windows
You need to set a variable called VCPKG_ROOT (root of your vcpkg) for the build to work with MSVC or MINGW.

```bash
cmake --preset=default
mkdir build
cmake --build build
```

### CLion
Set `-DCMAKE_TOOLCHAIN_FILE=path/to/your/vcpkg/scripts/buildsystems/vcpkg.cmake` in the Cmake options. Optionally you can set up vcpkg for CLion.

