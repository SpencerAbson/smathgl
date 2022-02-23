#ifndef MATRIX_VECTOR_OP_H_
#define MATRIX_VECTOR_OP_H_
#include "..\include/platform.h"
#include "matrices.h"
#include "simd/matrix4xm128f.h"
#include "vectorf.h"

inline static void vec4_outer_product(const vec4 input0, const vec4 input1, mat4x4 out)
{
    __m128 a0, a1;
    __m128 m0[4];
    a0 = _mm_load_ps(input0);
    a1 = _mm_load_ps(input1);

    mat4xm128_outer_product(a0, a1, m0);
    m128_store_mat4(m0, out);
}


inline static void mat4_vec_product(const mat4x4 mat, const vec4 vec, vec4 out)
{
    __m128 v0;
    __m128 m0[4];

    v0 = _mm_load_ps(vec);
    mat4_load_to_m128(mat, m0);

    _mm_store_ps(out, mat4xm128_vec4_product(m0, v0));
}


#endif // MATRIX_VECTOR_OP_H_
