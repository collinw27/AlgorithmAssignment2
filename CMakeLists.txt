cmake_minimum_required(VERSION 3.28)
project(Assignment2 LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(AlgorithmAssignment2 main.cpp)
target_compile_features(AlgorithmAssignment2 PRIVATE cxx_std_17)