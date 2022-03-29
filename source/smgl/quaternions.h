#ifndef SMATH_QUATERNIONS_H_
#define SMATH_QUATERNIONS_H_
#include "vectorf.h"
#include "matrices.h"
#include "simd/quaternionf128.h"

typedef enum QuaternionInterpolation_t
{
SMGL_QUAT_NLERP,
SMGL_QUAT_SLERP,
SMGL_QUAT_SQUAD
}QuatInterps_t;

/* Quaternions are stored {w, i, j, k} as __m128 vectors (4 packed floats) */
typedef union vec128f quat_t;

extern void quat_rotate(quat_t *out, quat_t const *q0, fvec3 const *axis, const float angle);
extern void quat_interpolate(quat_t *out, quat_t const*q0, quat_t const*q1, float interp_param, QuatInterps_t interpolation_method);
// rotate q0 around axis by angle and return mat4 representation of resultant quat
extern void quat_rotate_set_mat4_pure_simd(mat4_t *output, quat_t const *q0, fvec3 const* axis, const float angle); // for testing
extern void quat_to_mat4(mat4_t *output, quat_t const *input);

static inline void quat_integrate(quat_t *out, quat_t const *q0, fvec3 const *omega, float delta_t)
{
    SMGL_ASSERT(omega->size == 3);
    out->sse_register = quaternionf128_integrate(q0->sse_register, omega->data.sse_register, delta_t);
}

static inline void quat_rotate_set_mat4(mat4_t *output, quat_t const *q0, fvec_t const* axis, const float angle)
{
    quat_t q_of_rotation;
    quat_rotate(&q_of_rotation, q0, axis, angle);
    quat_to_mat4(output, &q_of_rotation);
}

static inline void quat_set_rotation_mat4(mat4_t *output, fvec3 const *axis, const float angle)
{
    quat_t q_of_rotation;
    __m128 rotator = _mm_set_ps(axis->data.values[2], axis->data.values[1], axis->data.values[0], angle);
    q_of_rotation.sse_register = quaternionf128_set_known_rotation(rotator, angle);
    quat_to_mat4(output, &q_of_rotation);
}

/* Primitve function-like-macros that aren't worth any overhead: */
void quat_normalize(quat_t q_out, quat_t const q_in);
void quat_inverse(quat_t q_out, quat_t const q_in);
void quat_mul(quat_t q_out, quat_t const q0, quat_t const q1);
void quat_init(quat_t q_out, float w, float x, float y, float z);
void quat_blend(quat_t q_out, quat_t const q0, quat_t const q1, float t);
void quat_rotateq(quat_t q_out, quat_t q0, quat_t q1);

#define quat_normalize(qout, qin)                                   \
    (qout).sse_register = vectorf128_normalize((qin).sse_register)

#define quat_inverse(qout, qin)                 \
    (qout).sse_register = quaternionf128_inverse((qin).sse_register)

#define quat_mul(qout, q0, q1)                 \
    (qout).sse_register = quaternionf128_mul((q0).sse_register, (q1).sse_register)

#define quat_init(qout, w, x, y, z)              \
    (qout).sse_register = _mm_set_ps((z), (y), (x), (w))

#define quat_rotateq(qout, q0, q1)              \
    (qout).sse_register = quaternionf128_pure_rotate((q0).sse_register, (q1).sse_register)

#define quat_blend(qout, q0, q1, t)             \
    (qout).sse_register = quaternionf128_Nlerp((q0).sse_register, (q1).sse_register, (t))
#endif // QUATERNIONS_H_
