#include <math.h>
#include "..\include/quaternions.h"
#include "..\include/vectors.h"
#include "..\include/matrices.h"


static inline __m128 quatf_multiply(__m128 a, __m128 b) // credit to Agner Fog https://github.com/vectorclass/add-on/blob/master/quaternion/quaternion.h
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


void quat_mul(const quat q0, const quat q1, quat out)
{
    __m128 a = _mm_load_ps(q0);
    __m128 b = _mm_load_ps(q1);

    _mm_store_ps(out, quatf_multiply(a, b));
}


void quat_div(const quat q0, const quat q1, quat out)
{
    __m128 a = _mm_load_ps(q0);
    __m128 b = _mm_load_ps(q1);

    _mm_store_ps(out, quatf_multiply(a, vectorf128_reciporical(b)));
}


void quat_rotate(const quat q0, const vec3 axis, const float angle, quat out)
{
    float half_ang = angle / 2.0f;
    __m128 total          = _mm_load_ps(q0);
    __m128 local_rotation = _mm_set_ps(axis[2], axis[1], axis[0], 1.0f);
    __m128 transform      = _mm_set_ps(sinf(half_ang), sinf(half_ang), sinf(half_ang), cosf(half_ang));

    local_rotation = _mm_mul_ps(local_rotation, transform);
    total          = quatf_multiply(local_rotation, total);
    _mm_store_ps(out, total);
}


void quat_rotation_set4x4(const quat q0, const vec3 axis, const float angle, mat4x4 out) // soon to be improved with intrins
{
    quat q_of_rotation;
    float double_x_sqr, double_y_sqr, double_z_sqr;

    quat_rotate(q0, axis, angle, q_of_rotation);
    double_x_sqr = 2 * powf(q_of_rotation[1], 2);
    double_y_sqr = 2 * powf(q_of_rotation[2], 2);
    double_z_sqr = 2 * powf(q_of_rotation[3], 2);

    out[0][0] = 1.0f - double_y_sqr - double_z_sqr; // 1-2y2-2z2
    out[0][1] = (2.0f * q_of_rotation[1] * q_of_rotation[2]) - (2.0f * q_of_rotation[0] * q_of_rotation[3]);
    out[0][2] = (2.0f * q_of_rotation[1] * q_of_rotation[3]) + (2.0f * q_of_rotation[0] * q_of_rotation[2]);
    out[0][3] = 0.0f;
    out[1][0] = (2.0f * q_of_rotation[1] * q_of_rotation[2]) + (2.0f * q_of_rotation[0] * q_of_rotation[3]);
    out[1][1] = 1.0f - double_x_sqr - double_z_sqr;
    out[1][2] = (2.0f * q_of_rotation[2] * q_of_rotation[3]) + (2.0f * q_of_rotation[0] * q_of_rotation[1]);
    out[1][3] = 0.0f;
    out[2][0] = (2.0f * q_of_rotation[1] * q_of_rotation[3]) - (2.0f * q_of_rotation[0] * q_of_rotation[2]);
    out[2][1] = (2.0f * q_of_rotation[2] * q_of_rotation[3]) - (2.0f * q_of_rotation[0] * q_of_rotation[1]);
    out[2][2] = 1.0f - double_x_sqr - double_y_sqr;
    out[2][3] = 0.0f;
    out[3][0] = 0.0f;
    out[3][1] = 0.0f;
    out[3][2] = 0.0f;
    out[3][3] = 1.0f;
}
