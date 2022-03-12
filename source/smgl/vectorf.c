#include <math.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "vectorf.h"
#include "simd/vectorf128.h"


float fvec_dot(fvec_t const *input0, fvec_t const *input1)
{
    assert(input0->size == input1->size);
    return vectorf128_dot(input0->data.sse_register, input1->data.sse_register);
}


void fvec_scale(fvec_t *output, fvec_t const *input, float scalar)
{
    __m128 scaling_vec = _mm_set_ps1(scalar);
    output->size = input->size;
    output->data.sse_register = _mm_mul_ps(input->data.sse_register, scaling_vec);
}


float fvec_min(fvec_t const *input)
{
    float min;
    __m128 min_vec = vectorf128_min(input->data.sse_register);
    _mm_store_ss(&min, min_vec);
    return min;
}


void fvec_display(fvec_t const *input)
{
    printf("[ ");
    for(uint32_t i = 0; i < input->size; i++)
        printf("%6.4f ", input->data.values[i]);
    printf(" ]");
}
