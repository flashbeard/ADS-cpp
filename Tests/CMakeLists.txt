cmake_minimum_required(VERSION 3.0.0)
project(Tests)

set(CMAKE_CXX_STANDARD 17)

include_directories(${TEST_SOURCE_DIR}/Libraries)

add_executable(NumberTheoryTests NumberTheory/main.cpp)
target_link_libraries(NumberTheoryTests PUBLIC NumberTheoryLibrary gtest_main)

add_executable(DataStructuresTests DataStructures/main.cpp)
target_link_libraries(DataStructuresTests PUBLIC DataStructuresLibrary gtest_main)