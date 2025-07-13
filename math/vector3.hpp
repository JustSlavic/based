#ifndef BASED__VECTOR3_HPP
#define BASED__VECTOR3_HPP

#include "base.h"
#include "vector2.hpp"


#define V3 make_vector3

struct vector3
{
    union
    {
        struct { float32 x, y, z; };
        struct { float32 r, g, b; };
        struct { vector2 xy; };
        struct { float32 _e1, _e2, _e3; };
        float32 e[3];
    };

    float32 operator [] (int32 index)
    {
        ASSERT(index < 3);
        return e[index];
    }

    static vector3 zero() { vector3 result = {}; return result; }
};

FORCE_INLINE vector3 make_vector3(float32 x, float32 y, float32 z)
{
    vector3 result;
    result.x = x;
    result.y = y;
    result.z = z;
    return result;
}

FORCE_INLINE vector3 make_vector3(vector2 xy, float32 z)
{
    vector3 result;
    result.xy = xy;
    result.z = z;
    return result;
}

FORCE_INLINE vector3 make_vector3(float32 t)
{
    vector3 result;
    result.x = result.y = result.z = t;
    return result;
}

FORCE_INLINE float32 *get_data(vector3 & a) { return &a._e1; }
FORCE_INLINE float32 const *get_data(vector3 const & a) { return &a._e1; }

FORCE_INLINE vector3 & operator += (vector3 & a, vector3 b) { a.x += b.x; a.y += b.y; a.z += b.z; return a; }
FORCE_INLINE vector3 & operator -= (vector3 & a, vector3 b) { a.x -= b.x; a.y -= b.y; a.z -= b.z; return a; }
FORCE_INLINE vector3 & operator *= (vector3 & a, float32 c) { a.x *= c;   a.y *= c;   a.z *= c;   return a; }
FORCE_INLINE vector3 & operator /= (vector3 & a, float32 c) { a.x /= c;   a.y /= c;   a.z /= c;   return a; }

FORCE_INLINE bool operator == (vector3 a, vector3 b) { bool result = (a.x == b.x) && (a.y == b.y) && (a.z == b.z); return result; }
FORCE_INLINE bool operator != (vector3 a, vector3 b) { bool result = !(a == b); return result; }

FORCE_INLINE vector3 operator - (vector3 a) { vector3 result; result.x = -a.x; result.y = -a.y; result.z = -a.z; return result; }
FORCE_INLINE vector3 operator + (vector3 a, vector3 b) { vector3 result; result.x = a.x + b.x; result.y = a.y + b.y; result.z = a.z + b.z; return result; }
FORCE_INLINE vector3 operator - (vector3 a, vector3 b) { vector3 result; result.x = a.x - b.x; result.y = a.y - b.y; result.z = a.z - b.z; return result; }
FORCE_INLINE vector3 operator * (vector3 a, float32 c) { vector3 result; result.x = a.x * c; result.y = a.y * c; result.z = a.z * c; return result; }
FORCE_INLINE vector3 operator * (float32 c, vector3 a) { vector3 result; result.x = c * a.x; result.y = c * a.y; result.z = c * a.z; return result; }
FORCE_INLINE vector3 operator / (vector3 a, float32 c) { vector3 result; result.x = a.x / c; result.y = a.y / c; result.z = a.z / c; return result; }

FORCE_INLINE float32 inner        (vector3 a, vector3 b) { float32 r = a.x * b.x + a.y * b.y + a.z * b.z; return r; }
FORCE_INLINE float32 norm_squared (vector3 a) { float32 result = inner(a, a); return result; }
FORCE_INLINE float32 norm         (vector3 a) { float32 result = square_root(norm_squared(a)); return result; }
FORCE_INLINE void    normalize    (vector3 & a) { float32 n = norm(a); if (n > 0) { a /= n; } }
FORCE_INLINE vector3 normalized   (vector3 a) { normalize(a); return a; }
FORCE_INLINE vector3 cross        (vector3 a, vector3 b) { vector3 result; result.x = a.y * b.z - a.z * b.y; result.y = a.z * b.x - a.x * b.z; result.z = a.x * b.y - a.y * b.x; return result; }
FORCE_INLINE vector3 bisector     (vector3 a, vector3 b) { vector3 result = norm(b)*a + norm(a)*b; return result; }
FORCE_INLINE vector3 lerp         (vector3 a, vector3 b, float32 t) { vector3 result = (1.f - t) * a + t * b; return result; }


#endif // BASED__VECTOR3_HPP
