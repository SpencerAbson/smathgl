#ifndef SMATH_VECTORI_H_
#define SMATH_VECTORI_H_
#include <stdint.h>
#include "..\..\include/platform.h"
#include "simd/vectori128.h"

/* i32 composed vectors, aligned on 16 byte boundary for 128 bit SSE */
typedef union vec128i
{
    SMGL_ALIGN_16 int32_t values[4];
    __m128i sse_register;
}vec128i;

typedef struct ivec {
    union vec128i  data;
    uint32_t size;
}ivec_t;

typedef ivec_t ivec2, ivec3, ivec4; // for better code clarity, but it should be known that these are identical in SSE.

/* i32 composed vector functions  */
extern void ivec_scale(ivec_t *output, ivec_t const *input, int32_t scalar);
extern int32_t ivec_min(ivec_t const *input);
extern void ivec_display(ivec_t const *input);

void ivec4_init(ivec4 vec, int32_t e0, int32_t e1, int32_t e2, int32_t e3);
void ivec3_init(ivec3 vec, int32_t e0, int32_t e1, int32_t e2);
void ivec2_init(ivec2 vec, int32_t e0, int32_t e1);

void ivec_add(ivec_t vout, ivec_t v0, ivec_t v1);
void ivec_sub(ivec_t vout, ivec_t v0, ivec_t v1);
void ivec_mul(ivec_t vout, ivec_t v0, ivec_t v1);
void ivec_cross(ivec_t vout, ivec_t v0, ivec_t v1);
void ivec_reverse(ivec_t vout, ivec_t v0);
int32_t ivec_dot(ivec_t v0, ivec_t v1);


/* Vector initers and function macro implementations */
#define ivec4_init(vec_out, x, y, z, w)      \
    (vec_out).size = 4;                      \
    (vec_out).data.sse_register = _mm_set_epi32((w), (z), (y), (x));

#define ivec3_init(vec_out, x, y, z)         \
    (vec_out).size = 3;                      \
    (vec_out).data.sse_register = _mm_set_epi32(0, (z), (y), (x));

#define ivec2_init(vec_out, x, y)            \
    (vec_out).size = 2;                      \
    (vec_out).data.sse_register = _mm_set_epi32(0, 0, (y), (x))

#define ivec_add(vec_out, v0, v1)            \
    SMGL_ASSERT((v0).size == (v1).size);          \
    (vec_out).size = (v0).size;              \
    (vec_out).data.sse_register = _mm_add_epi((v0).data.sse_register, (v1).data.sse_register)

#define ivec_sub(vec_out, v0, v1)            \
    SMGL_ASSERT((v0).size == (v1).size);          \
    (vec_out).size = (v0).size;              \
    (vec_out).data.sse_register = _mm_sub_epi((v0).data.sse_register, (v1).data.sse_register)

#define ivec_mul(vec_out, v0, v1)            \
    SMGL_ASSERT((v0).size == (v1).size);          \
    (vec_out).size = (v0).size;              \
    (vec_out).data.sse_register = v_mul_i32((v0).data.sse_register, (v1).data.sse_register)

#define ivec_dot(v0, v1)                     \
    vectori128_dot((v0).data.sse_register, (v1).data.sse_register)

#define ivec_cross(vec_out, v0, v1)          \
    SMGL_ASSERT((v0).size == (v1).size && (v0).size == 3);          \
    (vec_out).size = (v0).size;              \
    (vec_out).data.sse_register = vectori128_cross((v0).data.sse_register, (v1).data.sse_register)

#define ivec_reverse(vec_out, v0)            \
    (vec_out).size = (v0).size;              \
    (vec_out).data.sse_register = _mm_shuffle_epi32((v0).data.sse_registerm, _MM_SHUFFLE(0, 1, 2, 3));

#define ivec_mm_scale(vec_out, v0, scalar)   \
    (vec_out).size = (v0).size;              \
    (vec_out).data.sse_register = vectori128_scale((v0).data.sse_registerm, (scalar));

#endif // SMATH_SMATH_VECTORI_H_
