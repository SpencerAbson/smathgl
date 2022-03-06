#ifndef SMGL_MATRICES_H_
#define SMGL_MATRICES_H_
#include <string.h>
#include "..\..\include/platform.h"

typedef __m128 mat4xm128[4];

typedef union mat4x4
{
    SMGL_ALIGN_16 float values[4][4];
    __m128 sse_registers[4];
}mat4x4;

extern mat4x4 mat4_sub(mat4x4 const *input0, mat4x4 const *input1);
extern mat4x4 mat4_add(mat4x4 const *input0, mat4x4 const *input1);
extern mat4x4 mat4_transpose(mat4x4 const *input);
extern mat4x4 mat4_mul(mat4x4 const *input0, mat4x4 const *input1);
extern void mat4_display(const mat4x4 *mat);

inline mat4x4 mat4_init()
{
    mat4x4 output;
    output.sse_registers[0] = _mm_set_ps1(0.0f);
    output.sse_registers[1] = _mm_set_ps1(0.0f);
    output.sse_registers[2] = _mm_set_ps1(0.0f);
    output.sse_registers[3] = _mm_set_ps1(0.0f);

    return output;
}

inline mat4x4 mat4_init_translation(const float translator)
{
    mat4x4 output;
    output.sse_registers[0] = _mm_set_ps(0.0f, 0.0f, 0.0f, translator);
    output.sse_registers[1] = _mm_set_ps(0.0f, 0.0f, translator, 0.0f);
    output.sse_registers[2] = _mm_set_ps(0.0f, translator, 0.0f, 0.0f);
    output.sse_registers[3] = _mm_set_ps(translator, 0.0f, 0.0f, 0.0f);

    return output;
}

inline void mat4_clear(mat4x4 input){
    memset(input.values, 0, sizeof(float) * 16);
}

#endif // SMGL_MATRICES_H_
