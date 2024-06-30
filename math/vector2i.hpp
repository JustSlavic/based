#ifndef BASED__VECTOR2I_HPP
#define BASED__VECTOR2I_HPP

#include "base.h"


#define V2I make_vector2i

struct vector2i
{
    union
    {
        struct { int32 x, y; };
        struct { int32 u, v; };
        struct { int32 _e1, _e2; };
        int32 e[2];
    };

    int32 operator [] (int index)
    {
        ASSERT(index < 2);
        return e[index];
    }
};

FORCE_INLINE vector2i make_vector2i(int32 x, int32 y)
{
    vector2i result;
    result.x = x;
    result.y = y;
    return result;
}

FORCE_INLINE vector2i make_vector2i(int32 t)
{
    vector2i result;
    result.x = result.y = t;
    return result;
}

FORCE_INLINE vector2i & operator += (vector2i & a, vector2i b) { a.x += b.x; a.y += b.y; return a; }
FORCE_INLINE vector2i & operator -= (vector2i & a, vector2i b) { a.x -= b.x; a.y -= b.y; return a; }
FORCE_INLINE vector2i & operator *= (vector2i & a, int32 c) { a.x *= c;   a.y *= c;   return a; }
FORCE_INLINE vector2i & operator /= (vector2i & a, int32 c) { a.x /= c;   a.y /= c;   return a; }

FORCE_INLINE bool operator == (vector2i a, vector2i b) { bool r = (a.x == b.x) && (a.y == b.y); return r; }
FORCE_INLINE bool operator != (vector2i a, vector2i b) { bool r = !(a == b); return r; }

FORCE_INLINE vector2i operator - (vector2i a) { vector2i r; r.x = -a.x; r.y = -a.y; return r; }
FORCE_INLINE vector2i operator + (vector2i a, vector2i b) { vector2i r; r.x = a.x + b.x; r.y = a.y + b.y; return r; }
FORCE_INLINE vector2i operator - (vector2i a, vector2i b) { vector2i r; r.x = a.x - b.x; r.y = a.y - b.y; return r; }
FORCE_INLINE vector2i operator * (vector2i a, int32 c) { vector2i r; r.x = a.x * c; r.y = a.y * c; return r; }
FORCE_INLINE vector2i operator * (int32 c, vector2i a) { vector2i r; r.x = c * a.x; r.y = c * a.y; return r; }
FORCE_INLINE vector2i operator / (vector2i a, int32 c) { vector2i r; r.x = a.x / c; r.y = a.y / c; return r; }

FORCE_INLINE int32 inner        (vector2i a, vector2i b) { int32 r = (a.x * b.x) + (a.y * b.y); return r; }
FORCE_INLINE int32 norm_squared (vector2i a) { return inner(a, a); }
FORCE_INLINE int32 norm         (vector2i a) { return absolute(a.x) + absolute(a.y); }


#endif // BASED__VECTOR2I_HPP
