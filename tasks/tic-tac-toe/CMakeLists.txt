cmake_minimum_required(VERSION 3.10)
project(tic_tac_toe)

include(../cmake/BuildFlags.cmake)

add_library(opt SHARED
    PlayerStrategy.cpp
    opt_create_strategy.cpp
)
target_include_directories(opt PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(runner ./utility/runner.cpp)
target_link_libraries(runner dl)
