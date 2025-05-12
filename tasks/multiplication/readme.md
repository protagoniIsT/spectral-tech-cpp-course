# Multiplication

Check out the environment setup first: <https://gitlab.spectral.tech/study/effective-cpp-2025/-/blob/main/tasks/tutorials/environment.md>.

## Problem

This task is designed to get acquainted with the build system in this repository. Here you should implement `Multiply` function
in `multiplication.h`, which calculates a product of two numbers.

## Building

### CLion

Open `tasks` as a project in CLion. First of all its worth to add more building profiles (including ASAN) to the project:

1. Go to Cmake settings.
2. Initially there is only `Debug` profile. You can add a few more. Cmake automatically creates `Release` and some other profiles as well.
In this course we only need `Debug`, `Release` and `ASAN` profiles. Select any unused profile and change the name to `ASAN`. Add `-DCMAKE_BUILD_TYPE=ASAN` to CMake options as well.

After reloading cmake project you can now select build type and target to run (`test_multiplication` in this example).

### Terminal

Create `build` directory at `tasks` (**not in this directory**). Then run `cmake` with a target, for a example

```bash
cmake -DCMAKE_BUILD_TYPE=Release ..
```

In case you want to use another compiler, you can run it like this:

```bash
CXX=clang++-15 CC=clang-15 cmake -DCMAKE_BUILD_TYPE=Release ..
```

To build tests for a given task just type `make test_<task_name>`, for a example

```bash
make test_multiplication
```

And then run it `./test_multiplication`.

### ASAN Build

You can also build with address and ub-sanitizers enabled. For that create `build_asan` and then

```bash
cmake -DCMAKE_BUILD_TYPE=ASAN ..
```

And the rest is the same.
