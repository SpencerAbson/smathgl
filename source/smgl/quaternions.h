#ifndef SMATH_QUATERNIONS_H_
#define SMATH_QUATERNIONS_H_
#include "vectorf.h"
#include "matrices.h"
/* Quaternions are stored {w, i, j, k} as __m128 vectors (4 packed floats) */
typedef union vec128f quat;

quat quat_mul(quat* q0, quat* q1);
quat quat_rotate(quat* q0, fvec* axis, const float angle);
quat quat_inverse(quat* input);
quat quat_interpolate(quat* q0, quat* q1, float interp_param);
void quat_rotate_set4x4(quat* q0, fvec* axis, const float angle, mat4x4 out);

inline quat quat_init(float w, float i, float j, float k)
{
    quat output;
    output.sse_register = _mm_set_ps(k, j, i, w);
    return output;
}

#endif // QUATERNIONS_H_
