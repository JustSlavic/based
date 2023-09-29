#ifndef BASED__G3_HPP
#define BASED__G3_HPP

#include "base.h"
#include "g2.hpp"


struct vector3
{
    union
    {
        struct { float32 x, y, z; };
        struct { float32 r, g, b; };
        struct { float32 _1, _2, _3; };
        float32 e[3];

        struct { vector2 xy; };
    };
};

#define V3_1(xyz) vector3{ (float32) (xyz), (float32) (xyz), (float32) (xyz) }
#define V3_2(xy, z) vector3{ (xy).x, (xy).y, (float32) (z) }
#define V3_3(x, y, z) vector3{ (float32) (x), (float32) (y), (float32) (z) }

#define V3(...) MACRO_EXPAND(MACRO_OVERLOAD_3(__VA_ARGS__, V3_3, V3_2, V3_1)(__VA_ARGS__))

struct quaternion
{
    union
    {
        // This weird order appears because I use order or axis,
        // around which the rotation is performed.
        // z, k -> rotation in xy plane
        // x, i -> rotation in yz plane
        // y, j -> rotation in zx plane
        // Each of those unit elements correspond to the imaginary
        // unit in the plane, they operate upon.
        struct { float32 _0, _23, _31, _12; };
        struct { float32 w, x, y, z; };
        struct { float32 w; vector3 vector; };
        struct { float32 r, i, j, k; };
        float32 e[4];
    };
};

#define YZ quaternion{ 0.f, 1.f, 0.f, 0.f }
#define ZX quaternion{ 0.f, 0.f, 1.f, 0.f }
#define XY quaternion{ 0.f, 0.f, 0.f, 1.f }

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

    static matrix3 identity()
    {
        matrix3 result = {};
        result._11 = 1.f;
        result._22 = 1.f;
        result._33 = 1.f;
        return result;
    }
};

FORCE_INLINE float32 *get_data(vector3 & a) { return &a._1; }
FORCE_INLINE float32 const *get_data(vector3 const & a) { return &a._1; }

FORCE_INLINE bool operator == (vector3 a, vector3 b) { bool result = (a.x == b.x) && (a.y == b.y) && (a.z == b.z); return result; }
FORCE_INLINE bool operator != (vector3 a, vector3 b) { bool result = !(a == b); return result; }

FORCE_INLINE vector3 & operator += (vector3 & a, vector3 b) { a.x += b.x; a.y += b.y; a.z += b.z; return a; }
FORCE_INLINE vector3 & operator -= (vector3 & a, vector3 b) { a.x -= b.x; a.y -= b.y; a.z -= b.z; return a; }
FORCE_INLINE vector3 & operator *= (vector3 & a, float32 c) { a.x *= c;   a.y *= c;   a.z *= c;   return a; }
FORCE_INLINE vector3 & operator /= (vector3 & a, float32 c) { a.x /= c;   a.y /= c;   a.z /= c;   return a; }

FORCE_INLINE vector3 operator - (vector3 a) { vector3 result; result.x = -a.x; result.y = -a.y; result.z = -a.z; return result; }
FORCE_INLINE vector3 operator + (vector3 a, vector3 b) { vector3 result; result.x = a.x + b.x; result.y = a.y + b.y; result.z = a.z + b.z; return result; }
FORCE_INLINE vector3 operator - (vector3 a, vector3 b) { vector3 result; result.x = a.x - b.x; result.y = a.y - b.y; result.z = a.z - b.z; return result; }
FORCE_INLINE vector3 operator * (vector3 a, float32 c) { vector3 result; result.x = a.x * c; result.y = a.y * c; result.z = a.z * c; return result; }
FORCE_INLINE vector3 operator * (float32 c, vector3 a) { vector3 result; result.x = c * a.x; result.y = c * a.y; result.z = c * a.z; return result; }
FORCE_INLINE vector3 operator / (vector3 a, float32 c) { vector3 result; result.x = a.x / c; result.y = a.y / c; result.z = a.z / c; return result; }

FORCE_INLINE float32 inner(vector3 a, vector3 b)
{
    float32 r = a.x * b.x + a.y * b.y + a.z * b.z;
    return r;
}

FORCE_INLINE vector3 cross(vector3 a, vector3 b)
{
    vector3 result;
    result.x = a.y * b.z - a.z * b.y;
    result.y = a.z * b.x - a.x * b.z;
    result.z = a.x * b.y - a.y * b.x;
    return result;
}

FORCE_INLINE float32 length_squared(vector3 a)
{
    float32 result = inner(a, a);
    return result;
}

FORCE_INLINE float32 length(vector3 a)
{
    float32 result = math::square_root(length_squared(a));
    return result;
}

FORCE_INLINE void normalize(vector3 &a)
{
    float32 n = length(a);
    if (n > 0) { a /= n; }
}

FORCE_INLINE vector3 normalized(vector3 a)
{
    normalize(a);
    return a;
}

FORCE_INLINE quaternion operator * (vector3 a, vector3 b)
{
    quaternion r;
    r._0  = (a._1 * b._1) + (a._2 * b._2) + (a._3 * b._3);
    r._12 = (a._1 * b._2) - (a._2 * b._1);
    r._23 = (a._2 * b._3) - (a._3 * b._2);
    r._31 = (a._3 * b._1) - (a._1 * b._3);
    return r;
}

FORCE_INLINE quaternion & operator += (quaternion & a, quaternion b) { a._0 += b._0; a._12 += b._12; a._23 += b._23; a._31 += b._31; return a; }
FORCE_INLINE quaternion & operator -= (quaternion & a, quaternion b) { a._0 -= b._0; a._12 -= b._12; a._23 -= b._23; a._31 -= b._31; return a; }
FORCE_INLINE quaternion & operator *= (quaternion & a, float32 c) { a._0 *= c; a._12 *= c; a._23 *= c; a._31 *= c; return a; }
FORCE_INLINE quaternion & operator /= (quaternion & a, float32 c) { a._0 /= c; a._12 /= c; a._23 /= c; a._31 /= c; return a; }

FORCE_INLINE quaternion operator + (quaternion a, quaternion b) { quaternion r; r._0 = a._0 + b._0; r._12 = a._12 + b._12; r._23 = a._23 + b._23; r._31 = a._31 + b._31; return r; }
FORCE_INLINE quaternion operator - (quaternion a, quaternion b) { quaternion r; r._0 = a._0 - b._0; r._12 = a._12 - b._12; r._23 = a._23 - b._23; r._31 = a._31 - b._31; return r; }
FORCE_INLINE quaternion operator * (quaternion q, float32 c) { quaternion r; r._0 = q._0 * c; r._12 = q._12 * c; r._23 = q._23 * c; r._31 = q._31 * c; return r; }
FORCE_INLINE quaternion operator * (float32 c, quaternion q) { quaternion r; r._0 = c * q._0; r._12 = c * q._12; r._23 = c * q._23; r._31 = c * q._31; return r; }
FORCE_INLINE quaternion operator / (quaternion q, float32 c) { quaternion r; r._0 = q._0 / c; r._12 = q._12 / c; r._23 = q._23 / c; r._31 = q._31 / c; return r; }

FORCE_INLINE quaternion operator * (quaternion a, quaternion b)
{
    quaternion r;
    r._0  = a._0*b._0  - a._12*b._12 - a._23*b._23 - a._31*b._31;
    r._12 = a._0*b._12 + a._12*b._0  - a._23*b._31 + a._31*b._23;
    r._23 = a._0*b._23 + a._12*b._31 + a._23*b._0  - a._31*b._12;
    r._31 = a._0*b._31 - a._12*b._23 + a._23*b._12 + a._31*b._0;
    return r;
}

FORCE_INLINE void conjugate(quaternion & q)
{
    q._12 = -q._12;
    q._23 = -q._23;
    q._31 = -q._31;
}

FORCE_INLINE quaternion conjugated(quaternion q)
{
    conjugate(q);
    return q;
}

FORCE_INLINE float32 squared_norm(quaternion q)
{
    float32 result = q._0*q._0 + q._12*q._12 + q._23*q._23 + q._31*q._31;
    return result;
}

FORCE_INLINE float32 norm(quaternion q)
{
    float32 result = math::square_root(squared_norm(q));
    return result;
}

FORCE_INLINE void normalize(quaternion & q)
{
    float32 n = norm(q);
    if (!math::near_zero(n))
    {
        q /= norm(q);
    }
}

FORCE_INLINE quaternion normalized(quaternion q)
{
    normalize(q);
    return q;
}

quaternion inverse(quaternion q)
{
    quaternion result = conjugated(q) / squared_norm(q);
    return result;
}

vector3 rotate_by_unit_quaternion(quaternion q, vector3 v)
{
    quaternion qv;
    qv.w = 0.f;
    qv.vector = v;
    quaternion r = q * qv * conjugated(q);
    return r.vector;
}

vector3 rotate_by_quaternion(quaternion q, vector3 v)
{
    quaternion qv;
    qv.w = 0.f;
    qv.vector = v;
    normalize(q);
    quaternion r = q * qv * conjugated(q);
    return r.vector;
}

matrix3 to_matrix3(quaternion q)
{
    auto s = length(q);

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

matrix4 to_matrix4(quaternion q)
{
    auto m3 = to_matrix3(q);

    matrix4 result;
    result._1.xyz = m3._1;
    result._14 = 0.f;
    result._2.xyz = m3._2;
    result._24 = 0.f;
    result._3.xyz = m3._3;
    result._34 = 0.f;
    result._4 = V4(0, 0, 0, 1);
    return result;
}

FORCE_INLINE float32 *get_data(matrix3 & a) { return &a._11; }
FORCE_INLINE float32 const *get_data(matrix3 const & a) { return &a._11; }

FORCE_INLINE bool operator == (matrix3 a, matrix3 b) { return (a._11 == b._11) && (a._12 == b._12) && (a._13 == b._13) && (a._21 == b._21) && (a._22 == b._22) && (a._23 == b._23) && (a._31 == b._31) && (a._32 == b._32) && (a._33 == b._33); }
FORCE_INLINE bool operator != (matrix3 a, matrix3 b) { return !(a == b); }

FORCE_INLINE matrix3 & operator += (matrix3 & a, matrix3 b) { a._11 += b._11; a._12 += b._12; a._13 += b._13; a._21 += b._21; a._22 += b._22; a._23 += b._23; a._31 += b._31; a._32 += b._32; a._33 += b._33; return a; }
FORCE_INLINE matrix3 & operator -= (matrix3 & a, matrix3 b) { a._11 -= b._11; a._12 -= b._12; a._13 -= b._13; a._21 -= b._21; a._22 -= b._22; a._23 -= b._23; a._31 -= b._31; a._32 -= b._32; a._33 -= b._33; return a; }
FORCE_INLINE matrix3 & operator *= (matrix3 & a, float32 c) { a._11 *= c;     a._12 *= c;     a._13 *= c;     a._21 *= c;     a._22 *= c;     a._23 *= c;     a._31 *= c;     a._32 *= c;     a._33 *= c;     return a; }

FORCE_INLINE matrix3 operator - (matrix3 a) { matrix3 r; r._11 = -a._11; r._12 = -a._12; r._13 = -a._13; r._21 = -a._21; r._22 = -a._22; r._23 = -a._23; r._31 = -a._31; r._32 = -a._32; r._33 = -a._33; return r; }
FORCE_INLINE matrix3 operator + (matrix3 a, matrix3 b) { matrix3 r; r._11 = a._11 + b._11; r._12 = a._12 + b._12; r._13 = a._13 + b._13; r._21 = a._21 + b._21; r._22 = a._22 + b._22; r._23 = a._23 + b._23; r._31 = a._31 + b._31; r._32 = a._32 + b._32; r._33 = a._33 + b._33; return r; }
FORCE_INLINE matrix3 operator - (matrix3 a, matrix3 b) { matrix3 r; r._11 = a._11 - b._11; r._12 = a._12 - b._12; r._13 = a._13 - b._13; r._21 = a._21 - b._21; r._22 = a._22 - b._22; r._23 = a._23 - b._23; r._31 = a._31 - b._31; r._32 = a._32 - b._32; r._33 = a._33 - b._33; return r; }
FORCE_INLINE matrix3 operator * (matrix3 a, float32 c) { matrix3 r; r._11 = a._11 * c; r._12 = a._12 * c; r._13 = a._13 * c; r._21 = a._21 * c; r._22 = a._22 * c; r._23 = a._23 * c; r._31 = a._31 * c; r._32 = a._32 * c; r._33 = a._33 * c; return r; }
FORCE_INLINE matrix3 operator * (float32 c, matrix3 a) { matrix3 r; r._11 = c * a._11; r._12 = c * a._12; r._13 = c * a._13; r._21 = c * a._21; r._22 = c * a._22; r._23 = c * a._23; r._31 = c * a._31; r._32 = c * a._32; r._33 = c * a._33; return r; }
FORCE_INLINE matrix3 operator / (matrix3 a, float32 c) { matrix3 r; r._11 = a._11 / c; r._12 = a._12 / c; r._13 = a._13 / c; r._21 = a._21 / c; r._22 = a._22 / c; r._23 = a._23 / c; r._31 = a._31 / c; r._32 = a._32 / c; r._33 = a._33 / c; return r; }

FORCE_INLINE vector3 operator * (matrix3 a, vector3 v)
{
    vector3 result;

    result.x = a._11*v._1 + a._12*v._2 + a._13*v._3;
    result.y = a._21*v._1 + a._22*v._2 + a._23*v._3;
    result.z = a._31*v._1 + a._32*v._2 + a._33*v._3;

    return result;
}

FORCE_INLINE vector3 operator * (vector3 v, matrix3 a)
{
    vector3 result;

    result.x = v._1*a._11 + v._2*a._21 + v._3*a._31;
    result.y = v._1*a._12 + v._2*a._22 + v._3*a._32;
    result.z = v._1*a._13 + v._2*a._23 + v._3*a._33;

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
    if (!math::near_zero(det))
    {
        result = (1.0f / det) * adjoint(m);
    }

    return result;
}


#endif // BASED__G3_HPP
