#ifndef BASED__FLOATING_H
#define BASED__FLOATING_H

#include <base.h>
#include <math.h>


GLOBAL const float32 pi = 3.14159265358979323846f;
GLOBAL const float32 half_pi = 0.5f * pi;
GLOBAL const float32 infinity = HUGE_VALF;


#if 0
float32 sin(float32 x)
{
    float32 result = sinf(x);
    return result;
}

float32 cos(float32 x)
{
    float32 result = cosf(x);
    return result;
}

bool32 finite(float32 x)
{
    bool32 result = (-infinity < x) && (x < infinity);
    return result;
}
#endif

float32 absolute(float32 x)
{
    union { float32 f; uint32 u; } pun;
    pun.f = x;
    pun.u = (pun.u & 0x7fffffff);
    return pun.f;
}

float32 sign(float32 x)
{
    union { float32 f; uint32 u; } pun;
    pun.f = x;
    pun.u = (pun.u & 0x80000000) | 0x3f800000;
    return pun.f;
}

float32 square(float32 x)
{
    float32 result = x * x;
    return result;
}

float32 square_root(float32 x)
{
    float32 result = sqrtf(x);
    return result;
}

float32 to_radians(float32 degrees)
{
    float32 result = degrees * pi / 180.0f;
    return result;
}

float32 to_degrees(float32 radians)
{
    float32 result = radians * 180.0f / pi;
    return result;
}

bool32 near_zero(float32 x)
{
    bool32 result = (-EPSILON < x) && (x < EPSILON);
    return result;
}

bool32 near_zero_eps(float32 x, float32 eps)
{
    bool32 result = (-eps < x) && (x < eps);
    return result;
}

bool32 equal(float32 x, float32 y)
{
    bool32 result = near_zero(x - y);
    return result;
}

bool32 equal_eps(float32 x, float32 y, float32 eps)
{
    bool32 result = near_zero_eps(x - y, eps);
    return result;
}

float32 clamp(float32 x, float32 a, float32 b)
{
    if (x < a) return a;
    if (x > b) return b;
    return x;
}

float32 lerp(float32 a, float32 b, float32 t)
{
    float32 result = (1.f - t) * a + t * b;
    return result;
}

int32 truncate_to_int32(float32 x)
{
    int32 result = (int32) x;
    return result;
}

int32 round_to_int32(float32 x)
{
    int32 result = (int32) (x + sign(x) * 0.5f);
    return result;
}


#endif // BASED__FLOATING_H

