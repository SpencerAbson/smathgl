#ifndef SMATH_VECTORS_H_
#define SMATH_VECTORS_H_
#include <stdint.h>
#include <pmmintrin.h>
#include "platform.h"

/* f32 composed vectors, aligned on 16 byte boundary for 128 bit SSE */
SMGL_ALIGN_16 typedef float vec2[2];
SMGL_ALIGN_16 typedef float vec3[3];
SMGL_ALIGN_16 typedef float vec4[4];

/* i32 composed vectors, aligned on 16 byte boundary for 128 bit SSE */
SMGL_ALIGN_16 typedef int32_t ivec2[2];
SMGL_ALIGN_16 typedef int32_t ivec3[3];
SMGL_ALIGN_16 typedef int32_t ivec4[4];

/* f32 composed vector functions */
static inline __m128 vectorf128_reciporical(__m128 input)
{
    __m128 numerator =_mm_set_ps1(1.0f);  // was not aware of __m128 _mm_rcp_ps (__m128 a)...
    return _mm_div_ps(numerator, input);
}

static inline __m128 vectorf128_sum(__m128 input)
{
    input = _mm_hadd_ps(input, input);
    input = _mm_hadd_ps(input, input);
    return input;
}

void vec_add(float const *addr0, float const *addr1, float *out); // add vec2/3/4s and store result in out
void vec_sub(float const *addr0, float const *addr1, float *out); // subtract vec2/3/4s and stpre result in out
void vec_mul(float const *addr0, float const *addr1, float *out);
void vec_cross(const vec3 addr0, const vec3 addr1, vec3 addr_out); // calc cross product of a vec3
void vec_normalize(float const *addr_in, int size, float *addr_out); // normalize vec2/3/4s and store in out
void vec_scale(float const *addr_in, float scalar, float *out);
float vec_dot(float const *addr0, float const *addr1); // compute dot product of 2 vec2/3/4s
float vsum(float const *a, int size);

/* i32 composed vector functions  */
void ivec_add(int32_t const* addr0, int32_t const* addr1, int32_t* out); // add ivec2/3/4s and store result in out
void ivec_sub(int32_t const* addr0, int32_t const *addr1, int32_t *out); // subtract ivec2/3/4s and stpre result in out
void ivec_cross(const ivec3 addr0, const ivec3 addr1, ivec3 addr_out); // calc cross product of a vec3
void ivec_normalize(float const *addr_in, int size, int32_t *addr_out); // normalize ivec2/3/4s and store in out
void ivec_scale(int32_t const *addr_in, int32_t scalar, int32_t *out);
float ivec_dot(int32_t const* addr0, int32_t const *addr1); // compute dot product of 2 ivec2/3/4s

#endif // SMATH_VECTORS_H_
