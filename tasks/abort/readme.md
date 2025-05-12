# Abort

## Problem

In this task we'll use `LD_PRELOAD` and `dlsym` to wrap already existing functions in order to add some custom logic to them, which
can be quite useful in debugging sometimes (or hacking :).

There is `test.cpp` file, which is incorrect. There is an out-of-bounds access to `a[10]` there. Note, however, that its compiled with
`_GLIBCXX_DEBUG` (check out CMakeLists), which enables debug checks for all std containers in libstdc++. These checks call `abort` by default,
and there is also a custom `abort` handler in `test.cpp`. So if you just run this executable, it fails.

It would be convinient though to be able to see a full stacktrace if such thing happens in some real binary, especially if we already have it compiled
and can't recompile it. Hopefully, `abort` is defined externally in libc and like every other external function can be redefined via `LD_PRELOAD`,
which is explained on lecture.

Define your own `abort` function in `stacktrace.cpp`, which prints stacktrace to stderr using https://man7.org/linux/man-pages/man3/backtrace.3.html
and then calls default `abort`. Calling the replaced function from the replacer is quite common and can be done using `dlsym`.

As you probably know `dlsym` allows to dynamically find any external function (`abort` in this case). Its usually used together with `dlopen` to load the library with the function first.
However, in this case its more tricky, since the function is loaded already (since its in libc), but we redefine it. There is a special handler `RTLD_NEXT` just for this case. Check out the docs for more info: https://linux.die.net/man/3/dlsym.

## Testing

You don't need to modify `test.cpp` or `CMakeLists.txt`, just `stacktrace.cpp`. This task builds `test_abort` binary and `libabort_stacktrace.so`. In order to test your solution run `checker.py`:
```
python checker.py <path_to_binary> <path_to_library>
```

## Notes

- The same trick can be used to redefine `void __cxa_throw(void *ex, std::type_info *info, void (*dest)(void *))`, which is the function used for
`throw`.
- You'll get only a simple version of stacktrace here (with addresses, without files and line numbers like in gdb). A special work needs to be done to convert addresses to line numbers. Check out `addr2line` utility in order to do that (not needed in this task).
