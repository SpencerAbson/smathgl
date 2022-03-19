#ifndef SMATH_QUATERNIONS_H_
#define SMATH_QUATERNIONS_H_
#include "vectorf.h"
#include "matrices.h"
#include "simd/quaternionf128.h"

typedef enum QuaternionInterpolation_t
{
SM_QUAT_NLERP,
SM_QUAT_SLERP,
SM_QUAT_SQUAD
}QuatInterps_t;

/* Quaternions are stored {w, i, j, k} as __m128 vectors (4 packed floats) */
typedef union vec128f quat_t;

extern void quat_rotate(quat_t *out, quat_t const *q0, fvec_t const *axis, const float angle);
extern void quat_inverse(quat_t *out, quat_t const *input);
extern void quat_interpolate(quat_t *out, quat_t const*q0, quat_t const*q1, float interp_param, QuatInterps_t interpolation_method);
// rotate q0 around axis by angle and return mat4 representation of resultant quat
extern void quat_rotate_set_mat4(mat4_t *out, quat_t const *q0, fvec_t const *axis, float  angle);
static inline void quat_integrate(quat_t *out, quat_t const *q0, fvec_t const *omega, float delta_t)
{
    assert(omega->size == 3);
    out->sse_register = quaternionf128_integrate(q0->sse_register, omega->data.sse_register, delta_t);
}

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
