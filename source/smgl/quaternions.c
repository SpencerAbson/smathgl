#include <math.h>
#include <assert.h>
#include "quaternions.h"
#include "vectorf.h"
#include "simd/vectorf128.h"
#include "simd/quaternionf128.h"
#include "matrices.h"


void quat_rotate(quat *output, quat const *q0, fvec const *axis, const float angle)
{
    assert(axis->size == 3);
    __m128 q_of_rotation = _mm_set_ps(axis->data.values[2], axis->data.values[1],  // replace with shuffle
                                      axis->data.values[0], angle); // w, x, y, z

    output->sse_register = quaternionf128_pure_rotate(q0->sse_register, q_of_rotation);
    return;
}


void quat_interpolate(quat *output, quat const*q0, quat const *q1, float interp_param)
{
    assert(interp_param > 0.0f && interp_param < 1.0f);
    output->sse_register = quaternionf128_slerp(q0->sse_register, q1->sse_register, interp_param);

    return;
}


void quat_rotate_mat4(mat4x4 *out, quat const *q0, fvec const* axis, const float angle)
{
    quat q_of_rotation;
    float double_x_sqr, double_y_sqr, double_z_sqr;

    quat_rotate(&q_of_rotation, q0, axis, angle);
    double_x_sqr = 2.0f * powf(q_of_rotation.values[1], 2);
    double_y_sqr = 2.0f * powf(q_of_rotation.values[2], 2);
    double_z_sqr = 2.0f * powf(q_of_rotation.values[3], 2);

    out->values[0][0] = 1.0f - double_y_sqr - double_z_sqr; // 1-2y2-2z2
    out->values[1][0] = (2.0f * q_of_rotation.values[1] * q_of_rotation.values[2]) - (2.0f * q_of_rotation.values[0] * q_of_rotation.values[3]);
    out->values[2][0] = (2.0f * q_of_rotation.values[1] * q_of_rotation.values[3]) + (2.0f * q_of_rotation.values[0] * q_of_rotation.values[2]);
    out->values[3][0] = 0.0f;
    out->values[0][1] = (2.0f * q_of_rotation.values[1] * q_of_rotation.values[2]) + (2.0f * q_of_rotation.values[0] * q_of_rotation.values[3]);
    out->values[1][1] = 1.0f - double_x_sqr - double_z_sqr;
    out->values[1][2] = (2.0f * q_of_rotation.values[2] * q_of_rotation.values[3]) + (2.0f * q_of_rotation.values[0] * q_of_rotation.values[1]);
    out->values[3][1] = 0.0f;
    out->values[0][2] = (2.0f * q_of_rotation.values[1] * q_of_rotation.values[3]) - (2.0f * q_of_rotation.values[0] * q_of_rotation.values[2]);
    out->values[2][1] = (2.0f * q_of_rotation.values[2] * q_of_rotation.values[3]) - (2.0f * q_of_rotation.values[0] * q_of_rotation.values[1]);
    out->values[2][2] = 1.0f - double_x_sqr - double_y_sqr;
    out->values[3][2] = 0.0f;
    out->values[0][3] = 0.0f;
    out->values[1][3] = 0.0f;
    out->values[2][3] = 0.0f;
    out->values[3][3] = 1.0f;

    return;
}
