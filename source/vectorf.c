#include <math.h>
#include <assert.h>
#include "..\include/vectors.h"


void vec_add(float const* addr0, float const* addr1, float *out)
{
    __m128 *input0 = (__m128 *)addr0;
    __m128 *input1 = (__m128 *)addr1;

    _mm_store_ps(out, _mm_add_ps(*input0, *input1));
}


void vec_sub(float const* addr0, float const* addr1, float *out)
{
    __m128 *input0 = (__m128 *)addr0;
    __m128 *input1 = (__m128 *)addr1;

    _mm_store_ps(out, _mm_sub_ps(*input0, *input1));
}


void vec_mul(float const *addr0, float const *addr1, float *out)
{
    __m128 input0 = _mm_load_ps(addr0);
    __m128 input1 = _mm_load_ps(addr1);

    _mm_store_ps(out, _mm_mul_ps(input0, input1));
}


void vec_cross(const vec3 addr0, const vec3 addr1, vec3 addr_out)
{
    // cross product can be computed by (y1, z1, x1) * (z2, x2, y2) - (z1, x1, y1) * (y2, z2, x2 )
    __m128 a, b,  v1, v2;
    __m128 input0 = _mm_load_ps(addr0);
    __m128 input1 = _mm_load_ps(addr1);

    a = _mm_shuffle_ps(input0, input0, _MM_SHUFFLE(3, 0, 2, 1)); // a = (y1, z1, x1)
    b = _mm_shuffle_ps(input1, input1, _MM_SHUFFLE(3, 1, 0, 2)); // b = (z2, x2, y2)
    v1 = _mm_mul_ps(a, b);

    a = _mm_shuffle_ps(input0, input0, _MM_SHUFFLE(3, 1, 0, 2)); // a = (z1, x1, y1)
    b = _mm_shuffle_ps(input1, input1, _MM_SHUFFLE(3, 0, 2, 1)); // b = (y2, z2, x2)
    v2 = _mm_mul_ps(a, b);

    _mm_store_ps(addr_out, _mm_sub_ps(v1, v2));
}


float vec_dot(float const *addr0, float const *addr1)
{
    float out;
    __m128 input0 = _mm_load_ps(addr0);
    __m128 input1 = _mm_load_ps(addr1);

    __m128 product = _mm_mul_ps(input0, input1);
    _mm_store_ss(&out, vectorf128_sum(product));
    return out;
}


void vec_scale(float const *addr0, float scalar, float *out)
{
    __m128 input = _mm_load_ps(addr0);
    __m128 scaling_vec = _mm_load_ps1(&scalar);

    _mm_store_ps(out, _mm_mul_ps(input, scaling_vec));
}


#if ! defined( _MSC_VER) && SMGL_INSTRSET >= 3 // if you use a good compiler
void vec_normalize(float const* addr, int size,  float *addr_out)
{
    // Plenty of optimisation available here
    assert(size < 5 && size > 1);
    __m128 dividor;
    __m128 input0 = _mm_load_ps(addr);
    __m128 input1 = _mm_load_ps(addr);

    __m128 components_squared = _mm_mul_ps(input0, input1);
    float divisor = sqrtf(vsum((float*)(&components_squared), size)); // (a^2 + b^2 + c^2)^1/2
    dividor = _mm_load_ps1(&divisor);

    _mm_store_ps(addr_out, _mm_div_ps(input0, dividor));
}
#else

void vec_normalize(float const* addr, int size, float* addr_out)
{
    // Plenty of optimisation available here
    assert(size < 5 && size > 1);
    float vec_sqr_sum = 0.0f;
    for (int i = 0; i < size; i++)
        vec_sqr_sum += powf(addr[i], 2);

    float dividor = sqrtf(vec_sqr_sum);

    for (int i = 0; i < size; i++)
        addr_out[i] = addr[i] / dividor;

}
#endif

float vsum(float const *a, int n)
{
    float sum;
    __m128 vsum = _mm_set1_ps(0.0f);
    assert(((uintptr_t)a & 15) == 0);
    for (int i = 0; i < n; i += 4)
    {
        __m128 v = _mm_load_ps(&a[i]);
        vsum = _mm_add_ps(vsum, v);
    }
    vsum = _mm_hadd_ps(vsum, vsum);
    vsum = _mm_hadd_ps(vsum, vsum);
    _mm_store_ss(&sum, vsum);
    return sum;
}
