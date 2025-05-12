#pragma once

#include <cstring>
#include <algorithm>

template<class T>
inline T ChangeByteOrder(T value) {
    unsigned char buff[sizeof(T)];
    memcpy(buff, &value, sizeof(T));

    for (size_t i = 0; i < sizeof(T) / 2; i++) {
        std::swap(buff[i], buff[sizeof(T) - i - 1]);
    }

    T res;
    memcpy(&res, buff, sizeof(T));

    return res;
}
