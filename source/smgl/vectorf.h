#ifndef SMATH_VECTORF_H_
#define SMATH_VECTORF_H_
#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include "..\..\include/platform.h"

/* f32 composed vectors, aligned on 16 byte boundary for 128 bit SSE */
typedef union vec128f
{
    SMGL_ALIGN_16 float values[4];
    __m128 sse_register;
}vec128f;

typedef struct fvec {
    uint32_t size;
    union vec128f  data;
}fvec_t;

typedef fvec_t fvec2, fvec3, fvec4; // for better code clarity, but it should be known that these are identical in SSE.

/* f32 composed vector functions */
void fvec4_init(fvec4 vec, float e0, float e1, float e2, float e3);
void fvec3_init(fvec3 vec, float e0, float e1, float e2);
void fvec2_init(fvec2 vec, float e0, float e1);

void fvec_add(fvec_t vout, float v0, float v1);
void fvec_sub(fvec_t vout, float v0, float v1);
void fvec_mul(fvec_t vout, float v0, float v1);
void fvec_cross(fvec_t vout, fvec_t v0, float v1);
void fvec_reverse(fvec_t vout, fvec_t v0);
int32_t fvec_dot(fvec_t v0, fvec_t v1);


static inline void fvec_scale(fvec_t *output, fvec_t const *input, float scalar)
{
    __m128 scaling_vec = _mm_load_ss(&scalar);  // avoiding uing _mm_set_ps1
    _mm_shuffle_ps(scaling_vec, scaling_vec, _MM_SHUFFLE(3, 3, 3, 3));
    output->size = input->size;
    output->data.sse_register = _mm_mul_ps(input->data.sse_register, scaling_vec);
}

static inline float fvec_min(fvec_t const *input)
{
    float min;
    __m128 tmp = _mm_min_ps(input->data.sse_register, _mm_shuffle_ps(input->data.sse_register,
                                                                     input->data.sse_register, _MM_SHUFFLE(2, 1, 0, 3)));
    tmp = _mm_min_ps(tmp, _mm_shuffle_ps(tmp, tmp, _MM_SHUFFLE(1, 0, 3, 2)));;

    _mm_store_ss(&min, tmp);
    return min;
}

static inline void fvec_display(fvec_t const *input)
{
    printf("[ ");
    for(uint32_t i = 0; i < input->size; i++)
        printf("%4.3f ", input->data.values[i]);
    printf(" ]");
}

/* vector initialisers */
#define fvec4_init(vec_in, x, y, z, w)\
    (vec_in).size = 4;\
    (vec_in).data.sse_register = _mm_set_ps((w), (z), (y), (x));

#define fvec3_init(vec_in, x, y, z)          \
    (vec_in).size = 3;                          \
    (vec_in).data.sse_register = _mm_set_ps(0.0f, (z), (y), (x));

#define fvec2_init(vec_in, x, y)             \
    (vec_in).size = 2;                          \
    (vec_in).data.sse_register = _mm_set_ps(0.0f, 0.0f, (y), (x));

/* Primitve functions macro implementations: (yes, macros with side-effects - how evil) */
#define fvec_add(vec_out, v0, v1)            \
    assert((v0).size == (v1).size);             \
    (vec_out).size = (v0).size;                 \
    (vec_out).data.sse_register = _mm_add_ps((v0).data.sse_register, (v1).data.sse_register)

#define fvec_sub(vec_out, v0, v1)            \
    assert((v0).size == (v1).size);             \
    (vec_out).size = (v0).size;                 \
    (vec_out).data.sse_register = _mm_sub_ps((v0).data.sse_register, (v1).data.sse_register)

#define fvec_mul(vec_out, v0, v1)            \
    assert((v0).size == (v1).size);             \
    (vec_out).size = (v0).size; \
    (vec_out).data.sse_register = _mm_mul_ps((v0).data.sse_register, (v1).data.sse_register); \

#define fvec_dot(v0, v1)                     \
    vectorf128_dot((v0).data.sse_register, (v1).data.sse_register)

#define fvec_cross(vec_out, v0, v1)          \
    assert((v0).size == (v1).size && (v0).size == 3);   \
    (vec_out).size = 3;                         \
    (vec_out).data.sse_register = vectorf128_cross((v0).data.sse_register, (v1).data.sse_register)

#define fvec_normalize(vec_out, v0)          \
    (vec_out).size = (v0).size;                 \
    (vec_out).data.sse_register = vectorf128_normalize((v0).data.sse_register)

#define fvec_reverse(vec_out, v0)            \
    (vec_out).size = (v0).size;                 \
    (vec_out).data.sse_register = _mm_shuffle_ps((v0).data.sse_registerm, (v0).data.sse_register, _MM_SHUFFLE(0, 1, 2, 3));

#define fvec_floor(vec_out, v0)              \
    (vec_out).size = (v0).size;                 \
    (vec_out).data.sse_register = vectorf128_floor((v0).data.sse_register);

#define fvec_ceil(vec_out, v0)               \
    (vec_out).size = (v0).size;                 \
    (vec_out).data.sse_register = vectorf128_ceil((v0).data.sse_register);

#define fvec_round(vec_out, v0)              \
    (vec_out).size = (v0).size;                 \
    (vec_out).data.sse_register = vectorf128_round((v0).data.sse_register);

#define fvec_mm_scale(vec_out, v0, scalar)      \
    (vec_out).size = (v0).size;                 \
    (vec_out).data.sse_register = vectorf128_scale((v0).data.sse_register, (scalar));


#endif // SMATH_VECTORF_H_
