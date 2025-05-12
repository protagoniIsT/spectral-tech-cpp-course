#pragma once

#include <cstdint>
#include <cstddef>
#include <immintrin.h>
#include <cstring>

template<class Callback>
inline size_t SseSearch(const uint8_t* str, size_t size, uint64_t word, Callback cb) {
    if (size < 8) return 0;

    const __m128i pat = _mm_set1_epi64x(static_cast<int64_t>(word));
    const __m128i zero = _mm_setzero_si128();

    size_t found = 0;
    size_t i = 0;

    if (size >= 16) {
        for (; i <= size - 16; i += 8) {
            __m128i chunk = _mm_loadu_si128(reinterpret_cast<const __m128i*>(str + i));
            __m128i sad1 = _mm_mpsadbw_epu8(chunk, pat, 0x0F);
            __m128i sad2 = _mm_mpsadbw_epu8(chunk, pat, 0xF0);
            __m128i sad = _mm_or_si128(sad1, sad2);
            __m128i cmp = _mm_cmpeq_epi16(sad, zero);
            int mask  = _mm_movemask_epi8(cmp) & 0x5555;

            while (mask) {
                int bit = __builtin_ffs(mask) - 1;
                mask &= mask - 1;
                int offset = bit >> 1;
                size_t pos = i + offset;

                if (pos + 8 > size) continue;

                ++found;
                if (!cb(pos)) return found;
            }
        }
    }

    if (i < size) {
        alignas(16) uint8_t tmp[16];
        size_t tail_len = size - i;

        std::memcpy(tmp, str + i, tail_len);
        std::memset(tmp + tail_len, 0, 16 - tail_len);

        __m128i chunk = _mm_loadu_si128(reinterpret_cast<const __m128i*>(tmp));
        __m128i sad1 = _mm_mpsadbw_epu8(chunk, pat, 0x0F);
        __m128i sad2 = _mm_mpsadbw_epu8(chunk, pat, 0xF0);
        __m128i sad = _mm_or_si128(sad1, sad2);
        __m128i cmp = _mm_cmpeq_epi16(sad, zero);
        int mask = _mm_movemask_epi8(cmp) & 0x5555;

        while (mask) {
            int bit = __builtin_ffs(mask) - 1;
            mask &= mask - 1;
            int offset = bit >> 1;
            size_t pos = i + offset;

            if (pos + 8 > size) continue;

            ++found;
            if (!cb(pos)) return found;
        }
    }

    return found;
}
