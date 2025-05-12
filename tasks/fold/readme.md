# Fold

## Problem

Implement template function `Fold` in `fold.h`, which reduces given sequence to a single value by sequentially applying given binary function `f`.
There are 2 types of fold, depending on which order `f` is applied. Given an initial value `init` and function `f` __left fold__ can be defined
as
```math
foldl := f(\dots f(f(init, a_1), a_2), a_n)
```

Likewise for __right fold__:
```math
foldr := f(a_1, f(a_2, \dots f(a_n, init)))
```

In this task `f` is associative, so these definitions are equal.

Also implement some helper functors, which can be used with `fold`:
- `Sum`, that allows to calculate the sum of a sequence.
- `Prod`, which calculates the product of a sequence.
- `Concat`, which concatenates a sequence of vectors into a single vector.
- `Length`, which calculates the length of a sequence. Put the result into `int *` passed to the constructor.

You can find some examples in the tests.

**Usage of `<algorithm>` functions is forbidden in this task.**
