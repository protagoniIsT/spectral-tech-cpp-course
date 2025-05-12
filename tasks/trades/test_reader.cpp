#include <catch.hpp>

#include <sstream>
#include <fstream>
#include "reader.h"
#include <tuple> // local include

bool operator==(const Trade& a, const Trade& b) {
    return std::make_tuple(a.price, a.volume, a.timestamp, a.code) == std::make_tuple(b.price, b.volume, b.timestamp, b.code);
}

TEST_CASE("small") {
    Trade a{1, 2, 0, 7};
    Trade b{2, 3, 6, 9};
    Trade c{9, 1, 9, 11};
    std::stringstream ss;
    ss.write(reinterpret_cast<char*>(&a), 28);
    ss.write(reinterpret_cast<char*>(&b), 28);
    ss.write(reinterpret_cast<char*>(&c), 28);

    TradeReader reader(ss);
    auto t = reader.ReadNext();
    REQUIRE(t.value() == a);
    t = reader.ReadNext();
    REQUIRE(t.value() == b);
    t = reader.ReadNext();
    REQUIRE(t.value() == c);
    t = reader.ReadNext();
    REQUIRE(!t.has_value());
}

uint64_t CalcHash(const std::vector<Trade>& trades) {
    uint64_t res = 0;
    const uint64_t k = 17239;
    for (const auto& trade : trades) {
        res = res * k + trade.price;
        res = res * k + trade.volume;
        res = res * k + trade.timestamp;
        res = res * k + trade.code;
    }
    return res;
}

TEST_CASE("big") {
    std::ifstream fin("../trades/test.data", std::ios::binary);
    TradeReader reader(fin);
    auto trades = reader.ReadAll();
    REQUIRE(10544259621707596029ull == CalcHash(trades));
}
