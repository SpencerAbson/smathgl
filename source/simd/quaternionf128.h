#ifndef QUATERNIONF128_H_
#define QUATERNIONF128_H_
#include <math.h>
#include "..\..\include/platform.h"
#include "..\quaternions.h"

static inline __m128 quaternionf128_mul(__m128 a, __m128 b) // credit to Agner Fog https://github.com/vectorclass/add-on/blob/master/quaternion/quaternion.h
{
    __m128 a1123 = _mm_shuffle_ps(a,a,0xE5);
    __m128 a2231 = _mm_shuffle_ps(a,a,0x7A);
    __m128 b1000 = _mm_shuffle_ps(b,b,0x01);
    __m128 b2312 = _mm_shuffle_ps(b,b,0x9E);
    __m128 t1    = _mm_mul_ps(a1123, b1000);
    __m128 t2    = _mm_mul_ps(a2231, b2312);
    __m128 t12   = _mm_add_ps(t1, t2);
    const __m128i mask =_mm_set_epi32(0,0,0,0x80000000);
    __m128 t12m  = _mm_xor_ps(t12, _mm_castsi128_ps(mask));
    __m128 a3312 = _mm_shuffle_ps(a,a,0x9F);
    __m128 b3231 = _mm_shuffle_ps(b,b,0x7B);
    __m128 a0000 = _mm_shuffle_ps(a,a,0x00);
    __m128 t3    = _mm_mul_ps(a3312, b3231);
    __m128 t0    = _mm_mul_ps(a0000, b);
    __m128 t03   = _mm_sub_ps(t0, t3);

    return _mm_add_ps(t03, t12m);
}


static inline __m128 quaternionf128_rotate(const quat q0, const vec3 axis, const float angle)
{
    float half_ang = angle / 2.0f;
    __m128 total          = _mm_load_ps(q0);
    __m128 local_rotation = _mm_set_ps(axis[2], axis[1], axis[0], 1.0f);
    __m128 transform      = _mm_set_ps(sinf(half_ang), sinf(half_ang), sinf(half_ang), cosf(half_ang));

    local_rotation = _mm_mul_ps(local_rotation, transform);
    total          = quaternionf128_mul(local_rotation, total);

    return total;
}


static inline __m128 quaternionf128_pure_rotate(__m128 original, __m128 rotation)
{
    float angle, half_ang;
    _mm_store_ss(&angle, rotation);
    half_ang = angle / 2.0f;

    __m128 temp           = _mm_set_ss(angle - 1.0f);  // so that we get 1.0, x, y, z when we do next line
    __m128 local_rotation = _mm_sub_ps(rotation, temp);
    __m128 transform      = _mm_set_ps(sinf(half_ang), sinf(half_ang), sinf(half_ang), cosf(half_ang));

    local_rotation = _mm_mul_ps(local_rotation, transform);
    original       = quaternionf128_mul(local_rotation, original);

    return original;
}

#endif // QUATERNIONF128_H_
