# Set allocator

Its recommended to complete `static_allocator` task first.

## Problem

Complete the definition of `StaticStlAllocator` class in `static_stl_allocator.h`. This allocator can be used as a custom allocator
for an stl-container. After that define `Set<T>` type, which is `std::set` that is using this allocator.

This is the simplest version of static allocator. It receives a pointer to some memory chunk and it's size. This allocator puts all objects
into this memory and never deallocates it. But you can also adapt `StaticAllocator` from the previous task go get more reasonable logic.

- `allocate` method allocates memory for `count` objects, but doesn't construct them yet.
- `construct` method does the construction at a given address. Use placement new from the previous task.
- `deallocate` method generally should return memory back to allocator, but we don't do it in this task.
- `destroy` basically calls the destructor.

You may wonder what `rebind` is for. Whenever you create `Set<T>` for a example, you pass `Allocator<T>` to it. However, `std::set`
has to allocate not just objects of type `T`, but nodes of a binary search tree. Thats why allocator should define a way to allocate
objects of different types.

## Notes

- There are actually a lot of requirements for an allocator to meet https://en.cppreference.com/w/cpp/named_req/Allocator. This class doesn't follow all of it, so you may want to complete it in order to use in a real production code.
