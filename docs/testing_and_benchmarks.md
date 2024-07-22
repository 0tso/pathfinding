# Testing & benchmarks

The project uses [Catch2](https://github.com/catchorg/Catch2) for unit tests.
When building tests using the `tests` CMake preset, Catch2 is pulled from Github using CMake's FetchContent and compiled.

Instructions for building and running tests are in the main [README.md](../README.md) file.

### Unit tests

The following modules are unit tested extensively:
* [Util](../src/algorithms/util.hpp)

### Benchmarks
For running benchmarks, download maps (`.map`) and scenario files (`.scen`) from https://movingai.com/benchmarks/grids.html.
After placing them in any directory (for example in a "benchmarks" directory within the [tests](../tests/) directory), direct the tests executable to this directory by providing its path after `--benchmarks`.

Example:
```
build/tests --benchmarks tests/benchmarks
```
Unit tests will be skipped, and all the scenario files within that directory will be executed.
The output of benchmarks printed in standard output.

### Attributions

The program uses a modified version of the scenario and map loading functionalities of the [HOG2](https://github.com/nathansttt/hog2) project. Modification had to be done to remove dependencies on OpenGL and other parts of HOG2. These files are:
* [Map.h](../tests/hog2/Map.h)
* [Map.cpp](../tests/hog2/Map.cpp)
* [ScenarioLoader.h](../tests/hog2/ScenarioLoader.h)
* [ScenarioLoader.cpp](../tests/hog2/ScenarioLoader.cpp)

These files are used to load and process the `.map` and `.scen` files for benchmarking.