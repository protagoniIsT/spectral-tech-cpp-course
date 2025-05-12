#include <catch.hpp>

#include <iostream>
#include <vector>
#include <string>

#include <bgrep.h>
#include "commons.h"

#ifndef TASK_DIR
#define TASK_DIR "./"
#endif

inline std::string ConstructBasePath() {
    std::string result(TASK_DIR);
    return result;
}

const std::string kBasePath = ConstructBasePath();

struct Match {
    std::string path;
    std::vector<uint8_t> match;
    size_t offset;

    bool operator==(const Match& rhs) const {
        return path == rhs.path && match == rhs.match && offset == rhs.offset;
    }
};

std::ostream& operator<<(std::ostream& out, const Match& match) {
    out << "Path " << match.path << ", offset " << match.offset << ", data";
    for (auto x : match.match) {
        out << " " << static_cast<int>(x);
    }
    return out;
}

class CollectMatches : public BaseVisitor {
public:
    explicit CollectMatches(std::vector<Match>& matches) : matches_(matches) {}

    void OnMatch(const std::string& path, size_t offset, const void *data, size_t len) {
        auto ptr = reinterpret_cast<const uint8_t*>(data);
        std::vector<uint8_t> match(ptr, ptr + len);
        matches_.push_back(Match{path, std::move(match), offset});
    }

    void OnError(const std::string& what) {
        std::cerr << "Fail: " << what << "\n";
    }
private:
    std::vector<Match>& matches_;
};

TEST_CASE("simple") {
    std::vector<Match> matches;
    CollectMatches visitor(matches);
    std::vector<std::optional<uint8_t>> needle{0x76, 0x42, 0xe7, 0xbc, 0xdf, 0x2a, 0x98, 0x3b, 0x23, 0x67};
    Grep(kBasePath + "tests/test.bin", needle, visitor, GrepOptions());
    std::vector<Match> expected{Match{kBasePath + "tests/test.bin", ConvertToPlain(needle), 137}};
    REQUIRE(matches == expected);
}

TEST_CASE("bad_pattern") {
    BaseVisitor visitor;
    CHECK_THROWS(Grep("tests/test.bin", "aa bb c", visitor, GrepOptions()));
    CHECK_THROWS(Grep("tests/test.bin", "a 23", visitor, GrepOptions()));
    CHECK_THROWS(Grep("tests/test.bin", "?74", visitor, GrepOptions()));
    CHECK_THROWS(Grep("tests/test.bin", "ae ?", visitor, GrepOptions()));
}

TEST_CASE("multiple") {
    std::vector<Match> matches;
    CollectMatches visitor(matches);
    std::string needle = "04";
    GrepOptions options;
    options.offset = 5;
    options.max_matches = 4;
    Grep(kBasePath + "tests/pat.bin", needle, visitor, options);
    REQUIRE(matches.size() == 4);
    size_t ind = 9;
    for (const auto& m : matches) {
        REQUIRE(m.offset == ind);
        ind += 5;
    }
}

TEST_CASE("recursive") {
    std::vector<Match> matches;
    CollectMatches visitor(matches);
    std::string needle = "?? ?? ?? ?? 86 D1 2c 90 64 0b";
    Grep(kBasePath + "tests/dir", needle, visitor, GrepOptions());
    std::vector<Match> expected{
        Match{kBasePath + "tests/dir/a/1", std::vector<uint8_t>{0x4f, 0x5c, 0xc9, 0x2e, 0x86, 0xd1, 0x2c, 0x90, 0x64, 0xb}, 365},
        Match{kBasePath + "tests/dir/a/check", std::vector<uint8_t>{0x5c, 0xbd, 0xc9, 0x2e, 0x86, 0xd1, 0x2c, 0x90, 0x64, 0xb}, 40},
        Match{kBasePath + "tests/dir/b/2.bin", std::vector<uint8_t>{0x5c, 0x5c, 0x4, 0x2e, 0x86, 0xd1, 0x2c, 0x90, 0x64, 0xb}, 421},
        Match{kBasePath + "tests/dir/b/c/c", std::vector<uint8_t>{0x5c, 0x5c, 0xc9, 0xc4, 0x86, 0xd1, 0x2c, 0x90, 0x64, 0xb}, 203}
    };
    std::sort(matches.begin(), matches.end(), [](const Match& a, const Match& b) {
        return a.path < b.path;
    });
    REQUIRE(matches == expected);
}

#ifdef NDEBUG
TEST_CASE("offset") {
    const int needle_len = 1e5;
    const int file_len = 1e6;
    std::vector<Match> matches;
    CollectMatches visitor(matches);
    std::vector<std::optional<uint8_t>> needle;
    for (int i = 0; i < needle_len; ++i) {
        needle.push_back(0);
    }
    GrepOptions opts;
    opts.offset = file_len - needle_len;
    Grep(kBasePath + "tests/zero.bin", needle, visitor, opts);
    std::vector<Match> expected{Match{kBasePath + "tests/zero.bin", ConvertToPlain(needle), file_len - needle_len}};
    REQUIRE(matches == expected);
}

TEST_CASE("long") {
    const int needle_len = 1e5;
    const int file_len = 1e6;
    std::vector<Match> matches;
    CollectMatches visitor(matches);
    std::vector<std::optional<uint8_t>> needle;
    for (int i = 0; i < needle_len; ++i) {
        needle.push_back(0);
    }
    GrepOptions opts;
    opts.algo = GrepAlgorithm::kBoyerMoore;
    Grep(kBasePath + "tests/zero.bin", needle, visitor, opts);
    std::vector<Match> expected{Match{kBasePath + "tests/zero.bin", ConvertToPlain(needle), file_len - needle_len}};
    REQUIRE(matches == expected);
}
#endif
