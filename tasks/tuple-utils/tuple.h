#pragma once

#include <tuple>

template<typename T>
struct TupleHeadTail;

template<typename FirstT, typename ...RestT>
struct TupleHeadTail<std::tuple<FirstT, RestT...>> {
    using type_head = FirstT;
    using type_tail = std::tuple<RestT...>;
};

template<>
struct TupleHeadTail<std::tuple<>> {
    using type_head = void;
    using type_tail = std::tuple<>;
};

template<typename T>
using Head = typename TupleHeadTail<T>::type_head;

template<typename T>
using Tail = typename TupleHeadTail<T>::type_tail;



template<typename ...Ts>
struct ConcatTuples;

template<typename ...Ts>
struct ConcatTuples<std::tuple<Ts...>> {
    using type = std::tuple<Ts...>;
};

template<typename ...T1s, typename ...T2s>
struct ConcatTuples<std::tuple<T1s...>, std::tuple<T2s...>> {
    using type = std::tuple<T1s..., T2s...>;
};

template<typename Tuple1, typename Tuple2, typename ...RestTuples>
struct ConcatTuples<Tuple1, Tuple2, RestTuples...> {
    using type = typename ConcatTuples<typename ConcatTuples<Tuple1, Tuple2>::type, RestTuples...>::type;
};

template<typename ...Ts>
using Concat = typename ConcatTuples<Ts...>::type;



template<typename T, template<typename> class Operator>
struct TransformTuple;

template<template<typename> class Operator, typename... Ts>
struct TransformTuple<std::tuple<Ts...>, Operator> {
    using type = std::tuple<typename Operator<Ts>::type...>;
};

template<typename T, template<typename> class Operator>
using Transform = TransformTuple<T, Operator>::type;



template<typename Tuple1, typename Tuple2>
struct InterleaveTuples;

template<>
struct InterleaveTuples<std::tuple<>, std::tuple<>> {
    using type = std::tuple<>;
};

template<typename ...T1s>
struct InterleaveTuples<std::tuple<T1s...>, std::tuple<>> {
    using type = std::tuple<T1s...>;
};

template<typename ...T2s>
struct InterleaveTuples<std::tuple<>, std::tuple<T2s...>> {
    using type = std::tuple<T2s...>;
};

template<typename T1, typename ...T1s, typename T2, typename ...T2s>
struct InterleaveTuples<std::tuple<T1, T1s...>, std::tuple<T2, T2s...>> {
    using type = Concat<std::tuple<T1, T2>, typename InterleaveTuples<std::tuple<T1s...>, std::tuple<T2s...>>::type>;
};

template<typename Tuple1, typename Tuple2>
using Interleave = InterleaveTuples<Tuple1, Tuple2>::type;
