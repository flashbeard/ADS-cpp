cmake_minimum_required(VERSION 3.20)
project(NumberTheoryLibrary)

set(CMAKE_CXX_STANDARD 17)

add_library(NumberTheoryLibrary
        NumberTheory/Everything.h
        NumberTheory/Modular.h
        NumberTheory/Modular.cpp
        NumberTheory/Algorithms.h
        NumberTheory/Algorithms.cpp)
add_executable(NumberTheoryDemo NumberTheory/main.cpp)
target_link_libraries(NumberTheoryDemo NumberTheoryLibrary)

