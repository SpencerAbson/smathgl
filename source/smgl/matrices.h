#ifndef SMATH_MATRICES_H_
#define SMATH_MATRICES_H_
#include <string.h>
#include "..\..\include/platform.h"
#include "simd/matrix4xm128f.h"

typedef __m128 mat4xm128[4];

typedef union mat4x4
{
    SMGL_ALIGN_16 float values[4][4];
    __m128 sse_registers[4];
}mat4x4;

extern void mat4_init_diagonal(mat4x4 *out, const float translator);
extern void mat4_init_translation(mat4x4 *out, float x, float y, float z);
extern void mat4_display(const mat4x4 *mat);

/* Primitve functions that aren't worth the overhead: */
#define mat4_mm_mul(out, in0, in1)                 \
    mat4xm128_mul((in0).sse_registers, (in1).sse_registers, (out).sse_registers)

#define mat4_mm_add(out, in0, in1)                 \
    mat4xm128_add((in0).sse_registers, (in1).sse_registers, (out).sse_registers) \

#define mat4_mm_sub(out, in0, in1)                 \
    mat4xm128_sub((in0).sse_registers, (in1).sse_registers, (out).sse_registers)

#define mat4_mm_transpose(out, in0)                \
    mat4xm128_transpose((in0).sse_registers, (out).sse_registers)

static inline void mat4_clear(mat4x4 *input){
    memset(input->values, 0, sizeof(float) * 16);
}

#endif // SMATH_MATRICES_H_
