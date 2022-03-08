#ifndef MATRIX_VECTOR_OP_H_
#define MATRIX_VECTOR_OP_H_
#include "matrices.h"
#include "vectorf.h"

extern mat4x4 mat4_rotate(mat4x4 const *input, fvec const* axis, float angle_rad);
extern mat4x4 mat4_perspective(float angle_rad, float aspect_ratio, float near, float far);
extern mat4x4 mat4_lookat(fvec *position, fvec *target, fvec *up);
extern mat4x4 vec4_outer_product(fvec *input0,  fvec *input1);
extern fvec   mat4_vec_product(mat4x4 const *mat, fvec *vec);
#endif // MATRIX_VECTOR_OP_H_
