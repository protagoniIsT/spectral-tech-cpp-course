# Short View

This task is checked without ASAN.

## Problem

Implement class `ShortView` in `short_view.h`, which is essentialy `string_view` (so it references some continious chunk of memory of given size),
but with a small trick. We assume, that the size is 16-bits at max, so it can be put inside the data pointer, which is exactly what you have
to do. `sizeof(ShortView)` should be equal to 8 (a single pointer).

## Notes

- Note that although the high 16 bits of pointers are not used on x64, they are still checked whenever you reference a pointer.
The current rule is that these 16 bits should be equal to the bit 47. So whenever you return data pointer outside from the class, make sure
to set these high bits appropriately.
- Whenever you need to do something like this the typical workflow is `pointer -> 64-bit integer -> bit manipulations -> pointer`.
Its much more convinient to convert a pointer to `uint64_t` instead of `int64_t` considering how the sign extension works for integers
(https://en.wikipedia.org/wiki/Two%27s_complement#Sign_extension).
