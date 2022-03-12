#include <assert.h>
#include "matrix_vector_op.h"
#include "simd/matrix4xm128f.h"
#include "quaternions.h"


void mat4_perspective(mat4_t *out, float angle_rad, float aspect_ratio, float near, float far) // generate view perspective matrix
{
    // no need to init as all regs are due to be set
    float tan_half_fov = tanf(angle_rad / 2.0f);
    out->sse_registers[0] = _mm_set_ps(0.0f, 0.0f, 0.0f, 1.0f / (aspect_ratio * tan_half_fov));
    out->sse_registers[1] = _mm_set_ps(0.0f, 0.0f, 1.0f / tan_half_fov, 0.0f);
    out->sse_registers[2] = _mm_set_ps(-1.0f, -(far + near) / (far - near), 0.0f, 0.0f);
    out->sse_registers[3] = _mm_set_ps(0.0f, -(2 * far * near) / (far - near), 0.0f, 0.0f);
    return;
}


void mat4_lookat(mat4_t *out, fvec_t const *pos, fvec_t const *target, fvec_t const *up)
{
    assert(pos->size == 3 && target->size == 3 && up->size == 3);
    fvec_t cam_right, cam_up, cam_direction;
    fvec_mm_sub(cam_direction, *pos, *target);
    fvec_mm_normalize(cam_direction, cam_direction);
    fvec_mm_cross(cam_right, *up, cam_direction);
    fvec_mm_normalize(cam_right, cam_right);

    fvec_mm_cross(cam_up, cam_direction, cam_right);
    fvec_mm_normalize(cam_up, cam_up);
    mat4_t m1, m2;
    m1.sse_registers[0] = _mm_set_ps(0.0f, cam_direction.data.values[0], cam_up.data.values[0], cam_right.data.values[0]);
    m1.sse_registers[1] = _mm_set_ps(0.0f, cam_direction.data.values[1], cam_up.data.values[1], cam_right.data.values[1]);
    m1.sse_registers[2] = _mm_set_ps(0.0f, cam_direction.data.values[2], cam_up.data.values[2], cam_right.data.values[2]);
    m1.sse_registers[3] = _mm_set_ps(1.0f, 0.0f, 0.0f, 0.0f);

    mat4_init_diagonal(&m2, 1.0f);
    m2.sse_registers[3] = _mm_set_ps(1.0f, -pos->data.values[2], -pos->data.values[1], -pos->data.values[0]);
    mat4_mm_mul(*out, m1, m2);
}


void mat4_rotate(mat4_t *output, mat4_t const *input, fvec_t const* axis, float angle_rad)
{
    // generate quaternion rep of rotation by rotaing {1, 0, 0, 0} by input
    quat_t identity;
    quat_mm_init(identity, 1.0f, 0.0f, 0.0f, 0.0f);
    fvec_t normed_axis;
    fvec_mm_normalize(normed_axis, *axis);
    mat4_t rotator;
    quat_rotate_mat4(&rotator, &identity, &normed_axis, angle_rad);

    mat4_mm_mul(*output, *input, rotator);
}


void vec4_outer_product(mat4_t *output, fvec_t const *input0,  fvec_t const *input1)
{
    assert(input0->size == input1->size && input0->size == 4);
    mat4xm128_outer_product(input0->data.sse_register, input1->data.sse_register,
                            output->sse_registers);
    return;
}


void mat4_vec_product(fvec_t *output, mat4_t const *mat, fvec_t const *vec)
{
    assert(vec->size == 4);
    output->size = vec->size;
    output->data.sse_register = mat4xm128_vec4_product(mat->sse_registers, vec->data.sse_register);
    return;
}


void mat4_euler_rotation(mat4_t *rotation_matrix, float angle, fvec_t *unit_vector) // note angle in radians
{
    assert(unit_vector->size == 3);
    const float cos_a = cosf(angle);
    const float sin_a = sinf(angle);
    const float cos_a_n_1 = 1.0f - cos_a;
    const float u_x2 = powf(unit_vector->data.values[0], 2);
    const float u_y2 = powf(unit_vector->data.values[1], 2);
    const float u_z2 = powf(unit_vector->data.values[2], 2);

    // Euler rotation matrix
    rotation_matrix->values[0][0] = cos_a + (u_x2 * (1 - cos_a));
    rotation_matrix->values[1][0] = (unit_vector->data.values[0] * unit_vector->data.values[1] * cos_a_n_1) - (unit_vector->data.values[2] * sin_a);
    rotation_matrix->values[2][0] = (unit_vector->data.values[0] * unit_vector->data.values[2] * cos_a_n_1) + (unit_vector->data.values[1] * sin_a);
    rotation_matrix->values[0][3] = 0.0f;
    rotation_matrix->values[0][1] = (unit_vector->data.values[0] * unit_vector->data.values[1] * cos_a_n_1) + (unit_vector->data.values[2] * sin_a);
    rotation_matrix->values[1][1] = cos_a + (u_y2 * (cos_a_n_1));
    rotation_matrix->values[2][1] = (unit_vector->data.values[1] * unit_vector->data.values[2] * cos_a_n_1) - (unit_vector->data.values[0] * sin_a);
    rotation_matrix->values[1][3] = 0.0f;
    rotation_matrix->values[0][2] = (unit_vector->data.values[2] * unit_vector->data.values[0] * cos_a_n_1) - (unit_vector->data.values[1] * sin_a);
    rotation_matrix->values[1][2] = (unit_vector->data.values[2] * unit_vector->data.values[1] * cos_a_n_1) + (unit_vector->data.values[0] * sin_a);
    rotation_matrix->values[2][2] = cos_a + (u_z2 * (cos_a_n_1));
    rotation_matrix->values[2][3] = 0.0f;
    rotation_matrix->values[3][0] = 0.0f;
    rotation_matrix->values[3][1] = 0.0f;
    rotation_matrix->values[3][2] = 0.0f;
    rotation_matrix->values[3][3] = 1.0f;
}

/*
**    out.values[0][0] = 1 / (aspect_ratio * tan_half_fov);
    out.values[1][1] = 1 / tan_half_fov;
    out.values[2][2] = -(far + near) / (far - near);
    out.values[2][3] = -1;
    out.values[3][2] = -(2 * far * near) / (far - near);
**
*/
