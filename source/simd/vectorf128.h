#ifndef VECTORF128_H_
#define VECTORF128_H_
#include <pmmintrin.h>
#include <math.h>
#include "..\..\include/platform.h"


static inline __m128 vectorf128_cross(__m128 input0, __m128 input1)
{
    __m128 a, b,  v1, v2; // cross product can be computed by (y1, z1, x1) * (z2, x2, y2) - (z1, x1, y1) * (y2, z2, x2 )
    a = _mm_shuffle_ps(input0, input0, _MM_SHUFFLE(3, 0, 2, 1)); // a = (y1, z1, x1)
    b = _mm_shuffle_ps(input1, input1, _MM_SHUFFLE(3, 1, 0, 2)); // b = (z2, x2, y2)
    v1 = _mm_mul_ps(a, b);

    a = _mm_shuffle_ps(input0, input0, _MM_SHUFFLE(3, 1, 0, 2)); // a = (z1, x1, y1)
    b = _mm_shuffle_ps(input1, input1, _MM_SHUFFLE(3, 0, 2, 1)); // b = (y2, z2, x2)
    v2 = _mm_mul_ps(a, b);

    return _mm_sub_ps(v1, v2);
}


static inline __m128 vectorf128_sum(__m128 input)
{
    input = _mm_hadd_ps(input, input);
    input = _mm_hadd_ps(input, input);
    return input;
}


static inline __m128 vectorf128_min(__m128 v)
{
    v = _mm_min_ps(v, _mm_shuffle_ps(v, v, _MM_SHUFFLE(2, 1, 0, 3)));
    v = _mm_min_ps(v, _mm_shuffle_ps(v, v, _MM_SHUFFLE(1, 0, 3, 2)));
    return v;
}


static inline float vectorf128_dot(__m128 input0, __m128 input1)
{
    float out;
#if SMGL_INSTRSET > 4
    out = _mm_cvtss_f32(_mm_dp_ps(input0, input1, 0xff));
#endif
    __m128 product = _mm_mul_ps(input0, input1);
    _mm_store_ss(&out, vectorf128_sum(product));

    return out;
}


static inline __m128 vectorf128_vector_dot(__m128 input0, __m128 input1)
{
#if SMGL_INSTRSET > 4
    return _mm_dp_ps(input0, input1, 0xff);
#endif
    __m128 product = _mm_mul_ps(input0, input1);
    return vectorf128_sum(product);
}


static inline __m128 vectorf128_normalize(__m128 input)
{
    __m128 dot = vectorf128_vector_dot(input, input);
    __m128 isr = _mm_rsqrt_ps(dot);
     return _mm_mul_ps(input, isr);
}


static inline __m128 vectorf128_scale(__m128 input, float scalar)
{
    __m128 scaling_vec = _mm_load_ps1(&scalar);
    return _mm_mul_ps(input, scaling_vec);
}


#endif // VECTORF128_H_
