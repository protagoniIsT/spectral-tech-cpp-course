include(trades.cmake)
add_catch(test_trades_reader test_reader.cpp)
add_catch(test_trades_index test_index.cpp)

target_link_libraries(test_trades_reader trades_reader)
target_link_libraries(test_trades_index trades)
