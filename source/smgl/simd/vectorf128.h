#ifndef VECTORF128_H_
#define VECTORF128_H_
#include <pmmintrin.h>
#include <math.h>
#include "..\..\..\include/platform.h"


static inline __m128 vectorf128_cross(__m128 const input0, __m128 const input1)
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


static inline __m128 vectorf128_sum(__m128 const input)
{
    __m128 tmp;
    tmp = _mm_hadd_ps(input, input);
    tmp = _mm_hadd_ps(tmp, tmp);
    return tmp;
}


static inline __m128 vectorf128_min(__m128 const v)
{
    __m128 tmp;
    tmp = _mm_min_ps(v, _mm_shuffle_ps(v, v, _MM_SHUFFLE(2, 1, 0, 3)));
    tmp = _mm_min_ps(tmp, _mm_shuffle_ps(tmp, tmp, _MM_SHUFFLE(1, 0, 3, 2)));
    return tmp;
}


static inline __m128 vectorf128_round(__m128 const input)
{
#if SMGL_INSTRSET > 4 // _mm_round_ps from SSE 4.1
    return _mm_round_ps(input, _MM_FROUND_TO_NEAREST_INT);
#else                              // based on http://dss.stephanierct.com/DevBlog/?p=8
                                   // - edited to remove dogdy casting (cause of range value input error)
    __m128 v0 = _mm_setzero_ps();
    __m128 v1 = _mm_cmpeq_ps(v0, v0);
    __m128i tmp = _mm_castps_si128(v1);

    tmp = _mm_srli_epi32(tmp, 2);
    __m128 v_nearest = _mm_castsi128_ps(tmp);
    __m128i i = _mm_cvttps_epi32(input);
    __m128 a_trunc = _mm_cvtepi32_ps(i);
    __m128 rmd0 = _mm_sub_ps(input, a_trunc);
    __m128 rmd1 = _mm_mul_ps(rmd0, v_nearest);
    __m128i rmd1i = _mm_cvttps_epi32(rmd1);

    // back to m128
    __m128 conv_rmd_trunc = _mm_cvtepi32_ps(rmd1i);
    __m128 r = _mm_add_ps(a_trunc, conv_rmd_trunc);
    return r;

#endif
}


static inline __m128 vectorf128_ceil(__m128 const input)
{
#if SMGL_INSTRSET > 4 // _mm_ceil_ps from SSE 4.1
    return _mm_ceil_ps(input);
#else
    __m128i v0 = _mm_setzero_si128(); // based on http://dss.stephanierct.com/DevBlog/?p=8
    __m128i v1 = _mm_cmpeq_epi32(v0, v0);
    __m128i ji = _mm_srli_epi32(v1, 25);
    __m128i tmp = _mm_slli_epi32(ji, 23);
    __m128 j = _mm_castsi128_ps(tmp);
    __m128i i = _mm_cvttps_epi32(input);
    __m128 fi = _mm_cvtepi32_ps(i);
    __m128 igx = _mm_cmplt_ps(fi, input);
    j = _mm_and_ps(igx, j);
    return _mm_add_ps(fi, j);
#endif

}

static inline __m128 vectorf128_floor(__m128 const input)
{
#if SMGL_INSTRSET > 4 // _mm_floor_ps from SSE 4.1
    return _mm_floor_ps(input);
#else
    __m128i v0 = _mm_setzero_si128();
    __m128i v1 = _mm_cmpeq_epi32(v0, v0);
    __m128i ji = _mm_srli_epi32(v1, 25);
    __m128i tmp = _mm_slli_epi32(ji, 23);
    __m128 j = _mm_castsi128_ps(tmp);
    __m128i i = _mm_cvttps_epi32(input);
    __m128 fi = _mm_cvtepi32_ps(i);
    __m128 igx = _mm_cmpgt_ps(fi, input);
    j = _mm_and_ps(igx, j);

    return _mm_sub_ps(fi, j);
#endif
}


static inline float vectorf128_dot(__m128 const input0, __m128 const input1)
{
    float out;
    __m128 tmp;
#if SMGL_INSTRSET > 4
    tmp = _mm_dp_ps(input0, input1, 0xff);
#else  // lightning fast alternative
    tmp = _mm_mul_ps(input0, input1);
    tmp = _mm_add_ps(_mm_movehl_ps(tmp, tmp), tmp);
    tmp = _mm_add_ss(_mm_shuffle_ps(tmp, tmp, 1), tmp);
#endif
     _mm_store_ss(&out, tmp);
     return out;
}


static inline __m128 vectorf128_vector_dot(__m128 const input0, __m128 const input1)
{
#if SMGL_INSTRSET > 4
    return _mm_dp_ps(input0, input1, 0xff);
#endif
    __m128 tmp = _mm_mul_ps(input0, input1);
    tmp = _mm_add_ps(_mm_movehl_ps(tmp, tmp), tmp);
    tmp = _mm_add_ss(_mm_shuffle_ps(tmp, tmp, 1), tmp);
    return tmp;
}


static inline __m128 vectorf128_normalize(__m128 const input)
{
    __m128 dot = vectorf128_vector_dot(input, input);
    __m128 isr = _mm_rsqrt_ps(dot);
     return _mm_mul_ps(input, isr);
}


static inline __m128 vectorf128_scale(__m128 const input, float scalar)
{
    __m128 scaling_vec = _mm_set_ps1(scalar);
    return _mm_mul_ps(input, scaling_vec);
}


#endif // VECTORF128_H_
