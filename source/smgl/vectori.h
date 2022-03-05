#ifndef VECTORI_H_
#define VECTORI_H_
#include <stdint.h>
#include "..\..\include/platform.h"
/* i32 composed vectors, aligned on 16 byte boundary for 128 bit SSE */
SMGL_ALIGN_16 typedef int32_t ivec4[4];

union vec128i
{
    SMGL_ALIGN_16 int32_t values[4];
    __m128i sse_register;
}vec128f;

typedef struct ivec {
    union vec128i  data;
    uint32_t size;
}ivec;

/* i32 composed vector functions  */
ivec ivec_add(ivec *input0, ivec *input1); // add ivec2/3/4s and store result in dst
ivec ivec_sub(ivec *input0, ivec *input1); // subtract ivec2/3/4s and stpre result in dst
ivec ivec_cross(ivec *input0, ivec *input1); // calc cross product of a vec3
//ivec ivec_normalize(float const *addr_in, int size, int32_t *addr_out); // normalize ivec2/3/4s and store in out
ivec ivec_scale(ivec *addr_in, int32_t scalar);
ivec ivec_dot(ivec *input0, ivec *input1); // compute dot product of 2 ivec2/3/4s

#endif // VECTORI_H_
