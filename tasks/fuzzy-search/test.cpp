#include <catch.hpp>

#include <limits>
#include <random>
#include <algorithm>
#include <vector>
#include <fuzzy_search.h>
#include <iostream>

const uint32_t kMaxVal = std::numeric_limits<uint32_t>::max();

TEST_CASE("empty") {
    FuzzySearch f({});
    REQUIRE(f.Find(5u) == std::nullopt);
}

TEST_CASE("small") {
    std::vector<uint32_t> data{0u, 5u, (1u << 31) - 1};
    FuzzySearch f{data};
    REQUIRE(f.Find(0u) == 0u);
    REQUIRE(f.Find(2u) == std::nullopt);
    REQUIRE(f.Find(3u) == 5u);
    REQUIRE(f.Find(4u) == 5u);
    REQUIRE(f.Find(8u) == 5u);
    REQUIRE(f.Find(10u) == 5u);
    REQUIRE(f.Find(11u) == std::nullopt);
    REQUIRE(f.Find(data.back() + 1) == data.back());
    REQUIRE(f.Find(data.back() - 1) == data.back());
    REQUIRE(f.Find(kMaxVal) == std::nullopt);
    REQUIRE(f.Find(kMaxVal - 1) == data.back());
}

TEST_CASE("candidate equals 2*x") {
    std::vector<uint32_t> data{8u};
    FuzzySearch f{data};
    REQUIRE(f.Find(4u) == 8u);
}

TEST_CASE("candidate equals 2*x (2) and close cases") {
    std::vector<uint32_t> data{6u, 7u};
    FuzzySearch f{data};
    REQUIRE(f.Find(14u) == 7u);
    REQUIRE(f.Find(13u) == 7u);
    REQUIRE(f.Find(15u) == std::nullopt);
    REQUIRE(f.Find(12u) != std::nullopt);
}

TEST_CASE("candidate equals x") {
    std::vector<uint32_t> data{50u};
    FuzzySearch f{data};
    REQUIRE(f.Find(50u) == 50u);
}

TEST_CASE("multiple answers") {
    std::vector<uint32_t> data{1u, 3u, 8u, 16u, 31u, 32u};
    FuzzySearch f{data};
    auto found_val = f.Find(15u);
    REQUIRE((found_val == 8u || found_val == 16u));
}

TEST_CASE("small2") {
    std::vector<uint32_t> data{1u, 2u, 4u, 8u, kMaxVal - 1, kMaxVal};
    FuzzySearch f{data};
    REQUIRE(f.Find(9u) == 8u);
    REQUIRE(f.Find(0u) == std::nullopt);
    REQUIRE((f.Find(4u) == 2u || f.Find(4u) == 4u || f.Find(4u) == 8u));
    {
        auto res = f.Find(1u << 31);
        REQUIRE((res.value() == kMaxVal - 1 || res.value() == kMaxVal));
    }
    REQUIRE(f.Find((1u << 31) - 1) == kMaxVal - 1);
    REQUIRE(f.Find((1u << 31) - 2) == std::nullopt);

}

TEST_CASE("stress") {
    const int num_elements = 100000;
    const int num_queries = 100000;
    std::mt19937 gen(73467345);
    std::uniform_int_distribution<uint32_t> dist(0, kMaxVal);
    std::vector<uint32_t> data;
    data.reserve(num_elements);
    for (int i = 0; i < num_elements; ++i) {
        data.push_back(dist(gen));
    }
    FuzzySearch f{data};
    std::sort(data.begin(), data.end());

    auto is_close = [](uint32_t a, uint32_t b) {
        auto x = static_cast<uint64_t>(a);
        auto y = static_cast<uint64_t>(b);
        if (x > y) {
            std::swap(x, y);
        }
        return x * 2 >= y;
    };
    for (int i = 0; i < num_queries; ++i) {
        uint32_t x = dist(gen);
        auto res = f.Find(x);
        bool ans_exists = false;
        auto it = std::lower_bound(data.begin(), data.end(), x);
        if (it != data.end() && is_close(*it, x)) {
            ans_exists = true;
        }
        if (it != data.begin() && is_close(*(--it), x)) {
            ans_exists = true;
        }
        if (!ans_exists) {
            REQUIRE(!res.has_value());
        } else {
            REQUIRE(res.has_value());
            REQUIRE(std::binary_search(data.begin(), data.end(), *res));
        }
    }
}
