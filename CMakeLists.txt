cmake_minimum_required(VERSION 3.28)
project(Assignment1 LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(Proj main.cpp)
target_compile_features(Proj PRIVATE cxx_std_17)