#include <cstdio>
#include <cstdint>
#include <x86intrin.h>
#include <vector>

const int RUNS = 100;

int main() {
    const size_t N = 1 << 20;
    std::vector<int> a(N);
    std::vector<double> avg_cycles(9);
    size_t idx;

    for (size_t i = 0; i < RUNS; ++i) {
        idx = 0;
        for (size_t stride = 1; stride <= 256; stride <<= 1) {
            uint64_t t0 = __rdtsc();
            for (size_t i = 0; i < N; i += stride) {
                a[i] += 1;
            }
            uint64_t t1 = __rdtsc();
            double cycles = double(t1 - t0) / (N / stride);
            avg_cycles[idx] += cycles;
            ++idx;
        }
    }

    idx = 0;

    // csv output
    printf("stride,Avg cycles/access\n");
    for (size_t stride = 1; stride <= 256; stride <<= 1) {
        printf("%4zu,%.2f\n", stride * sizeof(int), avg_cycles[idx] / RUNS);
        ++idx;
    }
    return 0;
}
