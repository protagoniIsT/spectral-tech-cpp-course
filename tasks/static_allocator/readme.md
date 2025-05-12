# Static Allocator

## Problem

Implement class `StaticAllocator<T>` in `static_allocator.h`, which allows to preallocate enough memory for given amount of
objects and then make fast allocations and deallocations. Before doing this task make sure you understand the difference between allocating
a memory and constructing an object in it mentioned on the lecture.

After `StaticAllocator`'s creation there should be 0 dynamic allocations during usage of this object.

`New` receives arguments for the object's constructor. It constructs the object somewhere in preallocated memory and returns a pointer to it.
It can reuse previously deleted objects' memory. If there is not enough free space, return `nullptr`.
Use the following syntax to construct an object at the given address `p` (this is called `placement new`):
```c++
T *ptr = new (p) T(std::forward<Args>(args)...);
```

`Delete` returns memory back to the allocator and deconstructs the object. You can assume, that the pointer here came from some `New` invocation
and the object hasn't been deleted earlier. Otherwise, its the user's problem (the C++-way of handling errors). Don't do anything in case of `nullptr`.

Note that you can't use `delete obj` here, because `delete` tries to free memory as it has been allocated by the default allocator.
You manage memory by yourself here, but still need to call the object's destructor explicitly.

Another important thing is you shouldn't assume `T` has a default constructor, i.e. you can't just write
```c++
std::vector<T> data(count);
```

to preallocate, because `vector` requires `T` to have a default constructor. Create a vector of bytes of appropriate size and use
placement new operator mentioned earlier instead.

And finally don't forget about an alignment. If the type `T` has some specific alignment, you can't just allocate objects at random places of memory.
Make use of `std::aligned_storage` to align your array of bytes properly.

The destructor of static allocator should destroy all the currently alive allocated objects.

**New and Delete must have O(1) complexity.**

## Links

- https://en.cppreference.com/w/cpp/types/aligned_storage
- https://en.cppreference.com/w/cpp/language/new#Placement_new
