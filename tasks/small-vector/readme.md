# Small vector

## Problem

In this task you'll implement a simple version of `llvm::SmallVector`, which is defined here https://github.com/llvm/llvm-project/blob/llvmorg-19.1.7/llvm/include/llvm/ADT/SmallVector.h. 

Basically `SmallVector<T, N>` stores it's content on stack directly if the number of elements is no more than `N`. However, in this task you're
supposed to use the same trick as in llvm. The idea is to have another `VectorBase<T>` class, which is a base class of `SmallVector` and serves multiple purposes:
* implementation of all common logic for vector-like classes (it can even be used as a base for a regular vector).
* allows to type-erase the type parameter `N`, while providing almost the same interface as `SmallVector`.

In terms of layout, the base class only stores 3 vector-like pointers, which is enough to implement all the logic there. `SmallVector` adds on-stack
storage on top of that. The main trick is to be able to tell in the base class, whether vector is currently in _small_ state or not (because
you need to know this in `push_back` for a example). Its explained on lecture in detail, but you can also try to deduce the trick following llvm source
code.

The interface is given in `small_vector.h` and should be self-explanatory. There are a few remarks:
* `is_small` should return whether vector stores it's content on stack or not currently.
* `reset_small` tries to relocate the data back to stack (if the current size allows it) and returns true if the data is on stack now. It may be useful after a lot of `pop_back` calls.
* Note, that all contructors and assignment operators are defined in `SmallVector` and not in `VectorBase`. This is a simplification compared to
llvm, which defines them even for `VectorBase`.
