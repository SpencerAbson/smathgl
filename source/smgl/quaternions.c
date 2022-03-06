#include <math.h>
#include <assert.h>
#include "quaternions.h"
#include "vectorf.h"
#include "simd/vectorf128.h"
#include "simd/quaternionf128.h"
#include "matrices.h"


quat quat_mul(quat* q0, quat* q1)
{
    quat output;
    output.sse_register = quaternionf128_mul(q0->sse_register, q1->sse_register);
    return output;
}


quat quat_rotate(quat* q0, fvec* axis, const float angle)
{
    assert(axis->size == 3);
    quat output;
    __m128 q_of_rotation = _mm_set_ps(axis->data.values[2], axis->data.values[1],  // replace with shuffle
                                      axis->data.values[0], angle); // w, x, y, z

    output.sse_register = quaternionf128_pure_rotate(q0->sse_register, q_of_rotation);
    return output;
}


quat quat_interpolate(quat* q0, quat* q1, float interp_param)
{
    assert(interp_param > 0.0f && interp_param < 1.0f);
    quat output;
    output.sse_register = quaternionf128_slerp(q0->sse_register, q1->sse_register, interp_param);

    return output;
}


mat4x4 quat_rotate_mat4(quat* q0, fvec* axis, const float angle)
{
    quat q_of_rotation;
    mat4x4 out;
    float double_x_sqr, double_y_sqr, double_z_sqr;

    q_of_rotation = quat_rotate(q0, axis, angle);
    double_x_sqr = 2.0f * powf(q_of_rotation.values[1], 2);
    double_y_sqr = 2.0f * powf(q_of_rotation.values[2], 2);
    double_z_sqr = 2.0f * powf(q_of_rotation.values[3], 2);

    out.values[0][0] = 1.0f - double_y_sqr - double_z_sqr; // 1-2y2-2z2
    out.values[0][1] = (2.0f * q_of_rotation.values[1] * q_of_rotation.values[2]) - (2.0f * q_of_rotation.values[0] * q_of_rotation.values[3]);
    out.values[0][2] = (2.0f * q_of_rotation.values[1] * q_of_rotation.values[3]) + (2.0f * q_of_rotation.values[0] * q_of_rotation.values[2]);
    out.values[0][3] = 0.0f;
    out.values[1][0] = (2.0f * q_of_rotation.values[1] * q_of_rotation.values[2]) + (2.0f * q_of_rotation.values[0] * q_of_rotation.values[3]);
    out.values[1][1] = 1.0f - double_x_sqr - double_z_sqr;
    out.values[1][2] = (2.0f * q_of_rotation.values[2] * q_of_rotation.values[3]) + (2.0f * q_of_rotation.values[0] * q_of_rotation.values[1]);
    out.values[1][3] = 0.0f;
    out.values[2][0] = (2.0f * q_of_rotation.values[1] * q_of_rotation.values[3]) - (2.0f * q_of_rotation.values[0] * q_of_rotation.values[2]);
    out.values[2][1] = (2.0f * q_of_rotation.values[2] * q_of_rotation.values[3]) - (2.0f * q_of_rotation.values[0] * q_of_rotation.values[1]);
    out.values[2][2] = 1.0f - double_x_sqr - double_y_sqr;
    out.values[2][3] = 0.0f;
    out.values[3][0] = 0.0f;
    out.values[3][1] = 0.0f;
    out.values[3][2] = 0.0f;
    out.values[3][3] = 1.0f;

    return out;
}


quat quat_inverse(quat* input)
{
    quat output;
    output.sse_register = quaternionf128_inverse(input->sse_register);
    return output;
}


quat quat_normalize(quat *input)
{
    quat output;
    output.sse_register = vectorf128_normalize(input->sse_register);
    return output;
}
