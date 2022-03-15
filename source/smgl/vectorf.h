#ifndef SMATH_VECTORF_H_
#define SMATH_VECTORF_H_
#include <stdint.h>
#include <assert.h>
#include "..\..\include/platform.h"

/* f32 composed vectors, aligned on 16 byte boundary for 128 bit SSE */
typedef SMGL_ALIGN_16 float fvec4[4];
typedef union vec128f
{
    SMGL_ALIGN_16 float values[4];
    __m128 sse_register;
}vec128f;

typedef struct fvec {
    union vec128f  data;
    uint32_t size;
}fvec_t;

/* f32 composed vector functions */
extern float fvec_dot(fvec_t const *input0, fvec_t const *input1);  // compute dot product of 2 vec2/3/4se
extern float fvec_min(fvec_t const *input);
extern void  fvec_scale(fvec_t *out, fvec_t const *input, float scalar);
extern void  fvec_display(fvec_t const *input);

/* vector initers and primitive functions  */
#define fvec4_mm_init(vec_in, x, y, z, w)\
    (vec_in).size = 4;\
    (vec_in).data.sse_register = _mm_set_ps((w), (z), (y), (x));

#define fvec3_mm_init(vec_in, x, y, z)\
    (vec_in).size = 3;\
    (vec_in).data.sse_register = _mm_set_ps((0.0f), (z), (y), (x));

#define fvec2_mm_init(vec_in, x, y)\
    (vec_in).size = 2;\
    (vec_in).data.sse_register = _mm_set_ps((0.0f), (0.0f), (y), (x));

/* Primitve functions that aren't worth the overhead: (trust me it makes a difference) */
#define fvec_mm_add(vec_out, v0, v1)            \
    assert((v0).size == (v1).size);             \
    (vec_out).size = (v0).size;                 \
    (vec_out).data.sse_register = _mm_add_ps((v0).data.sse_register, (v1).data.sse_register)

#define fvec_mm_sub(vec_out, v0, v1)            \
    assert((v0).size == (v1).size);             \
    (vec_out).size = (v0).size;                 \
    (vec_out).data.sse_register = _mm_sub_ps((v0).data.sse_register, (v1).data.sse_register)

#define fvec_mm_mul(vec_out, v0, v1)            \
    assert((v0).size == (v1).size);             \
    (vec_out).size = (v0).size;                 \
    (vec_out).data.sse_register = _mm_mul_ps((v0).data.sse_register, (v1).data.sse_register)

#define fvec_mm_cross(vec_out, v0, v1)            \
    assert((v0).size == (v1).size && (v0).size == 3);             \
    (vec_out).size = 3;                 \
    (vec_out).data.sse_register = vectorf128_cross((v0).data.sse_register, (v1).data.sse_register)

#define fvec_mm_normalize(vec_out, v0)            \
    assert((v0).size < 5 && (v0).size > 1);   \
    (vec_out).size = (v0).size;                   \
    (vec_out).data.sse_register = vectorf128_normalize((v0).data.sse_register)

#define fvec_mm_reverse(vec_out, v0)               \
    (vec_out).size = (v0).size;                 \
    (vec_out).data.sse_register = _mm_shuffle_ps((v0).data.sse_registerm, (v0).data.sse_register, _MM_SHUFFLE(0, 1, 2, 3));

#endif // SMATH_VECTORF_H_
