# Fuzzy Search

## Problem

Implement `FuzzySearch(data)` class, where method `Find(x)` returns any `y` in `data`, such that `y` is no more than 2 times
bigger (or smaller) than `x`. Return `std::nullopt` if there is no such `y`.

**Memory consumption of FuzzySearch(data) and asymptotic complexity of Find shouldn't depend on data.size()**.

Note, that its possible to efficiently find such `k`, that `2^k <= x < 2^{k+1}` using `__builtin_clz`.
