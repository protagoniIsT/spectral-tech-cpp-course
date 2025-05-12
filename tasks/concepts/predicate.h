#pragma once

#include <type_traits>

template <class P, class T>
concept Predicate = requires(T v) {
    { P{}(v) } -> std::same_as<bool>;
};
