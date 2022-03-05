#ifndef SMATH_QUATERNIONS_H_
#define SMATH_QUATERNIONS_H_
#include "vectorf.h"
#include "matrices.h"

/* Quaternions are stored {w, i, j, k} as __m128 vectors (4 packed floats) */
//typedef SMGL_ALIGN_16 float quat[4]; // Aligned on a 16 byte boundary
typedef union vec128f quat;

quat quat_mul(quat* q0, quat* q1);
quat quat_rotate(quat* q0, fvec* axis, const float angle);
quat quat_inverse(quat* input);
quat quat_interpolate(quat* q0, quat* q1, float interp_param);
void quat_rotate_set4x4(quat* q0, fvec* axis, const float angle, mat4x4 out);


#endif // QUATERNIONS_H_
