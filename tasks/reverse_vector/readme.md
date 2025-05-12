# Reverse vector

**This is a practice task and not something you should do in a real code. Most of the stuff here is actually UB.**

This task is checked without ASAN.

## Problem

Implement class `ReverseVector<T>` in `reverse_vector.h`, so it patches a given vector `data` in such a way that:

- after `ReverseVector<T> rev(data)` is constructed, you can use negative indexes like `data[-1]` and get `data.size()`. Everything else can be broken though.
- after `rev` is destroyed, `data` gets back to it's normal state.

Assume the layout of vector told on the lecture.

## Tips

- Consider the case `int buffer[100]; int *data = buffer`. `data[i]` is equal to `*(data + i)`. So in order for `data[-1]` to work you need to set
`data = buffer + 100`. However, in case of vector `data[i]` is a call to `operator[]`, which is a method of vector. This method receives `size_t`, so passing negative numbers there doesn't make a lot of sense. But if you know by now how negative integers work and wrapped your head around pointers = offsets idea, you should realize that `size_t` there makes no difference at all. So all you have to do is to shift the base pointer in vector.
- For `size` method to work you also need to shift `finish` pointer. And in the destructor shift both pointers back.
