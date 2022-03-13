cmake_minimum_required(VERSION 3.0.0)
project(NumberTheoryLibrary)

set(CMAKE_CXX_STANDARD 17)

add_library(NumberTheoryLibrary INTERFACE)
target_include_directories(NumberTheoryLibrary INTERFACE .)