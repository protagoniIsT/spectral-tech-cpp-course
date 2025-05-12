#pragma once

#include <set>
#include <unordered_set>

template<class T>
concept HasEqual = requires(T v) {
    { v == v } -> std::convertible_to<bool>;
};

template<class T>
concept HasNotEqual = requires(T v) {
    { v != v } -> std::convertible_to<bool>;
};

template<class T>
concept IsHashable = requires(T v) {
    { std::hash<T>{}(v) } -> std::convertible_to<size_t>;
};

template<class T>
concept UnorderedMapNeeded = (HasEqual<T> || HasNotEqual<T>) && IsHashable<T>;


template<class T>
concept HasLess = requires(T v) {
    { v < v } -> std::convertible_to<bool>;
};

template<class T>
concept HasGreater = requires(T v) {
    { v > v } -> std::convertible_to<bool>;
};

template<class T>
concept SetNeeded = HasLess<T> || HasGreater<T>;

template<typename T>
struct SetComparator;

template<class T>
requires HasLess<T>
struct SetComparator<T> {
    bool operator()(const T& a, const T& b) const {
        return a < b;
    }
};

template<class T>
requires (HasGreater<T> && !HasLess<T>)
struct SetComparator<T> {
    bool operator()(const T& a, const T& b) const {
        return b > a;
    }
};


template<class T>
struct ContainerTypeTrait {
    using type = std::conditional_t<
        UnorderedMapNeeded<T>,
        std::unordered_set<T>,
        std::conditional_t<
            SetNeeded<T>,
            std::set<T, SetComparator<T>>,
            std::unordered_set<T*>>>;
};

template<class T, class S = ContainerTypeTrait<T>::type>
class CleverSet {
public:
    using result_type = std::conditional_t<
        std::is_pointer_v<typename S::value_type>,
        T*,
        T
    >;

    bool insert(const T& value) {
        return insertImpl(value);
    }

    bool erase(const T& value) {
        return data_.erase(value);
    }

    bool find(const T& value) const {
        return data_.find(value) != data_.end();
    }

    size_t size() const {
        return data_.size();
    }

private:
    S data_;

    bool insertImpl(const T& value) {
        if constexpr (std::is_same_v<result_type, T*>) {
            T* ptr = const_cast<T*>(&value);
            return data_.insert(ptr).second;
        } else {
            return data_.insert(value).second;
        }
    }
};
