# Fixstr utils

This task uses https://github.com/unterumarmung/fixed_string, which is already included in the repo as a submodule. This
class provides constexpr-friendly strings of fixed size. You can find more info on the corresponding github page.

## Problem

In this task you'll add a few util functions for `fixed_string`. You can find their usages in tests:
- constexpr function `IntToFixstr<val>` converts compile-time constant `val` to `fixed_string`
- constexpr function `Format<format_str, args...>` basically implements `fmt::format`'s analogue for `fixed_string`. Note, that it only needs
to support `fixed_string` args.

Also, there are a lot of `"some string"_cs` usages in the tests. `_cs` here is a special case of user-defined literal https://en.cppreference.com/w/cpp/language/user_literal. However, even in C++20 its not possible to use string-literal operator template, but its supported as GNU extension. You
also should complete the definition of `_cs` in `fixstr.h`. It should return `fixed_string` constructed from the given template params.
