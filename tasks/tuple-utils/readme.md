# Tuple utils

## Problem

Implement meta-functions `Head, Tail, Transform, Concat, Interleave` operating on tuples in `tuple.h` (you can find usages in tests). Note, that these are _meta_ functions,
so they operate on types and return types as well (i.e. when the word "tuple" is used in this task, the corresponding type is assumed). The semantics of these functions are following:
- `Head` returns the type of the first type in tuple
- `Tail` for the tuple of size `n` returns the tuple with the last `n - 1` types
- `Concat` concatenates all given tuples into a single tuple
- `Transform` applies a type operator from the second template parameter to all types in the tuple
- `Interleave` for tuples `a_1, a_2, ...` and `b_1, b_2, ...` creates tuple `a_1, b_1, a_2, b_2, ...`.
