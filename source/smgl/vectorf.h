#ifndef SMATH_VECTORS_H_
#define SMATH_VECTORS_H_
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
fvec  fvec_add(fvec *input0, fvec *inpu1); // add fvec2/3/4s and store result in dst
fvec  fvec_cross(fvec *input0, fvec *input1); // calc cross product of a fvec3t
float fvec_dot(fvec *input0, fvec *input1);  // compute dot product of 2 vec2/3/4s
fvec  fvec_sub(fvec *input0, fvec *input1);  // subtract vec2/3/4s and stpre result in dst
fvec  fvec_mul(fvec *input0, fvec *input1);
fvec  fvec_scale(fvec *input, float scalar);
fvec  fvec_normalize(fvec *input); // normalize vec2/3/4s and store in out


inline fvec fvec4_init(float x, float y, float z, float w)
{
    fvec output;
    output.size = 4;
    output.data.sse_register = _mm_set_ps(w, z, y, x);
    return output;
}

inline fvec fvec3_init(float x, float y, float z)
{
    fvec output;
    output.size = 3;
    output.data.sse_register = _mm_set_ps(0.0f, z, y, x);
    return output;
}


inline fvec fvec2_init(float x, float y)
{
    fvec output;
    output.size = 2;
    output.data.sse_register = _mm_set_ps(0.0f, 0.0f, y, x);
    return output;
}

#endif // SMATH_VECTORS_H_
