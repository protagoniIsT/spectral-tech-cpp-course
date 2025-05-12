#include <short_view.h>
#include <catch.hpp>
#include <random>
#include <algorithm>

static_assert(sizeof(ShortView) == 8, "Incorrect ShortView object size");

TEST_CASE("single") {
    std::string data("some random text");
    std::string real(data.data() + 6, 7);
    ShortView view(data.data() + 6, 7);
    REQUIRE(view.Size() == 7);
    REQUIRE(view.ToStringView() == real);
}

TEST_CASE("multiple") {
    std::string data("just another test sample");
    size_t offset = 0;
    const char *ptr = data.data();
    for (int i = 0; i < 3; ++i) {
        uint16_t size = (i + 1) * 3;
        std::string real(data.data() + offset, size);
        ShortView view(ptr, size);
        REQUIRE(real == view.ToStringView());
        ptr = view.Data() + 3;
        offset += 3;
    }
}

TEST_CASE("big") {
    std::mt19937 gen(63474);
    std::vector<char> data(60000);
    for (auto& x : data) {
        std::uniform_int_distribution<char> dist(0, 127);
        x = dist(gen);
    }
    for (int i = 0; i < 100; ++i) {
        std::uniform_int_distribution<size_t> idx(0, data.size() - 1);
        size_t index = idx(gen);
        std::uniform_int_distribution<size_t> len(1, data.size() - index);
        size_t size = len(gen);
        std::vector<char> real(data.begin() + index, data.begin() + index + size);
        ShortView view(&data[0] + index, size);
        REQUIRE(std::equal(&real[0], &real[0] + size, view.Data(), view.Data() + size));
    }
}
