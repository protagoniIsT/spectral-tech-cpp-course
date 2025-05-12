add_catch(test_bgrep test.cpp)
target_link_libraries(test_bgrep stdc++fs)
target_compile_definitions(test_bgrep PUBLIC TASK_DIR="${CMAKE_CURRENT_SOURCE_DIR}/")

if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/build.cmake)
  include(build.cmake)
endif()
