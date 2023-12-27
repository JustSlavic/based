#ifndef BASED__MATRIX4_HPP
#define BASED__MATRIX4_HPP

#include "base.h"
#include "vector4.hpp"
#include "quaternion.hpp"


struct matrix4
{
    union
    {
        struct
        {
            float32 _11, _12, _13, _14;
            float32 _21, _22, _23, _24;
            float32 _31, _32, _33, _34;
            float32 _41, _42, _43, _44;
        };
        struct
        {
            vector4 _1, _2, _3, _4;
        };
        vector4 row[4];
        float32 e[4][4];
    };

    float32 (& operator [] (int32 index)) [4]
    {
        ASSERT(index < 4);
        return e[index];
    }

    FORCE_INLINE static matrix4 identity () { matrix4 m = {}; m._11 = m._22 = m._33 = m._44 = 1.f; return m; }

    FORCE_INLINE static matrix4 scale_x (float32 sx) { matrix4 m = identity(); m._11 = sx; return m; }
    FORCE_INLINE static matrix4 scale_y (float32 sy) { matrix4 m = identity(); m._22 = sy; return m; }
    FORCE_INLINE static matrix4 scale_z (float32 sz) { matrix4 m = identity(); m._33 = sz; return m; }
    FORCE_INLINE static matrix4 scale   (float32 sx, float32 sy, float32 sz) { matrix4 m = identity(); m._11 = sx; m._22 = sy; m._33 = sz; return m; }
    FORCE_INLINE static matrix4 scale   (vector3 s)  { return scale(s.x, s.y, s.z); }
    FORCE_INLINE static matrix4 scale   (float32 s)  { return scale(s, s, s); }

    FORCE_INLINE static matrix4 translate_x (float32 tx) { matrix4 m = identity(); m._14 = tx; return m; }
    FORCE_INLINE static matrix4 translate_y (float32 ty) { matrix4 m = identity(); m._24 = ty; return m; }
    FORCE_INLINE static matrix4 translate_z (float32 tz) { matrix4 m = identity(); m._34 = tz; return m; }
    FORCE_INLINE static matrix4 translate   (float32 tx, float32 ty, float32 tz) { matrix4 m = identity(); m._14 = tx; m._24 = ty; m._34 = tz; return m; }
    FORCE_INLINE static matrix4 translate   (vector3 t)  { return translate(t.x, t.y, t.z); }
    FORCE_INLINE static matrix4 translate   (float32 t)  { return translate(t, t, t); }

    FORCE_INLINE static matrix4 rotate_x (float32 rx) { matrix4 m = identity(); float32 c = cosf(rx); float32 s = sinf(rx); m._22 = c; m._23 = -s; m._32 = s; m._33 = c; return m; }
    FORCE_INLINE static matrix4 rotate_y (float32 ry) { matrix4 m = identity(); float32 c = cosf(ry); float32 s = sinf(ry); m._11 = c; m._13 = -s; m._31 = s; m._33 = c; return m; }
    FORCE_INLINE static matrix4 rotate_z (float32 rz) { matrix4 m = identity(); float32 c = cosf(rz); float32 s = sinf(rz); m._11 = c; m._12 = -s; m._21 = s; m._22 = c; return m; }
};

FORCE_INLINE float32 *get_data(matrix4 & a) { return (float32 *) a.e; }
FORCE_INLINE float32 const *get_data(matrix4 const & a) { return (float32 const *) a.e; }

FORCE_INLINE matrix4 & operator += (matrix4 & a, matrix4 b) { a._11 += b._11; a._12 += b._12; a._13 += b._13; a._14 += b._14; a._21 += b._21; a._22 += b._22; a._23 += b._23; a._24 += b._24; a._31 += b._31; a._32 += b._32; a._33 += b._33; a._34 += b._34; a._41 += b._41; a._42 += b._42; a._43 += b._43; a._44 += b._44; return a; }
FORCE_INLINE matrix4 & operator -= (matrix4 & a, matrix4 b) { a._11 -= b._11; a._12 -= b._12; a._13 -= b._13; a._14 -= b._14; a._21 -= b._21; a._22 -= b._22; a._23 -= b._23; a._24 -= b._24; a._31 -= b._31; a._32 -= b._32; a._33 -= b._33; a._34 -= b._34; a._41 -= b._41; a._42 -= b._42; a._43 -= b._43; a._44 -= b._44; return a; }
FORCE_INLINE matrix4 & operator *= (matrix4 & a, float32 c) { a._11 *= c;     a._12 *= c;     a._13 *= c;     a._14 *= c;     a._21 *= c;     a._22 *= c;     a._23 *= c;     a._24 *= c;     a._31 *= c;     a._32 *= c;     a._33 *= c;     a._34 *= c;     a._41 *= c;     a._42 *= c;     a._43 *= c;     a._44 *= c;     return a; }
FORCE_INLINE matrix4 & operator /= (matrix4 & a, float32 c) { a._11 /= c;     a._12 /= c;     a._13 /= c;     a._14 /= c;     a._21 /= c;     a._22 /= c;     a._23 /= c;     a._24 /= c;     a._31 /= c;     a._32 /= c;     a._33 /= c;     a._34 /= c;     a._41 /= c;     a._42 /= c;     a._43 /= c;     a._44 /= c;     return a; }

INLINE bool operator == (matrix4 a, matrix4 b) { return (a._11 == b._11) && (a._12 == b._12) && (a._13 == b._13) && (a._14 == b._14) && (a._21 == b._21) && (a._22 == b._22) && (a._23 == b._23) && (a._24 == b._24) && (a._31 == b._31) && (a._32 == b._32) && (a._33 == b._33) && (a._34 == b._34) && (a._41 == b._41) && (a._42 == b._42) && (a._43 == b._43) && (a._44 == b._44); }
INLINE bool operator != (matrix4 a, matrix4 b) { bool result = !(a == b); return result; }

FORCE_INLINE matrix4 operator - (matrix4 a) { matrix4 r; r._11 = -a._11; r._12 = -a._12; r._13 = -a._13; r._14 = -a._14; r._21 = -a._21; r._22 = -a._22; r._23 = -a._23; r._24 = -a._24; r._31 = -a._31; r._32 = -a._32; r._33 = -a._33; r._34 = -a._34; r._41 = -a._41; r._42 = -a._42; r._43 = -a._43; r._44 = -a._44; return r; }
FORCE_INLINE matrix4 operator + (matrix4 a, matrix4 b) { matrix4 r; r._11 = a._11 + b._11; r._12 = a._12 + b._12; r._13 = a._13 + b._13; r._14 = a._14 + b._14; r._21 = a._21 + b._21; r._22 = a._22 + b._22; r._23 = a._23 + b._23; r._24 = a._24 + b._24; r._31 = a._31 + b._31; r._32 = a._32 + b._32; r._33 = a._33 + b._33; r._34 = a._34 + b._34; r._41 = a._41 + b._41; r._42 = a._42 + b._42; r._43 = a._43 + b._43; r._44 = a._44 + b._44; return r; }
FORCE_INLINE matrix4 operator - (matrix4 a, matrix4 b) { matrix4 r; r._11 = a._11 - b._11; r._12 = a._12 - b._12; r._13 = a._13 - b._13; r._14 = a._14 - b._14; r._21 = a._21 - b._21; r._22 = a._22 - b._22; r._23 = a._23 - b._23; r._24 = a._24 - b._24; r._31 = a._31 - b._31; r._32 = a._32 - b._32; r._33 = a._33 - b._33; r._34 = a._34 - b._34; r._41 = a._41 - b._41; r._42 = a._42 - b._42; r._43 = a._43 - b._43; r._44 = a._44 - b._44; return r; }
FORCE_INLINE matrix4 operator * (matrix4 a, float32 c) { matrix4 r; r._11 = a._11 * c; r._12 = a._12 * c; r._13 = a._13 * c; r._14 = a._14 * c; r._21 = a._21 * c; r._22 = a._22 * c; r._23 = a._23 * c; r._24 = a._24 * c; r._31 = a._31 * c; r._32 = a._32 * c; r._33 = a._33 * c; r._34 = a._34 * c; r._41 = a._41 * c; r._42 = a._42 * c; r._43 = a._43 * c; r._44 = a._44 * c; return r; }
FORCE_INLINE matrix4 operator * (float32 c, matrix4 a) { matrix4 r; r._11 = c * a._11; r._12 = c * a._12; r._13 = c * a._13; r._14 = c * a._14; r._21 = c * a._21; r._22 = c * a._22; r._23 = c * a._23; r._24 = c * a._24; r._31 = c * a._31; r._32 = c * a._32; r._33 = c * a._33; r._34 = c * a._34; r._41 = c * a._41; r._42 = c * a._42; r._43 = c * a._43; r._44 = c * a._44; return r; }
FORCE_INLINE matrix4 operator / (matrix4 a, float32 c) { matrix4 r; r._11 = a._11 / c; r._12 = a._12 / c; r._13 = a._13 / c; r._14 = a._14 / c; r._21 = a._21 / c; r._22 = a._22 / c; r._23 = a._23 / c; r._24 = a._24 / c; r._31 = a._31 / c; r._32 = a._32 / c; r._33 = a._33 / c; r._34 = a._34 / c; r._41 = a._41 / c; r._42 = a._42 / c; r._43 = a._43 / c; r._44 = a._44 / c; return r; }

FORCE_INLINE vector4 operator * (matrix4 a, vector4 v)
{
    vector4 result;

    result.x = a._11*v._e1 + a._12*v._e2 + a._13*v._e3 + a._14*v._e4;
    result.y = a._21*v._e1 + a._22*v._e2 + a._23*v._e3 + a._24*v._e4;
    result.z = a._31*v._e1 + a._32*v._e2 + a._33*v._e3 + a._34*v._e4;
    result.w = a._41*v._e1 + a._42*v._e2 + a._43*v._e3 + a._44*v._e4;

    return result;
}

FORCE_INLINE vector4 operator * (vector4 v, matrix4 a)
{
    vector4 result;

    result.x = a._11*v._e1 + a._21*v._e2 + a._31*v._e3 + a._41*v._e4;
    result.y = a._12*v._e1 + a._22*v._e2 + a._32*v._e3 + a._42*v._e4;
    result.z = a._13*v._e1 + a._23*v._e2 + a._33*v._e3 + a._43*v._e4;
    result.w = a._14*v._e1 + a._24*v._e2 + a._34*v._e3 + a._44*v._e4;

    return result;
}

FORCE_INLINE matrix4 operator * (matrix4 a, matrix4 b)
{
    matrix4 result;

    result._11 = a._11*b._11 + a._12*b._21 + a._13*b._31 + a._14*b._41;
    result._12 = a._11*b._12 + a._12*b._22 + a._13*b._32 + a._14*b._42;
    result._13 = a._11*b._13 + a._12*b._23 + a._13*b._33 + a._14*b._43;
    result._14 = a._11*b._14 + a._12*b._24 + a._13*b._34 + a._14*b._44;

    result._21 = a._21*b._11 + a._22*b._21 + a._23*b._31 + a._24*b._41;
    result._22 = a._21*b._12 + a._22*b._22 + a._23*b._32 + a._24*b._42;
    result._23 = a._21*b._13 + a._22*b._23 + a._23*b._33 + a._24*b._43;
    result._24 = a._21*b._14 + a._22*b._24 + a._23*b._34 + a._24*b._44;

    result._31 = a._31*b._11 + a._32*b._21 + a._33*b._31 + a._34*b._41;
    result._32 = a._31*b._12 + a._32*b._22 + a._33*b._32 + a._34*b._42;
    result._33 = a._31*b._13 + a._32*b._23 + a._33*b._33 + a._34*b._43;
    result._34 = a._31*b._14 + a._32*b._24 + a._33*b._34 + a._34*b._44;

    result._41 = a._41*b._11 + a._42*b._21 + a._43*b._31 + a._44*b._41;
    result._42 = a._41*b._12 + a._42*b._22 + a._43*b._32 + a._44*b._42;
    result._43 = a._41*b._13 + a._42*b._23 + a._43*b._33 + a._44*b._43;
    result._44 = a._41*b._14 + a._42*b._24 + a._43*b._34 + a._44*b._44;

    return result;
}

FORCE_INLINE vector4& operator *= (vector4& v, matrix4 const& m)
{
    v = v * m;
    return v;
}

FORCE_INLINE matrix4& operator *= (matrix4& a, matrix4 const & b)
{
    a = a * b;
    return a;
}

FORCE_INLINE void transpose(matrix4 & m)
{
    float32 tmp;
    tmp = m._12; m._12 = m._21; m._21 = tmp;
    tmp = m._13; m._13 = m._31; m._31 = tmp;
    tmp = m._14; m._14 = m._41; m._41 = tmp;
    tmp = m._23; m._23 = m._32; m._32 = tmp;
    tmp = m._24; m._24 = m._42; m._42 = tmp;
    tmp = m._34; m._34 = m._43; m._43 = tmp;
}

FORCE_INLINE matrix4 transposed(matrix4 m)
{
    transpose(m);
    return m;
}

FORCE_INLINE matrix4 to_matrix4(quaternion q)
{
    auto s = norm(q);

    matrix4 result;

    result._11 = 1.f - 2.f * s * (q.j * q.j + q.k * q.k);
    result._12 =       2.f * s * (q.i * q.j - q.k * q.r);
    result._13 =       2.f * s * (q.i * q.k + q.j * q.r);
    result._14 = 0.f;

    result._21 =       2.f * s * (q.i * q.j + q.k * q.r);
    result._22 = 1.f - 2.f * s * (q.i * q.i + q.k * q.k);
    result._23 =       2.f * s * (q.j * q.k - q.i * q.r);
    result._24 = 0.f;

    result._31 =       2.f * s * (q.i * q.k - q.j * q.r);
    result._32 =       2.f * s * (q.j * q.k + q.i * q.r);
    result._33 = 1.f - 2.f * s * (q.i * q.i + q.j * q.j);
    result._34 = 0.f;

    result._41 = 0.f;
    result._42 = 0.f;
    result._43 = 0.f;
    result._44 = 1.f;

    return result;
}


#endif // BASED__MATRIX4_HPP
