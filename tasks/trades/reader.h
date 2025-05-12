#pragma once

#include "trade.h"
#include <optional>
#include <sstream>
#include <vector>

class TradeReader {
public:
    explicit TradeReader(std::istream& istream) : istream_(istream) {}

    std::optional<Trade> ReadNext();

    std::vector<Trade> ReadAll();

private:
    std::istream& istream_;
};
