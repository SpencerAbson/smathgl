#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "matrices.h"


void mat4_init_diagonal(mat4_t *output, const float translator)
{
    output->sse_registers[0] = _mm_set_ps(0.0f, 0.0f, 0.0f, translator);
    output->sse_registers[1] = _mm_set_ps(0.0f, 0.0f, translator, 0.0f);
    output->sse_registers[2] = _mm_set_ps(0.0f, translator, 0.0f, 0.0f);
    output->sse_registers[3] = _mm_set_ps(translator, 0.0f, 0.0f, 0.0f);
    return;
}


void mat4_init_translation(mat4_t *output, float x, float y, float z)
{
    output->sse_registers[0] = _mm_set_ps(x, 0.0f, 0.0f, 1.0f);
    output->sse_registers[1] = _mm_set_ps(y, 0.0f, 1.0f, 0.0f);
    output->sse_registers[2] = _mm_set_ps(z, 1.0f, 0.0f, 0.0f);
    output->sse_registers[3] = _mm_set_ps(1.0f, 0.0f, 0.0f, 0.0f);
}


void mat4_display(const mat4_t *mat)
{
    for(uint32_t i = 0; i < 4; i++)
    {
        printf("\n");
        for(uint32_t j = 0; j < 4; j++){
            if(i == 0 && j == 0) printf("[");
            printf("%f ", mat->values[i][j]);
        }
    }
    printf("]");
    printf("\n");
}
