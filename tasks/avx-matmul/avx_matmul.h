#include <immintrin.h>
#include <cstdlib>

template<int M, int N, int K>
void AvxMatmul(const float* a, const float* raw_b, float *c) {
    constexpr int blocks = (N + 7) / 8;
    auto b_t = static_cast<float*>(aligned_alloc(32, blocks * K * 8 * sizeof(float)));
    for (int b = 0; b < blocks; ++b) {
        for (int k = 0; k < K; ++k) {
            for (int t = 0; t < 8; ++t) {
                int j = b * 8 + t;
                b_t[(b * K + k) * 8 + t] = (j < N ? raw_b[j * K + k] : 0.0f);
            }
        }
    }
    for (int i = 0; i < M; ++i) {
        for (int j_outer = 0; j_outer < N; j_outer += 8) {
            __m256 sum = _mm256_setzero_ps();
            for (int k = 0; k < K; ++k) {
                __m256 val_a = _mm256_set1_ps(a[i * K + k]);
                __m256 val_b = _mm256_load_ps(&b_t[(j_outer / 8 * K + k) * 8]);
                sum = _mm256_add_ps(sum, _mm256_mul_ps(val_a, val_b));
            }
            int remain = N - j_outer;
            if (remain >= 8) {
                _mm256_storeu_ps(&c[i * N + j_outer], sum);
            } else {
                float tmp[8];
                _mm256_storeu_ps(tmp, sum);
                for (int t = 0; t < remain; ++t)
                    c[i * N + j_outer + t] = tmp[t];
            }
        }
    }
    free(b_t);
}
