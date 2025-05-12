#include <catch.hpp>
#include <reverse_vector.h>
#include <vector>

TEST_CASE("basic") {
    std::vector<int> test{4, 7838, -4, 7, 12, 19};
    auto cpy = test;
    ReverseVector rev(test);
    REQUIRE(cpy.size() == test.size());
    for (int i = 0; i < cpy.size(); ++i) {
        REQUIRE(cpy[cpy.size() - i - 1] == test[-i-1]);
    }
}

struct Counter {
    int x;
    static int destructed;

    ~Counter() {
        ++destructed;
    }
};
int Counter::destructed = 0;

TEST_CASE("destructor") {
    const int count = 1000;
    {
        std::vector<Counter> data;
        for (int i = 0; i < count; ++i) {
            data.push_back(Counter{i});
        }

        Counter::destructed = 0;
        ReverseVector rev(data);
        REQUIRE(data.size() == count);
        for (int i = 1; i <= count; ++i) {
            REQUIRE(data[-i].x == count - i);
        }
    }
    REQUIRE(Counter::destructed == count);
}

TEST_CASE("restore") {
    std::vector<std::string> test{"aba", "caba"};
    {
        ReverseVector rev(test);
        REQUIRE(test[-1] == "caba");
    }
    REQUIRE(test[1] == "caba");
}
