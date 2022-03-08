#include <math.h>
#include <assert.h>
#include <string.h>
#include "vectorf.h"
#include "simd/vectorf128.h"


fvec fvec_add(fvec const *input0, fvec const *input1)
{
    fvec output;
    assert(input0->size == input1->size);
    output.size = input0->size;
    output.data.sse_register =  _mm_add_ps(input0->data.sse_register,
                                                input1->data.sse_register);
    return output;
}


fvec fvec_sub(fvec const *input0, fvec const *input1)
{
    fvec output;
    assert(input0->size == input1->size);
    output.size = input0->size;
    output.data.sse_register =  _mm_sub_ps(input0->data.sse_register,
                                                input1->data.sse_register);
    return output;
}


fvec fvec_cross(fvec const *input0, fvec const *input1)
{
    fvec output;
    assert(input0->size == input1->size && input1->size == 3);
    output.size = input0->size;
    output.data.sse_register =  vectorf128_cross(input0->data.sse_register,
                                                    input1->data.sse_register);

    return output;
}


float fvec_dot(fvec const *input0, fvec const *input1)
{
    assert(input0->size == input1->size);
    return vectorf128_dot(input0->data.sse_register, input1->data.sse_register);
}


fvec fvec_mul(fvec const *input0, fvec const *input1)
{
    fvec output;
    assert(input0->size == input1->size);
    output.size = input0->size;
    output.data.sse_register =  _mm_mul_ps(input0->data.sse_register,
                                                    input1->data.sse_register);

    return output;
}


fvec fvec_scale(fvec const *input, float scalar)
{
    fvec output;
    __m128 scaling_vec = _mm_set_ps1(scalar);
    output.size = input->size;
    output.data.sse_register = _mm_mul_ps(input->data.sse_register, scaling_vec);

    return output;
}


#if SMGL_INSTRSET >= 3 // _mm_hadd_ps for > SEE 3
fvec fvec_normalize(fvec const *input)
{
    assert(input->size < 5 && input->size > 1);
    fvec output;
    output.size = input->size;
    output.data.sse_register = vectorf128_normalize(input->data.sse_register);
    return output;
}

#else

fvec fvec_normalize(fvec const *input)
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


/* Vector initlisers  */
fvec fvec4_init(float x, float y, float z, float w)
{
    fvec output;
    output.size = 4;
    output.data.sse_register = _mm_set_ps(w, z, y, x);
    return output;
}


fvec fvec3_init(float x, float y, float z)
{
    fvec output;
    output.size = 3;
    output.data.sse_register = _mm_set_ps(0.0f, z, y, x);
    return output;
}


fvec fvec2_init(float x, float y)
{
    fvec output;
    output.size = 2;
    output.data.sse_register = _mm_set_ps(0.0f, 0.0f, y, x);
    return output;
}
