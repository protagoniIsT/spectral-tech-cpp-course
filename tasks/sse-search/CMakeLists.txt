add_catch(test_sse_search test.cpp)
target_compile_options(test_sse_search PUBLIC -msse4.2)
