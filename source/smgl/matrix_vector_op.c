#include "..\..\include/platform.h"
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

}


void mat4_lookat(mat4_t *out, fvec3 const *pos, fvec3 const *target, fvec3 const *up)
{
    SMGL_ASSERT(pos->size == 3 && target->size == 3 && up->size == 3);

    fvec_t cam_right, cam_up, cam_direction;
    fvec_sub(cam_direction, *pos, *target);
    fvec_normalize(cam_direction, cam_direction);
    fvec_cross(cam_right, *up, cam_direction);
    fvec_normalize(cam_right, cam_right);

    fvec_cross(cam_up, cam_direction, cam_right);
    fvec_normalize(cam_up, cam_up);
    mat4_t m1, m2;
    m1.sse_registers[0] = _mm_set_ps(0.0f, cam_direction.data.values[0], cam_up.data.values[0], cam_right.data.values[0]);
    m1.sse_registers[1] = _mm_set_ps(0.0f, cam_direction.data.values[1], cam_up.data.values[1], cam_right.data.values[1]);
    m1.sse_registers[2] = _mm_set_ps(0.0f, cam_direction.data.values[2], cam_up.data.values[2], cam_right.data.values[2]);
    m1.sse_registers[3] = _mm_set_ps(1.0f, 0.0f, 0.0f, 0.0f); // using _mm_set_ps when we know the sse registers are about to be used again

    mat4_init_diagonal(&m2, 1.0f);
    m2.sse_registers[3] = _mm_set_ps(1.0f, -pos->data.values[2], -pos->data.values[1], -pos->data.values[0]);
    mat4_mul(*out, m1, m2);
}


void mat4_set_euler_rotation(mat4_t *rotation_matrix, float angle, fvec3 *unit_vector) // purely for testing against actual SSE 3-7 implemetations
{
    SMGL_ASSERT(unit_vector->size == 3);

    const float cos_a = cosf(angle);
    register const float sin_a = sinf(angle);
    register const float cos_a_n_1 = 1.0f - cos_a;

    const float u_x2 = powf(unit_vector->data.values[0], 2);
    const float u_y2 = powf(unit_vector->data.values[1], 2);
    const float u_z2 = powf(unit_vector->data.values[2], 2);

    // eular rotation matrix values
    const float v0n = (unit_vector->data.values[0] * unit_vector->data.values[1] * cos_a_n_1) - (unit_vector->data.values[2] * sin_a);
    const float v0p = (unit_vector->data.values[0] * unit_vector->data.values[1] * cos_a_n_1) + (unit_vector->data.values[2] * sin_a);
    const float v1n = (unit_vector->data.values[1] * unit_vector->data.values[2] * cos_a_n_1) - (unit_vector->data.values[0] * sin_a);
    const float v1p = (unit_vector->data.values[2] * unit_vector->data.values[1] * cos_a_n_1) + (unit_vector->data.values[0] * sin_a);
    const float v2n = (unit_vector->data.values[2] * unit_vector->data.values[0] * cos_a_n_1) - (unit_vector->data.values[1] * sin_a);
    const float v2p = (unit_vector->data.values[0] * unit_vector->data.values[2] * cos_a_n_1) + (unit_vector->data.values[1] * sin_a);

    rotation_matrix->sse_registers[0] = _mm_set_ps(0.0f, v2n, v0p, cos_a + (u_x2 * (1 - cos_a))); // NOTE: reverse order for _mm_set_ps
    rotation_matrix->sse_registers[1] = _mm_set_ps(0.0f, v1p, cos_a + (u_y2 * (cos_a_n_1)), v0n);
    rotation_matrix->sse_registers[2] = _mm_set_ps(0.0f, cos_a + (u_z2 * (cos_a_n_1)), v1n, v2p);
    rotation_matrix->sse_registers[3] = _mm_set_ps(1.0f, 0.0f, 0.0f, 0.0f);

}


/*
**  out.values[0][0] = 1 / (aspect_ratio * tan_half_fov);
    out.values[1][1] = 1 / tan_half_fov;
    out.values[2][2] = -(far + near) / (far - near);
    out.values[2][3] = -1;
    out.values[3][2] = -(2 * far * near) / (far - near);
**

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
*/
