cmake_minimum_required(VERSION 3.20)
project(physim VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(PHYSIM_BUILD_TESTS    "Build unit tests"       ON)
option(PHYSIM_BUILD_EXAMPLES "Build example programs" ON)
option(PHYSIM_PYTHON         "Build Python bindings"  OFF)
option(PHYSIM_OPENMP         "Enable OpenMP"           ON)

file(GLOB_RECURSE PHYSIM_SOURCES src/*.cpp)
add_library(physim_core STATIC ${PHYSIM_SOURCES})
target_include_directories(physim_core PUBLIC include)

if(PHYSIM_OPENMP)
  find_package(OpenMP)
  if(OpenMP_CXX_FOUND)
    target_link_libraries(physim_core PUBLIC OpenMP::OpenMP_CXX)
    target_compile_definitions(physim_core PUBLIC PHYSIM_OPENMP)
  endif()
endif()

if(PHYSIM_PYTHON)
  find_package(pybind11 REQUIRED)
  pybind11_add_module(physim python/bindings.cpp)
  target_link_libraries(physim PRIVATE physim_core)
endif()

if(PHYSIM_BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()

if(PHYSIM_BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()
