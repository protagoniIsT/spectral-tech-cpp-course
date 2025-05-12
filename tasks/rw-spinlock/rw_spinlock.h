#pragma once

#include <atomic>
#include <thread>

struct RWSpinLock {
public:
    void LockRead() {
        int expected;
        int desired;
        while (true) {
            expected = rw_counter.load();
            if (expected & WRITER_BIT_MASK) {
                continue ;
            }
            desired = expected + 2;
            if (rw_counter.compare_exchange_strong(expected, desired)) {
                break;
            }
        }
    }

    void UnlockRead() {
        rw_counter.fetch_sub(2);
    }

    void LockWrite() {
        int expected;
        do {
            expected = 0;
        } while (!rw_counter.compare_exchange_strong(expected, WRITER_BIT_MASK));
    }

    void UnlockWrite() {
        rw_counter.fetch_and(~WRITER_BIT_MASK);
    }

private:
    const int WRITER_BIT_MASK = 1;

    std::atomic<int> rw_counter{0};
};
