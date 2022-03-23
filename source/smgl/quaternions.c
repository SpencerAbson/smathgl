#include <math.h>
#include <assert.h>
#include "quaternions.h"
#include "vectorf.h"
#include "simd/vectorf128.h"
#include "simd/quaternionf128.h"
#include "matrices.h"


void quat_rotate(quat_t *output, quat_t const *q0, fvec_t const *axis, float angle)
{
    assert(axis->size == 3);
    __m128 q_of_rotation;

    q_of_rotation =  _mm_set_ps(axis->data.values[2], axis->data.values[1],  // replace with shuffle
                                     axis->data.values[0], angle); // w, x, y, z

    output->sse_register = quaternionf128_known_rotate(q0->sse_register, q_of_rotation, angle);
    return;
}


void quat_interpolate(quat_t *output, quat_t const *q0, quat_t const *q1, float interp_param, QuatInterps_t type)
{
    assert(interp_param > 0.0f && interp_param < 1.0f);
    if(type == SM_QUAT_NLERP)
        output->sse_register = quaternionf128_Nlerp(q0->sse_register, q1->sse_register, interp_param);
    else if(type == SM_QUAT_SLERP)
        output->sse_register = quaternionf128_slerp(q0->sse_register, q1->sse_register, interp_param);
    // TODO: suitable squad implementation

    return;
}


void quat_to_mat4(mat4_t *output, quat_t const *input)
{
    register const float double_x_sqr = 2.0f * input->values[1] * input->values[1];
    register const float double_y_sqr = 2.0f * input->values[2] * input->values[2];
    register const float double_z_sqr = 2.0f * input->values[3] * input->values[3];

    const float v0n = (2.0f * input->values[1] * input->values[2]) - (2.0f * input->values[0] * input->values[3]);
    const float v0p = (2.0f * input->values[1] * input->values[2]) + (2.0f * input->values[0] * input->values[3]);
    const float v1p = (2.0f * input->values[1] * input->values[3]) + (2.0f * input->values[0] * input->values[2]);
    const float v1n = (2.0f * input->values[1] * input->values[3]) - (2.0f * input->values[0] * input->values[2]);
    const float v2n = (2.0f * input->values[2] * input->values[3]) - (2.0f * input->values[0] * input->values[1]);
    const float v2p = (2.0f * input->values[2] * input->values[3]) + (2.0f * input->values[0] * input->values[1]);

    output->sse_registers[0] = _mm_set_ps(0.0f, v1n, v0p, 1.0f - double_y_sqr - double_z_sqr);
    output->sse_registers[1] = _mm_set_ps(0.0f, v2p, 1.0f - double_x_sqr - double_z_sqr, v0n);
    output->sse_registers[2] = _mm_set_ps(0.0f, 1.0f - double_x_sqr - double_y_sqr, v2n, v1p);
    output->sse_registers[3] = _mm_set_ps(1.0f, 0.0f, 0.0f, 0.0f);
}


#if SMGL_INSTRSET > 4
/* This turns out to be incredibly difficult to do in SSE, and boasts little performance gain.
 * This ugly mess does the same thing as above. */
void quat_rotate_set_mat4_pure_simd(mat4_t *output, quat_t const *q0, fvec_t const* axis, const float angle)
{
    quat_t q_of_rotation;
    quat_rotate(&q_of_rotation, q0, axis, angle);

    __m128 fill_0f = _mm_set_ps1(0.0f);
    __m128 fill_1f = _mm_set_ps1(1.0f);
    __m128 fill_2f = _mm_set_ps1(2.0f); // tmp0 looks like [double_w_sqr, double_x_sqr, double_y_sqr, double_z_sqr]
    __m128 tmp0    = _mm_mul_ps(_mm_mul_ps(q_of_rotation.sse_register, q_of_rotation.sse_register), fill_2f);
    tmp0 = _mm_shuffle_ps(tmp0, tmp0, _MM_SHUFFLE(2, 2, 1, 0));

    __m128 tmp1 = _mm_sub_ps(fill_1f, tmp0);
    __m128 savior = _mm_sub_ps(tmp1, _mm_shuffle_ps(tmp1, tmp1, _MM_SHUFFLE(1, 0, 0, 0)));
    // savior = [1 - double_x_sqr - double_y_sqr, 1 - double_x_sqr - double_z_sqr, 1 - double_y_sqr - double_z_sqr, 1 - 4_z_sqr]
    savior = _mm_insert_ps(savior, fill_0f, 0x30);

    __m128 tmp2 = _mm_shuffle_ps(q_of_rotation.sse_register, q_of_rotation.sse_register, _MM_SHUFFLE(2, 2, 1, 0));
    __m128 tmp3 = _mm_shuffle_ps(q_of_rotation.sse_register, q_of_rotation.sse_register, _MM_SHUFFLE(1, 0, 0, 2));

    __m128 comp0 = _mm_mul_ps(tmp2, tmp3);
    comp0 = _mm_mul_ps(comp0, fill_2f);

    __m128 tmp4 = _mm_shuffle_ps(q_of_rotation.sse_register, q_of_rotation.sse_register, _MM_SHUFFLE(3, 3, 3, 3));
    __m128 tmp5 = _mm_shuffle_ps(tmp2, tmp2, _MM_SHUFFLE(0, 1, 2, 3)); // reverse tmp0

    __m128 comp1 = _mm_mul_ps(tmp4, tmp5);
    comp1 = _mm_mul_ps(comp1, fill_2f);

    __m128 vs_1 = _mm_addsub_ps(comp0, comp1);
    __m128 vs_2 = _mm_addsub_ps(comp1, comp0);

    output->sse_registers[0] = _mm_shuffle_ps(vs_1, savior, _MM_SHUFFLE(1, 0, 3, 2)); // yz, 4w, v0p, v1n
    output->sse_registers[0] = _mm_shuffle_ps(output->sse_registers[0], output->sse_registers[0], _MM_SHUFFLE(3, 1, 0, 2)); // correct

    __m128 a0 = _mm_shuffle_ps(vs_1, vs_2, _MM_SHUFFLE(3, 2, 1, 0)); // could still help us
    output->sse_registers[1] = _mm_unpacklo_ps(a0, savior); // [v0n, xz, v2p, 0]

    output->sse_registers[2] = _mm_shuffle_ps(vs_2, savior, _MM_SHUFFLE(2, 1, 0, 3));
    output->sse_registers[3] = _mm_set_ps(1.0f, 0.0f, 0.0f, 0.0f);
}

#endif
