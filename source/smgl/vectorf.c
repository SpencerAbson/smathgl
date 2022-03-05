#include <math.h>
#include <assert.h>
#include "vectorf.h"
#include "simd/vectorf128.h"


fvec fvec_add(fvec *input0, fvec *input1)
{
    fvec output;
    assert(input0->size == input1->size);
    output.size = input0->size;
    _mm_store_ps(output.data.values, _mm_add_ps(input0->data.sse_register,
                                                input1->data.sse_register));
    return output;
}


fvec fvec_sub(fvec *input0, fvec *input1)
{
    fvec output;
    assert(input0->size == input1->size);
    output.size = input0->size;
    _mm_store_ps(output.data.values, _mm_sub_ps(input0->data.sse_register,
                                                input1->data.sse_register));
    return output;
}


fvec fvec_cross(fvec *input0, fvec *input1)
{
    fvec output;
    assert(input0->size == input1->size && input1->size == 3);
    output.size = input0->size;
    _mm_store_ps(output.data.values, vectorf128_cross(input0->data.sse_register,
                                                    input1->data.sse_register));

    return output;
}


float fvec_dot(fvec *input0, fvec *input1)
{
    assert(input0->size == input1->size);
    return vectorf128_dot(input0->data.sse_register, input1->data.sse_register);
}


fvec fvec_mul(fvec *input0, fvec *input1)
{
    fvec output;
    assert(input0->size == input1->size && input1->size == 3);
    output.size = input0->size;
    _mm_store_ps(output.data.values, _mm_mul_ps(input0->data.sse_register,
                                                    input1->data.sse_register));

    return output;
}


fvec fvec_scale(fvec *input, float scalar)
{
    fvec output;
    __m128 scaling_vec = _mm_set_ps1(scalar);
    output.size = input->size;
    _mm_store_ps(output.data.values, _mm_mul_ps(input->data.sse_register, scaling_vec));

    return output;
}



#if SMGL_INSTRSET >= 3 // _mm_hadd_ps for > SEE 3

fvec fvec_normalize(fvec *input)
{
    assert(input->size < 5 && input->size > 1);
    fvec output;
    output.size = input->size;
    _mm_store_ps(output.data.values, vectorf128_normalize(input->data.sse_register));
    return output;
}

#else

fvec fvec_normalize(fvec *input)
{
    // this is... something
    assert(input->size < 5 && input->size > 1);
    fvec out;
    out.size = input->size;
    float vec_sqr_sum = 0.0f;
    for (uint32_t i = 0; i < input->size; i++)
        vec_sqr_sum += powf(input->data.values[i], 2);

    float dividor = sqrtf(vec_sqr_sum);

    for (uint32_t i = 0; i < input->size; i++)
        out.data.values[i] = input->data.values[i] / dividor;

    return out;
}
#endif
