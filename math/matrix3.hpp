#ifndef BASED__MATRIX3_HPP
#define BASED__MATRIX3_HPP

#include "base.h"
#include "vector3.hpp"


struct matrix3
{
    union
    {
        struct
        {
            float32 _11, _12, _13;
            float32 _21, _22, _23;
            float32 _31, _32, _33;
        };
        struct
        {
            vector3 _1, _2, _3;
        };
        vector3 row[3];
        float32 e[3][3];
    };

    vector3 & operator [] (int32 index)
    {
        ASSERT(index < 3);
        return row[index];
    }

    vector3 const & operator [] (int32 index) const
    {
        ASSERT(index < 3);
        return row[index];
    }

    FORCE_INLINE static matrix3 identity() { matrix3 m = {}; m._11 = m._22 = m._33 = 1.f; return m; }
};

FORCE_INLINE float32 *get_data(matrix3 & a) { return &a._11; }
FORCE_INLINE float32 const *get_data(matrix3 const & a) { return &a._11; }

FORCE_INLINE bool operator == (matrix3 a, matrix3 b) { return (a._11 == b._11) && (a._12 == b._12) && (a._13 == b._13) && (a._21 == b._21) && (a._22 == b._22) && (a._23 == b._23) && (a._31 == b._31) && (a._32 == b._32) && (a._33 == b._33); }
FORCE_INLINE bool operator != (matrix3 a, matrix3 b) { return !(a == b); }

FORCE_INLINE matrix3 & operator += (matrix3 & a, matrix3 b) { a._11 += b._11; a._12 += b._12; a._13 += b._13; a._21 += b._21; a._22 += b._22; a._23 += b._23; a._31 += b._31; a._32 += b._32; a._33 += b._33; return a; }
FORCE_INLINE matrix3 & operator -= (matrix3 & a, matrix3 b) { a._11 -= b._11; a._12 -= b._12; a._13 -= b._13; a._21 -= b._21; a._22 -= b._22; a._23 -= b._23; a._31 -= b._31; a._32 -= b._32; a._33 -= b._33; return a; }
FORCE_INLINE matrix3 & operator *= (matrix3 & a, float32 c) { a._11 *= c;     a._12 *= c;     a._13 *= c;     a._21 *= c;     a._22 *= c;     a._23 *= c;     a._31 *= c;     a._32 *= c;     a._33 *= c;     return a; }
FORCE_INLINE matrix3 & operator /= (matrix3 & a, float32 c) { a._11 /= c;     a._12 /= c;     a._13 /= c;     a._21 /= c;     a._22 /= c;     a._23 /= c;     a._31 /= c;     a._32 /= c;     a._33 /= c;     return a; }

FORCE_INLINE matrix3 operator - (matrix3 a) { matrix3 r; r._11 = -a._11; r._12 = -a._12; r._13 = -a._13; r._21 = -a._21; r._22 = -a._22; r._23 = -a._23; r._31 = -a._31; r._32 = -a._32; r._33 = -a._33; return r; }
FORCE_INLINE matrix3 operator + (matrix3 a, matrix3 b) { matrix3 r; r._11 = a._11 + b._11; r._12 = a._12 + b._12; r._13 = a._13 + b._13; r._21 = a._21 + b._21; r._22 = a._22 + b._22; r._23 = a._23 + b._23; r._31 = a._31 + b._31; r._32 = a._32 + b._32; r._33 = a._33 + b._33; return r; }
FORCE_INLINE matrix3 operator - (matrix3 a, matrix3 b) { matrix3 r; r._11 = a._11 - b._11; r._12 = a._12 - b._12; r._13 = a._13 - b._13; r._21 = a._21 - b._21; r._22 = a._22 - b._22; r._23 = a._23 - b._23; r._31 = a._31 - b._31; r._32 = a._32 - b._32; r._33 = a._33 - b._33; return r; }
FORCE_INLINE matrix3 operator * (matrix3 a, float32 c) { matrix3 r; r._11 = a._11 * c; r._12 = a._12 * c; r._13 = a._13 * c; r._21 = a._21 * c; r._22 = a._22 * c; r._23 = a._23 * c; r._31 = a._31 * c; r._32 = a._32 * c; r._33 = a._33 * c; return r; }
FORCE_INLINE matrix3 operator * (float32 c, matrix3 a) { matrix3 r; r._11 = c * a._11; r._12 = c * a._12; r._13 = c * a._13; r._21 = c * a._21; r._22 = c * a._22; r._23 = c * a._23; r._31 = c * a._31; r._32 = c * a._32; r._33 = c * a._33; return r; }
FORCE_INLINE matrix3 operator / (matrix3 a, float32 c) { matrix3 r; r._11 = a._11 / c; r._12 = a._12 / c; r._13 = a._13 / c; r._21 = a._21 / c; r._22 = a._22 / c; r._23 = a._23 / c; r._31 = a._31 / c; r._32 = a._32 / c; r._33 = a._33 / c; return r; }

FORCE_INLINE vector3 operator * (matrix3 a, vector3 v)
{
    vector3 result;

    result.x = a._11*v._e1 + a._12*v._e2 + a._13*v._e3;
    result.y = a._21*v._e1 + a._22*v._e2 + a._23*v._e3;
    result.z = a._31*v._e1 + a._32*v._e2 + a._33*v._e3;

    return result;
}

FORCE_INLINE vector3 operator * (vector3 v, matrix3 a)
{
    vector3 result;

    result.x = v._e1*a._11 + v._e2*a._21 + v._e3*a._31;
    result.y = v._e1*a._12 + v._e2*a._22 + v._e3*a._32;
    result.z = v._e1*a._13 + v._e2*a._23 + v._e3*a._33;

    return result;
}

FORCE_INLINE matrix3 operator * (matrix3 a, matrix3 b)
{
    matrix3 result;

    result._11 = a._11*b._11 + a._12*b._21 + a._13*b._31;
    result._12 = a._11*b._12 + a._12*b._22 + a._13*b._32;
    result._13 = a._11*b._13 + a._12*b._23 + a._13*b._33;

    result._21 = a._21*b._11 + a._22*b._21 + a._23*b._31;
    result._22 = a._21*b._12 + a._22*b._22 + a._23*b._32;
    result._23 = a._21*b._13 + a._22*b._23 + a._23*b._33;

    result._31 = a._31*b._11 + a._32*b._21 + a._33*b._31;
    result._32 = a._31*b._12 + a._32*b._22 + a._33*b._32;
    result._33 = a._31*b._13 + a._32*b._23 + a._33*b._33;

    return result;
}

FORCE_INLINE vector3 & operator *= (vector3 & v, matrix3 const & m)
{
    v = v * m;
    return v;
}

FORCE_INLINE matrix3 & operator *= (matrix3 & a, matrix3 const & b)
{
    a = a * b;
    return a;
}

FORCE_INLINE void transpose(matrix3 & m)
{
    float32 tmp;
    tmp = m._12; m._12 = m._21; m._21 = tmp;
    tmp = m._13; m._13 = m._31; m._31 = tmp;
    tmp = m._23; m._23 = m._32; m._32 = tmp;
}

FORCE_INLINE matrix3 transposed(matrix3 m)
{
    transpose(m);
    return m;
}

FORCE_INLINE float32 determinant(matrix3 const & m)
{
    float32 result = m._11 * (m._22 * m._33 - m._23 * m._32)
                   - m._12 * (m._21 * m._33 - m._23 * m._31)
                   + m._13 * (m._21 * m._32 - m._22 * m._31);
    return result;
}

FORCE_INLINE matrix3 adjoint(matrix3 const & m)
{
    matrix3 result;

    result._11 =  m._22 * m._33 - m._23 * m._32;
    result._12 = -m._12 * m._33 + m._13 * m._32;
    result._13 =  m._12 * m._23 - m._13 * m._22;

    result._21 = -m._21 * m._33 + m._23 * m._31;
    result._22 =  m._11 * m._33 - m._13 * m._31;
    result._23 = -m._11 * m._23 + m._13 * m._21;

    result._31 =  m._21 * m._32 - m._22 * m._31;
    result._32 = -m._11 * m._32 + m._12 * m._31;
    result._33 =  m._11 * m._22 - m._12 * m._21;

    return result;
}

FORCE_INLINE matrix3 inverse(matrix3 const & m)
{
    matrix3 result = {};

    float32 det = determinant(m);
    if (!near_zero(det))
    {
        result = (1.0f / det) * adjoint(m);
    }

    return result;
}


FORCE_INLINE matrix3 to_matrix3(quaternion q)
{
    auto s = norm(q);

    matrix3 result;

    result._11 = 1.f - 2.f * s * (q.j * q.j + q.k * q.k);
    result._12 =       2.f * s * (q.i * q.j - q.k * q.r);
    result._13 =       2.f * s * (q.i * q.k + q.j * q.r);

    result._21 =       2.f * s * (q.i * q.j + q.k * q.r);
    result._22 = 1.f - 2.f * s * (q.i * q.i + q.k * q.k);
    result._23 =       2.f * s * (q.j * q.k - q.i * q.r);

    result._31 =       2.f * s * (q.i * q.k - q.j * q.r);
    result._32 =       2.f * s * (q.j * q.k + q.i * q.r);
    result._33 = 1.f - 2.f * s * (q.i * q.i + q.j * q.j);

    return result;
}



#endif // BASED__MATRIX3_HPP
