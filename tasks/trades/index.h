#pragma once

#include "trade.h"
#include <vector>
#include <stdexcept>
#include <algorithm>

class TradeIndex {
public:
    explicit TradeIndex(std::vector<Trade> trades);

    int64_t TotalVolume(int64_t from, int64_t to);
private:
    std::vector<Trade> trades_;
    std::vector<int64_t> pref_sum_;
};
