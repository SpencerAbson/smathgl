#ifndef SMATH_VECTORS_H_
#define SMATH_VECTORS_H_
#include <stdint.h>
#include "..\..\include/platform.h"

/* f32 composed vectors, aligned on 16 byte boundary for 128 bit SSE */
//SMGL_ALIGN_16 typedef float vec2[2];
//SMGL_ALIGN_16 typedef float vec3[4];
SMGL_ALIGN_16 typedef float vec4[4];

/* f32 composed vector functions */
void vec_add(float const *addr0, float const *addr1, float *out); // add vec2/3/4s and store result in out
void vec_sub(float const *addr0, float const *addr1, float *out); // subtract vec2/3/4s and stpre result in out
void vec_mul(float const *addr0, float const *addr1, float *out);
void vec_cross(float const *addr0, float const *addr1, float *addr_out); // calc cross product of a vec3
void vec_normalize(float const *addr_in, int size, float *addr_out); // normalize vec2/3/4s and store in out
void vec_scale(float const *addr_in, float scalar, float *out);
float vec_dot(float const *addr0, float const *addr1); // compute dot product of 2 vec2/3/4s
float vsum(float const *a, int size);


#endif // SMATH_VECTORS_H_
