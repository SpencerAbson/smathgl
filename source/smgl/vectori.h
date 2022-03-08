#ifndef SMATH_VECTORI_H_
#define SMATH_VECTORI_H_
#include <stdint.h>
#include "..\..\include/platform.h"
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
extern ivec ivec_add(ivec const *input0, ivec const *input1); // add ivec2/3/4s and store result in dst
extern ivec ivec_sub(ivec const *input0, ivec const *input1); // subtract ivec2/3/4s and stpre result in dst
extern ivec ivec_cross(ivec const *input0, ivec const *input1); // calc cross product of a vec3
extern ivec ivec_scale(ivec const *addr_in, int32_t scalar);
extern ivec ivec_dot(ivec const *input0, ivec const *input1); // compute dot product of 2 ivec2/3/4s

/* Vector initers, will zero out unused componets. */
extern ivec ivec4_init(int32_t x, int32_t y, int32_t z, int32_t w);
extern ivec ivec3_init(int32_t x, int32_t y, int32_t z);
extern ivec ivec2_init(float x, float y);

#endif // SMATH_SMATH_VECTORI_H_
