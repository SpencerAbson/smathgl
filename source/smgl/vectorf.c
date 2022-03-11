#include <math.h>
#include <assert.h>
#include <string.h>
#include "vectorf.h"
#include "simd/vectorf128.h"


float fvec_dot(fvec const *input0, fvec const *input1)
{
    assert(input0->size == input1->size);
    return vectorf128_dot(input0->data.sse_register, input1->data.sse_register);
}


void fvec_scale(fvec *output, fvec const *input, float scalar)
{
    __m128 scaling_vec = _mm_set_ps1(scalar);
    output->size = input->size;
    output->data.sse_register = _mm_mul_ps(input->data.sse_register, scaling_vec);
}


float fvec_min(fvec const *input)
{
    float min;
    __m128 min_vec = vectorf128_min(input->data.sse_register);
    _mm_store_ss(&min, min_vec);
    return min;
}
