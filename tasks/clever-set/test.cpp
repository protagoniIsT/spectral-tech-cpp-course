#include <vector>
#include <ctime>

#include <catch.hpp>
#include <clever_set.h>


TEST_CASE("simple") {
    CleverSet<int> s;
    s.insert(1);
    s.insert(2);
    s.insert(3);
    REQUIRE(!s.find(4));
    REQUIRE(s.find(1));
    REQUIRE(s.erase(2));
}

struct FirstHashable {
    int x;

    bool operator==(const FirstHashable& r) const {
        return x == r.x;
    }
};

#define MAKE_HASH(TYPE) template<>\
    struct hash<TYPE> {\
        size_t operator()(const TYPE& x) const {\
            return hasher(x.x);\
        }\
        std::hash<int> hasher;\
    }

namespace std {
    MAKE_HASH(FirstHashable);
}

struct SecondHashable {
    int x;

    bool operator==(const SecondHashable& r) const {
        return x == r.x;
    }
    bool operator!=(const SecondHashable& r) const {
        return x != r.x;
    }
    bool operator<(const SecondHashable& r) const {
        return x < r.x;
    }
};

namespace std {
    MAKE_HASH(SecondHashable);
}

struct ThirdHashable {
    int x;
};

bool operator >(const ThirdHashable& a, const ThirdHashable& b) {
    return a.x > b.x;
}

namespace std {
    MAKE_HASH(ThirdHashable);
}

struct FirstComparable {
    int x;

    bool operator<(const FirstComparable& r) const {
        return x < r.x;
    }
};

struct SecondComparable {
    int x;

    friend bool operator>(const SecondComparable& a, const SecondComparable& b) {
        return a.x > b.x;
    }
};

namespace std {
    MAKE_HASH(SecondComparable);
}

struct FirstBad {
    int x;
};

struct SecondBad {
    int x;

    bool operator==(const SecondBad& r) const {
        return x == r.x;
    }
};

struct ThirdBad {
    int x;
};

namespace std {
    MAKE_HASH(ThirdBad);
}

template<class T>
void TestCase() {
    CleverSet<T> s;
    s.insert(T{1});
    REQUIRE(s.find(T{1}));
    REQUIRE(!s.insert(T{1}));
    REQUIRE(s.insert(T{2}));
    REQUIRE(s.erase(T{2}));
    REQUIRE(!s.erase(T{0}));
}

template<class T>
void LongTest() {
    CleverSet<T> s;
    const int n = 100000;
    for (int i = 0; i < n; ++i)
        REQUIRE(s.insert(T{n - i - 1}));
    for (int i = 0; i < n; ++i)
        REQUIRE(s.find(T{i}));
}

template<class T>
void AddressTest() {
    CleverSet<T> s;
    const size_t n = 100;
    std::vector<T> data(n, T{1});
    for (size_t i = 0; i < n; ++i)
        REQUIRE(s.insert(data[i]));
    REQUIRE(n == s.size());

    CleverSet<T> t;
    T one{2};
    REQUIRE(t.insert(one));
    for (size_t i = 0; i < n; ++i)
        REQUIRE(!t.insert(one));
    REQUIRE(1u == t.size());
}


TEST_CASE("hashable") {
    TestCase<FirstHashable>();
    TestCase<SecondHashable>();
    TestCase<ThirdHashable>();
    LongTest<FirstHashable>();
}

TEST_CASE("comparable") {
    TestCase<FirstComparable>();
    TestCase<SecondComparable>();
    LongTest<FirstComparable>();
}

TEST_CASE("address") {
    AddressTest<FirstBad>();
    AddressTest<SecondBad>();
    AddressTest<ThirdBad>();
}
