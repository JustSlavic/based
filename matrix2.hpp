#ifndef BASED__MATRIX2_HPP
#define BASED__MATRIX2_HPP

#include "base.h"
#include "vector2.hpp"


struct matrix2
{
    union
    {
        struct
        {
            float32 _11, _12;
            float32 _21, _22;
        };
        vector2 row[2];
        float32 e[2][2];
    };

    vector2 & operator [] (int32 index)
    {
        ASSERT(index < 2);
        return row[index];
    }

    vector2 const & operator [] (int32 index) const
    {
        ASSERT(index < 2);
        return row[index];
    }

    FORCE_INLINE static matrix2 identity() { matrix2 m = {}; m._11 = m._22 = 1.f; return m; }
};


FORCE_INLINE matrix2 & operator += (matrix2 & a, matrix2 b) { a._11 += b._11; a._12 += b._12; a._21 += b._21; a._22 += b._22; return a; }
FORCE_INLINE matrix2 & operator -= (matrix2 & a, matrix2 b) { a._11 -= b._11; a._12 -= b._12; a._21 -= b._21; a._22 -= b._22; return a; }
FORCE_INLINE matrix2 & operator *= (matrix2 & a, float32 c) { a._11 *= c;     a._12 *= c;     a._21 *= c;     a._22 *= c;     return a; }

FORCE_INLINE bool operator == (matrix2 a, matrix2 b) { bool result = (a._11 == b._11) && (a._12 == b._12) && (a._21 == b._21) && (a._22 == b._22); return result; }
FORCE_INLINE bool operator != (matrix2 a, matrix2 b) { bool result = !(a == b); return result; }

FORCE_INLINE matrix2 operator - (matrix2 a) { matrix2 r; r._11 = -a._11; r._12 = -a._12; r._21 = -a._21; r._22 = -a._22; return r; }
FORCE_INLINE matrix2 operator + (matrix2 a, matrix2 b) { matrix2 r; r._11 = a._11 + b._11; r._12 = a._12 + b._12; r._21 = a._21 + b._21; r._22 = a._22 + b._22; return r; }
FORCE_INLINE matrix2 operator - (matrix2 a, matrix2 b) { matrix2 r; r._11 = a._11 - b._11; r._12 = a._12 - b._12; r._21 = a._21 - b._21; r._22 = a._22 - b._22; return r; }
FORCE_INLINE matrix2 operator * (matrix2 a, float32 c) { matrix2 r; r._11 = a._11 * c; r._12 = a._12 * c; r._21 = a._21 * c; r._22 = a._22 * c; return r; }
FORCE_INLINE matrix2 operator * (float32 c, matrix2 a) { matrix2 r; r._11 = a._11 * c; r._12 = a._12 * c; r._21 = a._21 * c; r._22 = a._22 * c; return r; }
FORCE_INLINE matrix2 operator / (matrix2 a, float32 c) { matrix2 r; r._11 = a._11 / c; r._12 = a._12 / c; r._21 = a._21 / c; r._22 = a._22 / c; return r; }

FORCE_INLINE vector2 operator * (matrix2 m, vector2 v)
{
    vector2 result;
    result._e1 = m._11 * v._e1 + m._12 * v._e2;
    result._e2 = m._21 * v._e1 + m._22 * v._e2;
    return result;
}

FORCE_INLINE vector2 operator * (vector2 v, matrix2 m)
{
    vector2 result;
    result._e1 = v._e1 * m._11 + v._e2 * m._21;
    result._e2 = v._e1 * m._12 + v._e2 * m._22;
    return result;
}

FORCE_INLINE matrix2 operator * (matrix2 a, matrix2 b)
{
    matrix2 result;
    result._11 = a._11 * b._11 + a._12 * b._21;
    result._12 = a._11 * b._12 + a._12 * b._22;
    result._21 = a._21 * b._11 + a._22 * b._21;
    result._22 = a._21 * b._12 + a._22 * b._22;
    return result;
}

FORCE_INLINE vector2 & operator *= (vector2 & v, matrix2 const& m)
{
    v = v * m;
    return v;
}

FORCE_INLINE matrix2 & operator *= (matrix2 & a, matrix2 const & b)
{
    a = a * b;
    return a;
}

FORCE_INLINE void transpose(matrix2 & m)
{
    float32 tmp;
    tmp = m._12; m._12 = m._21; m._21 = tmp;
}

FORCE_INLINE matrix2 transposed(matrix2 m)
{
    transpose(m);
    return m;
}

FORCE_INLINE float32 determinant(matrix2 const & m)
{
    float32 result = m._11 * m._22 - m._12 * m._21;
    return result;
}

FORCE_INLINE matrix2 adjoint(matrix2 const & m)
{
    matrix2 result;
    result._11 =  m._22;
    result._12 = -m._12;
    result._21 = -m._21;
    result._22 =  m._11;
    return result;
}

FORCE_INLINE matrix2 inverse(matrix2 const & m)
{
    matrix2 result = {};

    float32 det = determinant(m);
    if (!near_zero(det))
    {
        result = (1.0f / det) * adjoint(m);
    }

    return result;
}


#endif // BASED__MATRIX2_HPP
