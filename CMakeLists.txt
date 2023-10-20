cmake_minimum_required(VERSION 3.27)

project(CPPDescent)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_DIR src)

set(SOURCES ${SOURCE_DIR}/main.cpp)

add_executable(cppdescent ${SOURCES})