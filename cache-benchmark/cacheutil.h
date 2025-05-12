#pragma once
#include <cstdint>
#include <intrin.h>
#include <windows.h>
#include <iostream>

#define CACHE_LINE_SIZE 64

#define ALLOC(size)  _aligned_malloc(size, CACHE_LINE_SIZE)
#define FREE(p)      _aligned_free(p)

static inline uint64_t rdtscp() {
    unsigned aux;
    return __rdtscp(&aux);
}

struct alignas(CACHE_LINE_SIZE) Node {
    Node* next;
    char pad[CACHE_LINE_SIZE - sizeof(Node*)];
};

void set_cpu_affinity(int core_id) {
    DWORD_PTR mask = static_cast<DWORD_PTR>(1) << core_id;
    DWORD_PTR result = SetThreadAffinityMask(GetCurrentThread(), mask);
    if (result == 0) {
        std::cerr << "SetThreadAffinityMask failed (error " << GetLastError() << ")\n";
    }
}

