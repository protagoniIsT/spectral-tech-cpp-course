#include <catch.hpp>

#include <fixed_string.hpp>
#include <fixstr.h>
#include <iostream>

TEST_CASE("format") {
    static_assert(Format<"hello world"_cs>() == "hello world"_cs);
    static_assert(Format<"some {} simple {} {}{}"_cs, "really", "checks", "test", "test">() == "some really simple checks testtest"_cs);
    static_assert(Format<""_cs>() == ""_cs);
    static_assert(Format<"{}"_cs, "123">() == "123"_cs);
}

TEST_CASE("integral") {
    static_assert(IntToFixstr<-1>() == "-1"_cs);
    static_assert(IntToFixstr<0>() == "0"_cs);
    static_assert(IntToFixstr<17u>() == "17"_cs);
    static_assert(IntToFixstr<-1000000000000ll>() == "-1000000000000"_cs);
    static_assert(IntToFixstr<1234567890123456789ull>() == "1234567890123456789"_cs);
}
