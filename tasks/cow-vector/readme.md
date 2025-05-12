# COW-vector

## Problem

Implement copy-on-write semantics for `std::vector<std::string>` in `cow_vector.h`.
Regular vector makes a __deep copy__ whenever there is a copy operation (copy constructor or assignement operator are called), which means
a new buffer is created and the content is copied there.

One can also implement __shallow copy__ semantics, in which the internal buffer is shared between all vectors. Its very cheap to make this type
of copy and its enough for readonly operations, however these copies can't be modified independently.

Copy-on-write semantics is a combination of these 2:
1. Create a shallow copy first.
2. Turn shallow copy into deep copy whenever the first write operation occurs.

You need to use reference counting in this task. This is essential to correctly free memory for shallow copies. Create a separate structure with vector and ref_count and store a pointer to it.
Check out video about smart pointers if you have troubles.

The interface is given in the file already. `RefCount` is used for testing purposes and should return the current reference count for the vector.
Don't forget to implement the following methods:
1. Copy constructor `(const COWVector&)`.
2. Copy assignment operator `operator=(const COWVector&)`.
3. Move constructor `(COWVector&&)`.
4. Move assignment operator `operator=(COWVector&&)`.
5. Destructor `~COWVector`.

**`shared_ptr` is forbidden in this task.**
