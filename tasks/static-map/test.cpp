#include <catch.hpp>
#include <strict_iterator.h>
#include <list>
#include <string>
#include <algorithm>
#include <static_map.h>

TEST_CASE("empty") {
    StaticMap<std::string, double> map({});

    std::string value;
    REQUIRE(nullptr == map.Find("key"));
}

TEST_CASE("small") {
    StaticMap<std::string, int> map({{"b", 1}, {"a", 2}});

    REQUIRE(1 == *map.Find("b"));
    REQUIRE(2 == *map.Find("a"));

    REQUIRE(nullptr == map.Find("c"));
}

TEST_CASE("iterators") {
    std::list<std::pair<int, int>> l{{2, 1}, {4, 3}, {6, 5}};
    StaticMap<int, int> m(MakeStrict(l.begin(), l.begin(), l.end()), MakeStrict(l.begin(), l.end(), l.end()));
    REQUIRE(1 == *m.Find(2));
    REQUIRE(3 == *m.Find(4));
    REQUIRE(5 == *m.Find(6));
    REQUIRE(nullptr == m.Find(3));
    REQUIRE(nullptr == m.Find(0));
    REQUIRE(nullptr == m.Find(8));
}

struct MyKey {
    int x;
    bool operator<(MyKey rhs) const {
        return x < rhs.x;
    }
};

struct MyValue {
    int x;
};

TEST_CASE("operators") {
    StaticMap<MyKey, MyValue> m({{MyKey{1}, MyValue{7}}, {MyKey{-1}, MyValue{-1}}, {MyKey{0}, MyValue{13}}});
    REQUIRE(-1 == m.Find(MyKey{-1})->x);
    REQUIRE(nullptr == m.Find(MyKey{2}));
    REQUIRE(7 == m.Find(MyKey{1})->x);
    REQUIRE(13 == m.Find(MyKey{0})->x);
}

TEST_CASE("stress") {
    const int num_elements = 100000;

    std::vector<std::pair<int, int64_t>> items;
    items.reserve(num_elements);
    for (int i = 0; i < num_elements; ++i) {
        items.emplace_back(i, static_cast<int64_t>(i) * i);
    }

    std::mt19937 gen(73467345);
    std::shuffle(items.begin(), items.end(), gen);
    StaticMap<int, int64_t> map(items);

    for (int i = 0; i < num_elements; ++i) {
        int64_t value = static_cast<int64_t>(i) * i;
        REQUIRE(value == *map.Find(i));
    }
}
