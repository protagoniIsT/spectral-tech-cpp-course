#include <gflags/gflags.h>
#include <iostream>
#include <iomanip>
#include <limits>
#include "bgrep.h"

DEFINE_string(algo, "brute", "Search algorithm: \"brute\" or \"boyer-moore\"");
DEFINE_uint64(offset, 0, "Start search in each file at this byte offset");
DEFINE_uint64(matches, 0, "Max matches per file (0 = unlimited)");

int main(int argc, char* argv[]) {
    gflags::SetUsageMessage(
        "Usage: bin-grep [--algo=brute|boyer-moore] [--offset=N] [--matches=N] <pattern> <file1> [file2 ...]\n"
    );

    gflags::ParseCommandLineFlags(&argc, &argv, true);

    if (argc < 3) {
        std::cerr << "Error: pattern and at least one file or directory required\n";
        std::cout << gflags::ProgramUsage() << "\n";
        return 1;
    }

    GrepOptions options;
    options.offset = FLAGS_offset;
    options.max_matches = (FLAGS_matches == 0
                        ? std::numeric_limits<size_t>::max()
                        : FLAGS_matches);
    options.algo = (FLAGS_algo == "boyer-moore") ? GrepAlgorithm::kBoyerMoore : GrepAlgorithm::kRegular;

    std::string pattern = argv[1];

    struct Printer {
        void OnMatch(const std::string& path, size_t offset, const uint8_t* data, size_t len) {
            std::cout << path << ':' << offset << ':';
            std::ios old_state(nullptr);
            old_state.copyfmt(std::cout);
            std::cout << std::hex << std::setfill('0');
            for (size_t i = 0; i < len; ++i) {
                std::cout << std::setw(2) << static_cast<int>(data[i]);
                if (i + 1 < len) {
                    std::cout << ' ';
                }
            }
            std::cout.copyfmt(old_state);
            std::cout << '\n';
        }

        void OnError(const std::string& path) {
            std::cerr << path << ": ERROR";
        }
    } visitor;

    for (int i = 2; i < argc; ++i) {
        try {
            Grep(argv[i], pattern, visitor, options);
        } catch (std::exception& e) {
            std::cerr << "Pattern error: " << e.what() << "\n";
            return 2;
        }
    }

    return 0;
}
