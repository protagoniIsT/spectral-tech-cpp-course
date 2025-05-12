cmake_minimum_required(VERSION 3.10)

project(shm-matrix)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(shm-matrix
    src/main.cpp
    src/matutil.cpp
    src/threadpool.cpp
)

target_include_directories(shm-matrix PRIVATE ${CMAKE_SOURCE_DIR}/src)
