#pragma once

#include <vector>
#include <optional>
#include <cstdint>

class FuzzySearch {
public:
    explicit FuzzySearch(const std::vector<uint32_t>& items) {
        values_.resize(33, std::nullopt);
        for (uint32_t item : items) {
            size_t prev_deg;
            if (item == 0) {
                prev_deg = LEN_BITS;
            } else {
                prev_deg = prev_deg2(item);
            }
            if (!values_[prev_deg].has_value()) {
                values_[prev_deg].emplace(std::pair<uint32_t, uint32_t>(item, item));
            } else {
                uint32_t min = values_[prev_deg]->first;
                uint32_t max = values_[prev_deg]->second;
                values_[prev_deg].emplace(std::pair<uint32_t, uint32_t>(std::min(min, item), std::max(max, item)));
            }
        }
    }

    std::optional<uint32_t> Find(uint32_t x) const {
        size_t prev_deg;
        if (x == 0) {
            return values_[LEN_BITS].has_value() ? values_[LEN_BITS]->first : std::optional<uint32_t>{std::nullopt};
        } else {
            prev_deg = prev_deg2(x);
        }

        if (prev_deg > 0 && values_[prev_deg - 1].has_value()) {
            uint32_t second = values_[prev_deg - 1]->second;
            if (static_cast<uint64_t>(x) * 2ULL >= second && static_cast<uint64_t>(second) * 2ULL >= x) return second;
        }

        if (values_[prev_deg].has_value()) {
            uint32_t first = values_[prev_deg]->first;
            uint32_t second = values_[prev_deg]->second;
            if (static_cast<uint64_t>(x) * 2ULL >= first && static_cast<uint64_t>(first) * 2ULL >= x) return first;
            if (static_cast<uint64_t>(x) * 2ULL >= second && static_cast<uint64_t>(second) * 2ULL >= x) return second;
        }

        if (prev_deg < LEN_BITS && values_[prev_deg + 1].has_value()) {
            uint32_t first = values_[prev_deg + 1]->first;
            if (static_cast<uint64_t>(x) * 2ULL >= first && static_cast<uint64_t>(first) * 2ULL >= x) return first;
        }
        return std::nullopt;
    }

private:
    static constexpr size_t LEN_BITS = sizeof(uint32_t) * 8;

    std::vector<std::optional<std::pair<uint32_t, uint32_t>>> values_;

    static constexpr size_t prev_deg2(uint32_t x) {
        return LEN_BITS - __builtin_clz(x) - 1;
    }
};
