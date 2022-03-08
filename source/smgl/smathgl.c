#include <math.h>
#include <stdint.h>
#include <assert.h>
#include <stdio.h>
#include "..\..\include/smathgl.h"
#include "..\..\include/platform.h"
// original functionality, slowly becoming deprecated


mat4x4 mat4_rotation(float angle, fvec *unit_vector) // note angle in radians
{
    mat4x4 rotation_matrix;
    assert(unit_vector->size == 3);
    const float cos_a = cosf(angle);
    const float sin_a = sinf(angle);
    const float cos_a_n_1 = 1.0f - cos_a;
    const float u_x2 = powf(unit_vector->data.values[0], 2);
    const float u_y2 = powf(unit_vector->data.values[1], 2);
    const float u_z2 = powf(unit_vector->data.values[2], 2);

    // Euler rotation matrix
    rotation_matrix.values[0][0] = cos_a + (u_x2 * (1 - cos_a));
    rotation_matrix.values[1][0] = (unit_vector->data.values[0] * unit_vector->data.values[1] * cos_a_n_1) - (unit_vector->data.values[2] * sin_a);
    rotation_matrix.values[2][0] = (unit_vector->data.values[0] * unit_vector->data.values[2] * cos_a_n_1) + (unit_vector->data.values[1] * sin_a);
    rotation_matrix.values[0][3] = 0.0f;
    rotation_matrix.values[0][1] = (unit_vector->data.values[0] * unit_vector->data.values[1] * cos_a_n_1) + (unit_vector->data.values[2] * sin_a);
    rotation_matrix.values[1][1] = cos_a + (u_y2 * (cos_a_n_1));
    rotation_matrix.values[2][1] = (unit_vector->data.values[1] * unit_vector->data.values[2] * cos_a_n_1) - (unit_vector->data.values[0] * sin_a);
    rotation_matrix.values[1][3] = 0.0f;
    rotation_matrix.values[0][2] = (unit_vector->data.values[2] * unit_vector->data.values[0] * cos_a_n_1) - (unit_vector->data.values[1] * sin_a);
    rotation_matrix.values[1][2] = (unit_vector->data.values[2] * unit_vector->data.values[1] * cos_a_n_1) + (unit_vector->data.values[0] * sin_a);
    rotation_matrix.values[2][2] = cos_a + (u_z2 * (cos_a_n_1));
    rotation_matrix.values[2][3] = 0.0f;
    rotation_matrix.values[3][0] = 0.0f;
    rotation_matrix.values[3][1] = 0.0f;
    rotation_matrix.values[3][2] = 0.0f;
    rotation_matrix.values[3][3] = 1.0f;

    return rotation_matrix;
}

/*
mat4x4 mat4_rotate(mat4x4 const *mat_in, float angle, fvec *unit_vector)
{
    assert(unit_vector->size == 3);
    mat4x4 rotation_matrix = mat4_rotation(angle, unit_vector);

    return mat4_mul(mat_in, &rotation_matrix);
}


mat4x4 mat4_perspective(float angle_rad, float aspect_ratio, float near, float far)
{
    mat4x4 out = mat4_init();
    float tan_half_fov = tanf(angle_rad / 2.0f);
    out.values[0][0] = 1 / (aspect_ratio * tan_half_fov);
    out.values[1][1] = 1 / tan_half_fov;
    out.values[2][2] = -(far + near) / (far - near);
    out.values[2][3] = -1;
    out.values[3][2] = -(2 * far * near) / (far - near);

    return out;
}


mat4x4 mat4_lookat(fvec *pos, fvec *target, fvec *up)
{
    assert(pos->size == 3 && target->size == 3 && up->size == 3);
    fvec cam_right, cam_up, cam_direction;
    cam_direction = fvec_sub(pos, target);
    cam_direction = fvec_normalize(&cam_direction);
    cam_right     = fvec_cross(up, &cam_direction);
    cam_right     = fvec_normalize(&cam_right);

    cam_up = fvec_cross(&cam_direction, &cam_right);
    cam_up = fvec_normalize(&cam_up);
    mat4x4 m1, m2;
    m1.sse_registers[0] = _mm_set_ps(0.0f, cam_direction.data.values[0], cam_up.data.values[0], cam_right.data.values[0]);
    m1.sse_registers[1] = _mm_set_ps(0.0f, cam_direction.data.values[1], cam_up.data.values[1], cam_right.data.values[1]);
    m1.sse_registers[2] = _mm_set_ps(0.0f, cam_direction.data.values[2], cam_up.data.values[2], cam_right.data.values[2]);
    m1.sse_registers[3] = _mm_set_ps(1.0f, 0.0f, 0.0f, 0.0f);

    m2 = mat4_init_translation(1.0f);
    m2.sse_registers[3] = _mm_set_ps(1.0f, -pos->data.values[2], -pos->data.values[1], -pos->data.values[0]);
    return mat4_mul(&m1, &m2);
}
*/

    /*m1[0][0] = cam_right.data.values[0];
    m1[1][0] = cam_right.data.values[1];
    m1[2][0] = cam_right.data.values[2];
    m1[3][0] = 0.0f;
    m1[0][1] = cam_up.data.values[0];
    m1[1][1] = cam_up.data.values[1];
    m1[2][1] = cam_up.data.values[2];
    m1[3][1] = 0.0f;
    m1[0][2] = cam_direction.data.values[0];
    m1[1][2] = cam_direction.data.values[1];
    m1[2][2] = cam_direction.data.values[2];
    m1[3][2] = 0.0f;
    m1[3][3] = 1.0f;

    m2[0][0] = 1.0f;
    m2[0][1] = 0.0f;
    m2[0][2] = 0.0f;
    m2[0][3] = 0.0f;
    m2[1][0] = 0.0f;
    m2[1][1] = 1.0f;
    m2[1][2] = 0.0f;
    m2[1][3] = 0.0f;
    m2[2][0] = 0.0f;
    m2[2][1] = 0.0f;
    m2[2][2] = 1.0f;
    m2[2][3] = 0.0f;
    m2[3][3] = 1.0f;
    m2[3][0] = -pos->data.values[0];
    m2[3][1] = -pos->data.values[1];
    m2[3][2] = -pos->data.values[2];*/
