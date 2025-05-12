add_catch(test_avx_matmul test.cpp)
target_compile_options(test_avx_matmul PUBLIC -mavx2)
