#ifndef SMATH_QUATERNIONS_H_
#define SMATH_QUATERNIONS_H_
#include "vectorf.h"
#include "matrices.h"

/* Quaternions are stored {w, i, j, k} as __m128 vectors (4 packed floats) */
typedef vec4 quat; // Aligned on a 16 byte boundary

void quat_mul(const quat q0, const quat q1, quat out);
void quat_rotate(const quat q0, const vec3 axis, const float ang, quat out);
void quat_rotation_set4x4(const quat q0, const vec3 axis, const float angle, mat4x4 out);
void quat_inverse(const quat input, quat out);
void quat_interpolate(const quat q0, const quat q1, float interp_param, quat out);

#endif // QUATERNIONS_H_