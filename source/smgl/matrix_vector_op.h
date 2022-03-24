#ifndef SMATH_MATRIX_VECTOR_OP_H_
#define SMATH_MATRIX_VECTOR_OP_H_
#include "matrices.h"
#include "vectorf.h"

extern void mat4_rotate(mat4_t *out, mat4_t const *input, fvec3 const* axis, float angle_rad);
extern void mat4_perspective(mat4_t *out, float angle_rad, float aspect_ratio, float near, float far);
extern void mat4_lookat(mat4_t *out, fvec3 const *position, fvec3 const *target, fvec3 const *up);
extern void mat4_set_euler_rotation(mat4_t *rotation_matrix, float angle, fvec3 *unit_vector);

static inline void vec4_outer_product(mat4_t *output, fvec3 const *input0,  fvec3 const *input1)
{
    SMGL_ASSERT(input0->size == input1->size && input0->size == 4);
    mat4xm128_outer_product(input0->data.sse_register, input1->data.sse_register,
                            output->sse_registers);
}

static inline void mat4_vec_product(fvec4 *output, mat4_t const *mat, fvec4 const *vec)
{
    SMGL_ASSERT(vec->size == 4);
    output->size = vec->size;
    output->data.sse_register = mat4xm128_vec4_product(mat->sse_registers, vec->data.sse_register);
}


#endif // SMATH_MATRIX_VECTOR_OP_H_
