#pragma once

#include <algorithm>

template<typename T>
class VectorBase;

template<typename T, size_t N>
struct StackStorage {
    alignas(T) char data[N * sizeof(T)];
};

template <class T, typename = void>
struct SmallVectorAlignmentAndOffset {
    alignas(VectorBase<T>) char Base[sizeof(VectorBase<T>)];
    alignas(T) char FirstEl[sizeof(T)];
};

template<typename T>
class VectorBase {
protected:
    VectorBase() : start_(nullptr), end_(nullptr), end_buffer_(nullptr) {}

public:

    VectorBase(const VectorBase& rhs) = delete;
    VectorBase(VectorBase&&) = delete;
    VectorBase& operator=(VectorBase&&) = delete;
    VectorBase& operator=(const VectorBase&) = delete;

    ~VectorBase() {
        clear();
        if (!is_small()) {
            ::operator delete(start_);
        }
    }

    void clear() {
        for (T* p = start_; p != end_; ++p) {
            p->~T();
        }
        end_ = start_;
    }

    bool is_small() const {
        return start_ == getFirstEl();
    }

    T& operator[](size_t idx) {
        return start_[idx];
    }

    const T& operator[](size_t idx) const {
        return start_[idx];
    }

    size_t size() const {
        return end_ - start_;
    }

    size_t capacity() const {
        return end_buffer_ - start_;
    }

    bool empty() const {
        return size() == 0;
    }

    void push_back(T&& elem) {
        pushBackPf(std::move(elem));
    }

    void push_back(const T& elem) {
        pushBackPf(elem);
    }

    void pop_back() {
        if (empty()) {
            throw std::out_of_range("Cannot pop back element: vector is empty");
        }
        --end_;
        end_->~T();
    }

protected:
    T* start_;
    T* end_;
    T* end_buffer_;

    T* allocateSz(size_t sz) const {
        return static_cast<T*>(::operator new(sz * sizeof(T)));
    }

    void* getFirstEl() const {
        return const_cast<void*>(reinterpret_cast<const void *>(
            reinterpret_cast<const char*>(this) +
            offsetof(SmallVectorAlignmentAndOffset<T>, FirstEl)));
    }

    void reassignStorage(T* new_start, T* new_end, T* new_end_buffer) {
        start_ = new_start;
        end_ = new_end;
        end_buffer_ = new_end_buffer;
    }

    void copyRange(size_t from, size_t to, const VectorBase& vec) {
        for (size_t i = from; i < to; ++i) {
            this->push_back(vec[i]);
        }
    }

    void moveRange(size_t from, size_t to, VectorBase& vec) {
        for (size_t i = from; i < to; ++i) {
            this->push_back(std::move(vec[i]));
        }
        vec.clear();
    }

private:
    static constexpr size_t GROWTH_CONST = 2;

    template<typename U>
    void pushBackPf(U&& elem) {
        if (end_ < end_buffer_) {
            new (end_) T(std::forward<U>(elem));
            ++end_;
            return;
        }

        size_t old_size = size();
        size_t old_capacity = capacity();
        size_t new_capacity = old_capacity > 0 ? old_capacity * GROWTH_CONST : 1;

        T* new_storage = allocateSz(new_capacity);

        for (size_t i = 0; i < old_size; ++i) {
            new (new_storage + i) T(std::move(start_[i]));
            start_[i].~T();
        }

        new (new_storage + old_size) T(std::forward<U>(elem));

        if (!is_small()) {
            ::operator delete(start_);
        }

        start_ = new_storage;
        end_ = new_storage + old_size + 1;
        end_buffer_ = new_storage + new_capacity;
    }
};

template<typename T, size_t N>
class SmallVector : public VectorBase<T> {
public:
    SmallVector() {
        this->reassignStorage(getStackStorage(), getStackStorage(), getStackStorage() + N);
    }

    SmallVector(const SmallVector& other) {
        this->clear();
        reserveStorage(other.size());
        this->copyRange(0, other.size(), other);
    }

    SmallVector(SmallVector&& other) noexcept {
        if (other.is_small()) {
            this->reassignStorage(getStackStorage(), getStackStorage(), getStackStorage() + N);
            this->moveRange(0, other.size(), other);
        } else {
            this->reassignStorage(other.start_, other.end_, other.end_buffer_);
            other.reassignStorage(other.getStackStorage(),
                                  other.getStackStorage(),
                                  other.getStackStorage() + N);
        }
    }


    explicit SmallVector(size_t sz, const T& val = T()) {
        reserveStorage(sz);
        for (size_t i = 0; i < sz; ++i) {
            this->push_back(val);
        }
    }

    SmallVector(std::initializer_list<T> l) {
        size_t sz = l.size();
        reserveStorage(sz);
        for (T value : l) {
            this->push_back(value);
        }
    }

    template<typename Iter>
    SmallVector(Iter begin, Iter end) {
        size_t sz = std::distance(begin, end);
        reserveStorage(sz);
        for (Iter it = begin; it != end; ++it) {
            this->push_back(*it);
        }
    }

    SmallVector& operator=(const SmallVector& other) {
        if (this != &other) {
            this->clear();
            this->copyRange(0, other.size(), other);
        }
        return *this;
    }

    SmallVector& operator=(SmallVector&& other) noexcept {
        if (this != &other) {
            this->clear();
            if (!this->is_small()) {
                ::operator delete(this->start_);
            }
            if (other.is_small()) {
                this->reassignStorage(getStackStorage(), getStackStorage(), getStackStorage() + N);
                this->moveRange(0, other.size(), other);
            } else {
                this->reassignStorage(other.start_, other.end_, other.end_buffer_);
                other.reassignStorage(nullptr, nullptr, nullptr);
            }
        }
        return *this;
    }

    bool reset_small() {
        size_t curr_size = this->size();
        if (curr_size > N) return false;
        if (!this->is_small()) {
            for (size_t i = 0; i < curr_size; ++i) {
                getStackStorage()[i] = std::move(this->start_[i]);
                this->start_[i].~T();
            }
            ::operator delete(this->start_);
            this->reassignStorage(getStackStorage(), getStackStorage() + curr_size, getStackStorage() + N);
        }
        return true;
    }

    T* getStackStorage() {
        return reinterpret_cast<T*>(&data_.data);
    }

    const T* getStackStorage() const {
        return reinterpret_cast<const T*>(&data_.data);
    }

private:
    StackStorage<T, N> data_;

    void reserveStorage(size_t sz) {
        if (sz > N) {
            T* new_storage = this->allocateSz(sz);
            this->reassignStorage(new_storage, new_storage, new_storage + sz);
        } else {
            this->reassignStorage(getStackStorage(), getStackStorage(), getStackStorage() + N);
        }
    }
};
