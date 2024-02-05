#ifndef BASED__QUATERNION_HPP
#define BASED__QUATERNION_HPP

#include "base.h"
#include "vector3.hpp"


struct quaternion
{
    union
    {
        struct { float32 _1, _e23, _e31, _e12; };
        struct { float32 w, x, y, z; };
        struct { float32 r, i, j, k; };
        float32 e[4];
    };
};

FORCE_INLINE quaternion & operator += (quaternion & a, quaternion b) { a._1 += b._1; a._e12 += b._e12; a._e23 += b._e23; a._e31 += b._e31; return a; }
FORCE_INLINE quaternion & operator -= (quaternion & a, quaternion b) { a._1 -= b._1; a._e12 -= b._e12; a._e23 -= b._e23; a._e31 -= b._e31; return a; }
FORCE_INLINE quaternion & operator *= (quaternion & a, float32 c) { a._1 *= c; a._e12 *= c; a._e23 *= c; a._e31 *= c; return a; }
FORCE_INLINE quaternion & operator /= (quaternion & a, float32 c) { a._1 /= c; a._e12 /= c; a._e23 /= c; a._e31 /= c; return a; }

FORCE_INLINE bool operator == (quaternion const & a, quaternion const & b) { bool result = (a.w == b.w) && (a.x == b.x) && (a.y == b.y) && (a.z == b.z); return result; }
FORCE_INLINE bool operator != (quaternion const & a, quaternion const & b) { return !(a == b); }

FORCE_INLINE quaternion operator + (quaternion a, quaternion b) { quaternion result; result._1 = a._1 + b._1; result._e12 = a._e12 + b._e12; result._e23 = a._e23 + b._e23; result._e31 = a._e31 + b._e31; return result; }
FORCE_INLINE quaternion operator - (quaternion a, quaternion b) { quaternion result; result._1 = a._1 - b._1; result._e12 = a._e12 - b._e12; result._e23 = a._e23 - b._e23; result._e31 = a._e31 - b._e31; return result; }
FORCE_INLINE quaternion operator * (quaternion q, float32 c) { quaternion result; result._1 = q._1 * c; result._e12 = q._e12 * c; result._e23 = q._e23 * c; result._e31 = q._e31 * c; return result; }
FORCE_INLINE quaternion operator * (float32 c, quaternion q) { quaternion result; result._1 = c * q._1; result._e12 = c * q._e12; result._e23 = c * q._e23; result._e31 = c * q._e31; return result; }
FORCE_INLINE quaternion operator / (quaternion q, float32 c) { quaternion result; result._1 = q._1 / c; result._e12 = q._e12 / c; result._e23 = q._e23 / c; result._e31 = q._e31 / c; return result; }

FORCE_INLINE quaternion operator * (vector3 a, vector3 b)
{
    quaternion result;
    result._1   = (a._e1 * b._e1) + (a._e2 * b._e2) + (a._e3 * b._e3);
    result._e12 = (a._e1 * b._e2) - (a._e2 * b._e1);
    result._e23 = (a._e2 * b._e3) - (a._e3 * b._e2);
    result._e31 = (a._e3 * b._e1) - (a._e1 * b._e3);
    return result;
}

FORCE_INLINE quaternion operator * (quaternion a, quaternion b)
{
    quaternion r;
    r._1   = a._1*b._1   - a._e12*b._e12 - a._e23*b._e23 - a._e31*b._e31;
    r._e12 = a._1*b._e12 + a._e12*b._1   - a._e23*b._e31 + a._e31*b._e23;
    r._e23 = a._1*b._e23 + a._e12*b._e31 + a._e23*b._1   - a._e31*b._e12;
    r._e31 = a._1*b._e31 - a._e12*b._e23 + a._e23*b._e12 + a._e31*b._1;
    return r;
}

FORCE_INLINE void       conjugate    (quaternion & q) { q._e12 = -q._e12; q._e23 = -q._e23; q._e31 = -q._e31; }
FORCE_INLINE quaternion conjugated   (quaternion q) { conjugate(q); return q; }
FORCE_INLINE float32    norm_squared (quaternion q) { float32 result = q._1*q._1 + q._e12*q._e12 + q._e23*q._e23 + q._e31*q._e31; return result; }
FORCE_INLINE float32    norm         (quaternion q) { float32 result = square_root(norm_squared(q)); return result; }
FORCE_INLINE void       normalize    (quaternion & q) { float32 n = norm(q); if (!near_zero(n)) { q /= norm(q); } }
FORCE_INLINE quaternion normalized   (quaternion q) { normalize(q); return q; }
FORCE_INLINE quaternion inverse      (quaternion q) { quaternion result = conjugated(q) / norm_squared(q); return result; }

vector3 apply_unit_quaternion(quaternion q, vector3 v)
{
    float32 ww = square(q.w);
    float32 xx = square(q.x);
    float32 yy = square(q.y);
    float32 zz = square(q.z);

    vector3 result;
    result.x = (ww + xx - yy - zz)*v.x + 2.f*(( q.y*v.z - q.z*v.y)*q.w + (q.y*v.y + q.z*v.z)*q.x);
    result.y = (ww - xx + yy - zz)*v.y + 2.f*((-q.x*v.z + q.z*v.x)*q.w + (q.x*v.x + q.z*v.z)*q.y);
    result.z = (ww - xx - yy + zz)*v.z + 2.f*(( q.x*v.y - q.y*v.x)*q.w + (q.x*v.x + q.y*v.y)*q.z);
    return result;
}

vector3 apply_quaternion(quaternion q, vector3 v)
{
    normalize(q);
    vector3 result = apply_unit_quaternion(q, v);
    return result;
}

#endif // BASED__QUATERNION_HPP
