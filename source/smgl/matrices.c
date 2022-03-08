#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "matrices.h"
#include "simd/matrix4xm128f.h"
// worst code in this project, not shippable version:


mat4x4 mat4_init_translation(const float translator)
{
    mat4x4 output;
    output.sse_registers[0] = _mm_set_ps(0.0f, 0.0f, 0.0f, translator);
    output.sse_registers[1] = _mm_set_ps(0.0f, 0.0f, translator, 0.0f);
    output.sse_registers[2] = _mm_set_ps(0.0f, translator, 0.0f, 0.0f);
    output.sse_registers[3] = _mm_set_ps(translator, 0.0f, 0.0f, 0.0f);

    return output;
}


mat4x4 mat4_init()
{
    mat4x4 output;
    output.sse_registers[0] = _mm_set_ps1(0.0f);
    output.sse_registers[1] = _mm_set_ps1(0.0f);
    output.sse_registers[2] = _mm_set_ps1(0.0f);
    output.sse_registers[3] = _mm_set_ps1(0.0f);

    return output;
}


mat4x4 mat4_mul(mat4x4 const *input0, mat4x4 const *input1)
{
    mat4x4 output;
    mat4xm128_mul(input0->sse_registers, input1->sse_registers,
                  output.sse_registers);
    return output;
}


mat4x4 mat4_transpose(mat4x4 const *input)
{
    mat4x4 output;
    mat4xm128_transpose(input->sse_registers, output.sse_registers);
    return output;
}


mat4x4 mat4_add(mat4x4 const *input0, mat4x4 const *input1)
{
    mat4x4 output;
    mat4xm128_add(input0->sse_registers, input1->sse_registers,
                  output.sse_registers);
    return output;
}


mat4x4 mat4_sub(mat4x4 const *input0, mat4x4 const *input1)
{
    mat4x4 output;
    mat4xm128_sub(input0->sse_registers, input1->sse_registers,
                  output.sse_registers);
    return output;
}


void mat4_display(const mat4x4 *mat)
{
    for(uint32_t i = 0; i < 4; i++)
    {
        printf("\n");
        for(uint32_t j = 0; j < 4; j++){
            if(i == 0 && j == 0) printf("[");
            printf("%f ", mat->values[i][j]);
        }
        if(i == 3) printf("]");
    }
    printf("\n");
}
