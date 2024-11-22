cmake_minimum_required(VERSION 3.20)

project(gradflow LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

set(SRC src/)
set(INCLUDE include/)
set(TEST test/)

include_directories(${INCLUDE})

# tests
add_library(compile_test ${TEST}/compile_test.cc)
add_executable(access_test ${TEST}/access_test.cc)
add_executable(computation_test ${TEST}/computation_test.cc)
add_executable(slice_test ${TEST}/slice_test.cc)
