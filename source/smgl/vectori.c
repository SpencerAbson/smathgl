#include <assert.h>
#include <math.h>
#include "vectori.h"
#include "..\..\include/platform.h"
#include "simd/vectori128.h"


ivec ivec_add(ivec const *input0, ivec const *input1)
{
    ivec output;
    assert(input0->size == input1->size);
    output.size = input0->size;
    output.data.sse_register =  _mm_add_epi32(input0->data.sse_register,
                                                input1->data.sse_register);
    return output;
}


ivec ivec_sub(ivec const *input0, ivec const *input1)
{
    ivec output;
    assert(input0->size == input1->size);
    output.size = input0->size;
    output.data.sse_register =  _mm_sub_epi32(input0->data.sse_register,
                                                input1->data.sse_register);
    return output;
}


ivec ivec_cross(ivec const *input0, ivec const *input1)
{
    ivec output;
    assert(input0->size == input1->size && input1->size == 3);
    output.size = input0->size;
    output.data.sse_register =  vectori128_cross(input0->data.sse_register,
                                                   input1->data.sse_register);

    return output;
}


int32_t ivec_dot(ivec const *input0, ivec const *input1)
{
    assert(input0->size == input1->size);
    return vectori128_dot(input0->data.sse_register, input1->data.sse_register);
}


ivec ivec_mul(ivec const *input0, ivec const *input1)
{
    ivec output;
    assert(input0->size == input1->size);
    output.size = input0->size;
    output.data.sse_register =  v_mul_i32(input0->data.sse_register,
                                                    input1->data.sse_register);

    return output;
}


ivec ivec_scale(ivec const *input, int32_t scalar)
{
    ivec output;
    __m128i scaling_vec = _mm_set1_epi32(scalar);
    output.size = input->size;
    output.data.sse_register = v_mul_i32(input->data.sse_register, scaling_vec);

    return output;
}


ivec ivec4_init(int32_t x, int32_t y, int32_t z, int32_t w)
{
    ivec output;
    output.size = 4;
    output.data.sse_register = _mm_set_epi32(w, z, y, x);
    return output;
}

ivec ivec3_init(int32_t x, int32_t y, int32_t z)
{
    ivec output;
    output.size = 3;
    output.data.sse_register = _mm_set_epi32(0, z, y, x);
    return output;
}

ivec ivec2_init(int32_t x, int32_t y)
{
    ivec output;
    output.size = 2;
    output.data.sse_register = _mm_set_epi32(0, 0, y, x);
    return output;
}
