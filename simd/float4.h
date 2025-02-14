#ifndef BASED__FLOAT4
#define BASED__FLOAT4

#if OS_WINDOWS
#include "float4_sse.h"
#elif OS_MAC
#include "float4_neon.h"
#endif


FORCE_INLINE float4 v_dot(float4 a, float4 b)
{
    float4 result = v_sum(v_mul(a, b));
    return result;
}


#endif // BASED__FLOAT4
