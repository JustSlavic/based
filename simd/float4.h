#ifndef BASED__FLOAT4
#define BASED__FLOAT4

#if OS_WINDOWS
#include "float4_sse.h"
#elif OS_MAC
#include "float4_neon.h"
#endif


FORCE_INLINE float4 v_dot(float4 a, float4 b)
{
    float4 c = v_mul(a, b);
    float4 result = v_hadd(c, c);
    return result;
}

FORCE_INLINE float4 q_mul(float4 a, float4 b)
{
    // [0123]
    // [3210]
    // [1032]
    // [2301]

    auto a1 = v_set_x(a);
    auto ax = v_set_y(a);
    auto ay = v_set_z(a);
    auto az = v_set_w(a);

    float4 result = v_add(v_mul(a1, b),
                    v_add(v_mul(ax, v_mul(v_set4(-1.f, -1.f, 1.f, 1.f), v_set_wzyx(b))),
                    v_add(v_mul(ay, v_mul(v_set4(-1.f, 1.f, -1.f, 1.f), v_set_yxwz(b))),
                          v_mul(az, v_mul(v_set4(-1.f, 1.f, 1.f, -1.f), v_set_zwxy(b))))));
    return result;
//     // quaternion r;
//     // r._1   = a._1*b._1   - a._e21*b._e21 - a._e32*b._e32 - a._e13*b._e13;
//     // r._e32 = a._1*b._e32 - a._e21*b._e13 + a._e32*b._1   + a._e13*b._e21;
//     // r._e13 = a._1*b._e13 + a._e21*b._e32 - a._e32*b._e21 + a._e13*b._1;
//     // r._e21 = a._1*b._e21 + a._e21*b._1   + a._e32*b._e13 - a._e13*b._e32;
//     // return r;
}


#endif // BASED__FLOAT4
