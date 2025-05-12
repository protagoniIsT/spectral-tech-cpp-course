#include "matutil.h"

void transpose(const uint32_t* mat, uint32_t* t_mat, int m) {
    for (size_t i = 0; i < m; ++i) {
        for (size_t j = 0; j < m; ++j) {
            t_mat[j * m + i] = mat[i * m + j];
        }
    }
}

void mat_multiply(const uint32_t* mat1, const uint32_t* mat2, 
                            std::vector<uint32_t>& transpose_buff, uint32_t* result, int m,
                                    ThreadPool& th_pool, unsigned int threads, size_t rows_per_thread) {
    transpose(mat2, transpose_buff.data(), m);

    auto worker = [&](size_t row_start, size_t row_end) {
        for (size_t i = row_start; i < row_end; ++i) {
            for (size_t j = 0; j < static_cast<size_t>(m); ++j) {
                uint64_t s = 0;
                const uint32_t* row_a = mat1 + i * m;
                const uint32_t* row_b = transpose_buff.data() + j * m;
                for (size_t k = 0; k < m; ++k) {
                    s += static_cast<uint64_t>(row_a[k]) * row_b[k];
                }
                result[i * m + j] = static_cast<uint32_t>(s);
            }
        }
    };

    std::atomic<int> tasks_left{static_cast<int>(threads)};

    for (unsigned int th = 0; th < threads; ++th) {
        size_t row_start = th * rows_per_thread;
        size_t row_end = std::min(row_start + rows_per_thread, static_cast<size_t>(m));
        auto w = [row_start, row_end, &tasks_left, &worker] { 
            worker(row_start, row_end);
            tasks_left.fetch_sub(1); 
        };
        th_pool.submit(std::move(w));
    }

    while (tasks_left.load() > 0) {
        std::this_thread::yield();
    }
}

void mat_pow(const uint32_t* mat, uint32_t* result, int m, int n) {
    unsigned int hw_threads = std::thread::hardware_concurrency();
    unsigned int threads = (m >= 300 && hw_threads > 2) ? hw_threads / 2 - 1 : 1;
    size_t rows_per_thread = (m + threads - 1) / threads;

    ThreadPool thread_pool(threads);

    std::vector<uint32_t> temp(m * m);
    memset(result, 0, m * m * sizeof(uint32_t));
    for (size_t i = 0; i < m; ++i) {
        result[i * m + i] = 1;
    }
    std::vector<uint32_t> transpose_buff(m * m);
    std::vector<uint32_t> base(mat, mat + m * m);
    while (n > 0) {
        if (n & 1) {
            mat_multiply(result, base.data(), transpose_buff, temp.data(), m, thread_pool, threads, rows_per_thread);
            memcpy(result, temp.data(), m * m * sizeof(uint32_t));
        }
        mat_multiply(base.data(), base.data(), transpose_buff, temp.data(), m, thread_pool, threads, rows_per_thread);
        memcpy(base.data(), temp.data(), m * m * sizeof(uint32_t));
        n >>= 1;
    }
}

