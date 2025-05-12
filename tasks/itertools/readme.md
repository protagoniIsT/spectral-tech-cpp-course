# Itertools

## Problems

In this task you should implement a few helper functions and classes for handling sequences. You might have seen these functions in python.

Specifically, implement `Range`, `Zip` and `Group`. For a example:
```c++
std::vector<int> first{4, 4, 5};
std::list<std::string> second{"aba", "caba"};

for (const auto& elem : Zip(first, second)) {
    // elem.first -> int
    // elem.second -> string
}

for (const auto& elem : Zip(second, Range(second.size()))) {
    // like enumerate
    // elem.first -> string
    // elem.second -> index: 0, 1, 2, ...
}

for (const auto& elem : Group(first)) {
    // elem groups equal elements
    for (const auto& val : elem) {
        // only equal elements here
    }
}
```

Note that these functions shouldn't create new containers (like vectors) to store values, implement the corresponding iterators instead.
For instance, `Range(100000)` doesn't create a vector of $10^5$ elements. It returns some entity, which supports range-based for.

You may use `IteratorRange` class from `itertools.h` as such entity. The object of `IteratorRange` contains two iterators to the beginning and the end of a sequence. It supports range-based for (as long as it's parameter supports it) and thus you may return it as a result of your functions.

`Range(from, to, step)` should generate sequence `from, from + step, ..., from + k * step` until it reaches `to` value (or greater).
You should also implement shortcut version of it with 1 and 2 arguments (check out tests for examples). You may assume, that `step > 0`.

`Zip` receives two sequences and generate another sequence of corresponding pairs. This function should cut the resulting sequence to the length of
the shortest sequence, i.e.
```c++
std::vector<int> a{1, 2, 3};
for (const auto& val : Zip(a, Range(1 << 30))) {
    // 3 iterations
}
```

`Group` should return a sequence of `IteratorRange` (basically a sequence of sequences), such that each `IteratorRange` addresses a subsequence of
equal elements. Use `operator==` only for comparing elements.

You only need to implement the basic iterator methods to support range-based for:
- constructor
- prefix ++
- dereference (\*)
- !=

## Notes

- There is a special library called `ranges` since c++20, which introduces a proper implementation of the mentioned functions and much more. You can find the docs here: https://en.cppreference.com/w/cpp/ranges. However, this is a study case, so this library is **forbidden** in this task.
