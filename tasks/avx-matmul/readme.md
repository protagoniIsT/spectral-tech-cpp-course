# AVX Matmul

Don't forget to look up instructions on https://www.intel.com/content/www/us/en/docs/intrinsics-guide/index.html.

## Problem

In this problem we'll implement AVX-powered matmul function. Note, that this is an educational task, it doesn't make much sense
to implement matmul manually these days, since there are more efficient implementations for different use-cases
(like https://oneapi-src.github.io/oneDNN/dev_guide_matmul.html, https://libxsmm.readthedocs.io/en/latest/ or ML-compilers in general). However,
its a good exercise to get a grasp of using simd intrinsics and understand some methods commonly used.

Specifically we'll implement $`C = A \cdot B^T`$. `A` is `MxK` and `B` is `NxK`, so `C` is `MxN` and `C[i][j]` is a dot product of i-th row of `A`
and j-th row of `B`. All elements are `float`.

In this task we use AVX2, which is capable of operating on vectors of 8 floats in particular. So one of the solutions can look like this:
```
for (int i = 0; i < M; ++i) {
    for (int j = 0; j < N; ++j) {
        _m256 sum = _mm256_setzero_ps();
        for (int k = 0; k < K; k += 8) {
            _m256 lhs = _mm256_loadu_ps(&a[i][k]);
            _m256 rhs = _mm256_loadu_ps(&b[j][k]);
            _m256 res = _mm256_mul_ps(lhs, rhs);
            sum = _mm256_add_ps(res, sum);
        }
        float to_store = do_horizontal_sum(sum);
        _mm256_storeu_ps(&c[i][j], to_store);
    }
}
```

We basically vectorized the inner loop, which does dot product calculation. Every iteration multiples two 8-elements vectors and accumulates the result in `sum`. In the end we sum everything in this register (`do_horizontal_sum`) to get the final result.

There are a few notes here:
1. We use `_mm256_loadu_ps` to load data from memory to register. Note, that there is also `_load_` version, but it only works if data is aligned by 32-byte boundary (otherwise
its `SIGSEGV`). The alignment is different for different operation sets. For a example it should be 64 for AVX-512.
The same goes for `store` operation. If you don't know whether the data is aligned, `_loadu_/_storeu_` are used instead. However, its slightly slower.
2. This code only works if `K` is divisible by 8. There are multiple ways of dealing with that:
  - pad data with zeros
  - calculate the residual sum using regular loops
  - use mask instructions. AVX-512 introduces a lot of instructions, which can receive mask to specify parts of data you're interested in. For a example, `_mm256_mask_load_ps` would work in our case. However, its AVX-512 and we only use AVX2 here. Unfortunately, AVX2 only has masked loads for integer
  vectors.
3. We need to sum elements inside the register (`do_horizontal_sum`), which is actully tricky and requires a bunch of instructions. Its one of the
reasons why this implementation isn't very efficient and we are not gonna implement it here.
4. The similar code can be written for `_m128` and `_m512`. Note, that there is `_mm256_fmadd_ps` for a example, which combines multiplication
and sum, but its not AVX2 as well.

Lets try to remove the need for horizontal sum here. In order to do that we have to vectorize by `j` and not by `k`. So instead of
computing `C[i][j]` we would compute `C[i][j], C[i][j + 1], ..., C[i][j + 7]` directly.

Note, that by reordering axes our matrix multiplication can be rewritten this way:
```
for(int i = 0; i < M; ++i) {
    for (int j_outer = 0; j_outer < N; j_outer += 8) {
        float sum[8] = {0};
        for (int k = 0; k < K; ++k) {
            for (int j_inner = 0; j_inner < 8; ++j_inner) {
                sum[j_inner] += a[i][k] * b[j_outer + j_inner][k];
            }
        }
        memcpy(&c[i][j_outer], sum, 8 * sizeof(float));
    }
}
```

This can almost be rewritten to AVX directly. However, note that access to `B` is non-sequential. In order to fix that lets create a new matrix B'
with a different layout. By default we have a row-major format, so for a example for 8-columns B data is stored like this:
```
1  2  3  4  5  6  7  8
9  10 11 ...
17 18 ...
25 26 ...
33 34 ...
41 42 ...
49 50 ...
57 58 ...
65 66 ...
...
```

We want to repack it in such a way, that all 8-elements column vectors are stored sequentially. So in this case the order would be:
```
1 9 17 25 33 41 49 57 2 10 18 26 34 42 50 58 3 11 ... 4 ... 8 ... 65 ... 66 ...
```

Basically now `B'` is of a size of `[roundup(N / 8)][K][8]`. Note, that it automatically solves the padding problem.
You might ask whether its a good idea to create a new matrix for every matmul here. Generally, the answer is no, however
in a lot of cases the second matrix would already be packed in some special way suited for matmul. This is very common for neural networks
for a example, where the second matrix is usually constant weights, which can be preprocessed in any way we need before running inference requests.

There is still one last question of how do we multiply scalar `a[i][k]` by vector with AVX? _Broadcasting_ typically is used for that, e.g.
`_mm256_set1_ps` instruction, which is quite cheap.

## Submitting

Implement the last version with AVX. Any other version would be banned. Note, that you can use aligned loads, since you can allocate `B'` with a proper
alignment (using `std::aligned_alloc`).
