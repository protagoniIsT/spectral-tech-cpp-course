# Shared Matrix

**This is a separate cmake project, so create a separate build in this directory.**

## Problem

In this task you should implement a small utility `shm-matrix`, that interacts with `tester.py` via shared memory. Namely, `shm-matrix`
receives a matrix (numpy array to be precise) `M` and integer `n`, and then calculates $M^n$ and writes the result back.

The interaction proceeds as follows:
1. Tester creates a shared memory region of 64MB in size and runs `shm-matrix`, passing the region's name as an argument.
2. The first 4 bytes (int) in shared memory are used as synchronization point between both sides. This status flag may be equal to:
  - 0, which means, that the next portion of input is ready for `shm-matrix` and it can start a new calculation
  - 1, which means, that `shm-matrix` has received the input and started the calculation
  - 2, which means, that `shm-matrix` has finished the current calculation and has written the result back
  - -1, which is a signal for `shm-matrix` to stop and exit
3. The next 4 bytes contain integer `n`
4. The next 4 bytes contain matrix size `m`.
5. Immediately after that $4m^2$ bytes contain the matrix itself. Every element is `uint32`, matrix is written in a row-major order.
6. `shm-matrix` should write the correct status flags, the resulting output should be written immediately after the input in shared memory. Use the same format as input.

You can see more details in `tester.py`. It also shows an example of manipulating numpy arrays directly in shared memory without making extra copies
for inter-process passing.

## Implementation details

There are a few points considering your utility:
1. Calculate the result in `uint32` (i.e. with overflows)
2. Use binary exponentiation algorithm, since $n$ can be quite large.
3. Implement a somewhat faster than naive version of matrix multiplication:
  - instead of multiplying `A * B` compute `A * B^T` and replace multiplication of `A(rows)xB(columns)` with `A(rows)xB^T(rows)`. This is much more
  cache-friendlier approach.
  - use multithreading (`std::thread::hardware_concurrency() / 2 - 1` threads for CI) for big matrices

Use `shm_open` and `mmap` to map shared memory region into your program. Note, that the first 12 bytes of shared memory basically form a header
(status, `n` and matrix size) and you can use `reinterpret_cast` on pointer from `mmap` to read these values directly.

Status flag should be `std::atomic`, since its accessed by `tester.py` simultaneously. Use active polling on this flag, i.e. spin in a loop while
status is 2.

Write your own cmake build in this task, the only requirement is that the target's name should be `shm-matrix`.

## Testing

`tester.py` is a testing utility itself in this task, so just run `python3 tester.py <path_to_your_binary>` (ignore the warnings). Note, that there is a time limit in the testing system, so you should use all methods from above to pass it.
