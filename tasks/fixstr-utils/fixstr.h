#pragma once

#include <fixed_string.hpp>
#include <type_traits>
#include <array>
#include <vector>
#include <utility>

template<typename char_t, char_t ...k_chars>
constexpr auto operator""_cs() {
    constexpr char_t str[] = { k_chars... , '\0'};
    return fixstr::fixed_string(str);
}

template<auto val>
constexpr size_t number_length() {
    auto val_copy = val;
    size_t len = (val_copy == 0 ? 1 : 0);
    if constexpr(std::is_signed_v<decltype(val)>) {
        if (val_copy < 0) {
            val_copy = -val_copy;
            ++len;
        }
    }
    while (val_copy != 0) {
        len++;
        val_copy /= 10;
    }
    return len;
}

template<auto val>
constexpr auto number_to_str() {
    constexpr size_t len = number_length<val>();
    std::array<char, len + 1> str{};
    size_t flag = 0;
    auto val_copy = val;
    if constexpr(std::is_signed_v<decltype(val)>) {
        if (val < 0) {
            flag = 1;
            str[0] = '-';
            val_copy *= -1;
        }
    }
    for (size_t i = 0; i < len - flag; ++i) {
        str[len - i - 1] = static_cast<char>(val_copy % 10 + '0');
        val_copy /= 10;
    }
    str[len] = '\0';
    return str;
}

template<auto k_val>
constexpr auto IntToFixstr() {
    constexpr auto arr = number_to_str<k_val>();
    char data[arr.size()] = {};
    for (size_t i = 0; i < arr.size(); ++i) {
        data[i] = arr[i];
    }
    return fixstr::fixed_string(data);
}

template<fixstr::fixed_string k_format_str, fixstr::fixed_string ...k_args>
constexpr auto Format() {
    constexpr size_t args_size = sizeof...(k_args);
    constexpr size_t args_str_len = (k_args.size() + ... + 0);
    constexpr size_t formatted_str_len = k_format_str.size() + args_str_len - 2 * args_size;
    if constexpr (formatted_str_len == 0) {
        return fixstr::fixed_string("");
    } else if constexpr (args_size == 0) {
        return k_format_str;
    } else {
        char data[formatted_str_len + 1];
        auto insert = [&](auto arg, size_t &i, size_t &v) {
            while (k_format_str[v] != '{') {
                data[i++] = k_format_str[v++];
            }
            v += 2;
            for (size_t j = 0; j < arg.size(); ++j) {
                data[i++] = arg[j];
            }
        };
        size_t i = 0;
        size_t v = 0;
        (insert(k_args, i, v), ...);
        data[formatted_str_len] = '\0';
        return fixstr::fixed_string(data);
    }
}
