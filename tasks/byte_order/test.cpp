#include <catch.hpp>
#include <byte_order.h>

TEST_CASE("int32") {
    REQUIRE(0x12345678 == ChangeByteOrder(0x78563412));
    REQUIRE(0x01000013u == ChangeByteOrder(0x13000001u));
}

TEST_CASE("int64") {
    REQUIRE(0x1122334455667788ll == ChangeByteOrder(0x8877665544332211ll));
    REQUIRE(0xaabbccddeeff1234ull == ChangeByteOrder(0x3412ffeeddccbbaaull));
}

TEST_CASE("int16") {
    int16_t value = 17239;
    int16_t rev_value = 22339;
    REQUIRE(rev_value == ChangeByteOrder(value));
}

TEST_CASE("int8") {
    uint8_t value     = 0b10011011;
    uint8_t rev_value = 0b10011011;
    REQUIRE(rev_value == ChangeByteOrder(value));
}
