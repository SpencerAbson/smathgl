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
}fvec;

/* f32 composed vector functions */
extern float fvec_dot(fvec const *input0, fvec const *input1);  // compute dot product of 2 vec2/3/4s
extern void  fvec_scale(fvec *out, fvec const *input, float scalar);
extern void  fvec_normalize(fvec *out, fvec const *input); // normalize vec2/3/4s and store in out

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
    (vec_out).size = (v0).size;                 \
    (vec_out).data.sse_register = vectorf128_cross((v0).data.sse_register, (v1).data.sse_register)

#endif // SMATH_VECTORF_H_
