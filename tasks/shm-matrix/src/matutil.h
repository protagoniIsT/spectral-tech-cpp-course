#pragma once

#include <cstdint>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <thread>
#include <algorithm>
#include <atomic>
#include <chrono>
#include "threadpool.h"

void transpose(const uint32_t*, uint32_t*, int);

void mat_multiply(const uint32_t*, const uint32_t*, std::vector<uint32_t>&, uint32_t*, int, ThreadPool&, unsigned int, size_t);

void mat_pow(const uint32_t*, uint32_t*, int, int);
