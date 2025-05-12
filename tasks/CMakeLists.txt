project(effective-cpp)

cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD             20)
set(CMAKE_MODULE_PATH              "${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_EXPORT_COMPILE_COMMANDS  ON)
set(GOOGLETEST_PATH "${CMAKE_SOURCE_DIR}/contrib/googletest" CACHE PATH "")

find_package(Catch REQUIRED)

add_subdirectory(contrib/benchmark)

include(cmake/TestSolution.cmake)
include(cmake/BuildFlags.cmake)
include_directories(util)
include_directories(contrib/fixed_string/include)

# Week 1
add_subdirectory(multiplication)
add_subdirectory(byte_order)
add_subdirectory(short_view)
add_subdirectory(reverse_vector)
add_subdirectory(static_allocator)
add_subdirectory(set_allocator)

# Week 2
add_subdirectory(fold)
add_subdirectory(itertools)
add_subdirectory(static-map)
add_subdirectory(intrusive-list)
add_subdirectory(fuzzy-search)

# Week 3
add_subdirectory(hashmap)
add_subdirectory(deque)
add_subdirectory(cow-vector)
add_subdirectory(small-vector)

# Week 4
add_subdirectory(concepts)
add_subdirectory(clever-set)
add_subdirectory(tuple-utils)
add_subdirectory(fixstr-utils)
add_subdirectory(meta-name)

# Week 5
if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/trades/trades.cmake)
  add_subdirectory(trades)
endif()
add_subdirectory(abort)

# Week 7
add_subdirectory(bgrep)
add_subdirectory(orderbook)

# Week 8
add_subdirectory(reduce)
add_subdirectory(buffered-channel)
add_subdirectory(rw-spinlock)

# Week 9
add_subdirectory(avx-matmul)
add_subdirectory(avx-vector)
add_subdirectory(sse-search)
