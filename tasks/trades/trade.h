#pragma once

#include <cstdint>

struct Trade {
    int64_t price;
    int64_t volume;
    int64_t timestamp;
    int code;
};
