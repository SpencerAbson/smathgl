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

// quat rotations are slow for now
static inline __m128 quaternionf128_pure_rotate(__m128 const original, __m128 const rotation)
{
    float const angle = _mm_cvtss_f32(rotation);
    float const half_ang = angle / 2.0f;
    float const sin_hang = sinf(half_ang);

    __m128 temp           = _mm_set_ss(angle - 1.0f);  // so that we get 1.0, x, y, z when we do next line
    __m128 local_rotation = _mm_sub_ps(rotation, temp);
    __m128 transform      = _mm_set_ps(sin_hang, sin_hang, sin_hang, cosf(half_ang));

    local_rotation = _mm_mul_ps(local_rotation, transform); // q of rotation
    temp           = quaternionf128_mul(local_rotation, original);

    return temp;
}


static inline __m128 quaternionf128_set_known_rotation(__m128 const rotation, float angle)
{
    float const half_ang = angle / 2.0f;
    float const sin_hang = sinf(half_ang);

    __m128 temp           = _mm_set_ss(angle - 1.0f);  // so that we get 1.0, x, y, z when we do next line
    __m128 local_rotation = _mm_sub_ps(rotation, temp);
    __m128 transform      = _mm_set_ps(sin_hang, sin_hang, sin_hang, cosf(half_ang));

    return _mm_mul_ps(local_rotation, transform);
}

/* In a few occasions, the user may have supplied the angle as an argument and it is us who have formatted it into the quaternion,
 * in such cases, it is illogical to now call _mm_store_ss to retreive said angle after just storing it - this can increase perf by 5-10% */
static inline __m128 quaternionf128_known_rotate(__m128 const original, __m128 const rotation, float angle)
{
    __m128 local_rotation = quaternionf128_set_known_rotation(rotation, angle);
    return quaternionf128_mul(local_rotation, original);
}


static inline __m128 quaternionf128_inverse(__m128 const input)  // q^-1 = q^* / |q|
{
    __m128 conjugator = _mm_set_ps(-0.0f, -0.0f, -0.0f, 0.0f);
    __m128 conjugated = _mm_xor_ps(input, conjugator); // negated vector component of quaternion

    __m128 square_norm = vectorf128_vector_dot(input, input);

    return _mm_div_ps(conjugated, square_norm);
}


// use this for fast reliable interpolation
static inline __m128 quaternionf128_Nlerp(__m128 const q0, __m128 const q1, float blend) // linear quaternion interpolation
{
    __m128 output;
    const float dot     = vectorf128_dot(q0, q1);
    const float blend_n = 1.0f - blend;

    __m128 v_blend   = _mm_set1_ps(blend); // [blend, blend, blend, blend]
    __m128 v_blend_n = _mm_set1_ps(blend_n);
    if(dot < 0.0f)
    {
        __m128 negat = _mm_set_ps(-0.0f, -0.0f, -0.0f, -0.0f);
        __m128 neg_1 = _mm_xor_ps(q1, negat);  // negating q1
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


#if SMGL_INSTRSET > 6

static inline __m128 quaternionf128_slerp(__m128 const input0, __m128 const input1, float interp_param)
{
    __m128 cos_theta = vectorf128_vector_dot(input0, input1);
    __m128 negation  = _mm_and_ps(_mm_cmplt_ps(cos_theta, _mm_setzero_ps()), SET_PS1_FROM_HEX(0x80000000));
    __m128 one_v     = _mm_set1_ps(1.0f);

    cos_theta = _mm_xor_ps(cos_theta, negation);  // ensure angle is +ve
    __m128 theta = M128_FAST_ACOS_APPROX(cos_theta);

    __m128 t_v       = _mm_set_ss(interp_param);
    __m128 one_sub_t = _mm_sub_ps(one_v, t_v); // 1-t, 1, 1, 1

    __m128 t_combined = _mm_shuffle_ps(t_v, one_sub_t, _MM_SHUFFLE(3, 3, 3, 3)); // (1-t), (1-t) t, t
    t_combined = _mm_mul_ps(theta, t_combined); // (1-t)*angle, <-, t*angle, <-

    __m128 theta_sqr = _mm_mul_ps(t_combined, t_combined); // sin(theta) approximation via known polynomial expansion
    __m128 sin_theta = M128_FAST_SIN_APPROX(t_combined, theta_sqr);

    __m128 q0_factor = _mm_shuffle_ps(sin_theta, sin_theta, _MM_SHUFFLE(3, 3, 3, 3));
    __m128 q1_factor = _mm_shuffle_ps(sin_theta, sin_theta, _MM_SHUFFLE(0, 0, 0, 0));

    q0_factor = _mm_xor_ps(q0_factor, negation);
    q0_factor = _mm_mul_ps(q0_factor, input0);

    __m128 res = _mm_fmadd_ps(input1, q1_factor, q0_factor);
    return res;//_mm_mul_ps(_mm_rsqrt_ps(vectorf128_vector_dot(res, res)), res); // normalizing

}

#else  // wayyyy slower, but due to be improved

static inline __m128 quaternionf128_slerp(__m128 const input0, __m128 const input1, float interp_param)
{
    float theta;
    __m128 cos_theta = vectorf128_vector_dot(input0, input1);
    __m128 negation  = _mm_and_ps(_mm_cmplt_ps(cos_theta, _mm_setzero_ps()), SET_PS1_FROM_HEX(0x80000000));
    cos_theta = _mm_xor_ps(cos_theta, negation);  // ensure angle is +ve

    float f_cos_theta = _mm_cvtss_f32(cos_theta);
    theta = acosf(f_cos_theta);
    __m128 sin_theta = _mm_set1_ps(sqrtf(1.0f - f_cos_theta * f_cos_theta));

    __m128 q0_scalar = _mm_set1_ps(sinf((1.0f - interp_param) * theta));
    __m128 q1_scalar = _mm_set1_ps(sinf(interp_param * theta));

    q0_scalar = _mm_xor_ps(q0_scalar, negation);

    __m128 res = _mm_add_ps(_mm_mul_ps(input0, q0_scalar), _mm_mul_ps(input1, q1_scalar));
    res = _mm_div_ps(res, sin_theta);
    return _mm_mul_ps(_mm_rsqrt_ps(vectorf128_vector_dot(res, res)), res); // normalizing
}

#endif


static inline __m128 quaternionf128_squad_interpolate(__m128 q0, __m128 q1, __m128 s0, __m128 s1, float t)
{
    // will pre-compute s values here in real-use so definition will likely become identical to slerp.
    return quaternionf128_slerp(quaternionf128_slerp(q0, q1, t), quaternionf128_slerp(s0, s1, t), 2 * t * (1.0f - t));
}


// integration from angular velocity
static inline __m128 quaternionf128_integrate(__m128 const q0, __m128 const omega, float delta_t)
{
    // q' = /\q q
    __m128 delta_q, s;
    __m128 tmp0  = _mm_set1_ps(1.0f);
    __m128 theta = vectorf128_scale(omega, 0.5f * delta_t);
    const float theta_mag_sqr = vectorf128_dot(theta, theta);

    // set the lowest value in theta to 1.0f
#if SMGL_INSTRSET > 4
    theta = _mm_shuffle_ps(theta, theta, _MM_SHUFFLE(3, 2, 1, 0));
    theta = _mm_insert_ps(theta, tmp0, 0x0);
#else
    __m128 tmp1 = _mm_unpackhi_ps(tmp0, theta);
    __m128 tmp2 = _mm_shuffle_ps(theta, theta, _MM_SHUFFLE(2, 2, 3, 1));
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
