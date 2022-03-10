#ifndef SMATH_MATRIX_VECTOR_OP_H_
#define SMATH_MATRIX_VECTOR_OP_H_
#include "matrices.h"
#include "vectorf.h"

extern void mat4_rotate(mat4x4 *out, mat4x4 const *input, fvec const* axis, float angle_rad);
extern void mat4_perspective(mat4x4 *out, float angle_rad, float aspect_ratio, float near, float far);
extern void mat4_lookat(mat4x4 *out, fvec const *position, fvec const *target, fvec const *up);
extern void vec4_outer_product(mat4x4 *out, fvec const *input0,  fvec const *input1);
extern void mat4_vec_product(fvec *out, mat4x4 const *mat, fvec const *vec);
#endif // SMATH_MATRIX_VECTOR_OP_H_
