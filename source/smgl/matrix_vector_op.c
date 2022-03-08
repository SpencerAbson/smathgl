#include <assert.h>
#include "matrix_vector_op.h"
#include "simd/matrix4xm128f.h"
#include "quaternions.h"


mat4x4 mat4_perspective(float angle_rad, float aspect_ratio, float near, float far)
{
    mat4x4 out = mat4_init();
    float tan_half_fov = tanf(angle_rad / 2.0f);
    out.sse_registers[0] = _mm_set_ps(0.0f, 0.0f, 0.0f, 1.0f / (aspect_ratio * tan_half_fov));
    out.sse_registers[1] = _mm_set_ps(0.0f, 0.0f, 1.0f / tan_half_fov, 0.0f);
    out.sse_registers[2] = _mm_set_ps(-1.0f, -(far + near) / (far - near), 0.0f, 0.0f);
    out.sse_registers[3] = _mm_set_ps(0.0f, -(2 * far * near) / (far - near), 0.0f, 0.0f);

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


mat4x4 mat4_rotate(mat4x4 const *input, fvec const* axis, float angle_rad)
{
    // generate quaternion rep of rotation by rotaing {1, 0, 0, 0} by input
    quat identity = quat_init(1.0f, 0.0f, 0.0f, 0.0f);
    fvec normed_axis = fvec_normalize(axis);
    mat4x4 rotator = quat_rotate_mat4(&identity, &normed_axis, angle_rad);

    return mat4_mul(input, &rotator);
}

mat4x4 vec4_outer_product(fvec *input0,  fvec *input1)
{
    assert(input0->size == input1->size && input0->size == 4);

    mat4x4 output;
    mat4xm128_outer_product(input0->data.sse_register, input1->data.sse_register,
                            output.sse_registers);
    return output;
}


fvec mat4_vec_product(mat4x4 const *mat, fvec *vec)
{
    assert(vec->size == 4);

    fvec output;
    output.size = vec->size;
    output.data.sse_register = mat4xm128_vec4_product(mat->sse_registers, vec->data.sse_register);
    return output;
}


/*
**    out.values[0][0] = 1 / (aspect_ratio * tan_half_fov);
    out.values[1][1] = 1 / tan_half_fov;
    out.values[2][2] = -(far + near) / (far - near);
    out.values[2][3] = -1;
    out.values[3][2] = -(2 * far * near) / (far - near);
**
*/
