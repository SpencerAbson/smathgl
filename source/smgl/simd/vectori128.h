#ifndef VECTORI128_H_
#define VECTORI128_H_
#include <stdint.h>
#include "..\..\..\include/platform.h"


#define SMM_STOREU_SI32(mem_addr, vector)         \
    _mm_store_ss((float*)(mem_addr), _mm_castsi128_ps((vector)))     // NOTE: not all compilers have _mm_storeu_si32 as of 2022,
                                                                      // but this works in gcc and msvc.

static inline __m128i v_mul_i32(__m128i input0, __m128i input1)
{
    // multiply two lower i32s and store in dst
    // shuffle upper two  i32s down to lower and mult again
    // shuffle the products back to the original position of what produced them
    // return summed products NOTE: optimisations not yet considered

    __m128i v0 = _mm_mul_epi32(input0, input1);
    __m128i v1 = _mm_shuffle_epi32(input0, _MM_SHUFFLE(2, 3, 0, 1));
    __m128i v2 = _mm_shuffle_epi32(input1, _MM_SHUFFLE(2, 3, 0, 1));
    __m128i v3 = _mm_mul_epi32(v1, v2);
    __m128i v4 = _mm_shuffle_epi32(v3, _MM_SHUFFLE(2, 3, 0, 1)); // re-shuffle

    return _mm_add_epi32(v0, v4); // (A1B1, 0, A2B2, 0) + (0, A2B2, 0, A3B3)

}


static inline __m128i vectori128_cross(__m128i input0, __m128i input1)
{
    __m128i tmp0 = _mm_shuffle_epi32(input0, _MM_SHUFFLE(3, 0, 2, 1));
    __m128i tmp1 = _mm_shuffle_epi32(input1, _MM_SHUFFLE(3, 1, 0, 2));

    __m128i tmp2 = v_mul_i32(tmp0, input1);
    __m128i tmp3 = v_mul_i32(tmp0, tmp1);

    __m128i tmp4 = _mm_shuffle_epi32(tmp2, _MM_SHUFFLE(3, 0, 2, 1));
    return _mm_sub_epi32(tmp3, tmp4);
}

/*
**    __m128 tmp0 = _mm_shuffle_ps(input0, input0, _MM_SHUFFLE(3, 0, 2, 1));
    __m128 tmp1 = _mm_shuffle_ps(input1, input1, _MM_SHUFFLE(3, 1, 0, 2));
    __m128 tmp2 = _mm_mul_ps(tmp0, input1);
    __m128 tmp3 = _mm_mul_ps(tmp0, tmp1);
    __m128 tmp4 = _mm_shuffle_ps(tmp2, tmp2, _MM_SHUFFLE(3, 0, 2, 1));
*/


static inline __m128i vectori128_sum(__m128i input)  // defeats the purpose of SIMD (sorry !)
{
    input = _mm_hadd_epi32(input, input); // two horizontal adds places (x + y + z + w) in all values
    input = _mm_hadd_epi32(input, input);
    return input;  // __m1128i contains sum in all fragments
}


static inline __m128i vectori128_min(__m128i v)
{
    v = _mm_min_epi32(v, _mm_shuffle_epi32(v, _MM_SHUFFLE(2, 1, 0, 3)));
    v = _mm_min_epi32(v, _mm_shuffle_epi32(v, _MM_SHUFFLE(1, 0, 3, 2)));
    return v;
}


static inline int32_t vectori128_dot(__m128i input0, __m128i input1)
{
    int32_t out;
    __m128i product = v_mul_i32(input0, input1);   /* TODO: consider replace horizontal sum with shuff/add (even
                                                    * if stated latency/throughput is the same) */
    SMM_STOREU_SI32(&out, vectori128_sum(product));

    return out;
}


static inline __m128i vectori128_vector_dot(__m128i input0, __m128i input1)
{
    __m128i product = v_mul_i32(input0, input1);
    return vectori128_sum(product);
}


static inline __m128i vectori128_scale(__m128i input, int32_t scalar)
{
    __m128i scaling_vec = _mm_set1_epi32(scalar);
    return _mm_mul_epi32(input, scaling_vec);
}


#endif // VECTORI128_H_
