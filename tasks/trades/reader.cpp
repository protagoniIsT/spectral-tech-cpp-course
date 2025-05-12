#include "reader.h"
#include <optional>

std::optional<Trade> TradeReader::ReadNext() {
    Trade trade{};
    istream_.read(reinterpret_cast<char*>(&trade), sizeof(int64_t) * 3 + sizeof(int));
    if (istream_.eof()) {
        return std::nullopt;
    }
    return trade;
}

std::vector<Trade> TradeReader::ReadAll() {
    std::vector<Trade> trades;
    std::optional<Trade> curr_trade = ReadNext();
    while (curr_trade.has_value()) {
        trades.push_back(curr_trade.value());
        curr_trade = ReadNext();
    }
    return trades;
}
