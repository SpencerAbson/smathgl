#include <math.h>
#include <assert.h>
#include "vectorf.h"
#include "simd/vectorf128.h"

void vec_cross(float const *addr0, float const *addr1, float *addr_out)
{
    __m128 input0 = _mm_load_ps(addr0);
    __m128 input1 = _mm_load_ps(addr1);

    _mm_store_ps(addr_out, vectorf128_cross(input0, input1));
}


float vec_dot(float const *addr0, float const *addr1)
{
    __m128 input0 = _mm_load_ps(addr0);
    __m128 input1 = _mm_load_ps(addr1);

    return vectorf128_dot(input0, input1);
}


void vec_add(float const* addr0, float const* addr1, float *out)
{
    __m128 input0 = _mm_load_ps(addr0);
    __m128 input1 = _mm_load_ps(addr1);

    _mm_store_ps(out, _mm_add_ps(input0, input1));
}


void vec_sub(float const* addr0, float const* addr1, float *out)
{
    __m128 input0 = _mm_load_ps(addr0);
    __m128 input1 = _mm_load_ps(addr1);

    _mm_store_ps(out, _mm_sub_ps(input0, input1));
}


void vec_mul(float const *addr0, float const *addr1, float *out)
{
    __m128 input0 = _mm_load_ps(addr0);
    __m128 input1 = _mm_load_ps(addr1);

    _mm_store_ps(out, _mm_mul_ps(input0, input1));
}


void vec_scale(float const *addr0, float scalar, float *out)
{
    __m128 input = _mm_load_ps(addr0);
    __m128 scaling_vec = _mm_load_ps1(&scalar);

    _mm_store_ps(out, _mm_mul_ps(input, scaling_vec));
}


#if SMGL_INSTRSET >= 3 // _mm_hadd_ps for > SEE 3
void vec_normalize(float const* addr, int size,  float *addr_out)
{
    assert(size < 5 && size > 1);
    __m128 input = _mm_load_ps(addr);

    _mm_store_ps(addr_out, vectorf128_normalize(input));
}
#else

void vec_normalize(float const* addr, int size, float* addr_out)
{
    // this is... something
    assert(size < 5 && size > 1);
    float vec_sqr_sum = 0.0f;
    for (int i = 0; i < size; i++)
        vec_sqr_sum += powf(addr[i], 2);

    float dividor = sqrtf(vec_sqr_sum);

    for (int i = 0; i < size; i++)
        addr_out[i] = addr[i] / dividor;

}
#endif
