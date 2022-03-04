#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "matrices.h"
#include "simd/matrix4xm128f.h"
// I'm aware this is a mess

void mat4_mul(const mat4x4 mat_a, const mat4x4 mat_b, mat4x4 out)
{
    mat4xm128 a, b, c;
    for(uint32_t i = 0; i < 4; i++){
        a[i] = _mm_load_ps(mat_a[i]);
        b[i] = _mm_load_ps(mat_b[i]);
    }

    mat4xm128_mul(a, b, c);
    for(uint32_t i = 0; i < 4; i++)
        _mm_store_ps(out[i], c[i]);
}


void mat4_transpose(const mat4x4 input, mat4x4 out)
{
    mat4xm128 a0;
    mat4_load_to_m128(input, a0);

    mat4xm128_transpose(a0, a0);
    m128_store_mat4(a0, out);
}


void mat4_add(const mat4x4 mat_a, const mat4x4 mat_b, mat4x4 out)
{
    mat4xm128 a0, a1, a2;
    mat4_load_to_m128(mat_a, a0);
    mat4_load_to_m128(mat_b, a1);

    mat4xm128_add(a0, a1, a2);
    m128_store_mat4(a2, out);
}


void mat4_sub(const mat4x4 mat_a, const mat4x4 mat_b, mat4x4 out)
{
    mat4xm128 a0, a1, a2;
    mat4_load_to_m128(mat_a, a0);
    mat4_load_to_m128(mat_b, a1);

    mat4xm128_sub(a0, a1, a2);
    m128_store_mat4(a2, out);
}


void mat4_display(const mat4x4 mat)
{
    for(uint32_t i = 0; i < 4; i++)
    {
        printf("\n");
        for(uint32_t j = 0; j < 4; j++){
            if(i == 0 && j == 0) printf("[");
            printf("%f ", mat[i][j]);
        }
        if(i == 3) printf("]");
    }
    printf("\n");
}



/*void mul_mat4x4(mat4x4 input0, mat4x4 input1, mat4x4 out)
{
    mat4xm128 a, b, c;
    a[0] = _mm_load_ps(input0[0]);
    a[1] = _mm_load_ps(input0[1]);
    a[2] = _mm_load_ps(input0[2]);
    a[3] = _mm_load_ps(input0[3]);

    b[0] = _mm_load_ps(input1[0]);
    b[1] = _mm_load_ps(input1[1]);
    b[2] = _mm_load_ps(input1[2]);
    b[3] = _mm_load_ps(input1[3]);

    mat4xm128_mul(a, b, c);
    _mm_store_ps(out[0], c[0]);
    _mm_store_ps(out[1], c[1]);
    _mm_store_ps(out[2], c[2]);
    _mm_store_ps(out[3], c[3]);

}*/
