#pragma once

template<class T>
concept RandomAccessIterable = requires(T v) {
    { std::begin(v) } -> std::random_access_iterator;
    { std::end(v) } -> std::random_access_iterator;
};

template<class T>
concept RandomAccessIndexable = requires(T v, size_t i) {
    requires(!std::is_void_v<decltype(v[i])>);
};

template <class T>
concept Indexable = RandomAccessIterable<T> || RandomAccessIndexable<T>;
