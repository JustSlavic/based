#ifndef BASED__fLOAT4_X86
#define BASED__fLOAT4_X86

#include "base.h"
#include <xmmintrin.h>
#include <immintrin.h>
#include <pmmintrin.h>

typedef __m128 float4;


// FORCE_INLINE float4 v_undefined(void) { return _mm_undefined_ps(); }
FORCE_INLINE float4 v_ld(float const *a) { return _mm_load_ps(a); }
FORCE_INLINE void   v_st(float *r, float4 a) { return _mm_store_ps(r, a); }
FORCE_INLINE float4 v_make_float4(float x, float y, float z, float w)
{
    alignas(16) float data[4] = { x, y, z, w };
    return v_ld(data);
}

FORCE_INLINE float4 v_add(float4 a, float4 b) { return _mm_add_ps(a, b); }
FORCE_INLINE float4 v_sub(float4 a, float4 b) { return _mm_sub_ps(a, b); }
FORCE_INLINE float4 v_mul(float4 a, float4 b) { return _mm_mul_ps(a, b); }
FORCE_INLINE float4 v_div(float4 a, float4 b) { return _mm_div_ps(a, b); }
#if defined(__FMA__)
FORCE_INLINE float4 v_madd(float4 a, float4 b, float4 c) { return _mm_fmadd_ps(a, b, c); }
#else
FORCE_INLINE float4 v_madd(float4 a, float4 b, float4 c) { return v_add(v_mul(a, b), c); }
#endif
FORCE_INLINE float4 v_sqrt(float4 a) { return _mm_sqrt_ps(a); }
FORCE_INLINE float4 v_hadd(float4 a, float4 b) { return _mm_hadd_ps(a, b); }


#endif // BASED__fLOAT4_X86
