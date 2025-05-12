#include <catch.hpp>
#include <avx_matmul.h>

#include <vector>
#include <cstring>
#include <random>

template<int M, int N, int K>
void ReferenceMatmul(const float *a, const float *b, float *c) {
    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < N; ++j) {
            c[i * N + j] = 0.0;
            for (int k = 0; k < K; ++k) {
                c[i * N + j] += a[i * K + k] * b[j * K + k];
            }
        }
    }
}

template<int M, int N>
void GenMatrix(float *data, std::mt19937& gen) {
    std::normal_distribution dist;
    for (int i = 0; i < M * N; ++i) {
        data[i] = dist(gen);
    }
}

template<int M, int N>
bool CheckIfClose(const float *a, const float *b, const float eps = 1e-5) {
    for (int i = 0; i < M * N; ++i) {
        if (std::abs(a[i] - b[i]) >= eps) {
            return false;
        }
    }
    return true;
}

#define RUN_TEST(M, N, K) do { \
    std::vector<float> a(M * K), b(N * K), c(M * N, 0.0), correct(M * N, 0.0);\
    std::mt19937 gen(3746);\
    GenMatrix<M, K>(a.data(), gen);\
    GenMatrix<N, K>(b.data(), gen);\
    ReferenceMatmul<M, N, K>(a.data(), b.data(), correct.data());\
    AvxMatmul<M, N, K>(a.data(), b.data(), c.data());\
    REQUIRE(CheckIfClose<M, N>(c.data(), correct.data()));\
} while (false)

TEST_CASE("block") {
    RUN_TEST(8, 8, 8);
}

TEST_CASE("multiple_blocks") {
    RUN_TEST(32, 32, 32);
}

TEST_CASE("tails") {
    RUN_TEST(17, 23, 37);
}

#ifdef NDEBUG
TEST_CASE("big") {
    RUN_TEST(1024, 1024, 1024);
}
#endif
