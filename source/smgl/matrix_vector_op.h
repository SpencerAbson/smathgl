#ifndef MATRIX_VECTOR_OP_H_
#define MATRIX_VECTOR_OP_H_
#include "matrices.h"
#include "vectorf.h"

extern mat4x4 vec4_outer_product(fvec *input0,  fvec *input1);
extern fvec mat4_vec_product(mat4x4 const *mat, fvec *vec);
#endif // MATRIX_VECTOR_OP_H_
