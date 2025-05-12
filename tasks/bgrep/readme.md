# BGrep

**Note, that some tests in this task are only executed for `Release` target**.

## Problem

In this task you should implement a simple utility `bin-grep` for searching binary sequences in binary files. The usage example:
```bash
> ./bin-grep '76 42 e7 bc ?? ?? ?? 3b 23 67' tests/test.bin
tests/test.bin:137:76 42 e7 bc df 2a 98 3b 23 67
```

This utility should support recursive traversal, search algorithm selection, simple pattern matching and some other options.
Namely implement 2 following overloads in `bgrep.h`:
```c++
template<class Visitor> void Grep(const std::string& path, const std::string& pattern, Visitor visitor, const GrepOptions& options);
template<class Visitor> void Grep(const std::string& path, const std::vector<std::optional<uint8_t>>& pattern,
                                  Visitor visitor, const GrepOptions& options);
```

`path` specifies the file or directory to search in. In case of a directory, `bgrep` should perform a recursive traversal of this directory.

`pattern` specifies the pattern to look for. In case of `std::string`, hex digits are used to encode bytes, like in the example above. Note, that letters can be both lower and uppercase, and there may be arbitrary amount of spaces (or none) between adjacent bytes. `??` matches with any byte.

In case of the second overload, bytes are specified directly. `std::nullopt` denotes `??` case.

`visitor` is an object from *Visitor* pattern, which is used to react to matches `bgrep` found. `Visitor` is an arbitrary type derived from `BaseVisitor` in `commons.h`. `bgrep` should call `OnMatch` with corresponding arguments for every match and `OnError` with error message for every error encountered. Check out tests for an example of such visitor.

`options` specifies options to use for search. `max_matches` sets the maximum amount of matches per file `bgrep` should report about. `offset` sets the offset for every file to start the search from. It may be used to skip large portions of data when needed. `algo` specifies the algorithm to use for searching:
- if the pattern is simple (doesn't contain `??`) and algo is `kBoyerMoore`, use `std::boyer_moore_searcher`
- otherwise if the pattern is simple, use `memcmp` for faster substring comparison
- otherwise use a naive approach

Use syscall `mmap` for reading files in this task. This is for learning purposes only, in general its better to use regular `read` for utilities like grep. Although, `mmap` may be much better if `offset` is big enough. In a production utility it might be a good idea to select reading method depending on usage.

`bgrep` should throw exceptions only for incorrect patterns, otherwise report them via `OnError` method.

Use `<filesystem>` library to traverse the filesystem. Namely, you need `recursive_directory_iterator`.

## Utility

Apart from the usual function's implementation, you should also build a command-line utility `bin-grep`. Specify your build in `build.cmake` file. The usage is:
```bash
bin-grep <pattern> [<files/directories>...] [flags]
```

Your utility should support the following flags:
- `-matches` for `max_matches` (unbounded by default)
- `-offset` for offset (0 by default)
- `-algo` (`brute` or `boyer-moore`, `brute` by default)

Use `gflags` library (https://gflags.github.io/gflags/) for flags parsing. Its already installed in the testing system.

You can find some test samples for your utility in `test_outputs` directory. `.args` contains the arguments used and `.txt` contains the correct output.
Use `validator.py` for tests:
```bash
python3 validator.py test_outputs/test0.txt ../build_release/bin-grep test_outputs/test0.args
python3 validator.py test_outputs/zero0.txt ../build_release/bin-grep test_outputs/zero0.args
```
