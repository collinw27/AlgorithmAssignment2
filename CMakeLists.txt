cmake_minimum_required(VERSION 3.28)
project(Assignment1 LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

file(
    GLOB src
)

add_executable(Proj ${src} main.cpp)
target_compile_features(Proj PRIVATE cxx_std_17)