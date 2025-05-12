#include <catch.hpp>
#include <sse_search.h>

#include <vector>
#include <cstring>
#include <random>

class CollectAll {
public:
    explicit CollectAll(std::vector<size_t>& matches) : matches_(matches) {}
    bool operator()(size_t pos) {
        matches_.push_back(pos);
        return true;
    }
private:
    std::vector<size_t>& matches_;
};

bool Dummy(size_t) {
    return true;
}

TEST_CASE("simple") {
    std::vector<size_t> m;
    uint64_t needle = 0x1a2b3c4dd4c3b2a1;
    uint8_t data[16] = {0x83, 0x73, 0x12, 0xa1, 0xb2, 0xc3, 0xd4, 0x4d, 0x3c, 0x2b, 0x1a, 0x0, 0x19, 0x17, 0x66, 0xaa};
    REQUIRE(1 == SseSearch(data, 16, needle, CollectAll(m)));
    REQUIRE(m == std::vector<size_t>{3});
}

TEST_CASE("corner") {
    uint64_t needle = 0;
    {
        uint8_t data[3] = {0};
        REQUIRE(0 == SseSearch(data, 3, needle, Dummy));
    }
    {
        uint8_t data[8] = {0};
        std::vector<size_t> m;
        REQUIRE(1 == SseSearch(data, 8, needle, CollectAll(m)));
        REQUIRE(m == std::vector<size_t>{0});
    }
    {
        uint8_t data[9] = {0};
        std::memset(data + 1, -1, 8);
        --needle;
        std::vector<size_t> m;
        REQUIRE(1 == SseSearch(data, 9, needle, CollectAll(m)));
        REQUIRE(m == std::vector<size_t>{1});
    }
}

TEST_CASE("corner2") {
    std::vector<uint8_t> data(17);
    for (int i = 0; i < 17; ++i) {
        data[i] = i % 8;
    }
    uint64_t needle = 0x0007060504030201;
    std::vector<size_t> m;
    REQUIRE(2 == SseSearch(data.data(), 17, needle, CollectAll(m)));
    REQUIRE(m == std::vector<size_t>{1, 9});
}

TEST_CASE("stop") {
    uint8_t data[28] = {0};
    uint64_t needle = 0;
    size_t cnt = 0;
    auto matches = SseSearch(data, sizeof(data), needle, [&cnt](size_t pos) {
        REQUIRE(pos == cnt++);
        return cnt < 2;
    });
    REQUIRE(matches == 2);
}

TEST_CASE("big") {
    std::mt19937_64 gen(637573);
    std::vector<uint8_t> data;
    uint64_t needle = gen();
    std::uniform_real_distribution<double> prob;
    std::uniform_int_distribution<uint8_t> bt;
    std::vector<size_t> result;
    uint8_t *needle_begin = reinterpret_cast<uint8_t*>(&needle);
    for (int i = 0; i < 10000; ++i) {
        double pr = prob(gen);
        if (pr < 0.3) {
            result.push_back(data.size());
            data.insert(data.end(), needle_begin, needle_begin + 8);
        } else {
            data.push_back(bt(gen));
        }
    }
    std::vector<size_t> m;
    REQUIRE(result.size() == SseSearch(data.data(), data.size(), needle, CollectAll(m)));
    REQUIRE(m == result);
}
