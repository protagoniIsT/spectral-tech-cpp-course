#include <catch.hpp>

#include <vector>
#include <random>
#include <fstream>

#include "index.h"
#include "reader.h"

TEST_CASE("small") {
    Trade a{1, 2, 10, 1};
    Trade b{2, 3, 10, 7};
    Trade c{3, 4, 15, 9};
    Trade d{1, 5, 17, 9};
    Trade e{2, 2, 20, 8};
    TradeIndex index{std::vector<Trade>{a, b, c, d, e}};

    REQUIRE(index.TotalVolume(5, 7) == 0);
    REQUIRE(index.TotalVolume(5, 21) == 29);
    REQUIRE(index.TotalVolume(15, 16) == 12);
    REQUIRE(index.TotalVolume(10, 15) == 8);
    REQUIRE(index.TotalVolume(11, 20) == 17);
    REQUIRE(index.TotalVolume(20, 25) == 4);
    REQUIRE(index.TotalVolume(11, 14) == 0);
    REQUIRE(index.TotalVolume(21, 24) == 0);
}

int64_t LinearSum(const std::vector<Trade>& trades, int64_t from, int64_t to) {
    int64_t s = 0;
    for (const auto& trade : trades) {
        if (trade.timestamp >= to) {
            break;
        }
        if (from <= trade.timestamp) {
            s += trade.price * trade.volume;
        }
    }
    return s;
}

TEST_CASE("stress") {
    std::ifstream fin("../trades/test.data");
    TradeReader reader(fin);
    auto trades = reader.ReadAll();
    TradeIndex index(trades);

    std::mt19937 gen(623734);
    std::uniform_int_distribution<size_t> dist(0, trades.size() - 1);
    std::uniform_int_distribution<int> shift(0, 10);

    for (int i = 0; i < 100000; ++i) {
        size_t start_idx = dist(gen);
        size_t end_idx = dist(gen);
        auto start_ts = trades[start_idx].timestamp + shift(gen);
        auto end_ts = trades[end_idx].timestamp + shift(gen);
        if (start_ts > end_ts) {
            std::swap(start_ts, end_ts);
        }
        auto vol = index.TotalVolume(start_ts, end_ts);
        REQUIRE(vol >= 0);
        if (i <= 100) {
            REQUIRE(LinearSum(trades, start_ts, end_ts) == vol);
        }
    }
}
