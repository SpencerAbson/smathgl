#ifndef MATRIX4XM128F_H_
#define MATRIX4XM128F_H_

#include "..\..\..\include/platform.h"
#include "..\..\..\include/smathgl.h"
#include "vectorf128.h"


static inline __m128 mat4xm128_vec4_product(const __m128 mat[4], __m128 vec)
{
    float x = _mm_cvtss_f32(vectorf128_sum(_mm_mul_ps(mat[0], vec)));
    float y = _mm_cvtss_f32(vectorf128_sum(_mm_mul_ps(mat[1], vec)));
    float z = _mm_cvtss_f32(vectorf128_sum(_mm_mul_ps(mat[2], vec)));
    float w = _mm_cvtss_f32(vectorf128_sum(_mm_mul_ps(mat[3], vec)));

    return _mm_set_ps(w, z, y, x); // backwards register indexing for SIMD
}


static inline void mat4xm128_transpose(const __m128 input[4], __m128 out[4])
{
    __m128 tmp0 = _mm_shuffle_ps(input[0], input[1], 0x44);
    __m128 tmp2 = _mm_shuffle_ps(input[0], input[1], 0xEE);
    __m128 tmp1 = _mm_shuffle_ps(input[2], input[3], 0x44);
    __m128 tmp3 = _mm_shuffle_ps(input[2], input[3], 0xEE);

    out[0] = _mm_shuffle_ps(tmp0, tmp1, 0x88);
    out[1] = _mm_shuffle_ps(tmp0, tmp1, 0xDD);
    out[2] = _mm_shuffle_ps(tmp2, tmp3, 0x88);
    out[3] = _mm_shuffle_ps(tmp2, tmp3, 0xDD);
}


static inline void mat4xm128_outer_product(const __m128 input0, const __m128 input1, __m128 out[4])
{
    out[0] = _mm_mul_ps(input0, _mm_shuffle_ps(input1, input1, _MM_SHUFFLE(0, 0, 0, 0)));
    out[1] = _mm_mul_ps(input0, _mm_shuffle_ps(input1, input1, _MM_SHUFFLE(1, 1, 1, 1)));
    out[2] = _mm_mul_ps(input0, _mm_shuffle_ps(input1, input1, _MM_SHUFFLE(2, 2, 2, 2)));
    out[3] = _mm_mul_ps(input0, _mm_shuffle_ps(input1, input1, _MM_SHUFFLE(3, 3, 3, 3)));
}


static inline void mat4xm128_add(const __m128 input0[4], const __m128 input1[4], __m128 out[4])
{
    out[0] = _mm_add_ps(input0[0], input1[0]);
    out[1] = _mm_add_ps(input0[1], input1[1]);
    out[2] = _mm_add_ps(input0[2], input1[2]);
    out[3] = _mm_add_ps(input0[3], input1[3]);
}


static inline void mat4xm128_sub(const __m128 input0[4], const __m128 input1[4], __m128 out[4])
{
    out[0] = _mm_sub_ps(input0[0], input1[0]);
    out[1] = _mm_sub_ps(input0[1], input1[1]);
    out[2] = _mm_sub_ps(input0[2], input1[2]);
    out[3] = _mm_sub_ps(input0[3], input1[3]);
}


static inline void mat4xm128_mul(const __m128 input0[4], const __m128 input1[4], __m128 out[4])
{
    {
        __m128 e0 = _mm_shuffle_ps(input1[0], input1[0], _MM_SHUFFLE(0, 0, 0, 0));
        __m128 e1 = _mm_shuffle_ps(input1[0], input1[0], _MM_SHUFFLE(1, 1, 1, 1));
        __m128 e2 = _mm_shuffle_ps(input1[0], input1[0], _MM_SHUFFLE(2, 2, 2, 2));
        __m128 e3 = _mm_shuffle_ps(input1[0], input1[0], _MM_SHUFFLE(3, 3, 3, 3));

        __m128 m0 = _mm_mul_ps(input0[0], e0);
        __m128 m1 = _mm_mul_ps(input0[1], e1);
        __m128 m2 = _mm_mul_ps(input0[2], e2);
        __m128 m3 = _mm_mul_ps(input0[3], e3);

        __m128 a0 = _mm_add_ps(m0, m1);
        __m128 a1 = _mm_add_ps(m2, m3);
        __m128 a2 = _mm_add_ps(a0, a1);

        out[0] = a2;
    }

    {
        __m128 e0 = _mm_shuffle_ps(input1[1], input1[1], _MM_SHUFFLE(0, 0, 0, 0));
        __m128 e1 = _mm_shuffle_ps(input1[1], input1[1], _MM_SHUFFLE(1, 1, 1, 1));
        __m128 e2 = _mm_shuffle_ps(input1[1], input1[1], _MM_SHUFFLE(2, 2, 2, 2));
        __m128 e3 = _mm_shuffle_ps(input1[1], input1[1], _MM_SHUFFLE(3, 3, 3, 3));

        __m128 m0 = _mm_mul_ps(input0[0], e0);
        __m128 m1 = _mm_mul_ps(input0[1], e1);
        __m128 m2 = _mm_mul_ps(input0[2], e2);
        __m128 m3 = _mm_mul_ps(input0[3], e3);

        __m128 a0 = _mm_add_ps(m0, m1);
        __m128 a1 = _mm_add_ps(m2, m3);
        __m128 a2 = _mm_add_ps(a0, a1);

        out[1] = a2;
    }

    {
        __m128 e0 = _mm_shuffle_ps(input1[2], input1[2], _MM_SHUFFLE(0, 0, 0, 0));
        __m128 e1 = _mm_shuffle_ps(input1[2], input1[2], _MM_SHUFFLE(1, 1, 1, 1));
        __m128 e2 = _mm_shuffle_ps(input1[2], input1[2], _MM_SHUFFLE(2, 2, 2, 2));
        __m128 e3 = _mm_shuffle_ps(input1[2], input1[2], _MM_SHUFFLE(3, 3, 3, 3));

        __m128 m0 = _mm_mul_ps(input0[0], e0);
        __m128 m1 = _mm_mul_ps(input0[1], e1);
        __m128 m2 = _mm_mul_ps(input0[2], e2);
        __m128 m3 = _mm_mul_ps(input0[3], e3);

        __m128 a0 = _mm_add_ps(m0, m1);
        __m128 a1 = _mm_add_ps(m2, m3);
        __m128 a2 = _mm_add_ps(a0, a1);

        out[2] = a2;
    }

    {
        __m128 e0 = _mm_shuffle_ps(input1[3], input1[3], _MM_SHUFFLE(0, 0, 0, 0));
        __m128 e1 = _mm_shuffle_ps(input1[3], input1[3], _MM_SHUFFLE(1, 1, 1, 1));
        __m128 e2 = _mm_shuffle_ps(input1[3], input1[3], _MM_SHUFFLE(2, 2, 2, 2));
        __m128 e3 = _mm_shuffle_ps(input1[3], input1[3], _MM_SHUFFLE(3, 3, 3, 3));

        __m128 m0 = _mm_mul_ps(input0[0], e0);
        __m128 m1 = _mm_mul_ps(input0[1], e1);
        __m128 m2 = _mm_mul_ps(input0[2], e2);
        __m128 m3 = _mm_mul_ps(input0[3], e3);

        __m128 a0 = _mm_add_ps(m0, m1);
        __m128 a1 = _mm_add_ps(m2, m3);
        __m128 a2 = _mm_add_ps(a0, a1);

        out[3] = a2;
    }
}


static inline __m128 matrixf128_vec4_mul(__m128 vec, __m128 mat[4])
{
    __m128 m0 = _mm_mul_ps(vec, mat[0]);
    __m128 m1 = _mm_mul_ps(vec, mat[1]);
    __m128 m2 = _mm_mul_ps(vec, mat[2]);
    __m128 m3 = _mm_mul_ps(vec, mat[3]);

    __m128 u0 = _mm_unpacklo_ps(m0, m1);
    __m128 u1 = _mm_unpackhi_ps(m0, m1);
    __m128 a0 = _mm_add_ps(u0, u1);

    __m128 u2 = _mm_unpacklo_ps(m2, m3);
    __m128 u3 = _mm_unpackhi_ps(m2, m3);
    __m128 a1 = _mm_add_ps(u2, u3);

    __m128 f0 = _mm_movelh_ps(a0, a1);
    __m128 f1 = _mm_movehl_ps(a1, a0);
    __m128 f2 = _mm_add_ps(f0, f1);

    return f2;
}


static inline void mat4xm128_rotate(__m128 const input[4], __m128 const axis, float angle, __m128 out[4])
{
    /* SSE 4.2 Matrix rotation from normalized fvec3 and angle (radians) - not pretty */

    float const cos_a = cosf(angle);
    float const sin_a = sinf(angle);

    __m128 const one   = _mm_set1_ps(1.0f);
    __m128 const zero  = _mm_setzero_ps();
    __m128 const neg_c = _mm_set1_ps(1.0f - cos_a);
    __m128 const temp  = _mm_mul_ps(axis, neg_c);

    __m128 rotation[4];
    __m128 asic = _mm_insert_ps(_mm_mul_ps(axis, _mm_set1_ps(sin_a)), _mm_sub_ps(one, neg_c), 0x30); // 1 - (1 - c) = c

    asic = _mm_xor_ps(asic, _mm_set_ps(-0.0f, 0.0f, 0.0f, 0.0f)); // negation to conform with order of operation in _mm_addsub_ps
    {
        __m128 tmp0 = _mm_shuffle_ps(temp, temp, _MM_SHUFFLE(0, 0, 0, 0));
        __m128 at0  = _mm_mul_ps(tmp0, axis);
        __m128 res0 = _mm_addsub_ps(at0, _mm_shuffle_ps(asic, asic, _MM_SHUFFLE(0, 1, 2, 3)));
        rotation[0] = _mm_insert_ps(res0, zero, 0x30); // zeroing the garbage values TODO: cleaner approach to this
    }

    asic = _mm_xor_ps(asic, _mm_set_ps(-0.0f, 0.0f, -0.0f, -0.0f)); // lowest value is negated to reverse previous change
    {
        __m128 tmp1 = _mm_shuffle_ps(temp, temp, _MM_SHUFFLE(1, 1, 1, 1));
        __m128 at1  = _mm_mul_ps(tmp1, axis);
        __m128 res1   = _mm_addsub_ps(at1, _mm_shuffle_ps(asic, asic, _MM_SHUFFLE(0, 0, 3, 2)));
        rotation[1] = _mm_insert_ps(res1, zero, 0x30);
    }

    asic = _mm_xor_ps(asic, _mm_set_ps(-0.0f, -0.0f, 0.0f, 0.0f));
    {
        __m128 tmp2 = _mm_shuffle_ps(temp, temp, _MM_SHUFFLE(2, 2, 2, 2));
        __m128 at2  = _mm_mul_ps(tmp2, axis);
        __m128 res2   = _mm_addsub_ps(at2, _mm_shuffle_ps(asic, asic, _MM_SHUFFLE(0, 3, 0, 1)));
        rotation[2] = _mm_insert_ps(res2, zero, 0x30);
    }

    /* set final row and rotate input by rotation */
    rotation[3] = _mm_set_ps(1.0f, 0.0f, 0.0f, 0.0f);
    mat4xm128_mul(input, rotation, out);
}

#endif // MATRIX4XM128F_H_
