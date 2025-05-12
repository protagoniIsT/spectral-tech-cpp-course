# Reduce

## Problem

In this task you should implement reduce (left) function (also known as fold)
```math
g(f, initial, a) = f(... f(f(initial, a_1), a_2), a_n)
```
f(f(f(initial, a_1), a_2), a_3) = f(initial, f(a_1, a_2))
$`f`$ is a binary, commutative and associative function, $`a`$ is an array of size $`n`$.

Given the associativity of `f`, write a multithreaded implementation of `Reduce`.

Note, that you might want to create a vector to store each thread's result in it's corresponding cell. To avoid false sharing consider using a temporary variable in each thread to store the result. Update the corresponding cell in the vector only at the end of thread's execution.

Use `std::thread::hardware_concurrency() / 2` for the amount of threads.

## Testing

For tasks with multithreading there is an additional `TSAN` target built with thread sanitizer. Your program should pass tests with it as well.

Apart from traditional `test.cpp`, there is also a benchmark in `run.cpp`. There are no time limits currently, but your implementation should finish successfully.
