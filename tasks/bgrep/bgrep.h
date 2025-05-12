#pragma once

#include <sys/mman.h>
#include <sys/stat.h>
#include <filesystem>
#include <optional>
#include <algorithm>
#include <functional>
#include <sstream>
#include <string>
#include <optional>
#include <iterator>
#include <fcntl.h>
#include <unistd.h>
#include <vector> 
#include <cerrno> 
#include <cstring>

namespace fs = std::filesystem;

enum class GrepAlgorithm {
    kRegular,
    kBoyerMoore
};

struct GrepOptions {
    GrepAlgorithm algo;
    size_t max_matches;
    size_t offset = 0;

    GrepOptions() {
        algo = GrepAlgorithm::kRegular;
        max_matches = -1;
    }
};

void parse_pattern(const std::string& pat, std::vector<std::optional<uint8_t>>& dst) {
    std::istringstream ss(pat);
    std::string token;
    while (ss >> token) {
        if (token == "??") {
            dst.emplace_back(std::nullopt);
        } else {
            if (token.size() != 2) {
                throw std::runtime_error("Invalid byte: " + token);
            }
            auto b = static_cast<uint8_t>(std::stoi(token, nullptr, 16));
            dst.emplace_back(b);
        }
    }
}

template <class Visitor>
inline void Grep(const std::string& path,
                 const std::vector<std::optional<uint8_t>>& pattern,
                 Visitor visitor,
                 const GrepOptions& options) {
    fs::path fspath{path};
    if (fs::is_directory(fspath)) {                                    
        for (auto it = fs::recursive_directory_iterator(fspath); it != fs::recursive_directory_iterator{}; ++it) {
            auto entry = it->path().string();
            GrepImpl(entry, pattern, std::move(visitor), options);
        }
    } else {
        GrepImpl(path, pattern, std::move(visitor), options);
    }
}

template <class Visitor>
inline void Grep(const std::string& path,
                 const std::string& pattern,
                 Visitor visitor,
                 const GrepOptions& options) {
    std::vector<std::optional<uint8_t>> parsed_pattern;
    parse_pattern(pattern, parsed_pattern);
    Grep(path, parsed_pattern, std::move(visitor), options);
}

template <class Visitor>
inline void GrepImpl(const std::string& path,
                     const std::vector<std::optional<uint8_t>>& pattern,
                     Visitor visitor,
                     const GrepOptions& options) {
    bool is_simple = std::all_of(pattern.begin(), pattern.end(),
                                 [](auto& o) { return o.has_value(); });

    int fd = open(path.c_str(), O_RDONLY);
    if (fd < 0) {
        visitor.OnError(path);
        return;
    }
    struct stat st;
    if (fstat(fd, &st) != 0) {
        visitor.OnError(path);
        close(fd);
        return;
    }
    size_t filesize = st.st_size;
    if (options.offset >= filesize) {
        close(fd);
        return;
    }

    void* raw_map = mmap(nullptr, filesize, PROT_READ, MAP_PRIVATE, fd, 0);
    if (raw_map == MAP_FAILED) {
        visitor.OnError(path);
        close(fd);
        return;
    }
    auto* data = reinterpret_cast<uint8_t*>(raw_map) + options.offset;
    size_t length = filesize - options.offset;
    size_t pattern_size = pattern.size();
    size_t matches = 0;

    std::vector<uint8_t> raw_pattern;
    if (is_simple) {
        raw_pattern.reserve(pattern.size());
        for (auto& o : pattern) {
            raw_pattern.push_back(*o);
        }
    }

    if (is_simple) {
        if (options.algo == GrepAlgorithm::kBoyerMoore) {
            auto searcher = std::boyer_moore_searcher(raw_pattern.begin(), raw_pattern.end());
            const uint8_t* data_begin = data;
            const uint8_t* data_end = data + length;
            auto it = std::search(data_begin, data_end, searcher);
            while (it != data_end && matches < options.max_matches) {
                size_t pos = it - data_begin;
                visitor.OnMatch(path, options.offset + pos, it, pattern_size);
                ++matches;
                it = std::search(it + 1, data_end, searcher);
            }
        } else {
            for (size_t i = 0; i + pattern_size <= length; ++i) {
                if (memcmp(data + i, raw_pattern.data(), pattern_size) == 0) {
                    visitor.OnMatch(path, options.offset + i, data + i, pattern_size);
                    if (++matches >= options.max_matches) break;
                }
            }
        }
    } else {
        for (size_t i = 0; i + pattern_size <= length; ++i) {
            bool matching = true;
            for (size_t j = 0; j < pattern_size; ++j) {
                if (pattern[j].has_value() && data[i + j] != *pattern[j]) {
                    matching = false;
                    break;
                }
            }
            if (matching) {
                visitor.OnMatch(path, options.offset + i, data + i, pattern_size);
                if (++matches >= options.max_matches) break;
            }
        }
    }

    if (munmap(raw_map, filesize) != 0) {
        visitor.OnError(path);
    }
    close(fd);
}
