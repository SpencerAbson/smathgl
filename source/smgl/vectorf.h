#ifndef SMATH_VECTORF_H_
#define SMATH_VECTORF_H_
#include <stdint.h>
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
extern fvec  fvec_add(fvec const *input0, fvec const *inpu1); // add fvec2/3/4s and store result in dst
extern fvec  fvec_cross(fvec const *input0, fvec const *input1); // calc cross product of a fvec3t
extern float fvec_dot(fvec const *input0, fvec const *input1);  // compute dot product of 2 vec2/3/4s
extern fvec  fvec_sub(fvec const *input0, fvec const *input1);  // subtract vec2/3/4s and stpre result in dst
extern fvec  fvec_mul(fvec const *input0, fvec const *input1);
extern fvec  fvec_scale(fvec const *input, float scalar);
extern fvec  fvec_normalize(fvec const*input); // normalize vec2/3/4s and store in out

/* vector initers, will zero out unused components. */
extern fvec fvec4_init(float x, float y, float z, float w);
extern fvec fvec3_init(float x, float y, float z);
extern fvec fvec2_init(float x, float y);

#endif // SMATH_VECTORF_H_
