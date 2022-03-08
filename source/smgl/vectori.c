#include "vectori.h"

ivec ivec4_init(int32_t x, int32_t y, int32_t z, int32_t w)
{
    ivec output;
    output.size = 4;
    output.data.sse_register = _mm_set_epi32(w, z, y, x);
    return output;
}

ivec ivec3_init(int32_t x, int32_t y, int32_t z)
{
    ivec output;
    output.size = 3;
    output.data.sse_register = _mm_set_epi32(0, z, y, x);
    return output;
}

ivec ivec2_init(float x, float y)
{
    ivec output;
    output.size = 2;
    output.data.sse_register = _mm_set_epi32(0, 0, y, x);
    return output;
}
