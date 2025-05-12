#pragma once

#include <immintrin.h>

class AvxVector {
public:
    static inline __mmask16 ins_mask[16];
    static inline __mmask16 ins_shift_mask[16];
    static inline __mmask16 erase_shift_mask[16];
    static inline __mmask16 erase_last_mask;

    AvxVector() : vec(_mm512_setzero_si512()) {}

    AvxVector(int data[16]) {
        vec = _mm512_load_si512(data);
    }

    void unpack(int res[16]) {
        _mm512_storeu_si512(res, vec);
    }

    static void init_masks() {
        erase_last_mask = ~(1u << 15);

        for (int idx = 0; idx < 16; ++idx) {
            ins_mask[idx] = 1u << idx;

            __mmask16 m_insert = 0;
            for (int i = 0; i < 16; ++i) {
                if (i >= idx + 1)
                    m_insert |= (1u << i);
            }
            ins_shift_mask[idx] = m_insert;

            __mmask16 m_erase = 0;
            for (int i = 0; i < 16; ++i) {
                if (i >= idx)
                    m_erase |= (1u << i);
            }
            erase_shift_mask[idx] = m_erase;
        }
    }

    void insert(int idx, int elem) {
        __m512i indices = _mm512_set_epi32(
            15, 14, 13, 12, 11, 10, 9, 8,
            7, 6, 5, 4, 3, 2, 1, 0);
        __m512i one = _mm512_set1_epi32(1);
        __m512i new_index = _mm512_mask_sub_epi32(indices, ins_shift_mask[idx], indices, one);
        __m512i shifted = _mm512_permutexvar_epi32(new_index, vec);
        __m512i v_elem = _mm512_set1_epi32(elem);
        vec = _mm512_mask_mov_epi32(shifted, ins_mask[idx], v_elem);
    }

    void set(int idx, int elem) {
        __m512i v_elem = _mm512_set1_epi32(elem);
        vec = _mm512_mask_mov_epi32(vec, ins_mask[idx], v_elem);
    }

    void erase(int idx) {
        __m512i indices = _mm512_set_epi32(
            15, 14, 13, 12, 11, 10, 9, 8,
            7, 6, 5, 4, 3, 2, 1, 0);
        __m512i one = _mm512_set1_epi32(1);
        __m512i new_index = _mm512_mask_add_epi32(indices, erase_shift_mask[idx], indices, one);
        __m512i shifted = _mm512_permutexvar_epi32(new_index, vec);
        vec = _mm512_maskz_mov_epi32(erase_last_mask, shifted);
    }

    int sum() const {
        return _mm512_reduce_add_epi32(vec);
    }

private:
    __m512i vec;
};
