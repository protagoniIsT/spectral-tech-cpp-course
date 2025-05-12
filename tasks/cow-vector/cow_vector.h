#pragma once

class COWVector {
public:
    using value_type = std::string;
    using storage_type = std::vector<value_type>;

    COWVector() : data_(new Data{new storage_type, new size_t(1)}) {};

    COWVector(const COWVector& other) : data_(other.data_) {
        ++*(data_->ref_count_);
    };

    COWVector(COWVector&& other) noexcept : data_(other.data_) {
        other.data_ = new Data{new storage_type, new size_t(1)};
    };

    COWVector& operator=(const COWVector& other) {
        if (this != &other) {
            COWVector temp(other);
            swap(temp);
        }
        return *this;
    };

    COWVector& operator=(COWVector&& other) noexcept {
        if (this != &other) {
            clear();
            data_ = other.data_;
            other.data_ = new Data{new storage_type, new size_t(1)};
        }
        return *this;
    }

    ~COWVector() {
        clear();
    };

    size_t Size() const {
        return data_->storage_->size();
    };

    void Resize(size_t size) {
        detach();
        data_->storage_->resize(size);
    };

    const value_type& Get(size_t at) const {
        return data_->storage_->at(at);
    };

    const value_type& Back() const noexcept {
        return (*(data_->storage_)).back();
    };

    void PushBack(const value_type& value) {
        detach();
        data_->storage_->push_back(value);
    };

    void Set(size_t at, const value_type& value) {
        detach();
        (*(data_->storage_))[at] = value;
    };

    size_t RefCount() const {
        return *(data_->ref_count_);
    };

private:
    struct Data {
        storage_type* storage_;
        size_t* ref_count_;
    };

    Data* data_;

    void swap(COWVector& other) {
        std::swap(data_, other.data_);
    }

    void detach() {
        if (*(data_->ref_count_) > 1) {
            --*(data_->ref_count_);
            data_ = new Data{new storage_type(*(data_->storage_)), new size_t(1)};
        }
    }

    void clear() {
        if (--*(data_->ref_count_) == 0) {
            delete data_->storage_;
            delete data_->ref_count_;
            delete data_;
        }
    }
};
