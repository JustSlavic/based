#ifndef BASED__G2_HPP
#define BASED__G2_HPP

#include "base.h"
#include <math/float32.hpp>


struct vector2
{
    union
    {
        struct { float32 x, y; };
        struct { float32 u, v; };
        struct { float32 _1, _2; };
        float32 e[2];
    };

    float32 operator [] (int index)
    {
        ASSERT(index < 2);
        return e[index];
    }
};

#define V2_1(c) vector2{ (float32) (c), (float32) (c) }
#define V2_2(x, y) vector2{ (float32) (x), (float32) (y) }
#define V2(...) MACRO_EXPAND(MACRO_OVERLOAD_2(__VA_ARGS__, V2_2, V2_1)(__VA_ARGS__))

struct complex
{
    union
    {
        struct { float32 re, im; };
        struct { float32 _0, _3; };
        float32 e[2];
    };
};

#define I complex{ 0.f, 1.f };

struct matrix2
{
    union
    {
        struct {
            float32 _11, _12;
            float32 _21, _22;
        };
        vector2 row[2];
        float32 e[2][2];
    };
};

FORCE_INLINE bool operator == (vector2 a, vector2 b) { bool r = (a.x == b.x) && (a.y == b.y); return r; }
FORCE_INLINE bool operator != (vector2 a, vector2 b) { bool r = !(a == b); return r; }

FORCE_INLINE vector2 & operator += (vector2 & a, vector2 b) { a.x += b.x; a.y += b.y; return a; }
FORCE_INLINE vector2 & operator -= (vector2 & a, vector2 b) { a.x -= b.x; a.y -= b.y; return a; }
FORCE_INLINE vector2 & operator *= (vector2 & a, float32 c) { a.x *= c;   a.y *= c;   return a; }
FORCE_INLINE vector2 & operator /= (vector2 & a, float32 c) { a.x /= c;   a.y /= c;   return a; }

FORCE_INLINE vector2 operator - (vector2 a) { vector2 r; r.x = -a.x; r.y = -a.y; return r; }
FORCE_INLINE vector2 operator + (vector2 a, vector2 b) { vector2 r; r.x = a.x + b.x; r.y = a.y + b.y; return r; }
FORCE_INLINE vector2 operator - (vector2 a, vector2 b) { vector2 r; r.x = a.x - b.x; r.y = a.y - b.y; return r; }
FORCE_INLINE vector2 operator * (vector2 a, float32 c) { vector2 r; r.x = a.x * c; r.y = a.y * c; return r; }
FORCE_INLINE vector2 operator * (float32 c, vector2 a) { vector2 r; r.x = c * a.x; r.y = c * a.y; return r; }
FORCE_INLINE vector2 operator / (vector2 a, float32 c) { vector2 r; r.x = a.x / c; r.y = a.y / c; return r; }

FORCE_INLINE float32 inner(vector2 a, vector2 b) { float32 r = (a.x * b.x) + (a.y * b.y); return r; }
FORCE_INLINE complex outer(vector2 a, vector2 b) { complex r; r.re = 0.f; r.im = (a.x * b.y) - (a.y * b.x); return r; }

FORCE_INLINE float32 length_squared(vector2 a) { return inner(a, a); }
FORCE_INLINE float32 length(vector2 a) { return math::square_root(length_squared(a)); }
FORCE_INLINE void normalize(vector2 & a) { float32 n = length(a); if (!near_zero(n)) { a /= n; } }
FORCE_INLINE vector2 normalized(vector2 a) { normalize(a); return a; }

FORCE_INLINE bool operator == (complex a, complex b) { bool r = (a.re == b.re) && (a.im == b.im); return r; }
FORCE_INLINE bool operator != (complex a, complex b) { bool r = !(a == b); return r; }

FORCE_INLINE complex & operator += (complex & a, complex b) { a.re += b.re; a.im += b.im; return a; }
FORCE_INLINE complex & operator -= (complex & a, complex b) { a.re -= b.re; a.im -= b.im; return a; }
FORCE_INLINE complex & operator *= (complex & a, float32 c) { a.re *= c;    a.im *= c;    return a; }
FORCE_INLINE complex & operator *= (complex & a, complex b) { a.re = a.re * b.re - a.im * b.im; a.im = a.re * b.im + a.im * b.re; return a; }

FORCE_INLINE complex operator - (complex a) { complex r; r.re = -a.re; r.im = -a.im; return r; }
FORCE_INLINE complex operator + (complex a, complex b) { complex r; r.re = a.re + b.re; r.im = a.im + b.im; return r; }
FORCE_INLINE complex operator + (complex a, float32 b) { complex r; r.re = a.re + b; r.im = a.im; return r; }
FORCE_INLINE complex operator + (float32 a, complex b) { complex r; r.re = a + b.re; r.im = b.im; return r; }
FORCE_INLINE complex operator - (complex a, complex b) { complex r; r.re = a.re - b.re; r.im = a.im - b.im; return r; }
FORCE_INLINE complex operator - (complex a, float32 b) { complex r; r.re = a.re - b; r.im = a.im; return r; }
FORCE_INLINE complex operator - (float32 a, complex b) { complex r; r.re = a - b.re; r.im = b.im; return r; }
FORCE_INLINE complex operator * (complex a, complex b) { complex r; r.re = a.re * b.re - a.im * b.im; r.im = a.re * b.im + a.im * b.re; return r; }
FORCE_INLINE complex operator * (complex a, float32 c) { complex r; r.re = a.re * c; r.im = a.im * c; return r; }
FORCE_INLINE complex operator * (float32 c, complex a) { complex r; r.re = c * a.re; r.im = c * a.im; return r; }
FORCE_INLINE complex operator / (complex a, float32 c) { complex r; r.re = a.re / c; r.im = a.im / c; return r; }

FORCE_INLINE complex operator * (vector2 a, vector2 b) {
    complex r;
    r.re = (a._1 * b._1) + (a._2 * b._2);
    r.im = (a._1 * b._2) - (a._2 * b._1);
    return r;
}

FORCE_INLINE complex to_complex(vector2 v) { complex r; r.re = v.x; r.im = v.y; return r; }
FORCE_INLINE vector2 to_vector2(complex a) { vector2 r; r.x = a.re; r.y = a.im; return r; }

FORCE_INLINE void conjugate(complex & a) { a.im = -a.im; }
FORCE_INLINE complex conjugated(complex a) { conjugate(a); return a; }

FORCE_INLINE bool operator == (matrix2 a, matrix2 b) { bool result = (a._11 == b._11) && (a._12 == b._12) && (a._21 == b._21) && (a._22 == b._22); return result; }
FORCE_INLINE bool operator != (matrix2 a, matrix2 b) { bool result = !(a == b); return result; }

FORCE_INLINE matrix2 & operator += (matrix2 & a, matrix2 b) { a._11 += b._11; a._12 += b._12; a._21 += b._21; a._22 += b._22; return a; }
FORCE_INLINE matrix2 & operator -= (matrix2 & a, matrix2 b) { a._11 -= b._11; a._12 -= b._12; a._21 -= b._21; a._22 -= b._22; return a; }
FORCE_INLINE matrix2 & operator *= (matrix2 & a, float32 c) { a._11 *= c;     a._12 *= c;     a._21 *= c;     a._22 *= c;     return a; }

FORCE_INLINE matrix2 operator - (matrix2 a) { matrix2 r; r._11 = -a._11; r._12 = -a._12; r._21 = -a._21; r._22 = -a._22; return r; }
FORCE_INLINE matrix2 operator + (matrix2 a, matrix2 b) { matrix2 r; r._11 = a._11 + b._11; r._12 = a._12 + b._12; r._21 = a._21 + b._21; r._22 = a._22 + b._22; return r; }
FORCE_INLINE matrix2 operator - (matrix2 a, matrix2 b) { matrix2 r; r._11 = a._11 - b._11; r._12 = a._12 - b._12; r._21 = a._21 - b._21; r._22 = a._22 - b._22; return r; }
FORCE_INLINE matrix2 operator * (matrix2 a, float32 c) { matrix2 r; r._11 = a._11 * c; r._12 = a._12 * c; r._21 = a._21 * c; r._22 = a._22 * c; return r; }
FORCE_INLINE matrix2 operator * (float32 c, matrix2 a) { matrix2 r; r._11 = a._11 * c; r._12 = a._12 * c; r._21 = a._21 * c; r._22 = a._22 * c; return r; }
FORCE_INLINE matrix2 operator / (matrix2 a, float32 c) { matrix2 r; r._11 = a._11 / c; r._12 = a._12 / c; r._21 = a._21 / c; r._22 = a._22 / c; return r; }

FORCE_INLINE vector2 operator * (matrix2 m, vector2 v)
{
    vector2 result;

    result._1 = m._11 * v._1 + m._12 * v._2;
    result._2 = m._21 * v._1 + m._22 * v._2;

    return result;
}

FORCE_INLINE vector2 operator * (vector2 v, matrix2 m)
{
    vector2 result;

    result._1 = v._1 * m._11 + v._2 * m._21;
    result._2 = v._1 * m._12 + v._2 * m._22;

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

FORCE_INLINE void transpose(matrix2& m)
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


#endif // BASED__G2_HPP
