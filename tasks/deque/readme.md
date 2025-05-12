# Deque

## Problem

Implement an analog of `std::deque` in `deque.h`. For simplicity, your `Deque` should store `int`s and use 512-byte block size.
Include following methods to your class:
1. Default constructor
2. Constructor from `std::initializer_list<int>`.
3. `PushBack/PopBack/PushFront/PopFront`.
4. `operator[]`, both const and non-const versions.
5. `Size`
6. `Clear`, which clears deque (you may not deallocate memory).
7. `Swap(Deque&)`, which swaps deque's content with an argument.

The algorithm is described in the third lecture.

**std::deque is forbidden in this task** (you may use `std::vector` though).
