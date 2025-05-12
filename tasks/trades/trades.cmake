add_library(trades_reader reader.cpp reader.h)

add_library(trades_index index.cpp index.h)

add_library(trades_reader_obj OBJECT reader.cpp)
add_library(trades_index_obj OBJECT index.cpp)

add_library(trades STATIC
        $<TARGET_OBJECTS:trades_reader_obj>
        $<TARGET_OBJECTS:trades_index_obj>
        )
