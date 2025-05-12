#pragma once

#include <vector>

template<class T>
class StaticAllocator {
public:
    explicit StaticAllocator(size_t count)
        : storage_(count), active_(count, false), count_(count)
    {
        free_indexes_.reserve(count_);
        for (size_t i = 0; i < count_; i++) {
            free_indexes_.push_back(count_ - i - 1);
        }
    }

    template<class... Args>
    T* New(Args&&... args) {
        if (free_indexes_.empty()) return nullptr;

        size_t insertion_idx = free_indexes_.back();
        free_indexes_.pop_back();
        active_[insertion_idx] = true;

        return new (&storage_[insertion_idx]) T(std::forward<Args>(args)...);
    }

    void Delete(T* obj) {
        if (obj == nullptr) return;

        size_t obj_index = reinterpret_cast<storage_type *>(obj) - storage_.data();
        obj->~T();
        active_[obj_index] = false;
        free_indexes_.push_back(obj_index);
    }

    ~StaticAllocator() {
        for (size_t i = 0; i < count_; i++) {
            if (active_[i]) {
                T* obj = reinterpret_cast<T*>(&storage_[i]);
                obj->~T();
            }
        }
    }

private:
    using storage_type = typename std::aligned_storage<sizeof(T), alignof(T)>::type;

    std::vector<storage_type> storage_;
    std::vector<size_t> free_indexes_;
    std::vector<bool> active_;
    size_t count_;
};
