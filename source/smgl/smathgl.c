#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include "..\..\include/smathgl.h"



void rotate4x4(mat4x4 mat_in, float angle, vec3 unit_vector, mat4x4 out)
{
    mat4x4 rotation_matrix;
    set_rotation4x4(rotation_matrix, angle, unit_vector);

    mat4_mul(mat_in, rotation_matrix, out);
}


void set_rotation4x4(mat4x4 rotation_matrix, float angle, vec3 unit_vector) // note angle in radians
{
    const float cos_a = cosf(angle);
    const float sin_a = sinf(angle);
    const float cos_a_n_1 = 1 - cos_a;
    const float u_x2 = powf(unit_vector[0], 2);
    const float u_y2 = powf(unit_vector[1], 2);
    const float u_z2 = powf(unit_vector[2], 2);

    // Euler rotation matrix
    rotation_matrix[0][0] = cos_a + (u_x2 * (1 - cos_a));
    rotation_matrix[1][0] = (unit_vector[0] * unit_vector[1] * cos_a_n_1) - (unit_vector[2] * sin_a);
    rotation_matrix[2][0] = (unit_vector[0] * unit_vector[2] * cos_a_n_1) + (unit_vector[1] * sin_a);
    rotation_matrix[0][3] = 0;
    rotation_matrix[0][1] = (unit_vector[0] * unit_vector[1] * cos_a_n_1) + (unit_vector[2] * sin_a);
    rotation_matrix[1][1] = cos_a + (u_y2 * (cos_a_n_1));
    rotation_matrix[2][1] = (unit_vector[1] * unit_vector[2] * cos_a_n_1) - (unit_vector[0] * sin_a);
    rotation_matrix[1][3] = 0;
    rotation_matrix[0][2] = (unit_vector[2] * unit_vector[0] * cos_a_n_1) - (unit_vector[1] * sin_a);
    rotation_matrix[1][2] = (unit_vector[2] * unit_vector[1] * cos_a_n_1) + (unit_vector[0] * sin_a);
    rotation_matrix[2][2] = cos_a + (u_z2 * (cos_a_n_1));
    rotation_matrix[2][3] = 0;
    rotation_matrix[3][0] = 0;
    rotation_matrix[3][1] = 0;
    rotation_matrix[3][2] = 0;
    rotation_matrix[3][3] = 1;
}


void set_perspective(float angle_rad, float aspect_ratio, float near, float far, mat4x4 out)
{
    float tan_half_fov = tanf(angle_rad / 2);

    out[0][0] = 1 / (aspect_ratio * tan_half_fov);
    out[1][1] = 1 / tan_half_fov;
    out[2][2] = -(far + near) / (far - near);
    out[2][3] = -1;
    out[3][2] = -(2 * far * near) / (far - near);
}


void set_lookat(const vec3 pos, const vec3 target, const vec3 up, mat4x4 out)
{
    vec3 cam_right, cam_up, cam_direction;
    vec_sub(pos, target, cam_direction);
    vec_normalize(cam_direction, 3, cam_direction);
    vec_cross(up, cam_direction, cam_right);
    vec_normalize(cam_right, 3, cam_right);

    vec_cross(cam_direction, cam_right, cam_up);
    vec_normalize(cam_up, 3, cam_up);
    // if not try other way round like cmo
    mat4x4 m1, m2;
    m1[0][0] = cam_right[0];
    m1[1][0] = cam_right[1];
    m1[2][0] = cam_right[2];
    m1[3][0] = 0.0f;
    m1[0][1] = cam_up[0];
    m1[1][1] = cam_up[1];
    m1[2][1] = cam_up[2];
    m1[3][1] = 0.0f;
    m1[0][2] = cam_direction[0];
    m1[1][2] = cam_direction[1];
    m1[2][2] = cam_direction[2];
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
    m2[3][0] = -pos[0];
    m2[3][1] = -pos[1];
    m2[3][2] = -pos[2];

    mat4_mul(m1, m2, out);
}


void scale4x4(mat4x4 in, const vec3 scaler, mat4x4 out)
{
    mat4x4 scale_mat = {{scaler[0], 0, 0, 0}, {0, scaler[1], 0, 0},
                        {0, 0, scaler[2], 0}, {0, 0, 0, 1} };

    mat4_mul(in, scale_mat, out);
}
