#ifndef BASED__VECTOR2_HPP
#define BASED__VECTOR2_HPP

#include "base.h"
#include "float32.h"


#define V2 make_vector2

struct vector2
{
    union
    {
        struct { float32 x, y; };
        struct { float32 u, v; };
        struct { float32 _e1, _e2; };
        float32 e[2];
    };

    float32 operator [] (int index)
    {
        ASSERT(index < 2);
        return e[index];
    }
};

FORCE_INLINE vector2 make_vector2(float32 x, float32 y)
{
    vector2 result;
    result.x = x;
    result.y = y;
    return result;
}

FORCE_INLINE vector2 make_vector2(float32 t)
{
    vector2 result;
    result.x = result.y = t;
    return result;
}

FORCE_INLINE vector2 & operator += (vector2 & a, vector2 b) { a.x += b.x; a.y += b.y; return a; }
FORCE_INLINE vector2 & operator -= (vector2 & a, vector2 b) { a.x -= b.x; a.y -= b.y; return a; }
FORCE_INLINE vector2 & operator *= (vector2 & a, float32 c) { a.x *= c;   a.y *= c;   return a; }
FORCE_INLINE vector2 & operator /= (vector2 & a, float32 c) { a.x /= c;   a.y /= c;   return a; }

FORCE_INLINE bool operator == (vector2 a, vector2 b) { bool r = (a.x == b.x) && (a.y == b.y); return r; }
FORCE_INLINE bool operator != (vector2 a, vector2 b) { bool r = !(a == b); return r; }

FORCE_INLINE vector2 operator - (vector2 a) { vector2 r; r.x = -a.x; r.y = -a.y; return r; }
FORCE_INLINE vector2 operator + (vector2 a, vector2 b) { vector2 r; r.x = a.x + b.x; r.y = a.y + b.y; return r; }
FORCE_INLINE vector2 operator - (vector2 a, vector2 b) { vector2 r; r.x = a.x - b.x; r.y = a.y - b.y; return r; }
FORCE_INLINE vector2 operator * (vector2 a, float32 c) { vector2 r; r.x = a.x * c; r.y = a.y * c; return r; }
FORCE_INLINE vector2 operator * (float32 c, vector2 a) { vector2 r; r.x = c * a.x; r.y = c * a.y; return r; }
FORCE_INLINE vector2 operator / (vector2 a, float32 c) { vector2 r; r.x = a.x / c; r.y = a.y / c; return r; }

FORCE_INLINE float32 inner        (vector2 a, vector2 b) { float32 r = (a.x * b.x) + (a.y * b.y); return r; }
FORCE_INLINE float32 norm_squared (vector2 a) { return inner(a, a); }
FORCE_INLINE float32 norm         (vector2 a) { return square_root(norm_squared(a)); }
FORCE_INLINE void    normalize    (vector2 & a) { float32 n = norm(a); if (!near_zero(n)) { a /= n; } }
FORCE_INLINE vector2 normalized   (vector2 a) { normalize(a); return a; }


#endif // BASED__VECTOR2_HPP
