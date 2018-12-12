cmake_minimum_required(VERSION 3.7)
project(Codigo)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp include/basics3.hpp src/basics3.cpp include/siman.hpp include/siman.hpp include/sweep.hpp include/Customer.h include/Route.h include/System.h include/Respuesta.h include/clark_wright.h src/siman.cpp src/sweep.cpp src/Customer.cpp src/Respuesta.cpp src/Route.cpp src/System.cpp src/clark_wright.cpp)

add_executable(Codigo ${SOURCE_FILES})