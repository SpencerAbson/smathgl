#ifndef MATRIX_VECTOR_OP_H_
#define MATRIX_VECTOR_OP_H_
#include <assert.h>
#include "..\..\include/platform.h"
#include "matrices.h"
#include "simd/matrix4xm128f.h"
#include "vectorf.h"



inline static void vec4_outer_product(fvec *input0,  fvec *input1, mat4x4 out)
{
    assert(input0->size == input1->size && input0->size == 4);
    __m128 m0[4];
    input0->data.sse_register = _mm_load_ps(input0->data.values);
    input1->data.sse_register = _mm_load_ps(input1->data.values);

    mat4xm128_outer_product(input0->data.sse_register, input1->data.sse_register, m0);
    m128_store_mat4(m0, out);
}


inline static fvec mat4_vec_product(const mat4x4 mat, fvec *vec)
{
    assert(vec->size == 4);
    fvec output;
    __m128 m0[4];
    output.size = vec->size;
    vec->data.sse_register = _mm_load_ps(vec->data.values);
    mat4_load_to_m128(mat, m0);
    output.data.sse_register = mat4xm128_vec4_product(m0, vec->data.sse_register);

    return output;
}

#endif // MATRIX_VECTOR_OP_H_
