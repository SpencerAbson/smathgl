#ifndef SMATH_VECTORI_H_
#define SMATH_VECTORI_H_
#include <stdint.h>
#include "..\..\include/platform.h"
#include "simd/vectori128.h"
/* i32 composed vectors, aligned on 16 byte boundary for 128 bit SSE */
SMGL_ALIGN_16 typedef int32_t ivec4[4];

typedef union vec128i
{
    SMGL_ALIGN_16 int32_t values[4];
    __m128i sse_register;
}vec128i;

typedef struct ivec {
    union vec128i  data;
    uint32_t size;
}ivec;

/* i32 composed vector functions  */
extern void ivec_scale(ivec *out, ivec const *addr_in, int32_t scalar);
extern int32_t ivec_dot(ivec const *input0, ivec const *input1); // compute dot product of 2 ivec2/3/4s
extern int32_t ivec_min(ivec const *input);
/* Vector initers and primitive function macro wrappers */
#define ivec4_mm_init(vec_out, x, y, z, w)      \
    (vec_out)->size = 4;                        \
    (vec_out)->data.sse_register = _mm_set_epi32((w), (z), (y), (x));

#define ivec3_mm_init(vec_out, x, y, z)         \
    (vec_out)->size = 3;                        \
    (vec_out)->data.sse_register = _mm_set_epi32(0, (z), (y), (x));

#define ivec2_mm_init(vec_out, x, y)            \
    (vec_out)->size = 2;                        \
    (vec_out)->data.sse_register = _mm_set_epi32(0, 0, (y), (x))

#define ivec_mm_add(vec_out, v0, v1)            \
    assert((v0).size == (v1).size);             \
    (vec_out).size = (v0).size;                 \
    (vec_out).data.sse_register = _mm_add_epi((v0).data.sse_register, (v1).data.sse_register)

#define ivec_mm_sub(vec_out, v0, v1)            \
    assert((v0).size == (v1).size);             \
    (vec_out).size = (v0).size;                 \
    (vec_out).data.sse_register = _mm_sub_epi((v0).data.sse_register, (v1).data.sse_register)

#define ivec_mm_mul(vec_out, v0, v1)            \
    assert((v0).size == (v1).size);             \
    (vec_out).size = (v0).size;                 \
    (vec_out).data.sse_register = v_mul_i32((v0).data.sse_register, (v1).data.sse_register)

#define ivec_mm_cross(vec_out, v0, v1)          \
    assert((v0).size == (v1).size && (v0).size == 3);             \
    (vec_out).size = (v0).size;                 \
    (vec_out).data.sse_register = vectori128_cross((v0).data.sse_register, (v1).data.sse_register)

#define ivec_mm_reverse(vec_out, v0)            \
    (vec_out).size = (v0).size;                 \
    (vec_out).data.sse_register = _mm_shuffle_epi32((v0).data.sse_registerm, _MM_SHUFFLE(0, 1, 2, 3));


#endif // SMATH_SMATH_VECTORI_H_
