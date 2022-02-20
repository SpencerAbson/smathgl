#ifndef MATRIX4XM128F_H_
#define MATRIX4XM128F_H_

#include "..\..\include/platform.h"
#include "vectorf128.h"

typedef __m128 mat4xm128[4];

#if SMGL_INSTRSET > 2
static inline __m128 mat4xm128_vec4_product(__m128 mat[4], __m128 vec)
{
    float x = _mm_cvtss_f32(vectorf128_sum(_mm_mul_ps(mat[0], vec)));
    float y = _mm_cvtss_f32(vectorf128_sum(_mm_mul_ps(mat[1], vec)));
    float z = _mm_cvtss_f32(vectorf128_sum(_mm_mul_ps(mat[2], vec)));
    float w = _mm_cvtss_f32(vectorf128_sum(_mm_mul_ps(mat[3], vec)));

    return _mm_set_ps(w, z, y, x); // backwards register indexing for SIMD
}


static inline void mat4xm128_transpose(__m128 input[4], __m128 out[4])
{
    __m128 tmp0 = _mm_shuffle_ps(input[0], input[1], 0x44);
    __m128 tmp2 = _mm_shuffle_ps(input[0], input[1], 0xEE);
    __m128 tmp1 = _mm_shuffle_ps(input[2], input[3], 0x44);
    __m128 tmp3 = _mm_shuffle_ps(input[2], input[3], 0xEE);

    out[0] = _mm_shuffle_ps(tmp0, tmp1, 0x88);
    out[1] = _mm_shuffle_ps(tmp0, tmp1, 0xDD);
    out[2] = _mm_shuffle_ps(tmp2, tmp3, 0x88);
    out[3] = _mm_shuffle_ps(tmp2, tmp3, 0xDD);
}


static inline void mat4xm128_outer_product(__m128 input0, __m128 input1, __m128 out[4])
{
    out[0] = _mm_mul_ps(input0, _mm_shuffle_ps(input1, input1, _MM_SHUFFLE(0, 0, 0, 0)));
    out[1] = _mm_mul_ps(input0, _mm_shuffle_ps(input1, input1, _MM_SHUFFLE(1, 1, 1, 1)));
    out[2] = _mm_mul_ps(input0, _mm_shuffle_ps(input1, input1, _MM_SHUFFLE(2, 2, 2, 2)));
    out[3] = _mm_mul_ps(input0, _mm_shuffle_ps(input1, input1, _MM_SHUFFLE(3, 3, 3, 3)));
}


static inline void mat4xm128_add(__m128 input0[4], __m128 input1[4], __m128 out[4])
{
    out[0] = _mm_add_ps(input0[0], input1[0]);
    out[1] = _mm_add_ps(input0[1], input1[1]);
    out[2] = _mm_add_ps(input0[2], input1[2]);
    out[3] = _mm_add_ps(input0[3], input1[3]);
}


static inline void mat4xm128_sub(__m128 input0[4], __m128 input1[4], __m128 out[4])
{
    out[0] = _mm_sub_ps(input0[0], input1[0]);
    out[1] = _mm_sub_ps(input0[1], input1[1]);
    out[2] = _mm_sub_ps(input0[2], input1[2]);
    out[3] = _mm_sub_ps(input0[3], input1[3]);
}

#endif
#endif // MATRIX4XM128F_H_
