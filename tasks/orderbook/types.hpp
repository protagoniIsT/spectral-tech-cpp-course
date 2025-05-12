#pragma once

struct Message {
    uint64_t ts;
    int64_t id;
    uint64_t price;
    uint64_t volume;
    uint8_t conf;
};
