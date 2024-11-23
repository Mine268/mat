cmake_minimum_required(VERSION 3.20)

project(matlib LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

set(SRC src/)
set(INCLUDE include/)
set(TEST test/)

include_directories(${INCLUDE})

# Range
add_library(Range ${SRC}/range.cc)

# tests
add_library(compile_test ${TEST}/compile_test.cc)
add_executable(access_test ${TEST}/access_test.cc)
add_executable(computation_test ${TEST}/computation_test.cc)
add_executable(slice_test ${TEST}/slice_test.cc)
add_executable(PLU_test ${TEST}/PLU_test.cc)

target_link_libraries(compile_test Range)
target_link_libraries(access_test Range)
target_link_libraries(computation_test Range)
target_link_libraries(slice_test Range)
target_link_libraries(PLU_test Range)
