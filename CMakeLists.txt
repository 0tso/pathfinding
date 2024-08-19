cmake_minimum_required(VERSION 3.13)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules")
project(pathfinding)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")


# main visual program
find_package(SFML COMPONENTS system window graphics REQUIRED)
file(GLOB_RECURSE PATHFINDING_SRC CONFIGURE_DEPENDS "src/*.cpp")
add_executable(pathfinding_visualizer "${PATHFINDING_SRC}")
target_link_libraries(pathfinding_visualizer sfml-graphics)
target_include_directories(pathfinding_visualizer PRIVATE "${PROJECT_SOURCE_DIR}/src")

# tests
file(GLOB_RECURSE TEST_SRC CONFIGURE_DEPENDS "src/algorithms/*.cpp" "src/state.hpp" "src/all_algorithms.cpp" "tests/*.cpp")
add_executable(tests EXCLUDE_FROM_ALL "${TEST_SRC}")
target_include_directories(tests PRIVATE "${PROJECT_SOURCE_DIR}/src" "${PROJECT_SOURCE_DIR}/tests")
target_compile_options(tests PRIVATE -Wno-unused-result)

# separate variable because fetching Catch2 takes some time
if(BUILD_TESTS)
  Include(FetchContent)
  FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.4.0
  )
  FetchContent_MakeAvailable(Catch2)
  target_link_libraries(tests PRIVATE Catch2::Catch2)
endif()

# coverage slows the program down, which is detrimental to the benchmarks.
if(ADD_COVERAGE)
  include(CodeCoverage)
  append_coverage_compiler_flags_to_target(tests)
  set(COVERAGE_EXCLUDES "${PROJECT_SOURCE_DIR}/tests*"
    "${PROJECT_SOURCE_DIR}/build*")
  setup_target_for_coverage_gcovr_html(
    NAME pathfinding_coverage
    EXECUTABLE tests
    DEPENDENCIES tests
  )
endif()