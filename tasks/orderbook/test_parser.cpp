#include <catch.hpp>

#include <sstream>
#include <fstream>
#include <iostream>
#include "pcapng.hpp"
#include "types.hpp"

#ifndef TASK_DIR
#define TASK_DIR "."
#endif

inline std::string ConstructBasePath() {
    std::string result(TASK_DIR);
    return result;
}

const std::string kBasePath = ConstructBasePath();

bool operator==(const Message& lhs, const Message& rhs) {
    return std::make_tuple(lhs.ts, lhs.id, lhs.price, lhs.volume, lhs.conf) == std::make_tuple(rhs.ts, rhs.id, rhs.price, rhs.volume, rhs.conf);
}

TEST_CASE("small") {
    std::ifstream fin(kBasePath + "eth.pcapng", std::ios_base::binary);
    std::vector<char> data((std::istreambuf_iterator<char>(fin)), std::istreambuf_iterator<char>());
    auto messages = Parse(std::string_view(data.data(), data.size()));
    REQUIRE(messages.at(0) == Message{9223372036854775810ull, 23719022747472965ll, 0, 0, 0});
    REQUIRE(messages.at(105) == Message{1652227200969501ull, 104, 234325, 349800000ull, 3});
    REQUIRE(messages.at(903) == Message{1652227200969501ull, 902, 236008, 1971500000ull, 3});
}
