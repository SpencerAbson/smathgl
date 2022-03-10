#include <assert.h>
#include <math.h>
#include "vectori.h"
#include "..\..\include/platform.h"
#include "simd/vectori128.h"


void ivec_add(ivec *output, ivec const *input0, ivec const *input1)
{
    assert(input0->size == input1->size);
    output->size = input0->size;
    output->data.sse_register =  _mm_add_epi32(input0->data.sse_register,
                                                input1->data.sse_register);
}


void ivec_sub(ivec *output, ivec const *input0, ivec const *input1)
{
    assert(input0->size == input1->size);
    output->size = input0->size;
    output->data.sse_register =  _mm_sub_epi32(input0->data.sse_register,
                                                input1->data.sse_register);
}


void ivec_cross(ivec *output, ivec const *input0, ivec const *input1)
{
    assert(input0->size == input1->size && input1->size == 3);
    output->size = input0->size;
    output->data.sse_register =  vectori128_cross(input0->data.sse_register,
                                                   input1->data.sse_register);
}


int32_t ivec_dot(ivec const *input0, ivec const *input1)
{
    assert(input0->size == input1->size);
    return vectori128_dot(input0->data.sse_register, input1->data.sse_register);
}


void ivec_mul(ivec *output, ivec const *input0, ivec const *input1)
{
    assert(input0->size == input1->size);
    output->size = input0->size;
    output->data.sse_register =  v_mul_i32(input0->data.sse_register,
                                                    input1->data.sse_register);
}


void ivec_scale(ivec *output, ivec const *input, int32_t scalar)
{
    __m128i scaling_vec = _mm_set1_epi32(scalar);
    output->size = input->size;
    output->data.sse_register = v_mul_i32(input->data.sse_register, scaling_vec);

}
