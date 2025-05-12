#include "threadpool.h"

ThreadPool::ThreadPool(size_t threads) : done(false) {
    for (size_t i = 0; i < threads; ++i) {
        workers.emplace_back([this] {
            while (true) {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> u_lock(lock);
                    cv.wait(u_lock, [&] {
                        return done || !tasks.empty();
                    });
                    if (done && tasks.empty()) return;
                    task = std::move(tasks.front());
                    tasks.pop();
                }
                task();
            }
        });
    }
}

ThreadPool::~ThreadPool() {
    {
        std::lock_guard<std::mutex> u_lock(lock);
        done = true;
    }
    cv.notify_all();
    for (auto &w: workers) w.join();
}

void ThreadPool::submit(std::function<void()> task) {
    {
        std::lock_guard<std::mutex> l_guard(lock);
        tasks.push(std::move(task));
    }
    cv.notify_one();
}
