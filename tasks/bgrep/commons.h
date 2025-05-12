#pragma once

#include <vector>
#include <optional>

inline std::vector<uint8_t> ConvertToPlain(const std::vector<std::optional<uint8_t>>& pattern) {
    std::vector<uint8_t> res(pattern.size());
    for (size_t i = 0; i < res.size(); ++i) {
        if (pattern[i].has_value()) {
            res[i] = *pattern[i];
        }
    }
    return res;
}

class BaseVisitor {
public:
    void OnMatch(const std::string& /*path*/, size_t /*offset*/, const void * /*data*/, size_t /*len*/) {
    }

    void OnError(const std::string& /*what*/) {
    }
};
