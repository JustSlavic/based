#ifndef BASED__QUATERNION_HPP
#define BASED__QUATERNION_HPP

#include "base.h"
#include "vector3.hpp"


struct quaternion
{
    /*
        Hamilton multiplication table
          |  i  j  k
        --+--------
        i | -1  k -j
        j | -k -1  i
        k |  j -i -1

        if we define the elements as
            e32 = i
            e13 = j
            e21 = k
        then,
            i^2 = j^2 = k^2 = -1
            ij = e32 e13 = e21 = k
            ik = e32 e21 = e31 = -j
            ji = ... = -k
            jk = e13 e21 = e32 = i
            ki = ... = j
            kj = ... = -i
        we will have the Hamilton multiplication
        rules correct.
    */
    union
    {
        struct { float32 _1, _e32, _e13, _e21; };
        struct { float32 w, x, y, z; };
        struct { float32 r, i, j, k; };
        struct { float32 pad1; vector3 axis; };
        float32 e[4];
    };

    FORCE_INLINE static quaternion identity() { quaternion result = {}; result.r = 1.0f; return result; }
    FORCE_INLINE static quaternion rotate_x(float32 rx) { quaternion result = {}; result._1 = cosf(0.5f * rx); result._e32 = sinf(0.5f * rx); return result; }
    FORCE_INLINE static quaternion rotate_y(float32 ry) { quaternion result = {}; result._1 = cosf(0.5f * ry); result._e13 = sinf(0.5f * ry); return result; }
    FORCE_INLINE static quaternion rotate_z(float32 rz) { quaternion result = {}; result._1 = cosf(0.5f * rz); result._e21 = sinf(0.5f * rz); return result; }
    FORCE_INLINE static quaternion rotate(float32 a, vector3 axis)
    {
        quaternion result = {};
        result._1 = cosf(0.5f * a);
        result.axis = sinf(0.5f * a) * axis;
        return result;
    }
};

FORCE_INLINE quaternion & operator += (quaternion & a, quaternion b) { a._1 += b._1; a._e21 += b._e21; a._e32 += b._e32; a._e13 += b._e13; return a; }
FORCE_INLINE quaternion & operator -= (quaternion & a, quaternion b) { a._1 -= b._1; a._e21 -= b._e21; a._e32 -= b._e32; a._e13 -= b._e13; return a; }
FORCE_INLINE quaternion & operator *= (quaternion & a, float32 c) { a._1 *= c; a._e21 *= c; a._e32 *= c; a._e13 *= c; return a; }
FORCE_INLINE quaternion & operator /= (quaternion & a, float32 c) { a._1 /= c; a._e21 /= c; a._e32 /= c; a._e13 /= c; return a; }

FORCE_INLINE bool operator == (quaternion const & a, quaternion const & b) { bool result = (a._1 == b._1) && (a._e32 == b._e32) && (a._e13 == b._e13) && (a._e21 == b._e21); return result; }
FORCE_INLINE bool operator != (quaternion const & a, quaternion const & b) { return !(a == b); }

FORCE_INLINE quaternion operator + (quaternion a, quaternion b) { quaternion result; result._1 = a._1 + b._1; result._e32 = a._e32 + b._e32; result._e13 = a._e13 + b._e13; result._e21 = a._e21 + b._e21; return result; }
FORCE_INLINE quaternion operator - (quaternion a, quaternion b) { quaternion result; result._1 = a._1 - b._1; result._e32 = a._e32 - b._e32; result._e13 = a._e13 - b._e13; result._e21 = a._e21 - b._e21; return result; }
FORCE_INLINE quaternion operator * (quaternion q, float32 c) { quaternion result; result._1 = q._1 * c; result._e32 = q._e32 * c; result._e13 = q._e13 * c; result._e21 = q._e21 * c; return result; }
FORCE_INLINE quaternion operator * (float32 c, quaternion q) { quaternion result; result._1 = c * q._1; result._e32 = c * q._e32; result._e13 = c * q._e13; result._e21 = c * q._e21; return result; }
FORCE_INLINE quaternion operator / (quaternion q, float32 c) { quaternion result; result._1 = q._1 / c; result._e32 = q._e32 / c; result._e13 = q._e13 / c; result._e21 = q._e21 / c; return result; }

FORCE_INLINE quaternion operator * (vector3 a, vector3 b)
{
    quaternion result;
    result._1   = (a._e1 * b._e1) + (a._e2 * b._e2) + (a._e3 * b._e3);
    result._e32 = (a._e3 * b._e2) - (a._e2 * b._e3);
    result._e13 = (a._e1 * b._e3) - (a._e3 * b._e1);
    result._e21 = (a._e2 * b._e1) - (a._e1 * b._e2);
    return result;
}

FORCE_INLINE quaternion operator * (quaternion a, quaternion b)
{
    quaternion r;
    r._1   = a._1*b._1   - a._e32*b._e32 - a._e13*b._e13 - a._e21*b._e21;
    r._e32 = a._1*b._e32 - a._e21*b._e13 + a._e32*b._1   + a._e13*b._e21;
    r._e13 = a._1*b._e13 + a._e21*b._e32 - a._e32*b._e21 + a._e13*b._1;
    r._e21 = a._1*b._e21 + a._e21*b._1   + a._e32*b._e13 - a._e13*b._e32;
    return r;
}

FORCE_INLINE quaternion & operator *= (quaternion & a, quaternion b)
{
    a = a * b;
    return a;
}

FORCE_INLINE void       conjugate    (quaternion & q) { q._e32 = -q._e32; q._e13 = -q._e13; q._e21 = -q._e21; }
FORCE_INLINE quaternion conjugated   (quaternion q) { conjugate(q); return q; }
FORCE_INLINE float32    norm_squared (quaternion q) { float32 result = q._1*q._1 + q._e32*q._e32 + q._e13*q._e13 + q._e21*q._e21; return result; }
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
