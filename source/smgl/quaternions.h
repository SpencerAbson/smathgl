#ifndef SMATH_QUATERNIONS_H_
#define SMATH_QUATERNIONS_H_
#include "vectorf.h"
#include "matrices.h"
/* Quaternions are stored {w, i, j, k} as __m128 vectors (4 packed floats) */
typedef union vec128f quat;

extern quat quat_mul(quat const *q0, quat const *q1);
extern quat quat_rotate(quat const *q0, fvec const *axis, const float angle);
extern quat quat_inverse(quat const *input);
extern quat quat_interpolate(quat const*q0, quat const*q1, float interp_param); // slerp
extern quat quat_normalize(quat const *input);
extern quat quat_init(float w, float i, float j, float k);

// rotate q0 around axis by angle and return mat4 representation of resultant quat
extern mat4x4 quat_rotate_mat4(quat const *q0, fvec const *axis, float const angle);
#endif // QUATERNIONS_H_
