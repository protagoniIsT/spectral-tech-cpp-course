#include <catch.hpp>
#include <tuple.h>

#include <type_traits>
#include <string>

struct Int {
    int x;
};

TEST_CASE("head_tail") {
    using t = std::tuple<int, double, std::string>;
    static_assert(std::is_same_v<Head<t>, int>);
    static_assert(std::is_same_v<Tail<t>, std::tuple<double, std::string>>);
    using e = std::tuple<Int>;
    static_assert(std::is_same_v<Int, Head<e>>);
    static_assert(std::is_same_v<Tail<e>, std::tuple<>>);
    static_assert(std::is_same_v<Tail<std::tuple<>>, std::tuple<>>);
}

TEST_CASE("concat") {
    static_assert(std::is_same_v<Concat<std::tuple<>>, std::tuple<>>);
    static_assert(std::is_same_v<Concat<std::tuple<int>>, std::tuple<int>>);
    static_assert(std::is_same_v<Concat<std::tuple<int, double>, std::tuple<int>>, std::tuple<int, double, int>>);
    static_assert(std::is_same_v<Concat<
        std::tuple<std::string>, std::tuple<>, std::tuple<int, int>, std::tuple<Int>, std::tuple<Int, Int, int>>,
        std::tuple<std::string, int, int, Int, Int, Int, int>>);
}

TEST_CASE("transform") {
    static_assert(std::is_same_v<Transform<std::tuple<>, std::make_signed>, std::tuple<>>);
    static_assert(std::is_same_v<Transform<std::tuple<int, Int&, const double&, int[4]>, std::decay>,
        std::tuple<int, Int, double, int*>>);
}

TEST_CASE("interleave") {
    static_assert(std::is_same_v<Interleave<std::tuple<>, std::tuple<>>, std::tuple<>>);
    static_assert(std::is_same_v<Interleave<std::tuple<int, double>, std::tuple<Int>>, std::tuple<int, Int, double>>);
    static_assert(std::is_same_v<Interleave<std::tuple<std::string, Int, Int>, std::tuple<int, float, int, float>>,
        std::tuple<std::string, int, Int, float, Int, int, float>>);
}
