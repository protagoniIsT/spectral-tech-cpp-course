#pragma once

#include <string>
#include <cstdint>
#include <string_view>

class ShortView {
public:
    ShortView(const char *data, uint16_t size) {
        uint64_t expanded_ptr = reinterpret_cast<uint64_t>(data);
        expanded_ptr = (static_cast<uint64_t>(size) << 48) | (static_cast<uint64_t>(expanded_ptr));
        data_ = reinterpret_cast<const char *>(expanded_ptr);
    }

    size_t Size() const {
        return reinterpret_cast<uint64_t>(data_) >> 48;
    }

    const char *Data() const {
        uint64_t p = reinterpret_cast<uint64_t>(data_) & MASK_ALL_ONES;
        if (p & (1ULL << 47)) {
            p |= MASK_FIRST16_ONES;
        }
        return reinterpret_cast<const char *>(p);
    }

    std::string_view ToStringView() const {
        return {Data(), Size()};
    }

private:
    const char *data_;
    static constexpr uint64_t MASK_ALL_ONES = 0xFFFFFFFFFFFFull;
    static constexpr uint64_t MASK_FIRST16_ONES = 0xFFFF000000000000ull;
};
