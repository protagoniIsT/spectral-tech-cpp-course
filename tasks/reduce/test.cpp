#include <gtest/gtest.h>
#include <reduce.h>
#include <vector>
#include <algorithm>
#include "commons.h"

TEST(Correctness, Empty) {
    std::vector<int> values{1, 2, 3};
    ASSERT_EQ(6,
              Reduce(values.begin(), values.end(), 0, [](int sum, int cur) { return sum + cur; }));

    std::vector<int> empty;
    ASSERT_EQ(0, Reduce(empty.begin(), empty.end(), 0, Summator<int>()));

    std::vector<int> one{1};
    ASSERT_EQ(1, Reduce(one.begin(), one.end(), 0, Summator<int>()));

    std::vector<int> two{1, 2};
    ASSERT_EQ(3, Reduce(two.begin(), two.end(), 0, Summator<int>()));
}

TEST(Correctness, SimpleTest) {
    std::vector<uint32_t> lst(GenTest(1000));

    auto func = [](uint32_t cur, uint32_t next) { return cur + next; };
    ASSERT_EQ(std::accumulate(lst.begin(), lst.end(), 0, func),
              Reduce(lst.begin(), lst.end(), 0, func));
}
