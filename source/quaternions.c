#include <math.h>
#include "..\include/quaternions.h"
#include "..\include/vectors.h"
#include "..\include/matrices.h"
#include "simd/quaternionf128.h"


void quat_mul(const quat q0, const quat q1, quat out)
{
    __m128 a = _mm_load_ps(q0);
    __m128 b = _mm_load_ps(q1);

    _mm_store_ps(out, quaternionf128_mul(a, b));
}


void quat_div(const quat q0, const quat q1, quat out)
{
    __m128 a = _mm_load_ps(q0);
    __m128 b = _mm_load_ps(q1);

    _mm_store_ps(out, quaternionf128_mul(a, vectorf128_reciporical(b)));
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


void quat_rotate(const quat q0, const vec3 axis, const float angle, quat out) // gonna change
{
    _mm_store_ps(out, quaternionf128_rotate(q0, axis, angle));
}
