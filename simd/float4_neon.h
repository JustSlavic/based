#ifndef BASED__fLOAT4_ARM
#define BASED__fLOAT4_ARM

#include "base.h"
#include <arm_neon.h>

typedef float32x4_t float4;


FORCE_INLINE float4 v_ld(float const *a) { return vld1q_f32(a); }
FORCE_INLINE void   v_st(float *r, float4 a) { vst1q_f32(r, a); }

FORCE_INLINE float4 v_make_float4(float x, float y, float z, float w)
{
    alignas(16) float data[4] = { x, y, z, w };
    return vld1q_f32(data);
}

FORCE_INLINE float4 v_add(float4 a, float4 b) { return vaddq_f32(a, b); }
FORCE_INLINE float4 v_sub(float4 a, float4 b) { return vsubq_f32(a, b); }
FORCE_INLINE float4 v_mul(float4 a, float4 b) { return vmulq_f32(a, b); }
FORCE_INLINE float4 v_div(float4 a, float4 b) { return vdivq_f32(a, b); }
FORCE_INLINE float4 v_hadd(float4 a, float4 b) { return vpaddq_f32(a, b); }
FORCE_INLINE float4 v_sqrt(float4 a) { return vsqrtq_f32(a); }
FORCE_INLINE float4 v_madd(float4 a, float4 b, float4 c) { return vmlaq_f32(c, a, b); }


#endif // BASED__fLOAT4_ARM
