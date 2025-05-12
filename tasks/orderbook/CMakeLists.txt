include(build.cmake)
add_catch(test_pcap_parser test_parser.cpp)
target_compile_definitions(test_pcap_parser PUBLIC TASK_DIR="${CMAKE_CURRENT_SOURCE_DIR}/")
