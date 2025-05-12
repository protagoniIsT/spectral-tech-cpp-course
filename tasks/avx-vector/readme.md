# Avx Vector

This task uses AVX-512F instruction set. In case you don't have it available locally (check via `lscpu`, there should be `avx512f` in flags),
use Intel's emulator https://www.intel.com/content/www/us/en/developer/articles/tool/software-development-emulator.html (its also used in the testing system). You can use it like this:
```
sde -spr ./test_avx_vector
```

This task is for educational purposes mostly. Its a good demonstration, that avx (especially avx512) should be used wisely. This implementation
of `AvxVector` might actually be worse, than a simple one over array of ints.

## Problem

In this task we'll use a single avx-512 register to implement some basic operations over 16-elements array of ints. Assuming we have such an array,
the operations are:
- `set(i, x)` sets i-th element to `x`
- `insert(i, x)` inserts `x` at position `i`. The elements are shifted to the right, the last element is removed.
- `erase(i)` removes element from position `i`. The rest of the elements are shifted to the left and also `0` is appended as a new last element
- `sum()` returns sum of all elements in the array

Implement these methods in `avx_vector.h`. There is also `unpack` method, which unpack the current register state back into memory. You can assume both in constructor and `unpack`, that
given array is properly aligned.

Note, that there is also `init_masks` call. Its very likely you might need to use `permute`-type instructions for insert and erase. These operations
expect masks in registers as well. The idea is to precompute these masks for all possible values of `i` in advance.

## Instructions

Your go-to in this task is https://www.intel.com/content/www/us/en/docs/intrinsics-guide/index.html, filter by AVX-512 and search instructions by substrings. How to find instructions:
- since you operate on 16-elements ints, generally you need operations with `_mm512` prefix and `epi32` suffix
- `insert` and `erase` can be implemented via `permute` instructions
- for `set` you need masked version (usually has `mask` substring)
- accumulating operations like `sum` are usually implemented as some form of `reduce`

## Restrictions

Every operation should be implemented in pure avx.
