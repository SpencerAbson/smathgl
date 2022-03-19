#ifndef QUATERNIONF128_H_
#define QUATERNIONF128_H_
#include <math.h>
#include <assert.h>
#include "..\..\..\include/platform.h"
#include "..\..\..\include/smathgl.h"
#include "vectorf128.h"


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


/* static inline __m128 quaternionf128_rotate(quat* q0, fvec *axis, const float angle)
{
    assert(axis->size == 3);
    float half_ang = angle / 2.0f;
    __m128 total          = _mm_load_ps(q0->values);
    __m128 local_rotation = _mm_set_ps(axis->data.values[2], axis->data.values[1], axis->data.values[0], 1.0f);
    __m128 transform      = _mm_set_ps(sinf(half_ang), sinf(half_ang), sinf(half_ang), cosf(half_ang));

    local_rotation   = _mm_mul_ps(local_rotation, transform);
    total = quaternionf128_mul(local_rotation, total);

    return total;
}*/


static inline __m128 quaternionf128_pure_rotate(__m128 const original, __m128 const rotation)
{
    float angle;
    _mm_store_ss(&angle, rotation);
    float const half_ang = angle / 2.0f;
    float const sin_hang = sinf(half_ang);

    __m128 temp           = _mm_set_ss(angle - 1.0f);  // so that we get 1.0, x, y, z when we do next line
    __m128 local_rotation = _mm_sub_ps(rotation, temp);
    __m128 transform      = _mm_set_ps(sin_hang, sin_hang, sin_hang, cosf(half_ang));

    local_rotation = _mm_mul_ps(local_rotation, transform); // q of rotation
    temp           = quaternionf128_mul(local_rotation, original);

    return temp;
}


static inline __m128 quaternionf128_inverse(__m128 const input)
{
    __m128 conjugator = _mm_set_ps(-1.0f, -1.0f, -1.0f, 1.0f); // q^-1 = q^* / |q|
    __m128 conjugated = _mm_mul_ps(input, conjugator); // negated vector component of quaternion

    __m128 square_norm = vectorf128_sum(_mm_mul_ps(input, input));

    return _mm_div_ps(conjugated, square_norm);
}


static inline __m128 quaternionf128_slerp(__m128 const input0, __m128 const input1, float interp_param)
{
    float theta, sin_theta;
    const float cos_theta = vectorf128_dot(input0, input1);

    theta = acosf(cos_theta);
    sin_theta = sqrtf(1.0f - cos_theta * cos_theta);
    float q0_scalar = sinf((1.0f - interp_param) * theta) / sin_theta;
    float q1_scalar = sinf(interp_param * theta) / sin_theta;

    return _mm_add_ps(vectorf128_scale(input0, q0_scalar), vectorf128_scale(input1, q1_scalar));
}


static inline __m128 quaternionf128_squad_interpolate(__m128 q0, __m128 q1, __m128 s0, __m128 s1, float t)
{
    // will pre-compute s values here in real-use so definition will likely become identical to slerp.
    return quaternionf128_slerp(quaternionf128_slerp(q0, q1, t), quaternionf128_slerp(s0, s1, t), 2 * t * (1.0f - t));
}


static inline __m128 quaternionf128_Nlerp(__m128 const q0, __m128 const q1, float blend) // linear quaternion interpolation
{
    __m128 output;
    const float dot     = vectorf128_dot(q0, q1);
    const float blend_n = 1.0f - blend;

    __m128 v_blend   = _mm_load_ss(&blend); // [blend, blend, blend, blend]
    __m128 v_blend_n = _mm_load_ss(&blend_n);
    if(dot < 0.0f)
    {
        __m128 neg_1 = vectorf128_scale(q1, -1.0f); // negated q1
        __m128 tmp0  = _mm_mul_ps(v_blend_n, q0);
        __m128 tmp1  = _mm_mul_ps(v_blend, neg_1);

        output = _mm_add_ps(tmp0, tmp1);
    }
    else
    {
        __m128 tmp0  = _mm_mul_ps(v_blend_n, q0);
        __m128 tmp1  = _mm_mul_ps(v_blend, q1);

        output = _mm_add_ps(tmp0, tmp1);
    }

    return vectorf128_normalize(output);
}


// integration from angular velocity
static inline __m128 quaternionf128_integrate(__m128 const q0, __m128 const omega, float delta_t)
{
    // q' = /\q q
    __m128 delta_q, s;
    __m128 tmp0  = _mm_set_ps1(1.0f);
    __m128 theta = vectorf128_scale(omega, 0.5f * delta_t);
    const float theta_mag_sqr = vectorf128_dot(theta, theta);

    // set the lowest value in theta to 1.0f
#if SMGL_INSTRSET > 4
    theta = _mm_insert_ps(theta, tmp0, 0x0);
#else
    __m128 tmp1 = _mm_unpackhi_ps(tmp0, theta);
    __m128 tmp2 = _mm_shuffle_ps(theta, theta, _MM_SHUFFLE(2, 0, 1, 3));
    theta       = _mm_unpacklo_ps(tmp1, tmp2); // theta = (1.0f , x, y, z);
#endif
    if(theta_mag_sqr * theta_mag_sqr / 24.0f < 0.0006f) // it is appropriate to use taylor series approximation
                                                        // NOTE: best guard value not determined
    {
        float ftmp = 1.0f - theta_mag_sqr / 6.0f;
        float w_component = 1.0f - theta_mag_sqr / 2.0f;
        s = _mm_set_ps(ftmp, ftmp, ftmp, w_component);
    }
    else
    {
        float theta_mag = sqrtf(theta_mag_sqr);
        float s_val = sinf(theta_mag) / theta_mag;
        s = _mm_set_ps(s_val, s_val, s_val, cosf(theta_mag));
    }

    delta_q = _mm_mul_ps(theta, s);
    return quaternionf128_mul(delta_q, q0);

}
#endif // QUATERNIONF128_H_
