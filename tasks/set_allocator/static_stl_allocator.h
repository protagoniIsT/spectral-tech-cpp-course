#pragma once

#include <set>

template <class T>
class StaticStlAllocator : public std::allocator<T> {
public:
    explicit StaticStlAllocator(char* mempool, size_t size, size_t offset = 0) : mempool_(mempool), size_(size), offset_(offset)
    {}

    StaticStlAllocator(const StaticStlAllocator&) = default;
    StaticStlAllocator& operator=(const StaticStlAllocator&) = default;

    template <class U>
    StaticStlAllocator(const StaticStlAllocator<U>& other) : StaticStlAllocator(other.mempool_, other.size_, other.offset_) {}

    T* allocate(size_t count) {
        size_t bytes_to_alloc = count * sizeof(T);
        if (offset_ + bytes_to_alloc > size_) {
            throw std::bad_alloc();
        }
        T* p = reinterpret_cast<T *>(mempool_ + offset_);
        offset_ += bytes_to_alloc;
        return p;
    }

    void deallocate(T* ptr, size_t) {
        // do nothing
    }

    template<class U, class... Args>
    void construct(U* p, Args&&... args) {
        new (p) U(std::forward<Args>(args)...);
    }

    template<class U>
    void destroy(U* p) {
        p->~U();
    }

    template <class U>
    struct rebind {
        using other = StaticStlAllocator<U>;
    };

private:
    template <class U>
    friend class StaticStlAllocator;

    char* mempool_;
    size_t size_;
    size_t offset_;
};

template <class T, class Compare = std::less<T>>
using Set = std::set<T, Compare, StaticStlAllocator<T>>;
