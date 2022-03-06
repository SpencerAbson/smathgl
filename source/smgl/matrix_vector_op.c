#include <assert.h>
#include "matrix_vector_op.h"
#include "simd/matrix4xm128f.h"

mat4x4 vec4_outer_product(fvec *input0,  fvec *input1)
{
    assert(input0->size == input1->size && input0->size == 4);

    mat4x4 output;
    mat4xm128_outer_product(input0->data.sse_register, input1->data.sse_register,
                            output.sse_registers);
    return output;
}


fvec mat4_vec_product(mat4x4 const *mat, fvec *vec)
{
    assert(vec->size == 4);

    fvec output;
    output.size = vec->size;
    output.data.sse_register = mat4xm128_vec4_product(mat->sse_registers, vec->data.sse_register);
    return output;
}
