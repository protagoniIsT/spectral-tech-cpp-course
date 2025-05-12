#include <execinfo.h>
#include <iostream>
#include <dlfcn.h>
#include <signal.h>

void abort() {
    static constexpr size_t stacktrace_size = 50;

    void* buff[stacktrace_size];
    int returned_ptrs = backtrace(buff, stacktrace_size);
    fprintf(stderr, "Stacktrace (%d frames):\n", returned_ptrs);

    char** symbols = backtrace_symbols(buff, returned_ptrs);
    if (symbols) {
        for (int i = 0; i < returned_ptrs; ++i) {
            fprintf(stderr, "%s\n", symbols[i]);
        }
        free(symbols);
    }

    using abort_func = void (*)();
    static abort_func real_abort = (abort_func) dlsym(RTLD_NEXT, "abort");
    real_abort();
}
