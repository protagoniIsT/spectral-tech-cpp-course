#include <catch.hpp>

#include <string>
#include <vector>
#include <random>
#include <deque>

#include <deque.h>

#define CheckDeque(__actual, __expected) do { \
    auto actual = __actual;\
    auto expected = __expected;\
    REQUIRE(actual.Size() == expected.size()); \
    for (size_t i = 0; i < actual.Size(); ++i) { \
        REQUIRE(actual[i] == expected[i]); \
    }\
} while (false)

TEST_CASE("constructors") {
    {
        Deque a;
        REQUIRE(a.Size() == 0u);
    }
    {
        Deque a{1, 2, 3, 4};
        CheckDeque(a, (std::vector<int>{1, 2, 3, 4}));
    }
}

TEST_CASE("basic") {
    Deque a{1, 3, 5};
    CheckDeque(a, (std::vector<int>{1, 3, 5}));

    a.PopBack();
    CheckDeque(a, (std::vector<int>{1, 3}));
    a.PopFront();
    CheckDeque(a, (std::vector<int>{3}));
    a.PushFront(5);
    CheckDeque(a, (std::vector<int>{5, 3}));
    a.PushBack(1);
    CheckDeque(a, (std::vector<int>{5, 3, 1}));

    a.Clear();
    CheckDeque(a, (std::vector<int>()));

    a.PushBack(3);
    Deque b{2, 4};
    a.Swap(b);
    CheckDeque(a, (std::vector<int>{2, 4}));
    CheckDeque(b, (std::vector<int>{3}));
}

TEST_CASE("index") {
    Deque a{9, 1, 1};
    a[0] = 1;
    a[1] = 2;
    a[2] = 3;
    CheckDeque(a, (std::vector<int>{1, 2, 3}));
}

TEST_CASE("layout") {
    Deque a;
    for (int i = 0; i < 129; ++i) {
        a.PushBack(0);
    }
    for (int i = 0; i < 127; ++i) {
        REQUIRE(&a[i] + 1 == &a[i + 1]);
    }
}

TEST_CASE("realloc") {
    Deque a;
    const int iterations = 1e6;
    std::vector<int*> addr;
    for (int i = 0; i < iterations; ++i) {
        a.PushBack(i);
        addr.push_back(&a[i]);
    }

    for (int i = 0; i < iterations; ++i) {
        REQUIRE(*addr[i] == i);
    }
}

TEST_CASE("copy") {
    Deque a;
    Deque b(a);
    b.PushBack(1);
    CheckDeque(a, (std::vector<int>()));
    CheckDeque(b, (std::vector<int>{1}));

    b = b;
    CheckDeque(b, (std::vector<int>{1}));
    a = b;
    CheckDeque(a, (std::vector<int>{1}));

    b = std::move(a);
    CheckDeque(b, (std::vector<int>{1}));
    Deque c(std::move(b));
    CheckDeque(c, (std::vector<int>{1}));

    Deque d{3, 4, 5};
    Deque e(d);
    CheckDeque(e, (std::vector<int>{3, 4, 5}));
    d.Swap(c);
    CheckDeque(e, (std::vector<int>{3, 4, 5}));
    CheckDeque(d, (std::vector<int>{1}));
    CheckDeque(c, (std::vector<int>{3, 4, 5}));
}

TEST_CASE("stress") {
    const int iterations = 1e6;
    Deque a;
    std::deque<int> b;
    std::mt19937 gen(735675);
    std::uniform_int_distribution<int> dist(1, 5);

    for (int i = 0; i < iterations; ++i) {
        a.PushFront(i);
        b.push_front(i);
    }

    for (int i = 0; i < iterations; ++i) {
        int code = dist(gen);
        int value = gen();
        if (code == 1) {
            a.PushFront(value);
            b.push_front(value);
        } else if (code == 2) {
            a.PushBack(value);
            b.push_back(value);
        } else if (code == 3) {
            a.PopFront();
            b.pop_front();
        } else if (code == 4) {
            a.PopBack();
            b.pop_back();
        } else {
            int index = static_cast<int>(value % a.Size());
            REQUIRE(a[index] == b[index]);
        }
    }
}

TEST_CASE("empty") {
    const size_t test_size = 1e3;
    {
        Deque a;
        for (size_t idx = 0; idx < test_size; ++idx) {
            a.PushBack(idx);
        }
        for (size_t idx = 0; idx < test_size; ++idx) {
            a.PopBack();
        }
        REQUIRE(a.Size() == 0u);
        CheckDeque(a, (std::vector<int>()));
    }

    {
        Deque a;
        for (size_t idx = 0; idx < test_size; ++idx) {
            a.PushBack(idx);
        }
        for (size_t idx = 0; idx < test_size; ++idx) {
            a.PopFront();
        }
        REQUIRE(a.Size() == 0u);
        CheckDeque(a, (std::vector<int>()));
    }

    {
        Deque a;
        for (size_t idx = 0; idx < test_size; ++idx) {
            a.PushFront(idx);
        }
        for (size_t idx = 0; idx < test_size; ++idx) {
            a.PopBack();
        }
        REQUIRE(a.Size() == 0u);
        CheckDeque(a, (std::vector<int>()));
    }

    {
        Deque a;
        for (size_t idx = 0; idx < test_size; ++idx) {
            a.PushFront(idx);
        }
        for (size_t idx = 0; idx < test_size; ++idx) {
            a.PopFront();
        }
        REQUIRE(a.Size() == 0u);
        CheckDeque(a, (std::vector<int>()));
    }
}
