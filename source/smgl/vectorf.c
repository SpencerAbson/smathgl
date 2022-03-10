#include <math.h>
#include <assert.h>
#include <string.h>
#include "vectorf.h"
#include "simd/vectorf128.h"


void fvec_cross(fvec *output, fvec const *input0, fvec const *input1)
{
    assert(input0->size == input1->size && input1->size == 3);
    output->size = input0->size;
    output->data.sse_register =  vectorf128_cross(input0->data.sse_register,
                                                    input1->data.sse_register);
}


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


#if SMGL_INSTRSET >= 3 // _mm_hadd_ps for > SEE 3
void fvec_normalize(fvec *output, fvec const *input)
{
    assert(input->size < 5 && input->size > 1);
    output->size = input->size;
    output->data.sse_register = vectorf128_normalize(input->data.sse_register);
}

#else

void fvec_normalize(fvec *out, fvec const *input)
{
    // this is... something
    assert(input->size < 5 && input->size > 1);
    out->size = input->size;
    float vec_sqr_sum = 0.0f;
    for (uint32_t i = 0; i < input->size; i++)
        vec_sqr_sum += powf(input->data.values[i], 2);

    float dividor = sqrtf(vec_sqr_sum);

    for (uint32_t i = 0; i < input->size; i++)
        out->data.values[i] = input->data.values[i] / dividor;
}

#endif
