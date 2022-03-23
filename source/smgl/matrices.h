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
}mat4_t;

extern void mat4_init_diagonal(mat4_t *out, const float translator);
extern void mat4_init_translation(mat4_t *out, float x, float y, float z);
extern void mat4_display(const mat4_t *mat);

/* Primitve function-like-macros that aren't worth any overhead: */
void mat4_mul(mat4_t out, mat4_t const in0, mat4_t const in1);
void mat4_add(mat4_t out, mat4_t const in0, mat4_t const in1);
void mat4_sub(mat4_t out, mat4_t const in0, mat4_t const in1);
void mat4_transpose(mat4_t out, mat4_t const in);

#define mat4_mul(out, in0, in1)                 \
    mat4xm128_mul((in0).sse_registers, (in1).sse_registers, (out).sse_registers) \

#define mat4_add(out, in0, in1)                 \
    mat4xm128_add((in0).sse_registers, (in1).sse_registers, (out).sse_registers) \

#define mat4_sub(out, in0, in1)                 \
    mat4xm128_sub((in0).sse_registers, (in1).sse_registers, (out).sse_registers) \

#define mat4_transpose(out, in0)                \
    mat4xm128_transpose((in0).sse_registers, (out).sse_registers) \

#endif // SMATH_MATRICES_H_
