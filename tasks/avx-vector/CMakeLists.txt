add_catch(test_avx_vector test.cpp)
target_compile_options(test_avx_vector PUBLIC "-mavx512f")
