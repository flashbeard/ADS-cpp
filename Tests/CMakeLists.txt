#*
#* Created by flashbeard.
#* tests
#*

cmake_minimum_required(VERSION 3.20)
project(Tests)

set(CMAKE_CXX_STANDARD 17)

include_directories(${TEST_SOURCE_DIR}/Libraries)

add_executable(NumberTheoryTests NumberTheory/main.cpp)
target_link_libraries(NumberTheoryTests NumberTheoryLibrary)