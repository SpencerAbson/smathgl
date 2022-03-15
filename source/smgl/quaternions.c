#include <math.h>
#include <assert.h>
#include "quaternions.h"
#include "vectorf.h"
#include "simd/vectorf128.h"
#include "simd/quaternionf128.h"
#include "matrices.h"


void quat_rotate(quat_t *output, quat_t const *q0, fvec_t const *axis, const float angle)
{
    assert(axis->size == 3);
    __m128 q_of_rotation = _mm_set_ps(axis->data.values[2], axis->data.values[1],  // replace with shuffle
                                      axis->data.values[0], angle); // w, x, y, z

    output->sse_register = quaternionf128_pure_rotate(q0->sse_register, q_of_rotation);
    return;
}


void quat_interpolate(quat_t *output, quat_t const*q0, quat_t const *q1, float interp_param)
{
    assert(interp_param > 0.0f && interp_param < 1.0f);
    output->sse_register = quaternionf128_slerp(q0->sse_register, q1->sse_register, interp_param);

    return;
}


void quat_rotate_mat4(mat4_t *output, quat_t const *q0, fvec_t const* axis, const float angle)
{
    quat_t q_of_rotation;
    quat_rotate(&q_of_rotation, q0, axis, angle);
    register const float double_x_sqr = 2.0f * q_of_rotation.values[1] * q_of_rotation.values[1];
    register const float double_y_sqr = 2.0f * q_of_rotation.values[2] * q_of_rotation.values[2];
    register const float double_z_sqr = 2.0f * q_of_rotation.values[3] * q_of_rotation.values[3];

    const float v0n = (2.0f * q_of_rotation.values[1] * q_of_rotation.values[2]) - (2.0f * q_of_rotation.values[0] * q_of_rotation.values[3]);
    const float v0p = (2.0f * q_of_rotation.values[1] * q_of_rotation.values[2]) + (2.0f * q_of_rotation.values[0] * q_of_rotation.values[3]);
    const float v1p = (2.0f * q_of_rotation.values[1] * q_of_rotation.values[3]) + (2.0f * q_of_rotation.values[0] * q_of_rotation.values[2]);
    const float v1n = (2.0f * q_of_rotation.values[1] * q_of_rotation.values[3]) - (2.0f * q_of_rotation.values[0] * q_of_rotation.values[2]);
    const float v2n = (2.0f * q_of_rotation.values[2] * q_of_rotation.values[3]) - (2.0f * q_of_rotation.values[0] * q_of_rotation.values[1]);
    const float v2p = (2.0f * q_of_rotation.values[2] * q_of_rotation.values[3]) + (2.0f * q_of_rotation.values[0] * q_of_rotation.values[1]);

    output->sse_registers[0] = _mm_set_ps(0.0f, v1n, v0p, 1.0f - double_y_sqr - double_z_sqr);
    output->sse_registers[1] = _mm_set_ps(0.0f, v2p, 1.0f - double_x_sqr - double_z_sqr, v0n);
    output->sse_registers[2] = _mm_set_ps(0.0f, 1.0f - double_x_sqr - double_y_sqr, v2n, v1p);
    output->sse_registers[3] = _mm_set_ps(1.0f, 0.0f, 0.0f, 0.0f);
}
