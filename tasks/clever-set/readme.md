# Clever Set

## Problem

Implement class `CleverSet` in `clever_set.h`, which automatically chooses the appropriate method of storing data depending on type `T`. Namely,
1. if operator `==` or `!=` is defined for `T`, as well as `std::hash<T>`, then `std::unordered_map<T>` is used.
2. otherwise, if operator `<` or `>` is defined for `T`, `std::set<T>` is used.
3. otherwise `std::unordered_map<T*>` is used, i.e. `CleverSet` manipulates pointers to objects in this case. Note, that the interface of class remains the same in this case.

You don't have to implement 3 classes here. One way to do it is to introduce an additional template parameter, and calculate the internal container's
type there. You may also implement a helper function, that returns a pointer or a reference to `value` depending on a type. A quick
remainder, that different overloaded functions can return values of different types in C++.

Use concepts in this task, sfinae solution is really ugly here.
