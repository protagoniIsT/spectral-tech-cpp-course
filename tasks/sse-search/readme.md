# SSE Search

## Problem

In this task you should implement a simplified sse-powered substring search function. Namely, function `SseSearch` in `sse_search.h`
searches for occurrences of a given `uint64_t word` in a byte sequence `str` of size `size`. Call given callback `cb` with the index of
every occurence (starting position). If `cb` returns false, it means, that you should stop the search. The function itself should return
the total number of occurrences found.

Use http://0x80.pl/articles/simd-strfind.html as a starting point for your implementation. Namely, use the second algorithm with `MPSADBW`
instruction.

**Note, that its forbidden to use any kind of bytes comparison, except for sse instructions.** You can also scan `str` only once.

So you should modify given algorithm (or write your own version):
- Study the docs https://www.intel.com/content/www/us/en/docs/intrinsics-guide/index.html to learn what each instruction does
- You need to modify the algorithm in such a way, that the last 4 bytes of `word` are checked too.
- Check out the details of `mpsadbw` (here is a very good illustration https://www.officedaytime.com/simd512e/simdimg/si.php?f=mpsadbw) to do that.
- Carefully manage the last block of `str` (which can be less, than 16 bytes).
- Use `__builtin_ffs` to quickly iterate over 1-bits in any mask.

Here is a good intro to SSE in general: https://shybovycha.github.io/2017/02/21/speeding-up-algorithms-with-sse.html. Its also recommended to solve
`avx-matmul` task first.
