# Intrusive list

## Problem

Implement template class `IntrusiveList<T>` in `intrusive_list.h`. The main difference between intrusive and regular containers is
that elements are not copied inside a container (like in `std::list` for a example). An intrusive container basically defines a logic, which can be
inherited by a user of this container. In an intrusive list links to the next and previous elements are stored right in the user's type `T`.
To make this work `T` should be derived from `ListHook` class, where all the list logic is actually implemented.

You can find a more detailed description here: https://www.boost.org/doc/libs/1_81_0/doc/html/intrusive/intrusive_vs_nontrusive.html. Also there is
a separate video about this task, so its recommended to check it out.
