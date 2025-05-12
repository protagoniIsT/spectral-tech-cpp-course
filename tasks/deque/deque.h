#pragma once

#include <initializer_list>
#include <algorithm>

class Deque {
public:

    Deque() : storage_(),
              head_index_(0),
              tail_block_(0), tail_index_(0),
              size_(0) {}

    Deque(std::initializer_list<int> initializer_list) {
        size_ = initializer_list.size();
        size_t blocks_needed = std::ceil(static_cast<double>(size_) / BLOCK_SIZE);

        for (size_t i = 0; i < blocks_needed; ++i) {
            storage_.push_back(new std::array<int, BLOCK_SIZE>);
        }

        head_index_ = 0;
        tail_block_ = blocks_needed - 1;
        tail_index_ = size_ % BLOCK_SIZE;
        if (tail_index_ == 0 && size_ != 0) {
            tail_index_ = BLOCK_SIZE;
            ++tail_block_;
        }

        size_t curr_block = 0;
        size_t curr_index = 0;
        for (int value : initializer_list) {
            (*storage_[curr_block])[curr_index] = value;
            ++curr_index;
            if (curr_index >= BLOCK_SIZE) {
                curr_index = 0;
                ++curr_block;
            }
        }
    }

    Deque(const Deque& other)
        : storage_(),
          head_index_(other.head_index_),
          tail_block_(other.tail_block_),
          tail_index_(other.tail_index_),
          size_(other.size_)
    {
        if (other.Empty()) {
            head_index_ = tail_index_ = tail_block_ = 0;
            return;
        }
        storage_.resize(other.storage_.size());
        for (size_t i = 0; i < other.storage_.size(); ++i) {
            storage_[i] = new std::array<int, BLOCK_SIZE>(*other.storage_[i]);
        }
    }

    Deque(Deque&& other) noexcept
        : storage_(std::move(other.storage_)),
          head_index_(other.head_index_),
          tail_block_(other.tail_block_),
          tail_index_(other.tail_index_),
          size_(other.size_) {
        other.head_index_ = 0;
        other.tail_block_ = 0;
        other.tail_index_ = 0;
        other.size_ = 0;
    }

    Deque& operator=(const Deque& other) {
        if (this != &other) {
            for (auto* block : storage_) {
                delete block;
            }
            storage_.clear();

            storage_.resize(other.storage_.size());
            for (size_t i = 0; i < other.storage_.size(); ++i) {
                storage_[i] = new std::array<int, BLOCK_SIZE>(*other.storage_[i]);
            }

            head_index_ = other.head_index_;
            tail_block_ = other.tail_block_;
            tail_index_ = other.tail_index_;
            size_ = other.size_;
        }
        return *this;
    }

    ~Deque() {
        for (auto* block : storage_) {
            delete block;
        }
        storage_.clear();
    }

    void PushBack(int value) {
        if (tail_block_ >= storage_.size() || storage_[tail_block_] == nullptr) {
            storage_.push_back(new std::array<int, BLOCK_SIZE>);
        }
        (*storage_[tail_block_])[tail_index_] = value;
        ++tail_index_;
        if (tail_index_ >= BLOCK_SIZE) {
            tail_index_ = 0;
            ++tail_block_;
        }
        ++size_;
    }

    int PopBack() {
        if (Empty()) {
            throw std::out_of_range("Could not pop back element: deque is empty");
        }

        if (tail_index_ == 0) {
            tail_index_ = BLOCK_SIZE - 1;
            --tail_block_;
        } else {
            --tail_index_;
        }

        if (tail_block_ + 1 == storage_.size() - 1) {
            delete storage_.back();
            storage_.pop_back();
        }

        int ret_val = (*storage_[tail_block_])[tail_index_];
        --size_;

        return ret_val;
    }

    void PushFront(int value) {
        if (head_index_ == 0) {
            storage_.insert(storage_.begin(), new std::array<int, BLOCK_SIZE>);
            head_index_ = BLOCK_SIZE;
            ++tail_block_;
        }
        --head_index_;
        (*storage_[0])[head_index_] = value;
        ++size_;
    }

    int PopFront() {
        if (Empty()) {
            throw std::out_of_range("Could not pop front element: deque is empty");
        }

        int ret_val = (*storage_[0])[head_index_];
        ++head_index_;
        --size_;

        if (head_index_ >= BLOCK_SIZE) {
            head_index_ = 0;
            delete storage_.front();
            storage_.erase(storage_.begin());
            --tail_block_;
        }

        return ret_val;
    }

    int& operator[](size_t index) {
        if (index >= size_) {
            throw std::out_of_range("Specified index is out of range");
        }
        std::pair<size_t, size_t> indexes = GetIndexes(index);
        return (*storage_[indexes.first])[indexes.second];
    }

    const int& operator[](size_t index) const {
        if (index >= size_) {
            throw std::out_of_range("Specified index is out of range");
        }
        std::pair<size_t, size_t> indexes = GetIndexes(index);
        return (*storage_[indexes.first])[indexes.second];
    }

    size_t Size() const {
        return size_;
    }

    bool Empty() const {
        return Size() == 0;
    }

    void Clear() {
        head_index_ = 0;
        tail_block_ = 0;
        tail_index_ = 0;
        size_ = 0;
    }

    void Swap(Deque& other) {
        std::swap(storage_, other.storage_);
        std::swap(head_index_, other.head_index_);
        std::swap(tail_block_, other.tail_block_);
        std::swap(tail_index_, other.tail_index_);
        std::swap(size_, other.size_);
    }

private:
    static constexpr size_t BLOCK_BYTES = 512;
    static constexpr size_t BLOCK_SIZE = BLOCK_BYTES / sizeof(int);

    std::vector<std::array<int, BLOCK_SIZE>*> storage_;

    size_t head_index_;
    size_t tail_block_, tail_index_;
    size_t size_;

    std::pair<size_t, size_t> GetIndexes(size_t index) const {
        size_t offset = head_index_ + index;
        size_t block = offset / BLOCK_SIZE;
        size_t idx = offset % BLOCK_SIZE;
        return {block, idx};
    }
};
