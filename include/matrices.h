#ifndef SMATH_MATRICES_H_
#define SMATH_MATRICES_H_
#include <emmintrin.h>
#include <intrin.h>
typedef float mat3x3[3][3];
typedef float mat4x4[4][4];

void set_translation4x4(mat4x4 out, const float translator);
void mult4x4(mat4x4 mat_a, mat4x4 mat_b, mat4x4 out);
void display4x4(mat4x4 mat);
void clear4x4(mat4x4 mat);
#endif // MATRICES_H_
