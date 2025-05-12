#pragma once

template<class T>
class ReverseVector {
public:
    explicit ReverseVector(std::vector<T>& data) : data_(data) {
        auto* rev_v_meta = reinterpret_cast<RevVectorInternal *>(&data_);

        original_start_ = rev_v_meta->start_;
        original_end_ = rev_v_meta->end_;

        size_ = original_end_ - original_start_;

        rev_v_meta->start_ = original_end_;
        rev_v_meta->end_ = original_end_ + size_;
    }

    ~ReverseVector() {
        auto* rev_v_meta = reinterpret_cast<RevVectorInternal *>(&data_);
        rev_v_meta->start_ = original_start_;
        rev_v_meta->end_ = original_end_;
    }

private:
    std::vector<T>& data_;
    T* original_start_;
    T* original_end_;
    size_t size_;

    struct RevVectorInternal {
        T* start_;
        T* end_;
        T* end_capacity_;
    };
};
