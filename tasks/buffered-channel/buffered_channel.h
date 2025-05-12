#pragma once

#include <condition_variable>
#include <optional>
#include <deque>
#include <thread>
#include <utility>

template <class T>
class BufferedChannel {
public:
    explicit BufferedChannel(int size) : capacity(size) {}

    void Send(const T& value) {
        std::unique_lock<std::mutex> u_lock(lock);
        c_not_full.wait(u_lock, [&] {
            return channel.size() < capacity || is_closed;
        });
        if (is_closed) {
            throw std::runtime_error("Buffered channel is closed");
        }
        channel.push_back(value);
        c_not_empty.notify_one();
    }

    std::optional<T> Recv() {
        std::unique_lock<std::mutex> u_lock(lock);
        c_not_empty.wait(u_lock, [&] {
            return !channel.empty() || is_closed;
        });
        if (channel.empty()) {
            return std::nullopt;
        }
        T val = std::move(channel.front());
        channel.pop_front();
        c_not_full.notify_one();
        return val;
    }

    void Close() {
        {
            std::lock_guard<std::mutex> l_guard(lock);
            is_closed = true;
        }
        c_not_empty.notify_all();
        c_not_full.notify_all();
    }

private:
    int capacity;
    std::deque<T> channel;
    std::mutex lock;
    std::condition_variable c_not_empty;
    std::condition_variable c_not_full;
    bool is_closed = false;
};
