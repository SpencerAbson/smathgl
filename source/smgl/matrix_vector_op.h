#ifndef SMATH_MATRIX_VECTOR_OP_H_
#define SMATH_MATRIX_VECTOR_OP_H_
#include "matrices.h"
#include "vectorf.h"

extern void mat4_rotate(mat4_t *out, mat4_t const *input, fvec_t const* axis, float angle_rad);
extern void mat4_perspective(mat4_t *out, float angle_rad, float aspect_ratio, float near, float far);
extern void mat4_lookat(mat4_t *out, fvec_t const *position, fvec_t const *target, fvec_t const *up);
extern void vec4_outer_product(mat4_t *out, fvec_t const *input0,  fvec_t const *input1);
extern void mat4_vec_product(fvec_t *out, mat4_t const *mat, fvec_t const *vec);
extern void mat4_set_euler_rotation(mat4_t *rotation_matrix, float angle, fvec_t *unit_vector);
#endif // SMATH_MATRIX_VECTOR_OP_H_
