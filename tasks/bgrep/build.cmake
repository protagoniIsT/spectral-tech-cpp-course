cmake_minimum_required(VERSION 3.10)
project(bin-grep LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(gflags REQUIRED)
find_package(Threads REQUIRED)

add_executable(bin-grep main.cpp)

target_include_directories(bin-grep PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(bin-grep PRIVATE gflags_shared Threads::Threads)
