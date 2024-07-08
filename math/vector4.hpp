#ifndef BASED__VECTOR4_HPP
#define BASED__VECTOR4_HPP

#include "base.h"
#include "vector3.hpp"


#define V4_1(xyzw) make_vector4( (float32) (xyzw), (float32) (xyzw), (float32) (xyzw), (float32) (xyzw) )
#define V4_2(xyz, w) make_vector4( (xyz).x, (xyz).y, (xyz).z, (float32) (w) )
#define V4_3(xy, z, w) make_vector4( (xy).x, (xy).y, (float32) z, (float32) (w) )
#define V4_4(x, y, z, w) make_vector4( (float32) (x), (float32) (y), (float32) (z), (float32) (w) )
#define V4(...) MACRO_EXPAND(MACRO_OVERLOAD_4(__VA_ARGS__, V4_4, V4_3, V4_2, V4_1)(__VA_ARGS__))

struct vector4
{
    union
    {
        struct { float32 x, y, z, w; };
        struct { float32 r, g, b, a; };
        struct { vector2 xy, zw; };
        struct { vector2 rg, ba; };
        struct { vector3 xyz; };
        struct { vector3 rgb; };
        struct { float32 _e1, _e2, _e3, _e4; };
        float32 e[4];
    };

    float32 operator [] (int32 index)
    {
        ASSERT(index < 4);
        return e[index];
    }
};

FORCE_INLINE vector4 make_vector4(float32 x, float32 y, float32 z, float32 w)
{
    vector4 result;
    result.x = x;
    result.y = y;
    result.z = z;
    result.w = w;
    return result;
}

FORCE_INLINE vector4 make_vector4(vector2 xy, float32 z, float32 w)
{
    vector4 result;
    result.xy = xy;
    result.z = z;
    result.w = w;
    return result;
}

FORCE_INLINE vector4 make_vector4(vector3 xyz, float32 w)
{
    vector4 result;
    result.xyz = xyz;
    result.w = w;
    return result;
}

FORCE_INLINE vector4 make_vector4(float32 t)
{
    vector4 result;
    result.x = result.y = result.z = result.w = t;
    return result;
}

FORCE_INLINE vector4 & operator += (vector4 & a, vector4 b) { a.x += b.x; a.y += b.y; a.z += b.z; a.w += b.w; return a; }
FORCE_INLINE vector4 & operator -= (vector4 & a, vector4 b) { a.x -= b.x; a.y -= b.y; a.z -= b.z; a.w -= b.w; return a; }
FORCE_INLINE vector4 & operator *= (vector4 & a, float32 c) { a.x *= c;   a.y *= c;   a.z *= c;   a.w *= c;   return a; }
FORCE_INLINE vector4 & operator /= (vector4 & a, float32 c) { a.x /= c;   a.y /= c;   a.z /= c;   a.w /= c;   return a; }

FORCE_INLINE bool operator == (vector4 a, vector4 b) { bool result = (a.x == b.x) && (a.y == b.y) && (a.z == b.z) && (a.w == b.w); return result; }
FORCE_INLINE bool operator != (vector4 a, vector4 b) { bool result = !(a == b); return result; }

FORCE_INLINE vector4 operator - (vector4 a)            { vector4 result; result.x = -a.x; result.y = -a.y; result.z = -a.z; result.w = -a.w; return result; }
FORCE_INLINE vector4 operator + (vector4 a, vector4 b) { vector4 result; result.x = a.x + b.x; result.y = a.y + b.y; result.z = a.z + b.z; result.w = a.w + b.w; return result; }
FORCE_INLINE vector4 operator - (vector4 a, vector4 b) { vector4 result; result.x = a.x - b.x; result.y = a.y - b.y; result.z = a.z - b.z; result.w = a.w - b.w; return result; }
FORCE_INLINE vector4 operator * (vector4 a, float32 c) { vector4 result; result.x = a.x * c; result.y = a.y * c; result.z = a.z * c; result.w = a.w * c; return result; }
FORCE_INLINE vector4 operator * (float32 c, vector4 a) { vector4 result; result.x = c * a.x; result.y = c * a.y; result.z = c * a.z; result.w = c * a.w; return result; }
FORCE_INLINE vector4 operator / (vector4 a, float32 c) { vector4 result; result.x = a.x / c; result.y = a.y / c; result.z = a.z / c; result.w = a.w / c; return result; }

FORCE_INLINE float32 inner        (vector4 a, vector4 b) { float32 result = (a.x * b.x) + (a.y * b.y) + (a.z * b.z) + (a.w * b.w); return result; }
FORCE_INLINE float32 norm_squared (vector4 a) { float32 result = inner(a, a); return result; }
FORCE_INLINE float32 norm         (vector4 a) { float32 result = square_root(norm_squared(a)); return result; }
FORCE_INLINE void    normalize    (vector4 & a) { float32 n = norm(a); if (n > 0) { a /= n; } }
FORCE_INLINE vector4 normalized   (vector4 a) { normalize(a); return a; }


#endif // BASED__VECTOR4_HPP
