add_catch(test_fixstr_utils test.cpp)
target_compile_options(test_fixstr_utils PRIVATE "-Wno-gnu-string-literal-operator-template")
