#include <assert.h>
#include <math.h>
#include "vectori.h"
#include "..\..\include/platform.h"
#include "simd/vectori128.h"

/* will hold more complex functions using multiple built-up simd instructions (worth the overhead)*/

int32_t ivec_dot(ivec const *input0, ivec const *input1)
{
    assert(input0->size == input1->size);
    return vectori128_dot(input0->data.sse_register, input1->data.sse_register);
}


void ivec_scale(ivec *output, ivec const *input, int32_t scalar)
{
    __m128i scaling_vec = _mm_set1_epi32(scalar);
    output->size = input->size;
    output->data.sse_register = v_mul_i32(input->data.sse_register, scaling_vec);
}
