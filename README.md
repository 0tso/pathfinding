# Pathfinding comparison

A C++ project for comparing the performance of different pathfinding algorithms and inspecting their behaviour visually.

Algborithms compared:
* A* (a basic implementation _vs._ a more optimized version)
* Jump point search
* Non-optimal bidirectional breadth-first search

For a more detailed look at how the aforementioned algorithms work, see [structure.md](./docs/structure.md). 

For graphs of how they perform against each other, see [testing_and_benchmarks.md](./docs/testing_and_benchmarks.md).

## Prerequisites
The following programs must be installed before building the application:
* A C++ compiler like GCC
* Make (for building)
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

After building, the executable `pathfinding_visualizer` can be found and executed in the newly created `build` subdirectory.

## Documentation
Documentation provided in the [docs](./docs/) folder.

* [Usage instructions](./docs/usage_instructions.md)
* [Program & algorithm structure](./docs/structure.md)
* [Testing & benchmarks](./docs/testing_and_benchmarks.md)