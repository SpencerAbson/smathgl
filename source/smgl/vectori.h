#ifndef VECTORI_H_
#define VECTORI_H_
#include <stdint.h>
#include "..\include/platform.h"
/* i32 composed vectors, aligned on 16 byte boundary for 128 bit SSE */
SMGL_ALIGN_16 typedef int32_t ivec2[2];
SMGL_ALIGN_16 typedef int32_t ivec3[3];
SMGL_ALIGN_16 typedef int32_t ivec4[4];


/* i32 composed vector functions  */
void ivec_add(int32_t const* addr0, int32_t const* addr1, int32_t* out); // add ivec2/3/4s and store result in out
void ivec_sub(int32_t const* addr0, int32_t const *addr1, int32_t *out); // subtract ivec2/3/4s and stpre result in out
void ivec_cross(const ivec3 addr0, const ivec3 addr1, ivec3 addr_out); // calc cross product of a vec3
void ivec_normalize(float const *addr_in, int size, int32_t *addr_out); // normalize ivec2/3/4s and store in out
void ivec_scale(int32_t const *addr_in, int32_t scalar, int32_t *out);
float ivec_dot(int32_t const* addr0, int32_t const *addr1); // compute dot product of 2 ivec2/3/4s

#endif // VECTORI_H_
