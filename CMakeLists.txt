cmake_minimum_required(VERSION 3.7)
project(Codigo)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp include/basics3.hpp src/basics3.cpp include/siman.h include/siman.h include/sweep.hpp src/siman.cpp src/sweep.cpp)

add_executable(Codigo ${SOURCE_FILES})