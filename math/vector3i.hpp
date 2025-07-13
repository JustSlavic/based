#ifndef BASED__VECTOR3I_HPP
#define BASED__VECTOR3I_HPP

#include "base.h"
#include "vector3.hpp"


struct vector3i
{
    union
    {
        struct { int32 x, y, z; };
        int32 e[3];
    };

    int32 operator [] (int32 index)
    {
        ASSERT(index < 3);
        return e[index];
    }

    static vector3i zero() { vector3i result = {}; return result; }
};

FORCE_INLINE vector3i make_vector3i(int32 x, int32 y, int32 z)
{
    vector3i result;
    result.x = x;
    result.y = y;
    result.z = z;
    return result;
}

FORCE_INLINE vector3i make_vector3i(int32 t)
{
    vector3i result;
    result.x = result.y = result.z = t;
    return result;
}

FORCE_INLINE int32 *get_data(vector3i & a) { return a.e; }
FORCE_INLINE int32 const *get_data(vector3i const & a) { return a.e; }

FORCE_INLINE vector3i & operator += (vector3i & a, vector3i b) { a.x += b.x; a.y += b.y; a.z += b.z; return a; }
FORCE_INLINE vector3i & operator -= (vector3i & a, vector3i b) { a.x -= b.x; a.y -= b.y; a.z -= b.z; return a; }
FORCE_INLINE vector3i & operator *= (vector3i & a, int32 c) { a.x *= c;   a.y *= c;   a.z *= c;   return a; }
FORCE_INLINE vector3i & operator /= (vector3i & a, int32 c) { a.x /= c;   a.y /= c;   a.z /= c;   return a; }

FORCE_INLINE bool operator == (vector3i a, vector3i b) { bool result = (a.x == b.x) && (a.y == b.y) && (a.z == b.z); return result; }
FORCE_INLINE bool operator != (vector3i a, vector3i b) { bool result = !(a == b); return result; }

FORCE_INLINE vector3i operator - (vector3i a) { vector3i result; result.x = -a.x; result.y = -a.y; result.z = -a.z; return result; }
FORCE_INLINE vector3i operator + (vector3i a, vector3i b) { vector3i result; result.x = a.x + b.x; result.y = a.y + b.y; result.z = a.z + b.z; return result; }
FORCE_INLINE vector3i operator - (vector3i a, vector3i b) { vector3i result; result.x = a.x - b.x; result.y = a.y - b.y; result.z = a.z - b.z; return result; }
FORCE_INLINE vector3i operator * (vector3i a, int32 c) { vector3i result; result.x = a.x * c; result.y = a.y * c; result.z = a.z * c; return result; }
FORCE_INLINE vector3i operator * (int32 c, vector3i a) { vector3i result; result.x = c * a.x; result.y = c * a.y; result.z = c * a.z; return result; }

vector3 to_vector3(vector3i vi) { vector3 result; result.x = (float32) vi.x; result.y = (float32) vi.y; result.z = (float32) vi.z; return result; }


#endif // BASED__VECTOR3I_HPP
