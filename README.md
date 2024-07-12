# Pathfinding comparison

A C++ project for comparing the performance of different pathfinding algorithms and inspecting their behaviour visually.

Algborithms compared:
* A*
* Jump point search

## Prerequisites
The following programs must be installed before building the application (the italicized programs are installed by default on Ubuntu):
* (_A C++ compiler like GCC_)
* (_Make (for building)_)
* [CMake](https://cmake.org/) (for build automation & generation)

If you want to use a package manager, install [vcpkg](https://learn.microsoft.com/en-us/vcpkg/get_started/get-started).

_(Recommended on Linux)_ Building without a package manager requires manually installing the only dependency of the project: [SFML](https://www.sfml-dev.org/). On Debian-like distributions, for example, this can be done with the following command:
```
sudo apt install libsfml-dev
```
This is the recommended way on Linux distributions because vcpkg will require you to fix the [system dependencies](https://learn.microsoft.com/en-us/vcpkg/troubleshoot/build-failures#missing-system-dependencies) yourself anyway.

## Building & executing
To build the project, execute the following commands in the project root directory:
```
cmake --preset=release
cmake --build build --config Release
```
With vcpkg, the commands would be:
```
cmake --preset=vcpkg
cmake --build build --config Release
```
In case of [missing system dependencies](https://learn.microsoft.com/en-us/vcpkg/troubleshoot/build-failures#missing-system-dependencies) (for example from `alsa`), install those manually.

<hr>

After building, the executable `pathfinding` can be found and executed in the newly created `build` subdirectory.