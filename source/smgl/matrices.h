#ifndef SMGL_MATRICES_H_
#define SMGL_MATRICES_H_
#include <string.h>
#include "..\..\include/platform.h"
SMGL_ALIGN_16 typedef float mat3x3[3][3];
SMGL_ALIGN_16 typedef float mat4x4[4][4];
typedef __m128 mat4xm128[4];

void mat4_mul(const mat4x4 mat_a, const mat4x4 mat_b, mat4x4 out);
void mat4_transpose(const mat4x4 input, mat4x4 out);
void mat4_sub(const mat4x4 mat_a, const mat4x4 mat_b, mat4x4 out);
void mat4_add(const mat4x4 mat_a, const mat4x4 mat_b, mat4x4 out);
void mat4_display(const mat4x4 input);

inline void mat4_set_translation(mat4x4 out, const float translator)
{
    out[0][0] = translator;
    out[1][1] = translator;
    out[2][2] = translator;
    out[3][3] = translator;
}

static inline void mat4_load_to_m128(const mat4x4 input, mat4xm128 out)
{
    out[0] = _mm_load_ps(input[0]);
    out[1] = _mm_load_ps(input[1]);
    out[2] = _mm_load_ps(input[2]);
    out[3] = _mm_load_ps(input[3]);
}

static inline void m128_store_mat4(const mat4xm128 input, mat4x4 out)
{
    _mm_store_ps(out[0], input[0]);
    _mm_store_ps(out[1], input[1]);
    _mm_store_ps(out[2], input[2]);
    _mm_store_ps(out[3], input[3]);
}

inline void mat4_clear(mat4x4 input){
    memset(input, 0, sizeof(float) * 16);
}

#endif // SMGL_MATRICES_H_
