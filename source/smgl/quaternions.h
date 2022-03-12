#ifndef SMATH_QUATERNIONS_H_
#define SMATH_QUATERNIONS_H_
#include "vectorf.h"
#include "matrices.h"
#include "simd/quaternionf128.h"

/* Quaternions are stored {w, i, j, k} as __m128 vectors (4 packed floats) */
typedef union vec128f quat_t;

extern void quat_rotate(quat_t *out, quat_t const *q0, fvec_t const *axis, const float angle);
extern void quat_inverse(quat_t *out, quat_t const *input);
extern void quat_interpolate(quat_t *out, quat_t const*q0, quat_t const*q1, float interp_param); // slerp
// rotate q0 around axis by angle and return mat4 representation of resultant quat
extern void quat_rotate_mat4(mat4_t *out, quat_t const *q0, fvec_t const *axis, float const angle);

/* Primitve functions that aren't worth the overhead: */
#define quat_mm_normalize(qout, qin)                                   \
    (qout).sse_register = vectorf128_normalize((qin).sse_register)

#define quat_mm_inverse(qout, qin)                 \
    (qout).sse_register = quaternionf128_inverse((qin).sse_register)

#define quat_mm_mul(qout, q0, q1)                 \
    (qout).sse_register = quaternionf128_mul((q0).sse_register, (q1).sse_register)

#define quat_mm_init(qout, w, x, y, z)              \
    (qout).sse_register = _mm_set_ps((z), (y), (x), (w))

#endif // QUATERNIONS_H_
