#ifndef SMATH_QUATERNIONS_H_
#define SMATH_QUATERNIONS_H_
#include "vectorf.h"
#include "matrices.h"
/* Quaternions are stored {w, i, j, k} as __m128 vectors (4 packed floats) */
typedef union vec128f quat;

extern quat quat_mul(quat* q0, quat* q1);
extern quat quat_rotate(quat* q0, fvec* axis, const float angle);
extern quat quat_inverse(quat* input);
extern quat quat_interpolate(quat* q0, quat* q1, float interp_param);
extern mat4x4 quat_rotate_mat4(quat* q0, fvec* axis, const float angle);
extern quat quat_normalize(quat *input);

inline quat quat_init(float w, float i, float j, float k)
{
    quat output;
    output.sse_register = _mm_set_ps(k, j, i, w);
    return output;
}

#endif // QUATERNIONS_H_
