#pragma once

#include <vector>
#include <thread>
#include <algorithm>
#include <iterator>

template <class RandomAccessIterator, class T, class Func>
T Reduce(RandomAccessIterator first, RandomAccessIterator last, const T& initial_value, Func func) {
    auto length = std::distance(first, last);
    if (length == 0) return initial_value;

    unsigned int thread_cnt = std::min(std::thread::hardware_concurrency() / 2, static_cast<unsigned int>(length));
    auto partition_size = length / thread_cnt;
    auto remain = length % thread_cnt;

    std::vector<T> partial_results(thread_cnt);
    std::vector<std::thread> workers;
    workers.reserve(thread_cnt);

    RandomAccessIterator start = first;

    for (unsigned int i = 0; i < thread_cnt; ++i) {
        auto block_size = partition_size + (i < remain ? 1 : 0);
        auto end = std::next(start, block_size);

        workers.emplace_back([i, start, end, &partial_results, &func, &initial_value]() {
            T accumulated;
            auto it = start;
            if (i == 0) {
                accumulated = initial_value;
            } else {
                accumulated = *it;
                ++it;
            }
            for (it; it != end; ++it) {
                accumulated = func(accumulated, *it);
            }
            partial_results[i] = accumulated;
        });

        start = end;
    }

    for (auto& w : workers) {
        w.join();
    }

    T res = partial_results[0];
    for (size_t i = 1; i < thread_cnt; ++i) {
        res = func(res, partial_results[i]);
    }

    return res;
}
