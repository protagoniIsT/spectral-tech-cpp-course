#include <catch.hpp>
#include <avx_vector.h>
#include <random>

struct Startup {
    Startup() {
        AvxVector::init_masks();
    }
};

Startup startup;

TEST_CASE("simple") {
    AvxVector reg;
    for (int i = 0; i < 4; ++i) {
        reg.set(i, i + 1);
    }
    REQUIRE(reg.sum() == 10);
    reg.insert(1, 5);
    reg.erase(0);
    REQUIRE(reg.sum() == 14);
    alignas(64) int data[16];
    reg.unpack(data);
    REQUIRE(data[0] == 5);
    REQUIRE(data[1] == 2);
    REQUIRE(data[2] == 3);
    REQUIRE(data[3] == 4);
    reg.set(15, 3);
    REQUIRE(reg.sum() == 17);
    reg.insert(0, 2);
    REQUIRE(reg.sum() == 16);
}

TEST_CASE("stress") {
    alignas(64) int correct[16];
    for (int i = 0; i < 16; ++i) {
        correct[i] = i + 1;
    }
    AvxVector reg(correct);

    auto insert = [&reg, &correct](int idx, int elem) {
        reg.insert(idx, elem);
        for (int i = 15; i > idx; --i) {
            correct[i] = correct[i - 1];
        }
        correct[idx] = elem;
    };
    auto erase = [&reg, &correct](int idx) {
        reg.erase(idx);
        for (int i = idx; i < 15; ++i) {
            correct[i] = correct[i + 1];
        }
        correct[15] = 0;
    };
    auto set = [&reg, &correct](int idx, int elem) {
        reg.set(idx, elem);
        correct[idx] = elem;
    };

    std::mt19937 gen(274432);
    std::uniform_int_distribution<int> op_code(1, 3);
    std::uniform_int_distribution<int> idx_dist(0, 15);
    std::uniform_int_distribution<int> elem_dist(0, 10000);
    const int iter_count = 1e5;
    for (int i = 0; i < iter_count; ++i) {
        int op = op_code(gen);
        int idx = idx_dist(gen);
        int elem = elem_dist(gen);
        if (op == 1) {
            insert(idx, elem);
        } else if (op == 2) {
            erase(idx);
        } else {
            set(idx, elem);
        }

        if (i % 64 == 0) {
            alignas(64) int check[16];
            int sum = 0;
            reg.unpack(check);
            for (int i = 0; i < 16; ++i) {
                sum += correct[i];
                REQUIRE(check[i] == correct[i]);
            }
            REQUIRE(sum == reg.sum());
        }
    }
}
