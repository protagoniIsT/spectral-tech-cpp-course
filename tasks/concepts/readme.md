# Concepts

## Problem

In this task you should implement 2 concepts.

### Predicate

Implement concept `Predicate` in `predicate.h`, which checks, that type `P` is a predicate over `T`, that is `P: T -> bool`. Note that `P(T)` should
produce exactly `bool`.

### Indexable

Implement concept `Indexable` in `indexable.h`, which checks, that `T` can be accessed in a random-access manner, that is:
  - it supports `operator[]` with `size_t` as index, which returns some non-void value.
  - or `std::begin(T)` and `std::end(T)` work and return some random-access iterator.
