#include <assert.h>
#include <math.h>
#include <stdio.h>
#include "vectori.h"
#include "..\..\include/platform.h"
#include "simd/vectori128.h"

/* will hold more complex functions using multiple built-up simd instructions (worth the overhead)*/

int32_t ivec_dot(ivec_t const *input0, ivec_t const *input1)
{
    assert(input0->size == input1->size);
    return vectori128_dot(input0->data.sse_register, input1->data.sse_register);
}


int32_t ivec_min(ivec_t const *input)
{
    int32_t min;
    __m128i min_vec = vectori128_min(input->data.sse_register);
    v_storeu_i32(&min, min_vec);

    return min;
}


void ivec_scale(ivec_t *output, ivec_t const *input, int32_t scalar)
{
    __m128i scaling_vec = _mm_set1_epi32(scalar);
    output->size = input->size;
    output->data.sse_register = v_mul_i32(input->data.sse_register, scaling_vec);
}


void ivec_display(ivec_t const *input)
{
    printf("[ ");
    for(uint32_t i = 0; i < input->size; i++)
        printf("%d ", input->data.values[i]);
    printf(" ]");
}
