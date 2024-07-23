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

If you want to use a C++ package manager, install [vcpkg](https://learn.microsoft.com/en-us/vcpkg/get_started/get-started).

_(Recommended on Linux)_ Building without a C++ package manager requires manually installing the only dependency of the project: [SFML](https://www.sfml-dev.org/). On Debian-like distributions, for example, this can be done with the following command:
```
sudo apt install libsfml-dev
```
This is the recommended way on Linux distributions because vcpkg will require you to fix the [system dependencies](https://learn.microsoft.com/en-us/vcpkg/troubleshoot/build-failures#missing-system-dependencies) yourself anyway.

## Building & executing
First, download the repository, for example by using the following Git command
```
git clone https://github.com/0tso/pathfinding.git
```
or by left-clicking the green "Code" button on Github and downloading the ZIP file.

Then, to build the project, execute the following commands in the project root directory:
```
cmake --preset=release
cmake --build build --config Release
```
When using vcpkg, set the `preset` variable to `vcpkg` instead of `release` in the first command.
In case of [missing system dependencies](https://learn.microsoft.com/en-us/vcpkg/troubleshoot/build-failures#missing-system-dependencies) (for example from `alsa`), install those manually.

After building, the executable `pathfinding` can be found and executed in the newly created `build` subdirectory.

## Tests & benchmarks
To build and execute the tests (provided in the [tests](./tests/) folder), run the following commands:
```
cmake --preset=tests
cmake --build build --config Release --target tests
./build/tests
```
For benchmarking, see [testing_and_benchmarks.md](./docs/testing_and_benchmarks.md).

## Documentation
Documentation provided in the [docs](./docs/) folder.