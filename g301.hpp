/*
                                   G(3, 0, 1) projective geometry

The elements of this geometry are:
    scalar: 1
    vector components: e1, e2, e3, e4
    bivector components: e23, e31, e12, e43, e42, e41
    trivector components: e321, e412, e431, e423
    pseudoscalar: e1234

This geometry is called G301 because there are 3 elements that square to 1: e1, e2, e3
and one element that squares to 0: e4.

The matrix4 struct represents the usual matrix in projective geometry, where
multiplication by vector is expected from the right (vec = mat * vec).

Therefore the usual form of this matrix and vectors are:
    | a  b  c tx |   | x |   | x' + tx |
    | d  e  f ty | * | y | = | y' + ty |
    | g  h  i tz |   | z |   | z' + tz |
    | 0  0  0  1 |   | 1 |   |    1    |
where
    x' = ax + by + cz
    y' = dx + ey + fz
    z' = gx + hy + iz
*/

#ifndef BASED__PROJECTIVE_GEOMETRY_HPP
#define BASED__PROJECTIVE_GEOMETRY_HPP

#include "base.h"
#include "g2.hpp"
#include "g3.hpp"


struct vector4
{
    union
    {
        struct { float32 x, y, z, w; };
        struct { float32 r, g, b, a; };
        struct { float32 _1, _2, _3, _4; };
        struct { vector2 xy, zw; };
        struct { vector2 rg, ba; };
        struct { vector3 xyz; };
        struct { vector3 rgb; };
        struct { vector3 bulk; float32 weight; };
        float32 e[4];
    };
};

#define V4_1(xyzw) vector4{ (float32) (xyzw), (float32) (xyzw), (float32) (xyzw), (float32) (xyzw) }
#define V4_2(xyz, w) vector4{ (xyz).x, (xyz).y, (xyz).z, (float32) (w) }
#define V4_3(xy, z, w) vector4{ (xy).x, (xy).y, (float32) z, (float32) (w) }
#define V4_4(x, y, z, w) vector4{ (float32) (x), (float32) (y), (float32) (z), (float32) (w) }

#define V4(...) MACRO_EXPAND(MACRO_OVERLOAD_4(__VA_ARGS__, V4_4, V4_3, V4_2, V4_1)(__VA_ARGS__))

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
};


FORCE_INLINE float32 *get_data(vector4 & a) { return &a._1; }
FORCE_INLINE float32 const *get_data(vector4 const & a) { return &a._1; }

FORCE_INLINE bool operator == (vector4 a, vector4 b) { bool result = (a.x == b.x) && (a.y == b.y) && (a.z == b.z) && (a.w == b.w); return result; }
FORCE_INLINE bool operator != (vector4 a, vector4 b) { bool result = !(a == b); return result; }

FORCE_INLINE vector4 & operator += (vector4 & a, vector4 b) { a.x += b.x; a.y += b.y; a.z += b.z; a.w += b.w; return a; }
FORCE_INLINE vector4 & operator -= (vector4 & a, vector4 b) { a.x -= b.x; a.y -= b.y; a.z -= b.z; a.w -= b.w; return a; }
FORCE_INLINE vector4 & operator *= (vector4 & a, float32 c) { a.x *= c;   a.y *= c;   a.z *= c;   a.w *= c;   return a; }
FORCE_INLINE vector4 & operator /= (vector4 & a, float32 c) { a.x /= c;   a.y /= c;   a.z /= c;   a.w /= c;   return a; }

FORCE_INLINE vector4 operator - (vector4 a) { vector4 result; result.x = -a.x; result.y = -a.y; result.z = -a.z; result.w = -a.w; return result; }
FORCE_INLINE vector4 operator + (vector4 a, vector4 b) { vector4 result; result.x = a.x + b.x; result.y = a.y + b.y; result.z = a.z + b.z; result.w = a.w + b.w; return result; }
FORCE_INLINE vector4 operator - (vector4 a, vector4 b) { vector4 result; result.x = a.x - b.x; result.y = a.y - b.y; result.z = a.z - b.z; result.w = a.w - b.w; return result; }
FORCE_INLINE vector4 operator * (vector4 a, float32 c) { vector4 result; result.x = a.x * c; result.y = a.y * c; result.z = a.z * c; result.w = a.w * c; return result; }
FORCE_INLINE vector4 operator * (float32 c, vector4 a) { vector4 result; result.x = c * a.x; result.y = c * a.y; result.z = c * a.z; result.w = c * a.w; return result; }
FORCE_INLINE vector4 operator / (vector4 a, float32 c) { vector4 result; result.x = a.x / c; result.y = a.y / c; result.z = a.z / c; result.w = a.w / c; return result; }

FORCE_INLINE float32 inner(vector4 a, vector4 b) { float32 result = (a.x * b.x) + (a.y * b.y) + (a.z * b.z) + (a.w * b.w); return result; }
FORCE_INLINE float32 length_squared(vector4 a) { float32 result = inner(a, a); return result; }
FORCE_INLINE float32 length(vector4 a) { float32 result = math::square_root(length_squared(a)); return result; }
FORCE_INLINE void normalize(vector4 &a) { float32 n = length(a); if (n > 0) { a /= n; } }
FORCE_INLINE vector4 normalized(vector4 a) { normalize(a); return a; }

FORCE_INLINE float32 *get_data(matrix4 & a) { return &a._11; }
FORCE_INLINE float32 const *get_data(matrix4 const & a) { return &a._11; }

INLINE bool operator == (matrix4 a, matrix4 b) { return (a._11 == b._11) && (a._12 == b._12) && (a._13 == b._13) && (a._14 == b._14) && (a._21 == b._21) && (a._22 == b._22) && (a._23 == b._23) && (a._24 == b._24) && (a._31 == b._31) && (a._32 == b._32) && (a._33 == b._33) && (a._34 == b._34) && (a._41 == b._41) && (a._42 == b._42) && (a._43 == b._43) && (a._44 == b._44); }
INLINE bool operator != (matrix4 a, matrix4 b) { bool result = !(a == b); return result; }

FORCE_INLINE matrix4 & operator += (matrix4 & a, matrix4 b) { a._11 += b._11; a._12 += b._12; a._13 += b._13; a._14 += b._14; a._21 += b._21; a._22 += b._22; a._23 += b._23; a._24 += b._24; a._31 += b._31; a._32 += b._32; a._33 += b._33; a._34 += b._34; a._41 += b._41; a._42 += b._42; a._43 += b._43; a._44 += b._44; return a; }
FORCE_INLINE matrix4 & operator -= (matrix4 & a, matrix4 b) { a._11 -= b._11; a._12 -= b._12; a._13 -= b._13; a._14 -= b._14; a._21 -= b._21; a._22 -= b._22; a._23 -= b._23; a._24 -= b._24; a._31 -= b._31; a._32 -= b._32; a._33 -= b._33; a._34 -= b._34; a._41 -= b._41; a._42 -= b._42; a._43 -= b._43; a._44 -= b._44; return a; }
FORCE_INLINE matrix4 & operator *= (matrix4 & a, float32 c) { a._11 *= c;     a._12 *= c;     a._13 *= c;     a._14 *= c;     a._21 *= c;     a._22 *= c;     a._23 *= c;     a._24 *= c;     a._31 *= c;     a._32 *= c;     a._33 *= c;     a._34 *= c;     a._41 *= c;     a._42 *= c;     a._43 *= c;     a._44 *= c;     return a; }

FORCE_INLINE matrix4 operator - (matrix4 a) { matrix4 r; r._11 = -a._11; r._12 = -a._12; r._13 = -a._13; r._14 = -a._14; r._21 = -a._21; r._22 = -a._22; r._23 = -a._23; r._24 = -a._24; r._31 = -a._31; r._32 = -a._32; r._33 = -a._33; r._34 = -a._34; r._41 = -a._41; r._42 = -a._42; r._43 = -a._43; r._44 = -a._44; return r; }
FORCE_INLINE matrix4 operator + (matrix4 a, matrix4 b) { matrix4 r; r._11 = a._11 + b._11; r._12 = a._12 + b._12; r._13 = a._13 + b._13; r._14 = a._14 + b._14; r._21 = a._21 + b._21; r._22 = a._22 + b._22; r._23 = a._23 + b._23; r._24 = a._24 + b._24; r._31 = a._31 + b._31; r._32 = a._32 + b._32; r._33 = a._33 + b._33; r._34 = a._34 + b._34; r._41 = a._41 + b._41; r._42 = a._42 + b._42; r._43 = a._43 + b._43; r._44 = a._44 + b._44; return r; }
FORCE_INLINE matrix4 operator - (matrix4 a, matrix4 b) { matrix4 r; r._11 = a._11 - b._11; r._12 = a._12 - b._12; r._13 = a._13 - b._13; r._14 = a._14 - b._14; r._21 = a._21 - b._21; r._22 = a._22 - b._22; r._23 = a._23 - b._23; r._24 = a._24 - b._24; r._31 = a._31 - b._31; r._32 = a._32 - b._32; r._33 = a._33 - b._33; r._34 = a._34 - b._34; r._41 = a._41 - b._41; r._42 = a._42 - b._42; r._43 = a._43 - b._43; r._44 = a._44 - b._44; return r; }
FORCE_INLINE matrix4 operator * (matrix4 a, float32 c) { matrix4 r; r._11 = a._11 * c; r._12 = a._12 * c; r._13 = a._13 * c; r._14 = a._14 * c; r._21 = a._21 * c; r._22 = a._22 * c; r._23 = a._23 * c; r._24 = a._24 * c; r._31 = a._31 * c; r._32 = a._32 * c; r._33 = a._33 * c; r._34 = a._34 * c; r._41 = a._41 * c; r._42 = a._42 * c; r._43 = a._43 * c; r._44 = a._44 * c; return r; }
FORCE_INLINE matrix4 operator * (float32 c, matrix4 a) { matrix4 r; r._11 = c * a._11; r._12 = c * a._12; r._13 = c * a._13; r._14 = c * a._14; r._21 = c * a._21; r._22 = c * a._22; r._23 = c * a._23; r._24 = c * a._24; r._31 = c * a._31; r._32 = c * a._32; r._33 = c * a._33; r._34 = c * a._34; r._41 = c * a._41; r._42 = c * a._42; r._43 = c * a._43; r._44 = c * a._44; return r; }
FORCE_INLINE matrix4 operator / (matrix4 a, float32 c) { matrix4 r; r._11 = a._11 / c; r._12 = a._12 / c; r._13 = a._13 / c; r._14 = a._14 / c; r._21 = a._21 / c; r._22 = a._22 / c; r._23 = a._23 / c; r._24 = a._24 / c; r._31 = a._31 / c; r._32 = a._32 / c; r._33 = a._33 / c; r._34 = a._34 / c; r._41 = a._41 / c; r._42 = a._42 / c; r._43 = a._43 / c; r._44 = a._44 / c; return r; }

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

FORCE_INLINE vector4 operator * (matrix4 a, vector4 v)
{
    vector4 result;

    result.x = a._11*v._1 + a._12*v._2 + a._13*v._3 + a._14*v._4;
    result.y = a._21*v._1 + a._22*v._2 + a._23*v._3 + a._24*v._4;
    result.z = a._31*v._1 + a._32*v._2 + a._33*v._3 + a._34*v._4;
    result.w = a._41*v._1 + a._42*v._2 + a._43*v._3 + a._44*v._4;

    return result;
}

FORCE_INLINE vector4 operator * (vector4 v, matrix4 a)
{
    vector4 result;

    result.x = a._11*v._1 + a._21*v._2 + a._31*v._3 + a._41*v._4;
    result.y = a._12*v._1 + a._22*v._2 + a._32*v._3 + a._42*v._4;
    result.z = a._13*v._1 + a._23*v._2 + a._33*v._3 + a._43*v._4;
    result.w = a._14*v._1 + a._24*v._2 + a._34*v._3 + a._44*v._4;

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

FORCE_INLINE matrix4 matrix4__identity()
{
    matrix4 m = {};
    m._11 = m._22 = m._33 = m._44 = 1.f;
    return m;
}
FORCE_INLINE matrix4 matrix4__scale_x(float32 sx) { matrix4 m = matrix4__identity(); m._11 = sx; return m; }
FORCE_INLINE matrix4 matrix4__scale_y(float32 sy) { matrix4 m = matrix4__identity(); m._22 = sy; return m; }
FORCE_INLINE matrix4 matrix4__scale_z(float32 sz) { matrix4 m = matrix4__identity(); m._33 = sz; return m; }
FORCE_INLINE matrix4 matrix4__scale(float32 sx, float32 sy, float32 sz)
{
    matrix4 m = matrix4__identity();
    m._11 = sx;
    m._22 = sy;
    m._33 = sz;
    return m;
}
FORCE_INLINE matrix4 matrix4__scale(float s)
{
    matrix4 m = matrix4__identity();
    m._11 = m._22 = m._33 = s;
    return m;
}
FORCE_INLINE matrix4 matrix4__scale(vector3 s) { return matrix4__scale(s.x, s.y, s.z); }
FORCE_INLINE matrix4 matrix4__translate_x(float32 tx) { matrix4 m = matrix4__identity(); m._14 = tx; return m; }
FORCE_INLINE matrix4 matrix4__translate_y(float32 ty) { matrix4 m = matrix4__identity(); m._24 = ty; return m; }
FORCE_INLINE matrix4 matrix4__translate_z(float32 tz) { matrix4 m = matrix4__identity(); m._34 = tz; return m; }
FORCE_INLINE matrix4 matrix4__translate(float32 tx, float32 ty, float32 tz)
{
    matrix4 m = matrix4__identity();
    m._14 = tx;
    m._24 = ty;
    m._34 = tz;
    return m;
}
FORCE_INLINE matrix4 matrix4__translate(vector3 t) { return matrix4__translate(t.x, t.y, t.z); }

FORCE_INLINE matrix4 matrix4__rotate_x(float32 rx)
{
    matrix4 m = matrix4__identity();
    float32 c = math::cos(rx);
    float32 s = math::sin(rx);
    m._22 = c; m._23 = -s;
    m._32 = s; m._33 =  c;
    return m;
}
FORCE_INLINE matrix4 matrix4__rotate_y(float32 ry)
{
    matrix4 m = matrix4__identity();
    float32 c = math::cos(ry);
    float32 s = math::sin(ry);
    m._11 = c; m._13 = -s;
    m._31 = s; m._33 =  c;
    return m;
}
FORCE_INLINE matrix4 matrix4__rotate_z(float32 rz)
{
    matrix4 m = matrix4__identity();
    float32 c = math::cos(rz);
    float32 s = math::sin(rz);
    m._11 = c; m._12 = -s;
    m._21 = s; m._22 =  c;
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


// Line containing points P and Q
// line join(point p, point q)
// {
//     line r;
//     r._41 = (q.x * p.w - p.x * q.w);
//     r._42 = (q.y * p.w - p.y * q.w);
//     r._43 = (q.z * p.w - p.z * q.w);
//     r._23 = (p.y * q.z - p.z * q.y);
//     r._31 = (p.z * q.x - p.x * q.z);
//     r._12 = (p.x * q.y - p.y * q.x);
//     return r;
// }

// Plane containing line L and point P
// plane join(line l, point p)
// {
//     plane r;
//     r._423 =  (l.vy * p.z - l.vz * p.y + l.mx * p.w);
//     r._431 =  (l.vz * p.x - l.vx * p.z + l.my * p.w);
//     r._412 =  (l.vx * p.y - l.vy * p.x + l.mz * p.w);
//     r._321 = -(l.mx * p.x + l.my * p.y + l.mz * p.z);
//     return r;
// }

// Line where two planes f and g intersect
// line meet(plane f, plane g)
// {
//     line r;
//     r._41 = (f.z * g.y - f.y * g.z);
//     r._42 = (f.x * g.z - f.z * g.x);
//     r._43 = (f.y * g.x - f.x * g.y);
//     r._23 = (f.x * g.w - g.x * f.w);
//     r._31 = (f.y * g.w - g.y * f.w);
//     r._12 = (f.z * g.w - g.z * f.w);
//     return r;
// }

// Point where line L intersects plane f
// point meet(line l, plane f)
// {
//     point r;
//     r._1 =  (l.my * f.z - l.mz * f.y + l.vx * f.w);
//     r._2 =  (l.mz * f.x - l.mx * f.z + l.vy * f.w);
//     r._3 =  (l.mx * f.y - l.my * f.x + l.vz * f.w);
//     r._4 = -(l.vx * f.x + l.vy * f.y + l.vz * f.z);
//     return r;
// }

// Line perpendicular to a plane f and passing through point P
// line get_line_from_plane_point(plane f, point p)
// {
//     line r;
//     r._41 = -f.x * p.w;
//     r._42 = -f.y * p.w;
//     r._43 = -f.z * p.w;
//     r._23 = (f.y * p.z - f.z * p.y);
//     r._31 = (f.z * p.x - f.x * p.z);
//     r._12 = (f.x * p.y - f.y * p.x);
//     return r;
// }

// Plane perpendicular to line L and containing point P
// plane get_plane_from_line_point(line L, point P)
// {
//     plane r;
//     r._423 = -L.vx * P.w;
//     r._431 = -L.vy * P.w;
//     r._412 = -L.vz * P.w;
//     r._321 = (L.vx * P.x + L.vy * P.y + L.vz * P.z);
//     return r;
// }

// Plane perpendicular to plane f and containing line L
// plane get_plane_from_plane_line(plane f, line L)
// {
//     plane r;
//     r._423 =  (L.vy * f.z - L.vz * f.y);
//     r._431 =  (L.vz * f.x - L.vx * f.z);
//     r._412 =  (L.vx * f.y - L.vy * f.x);
//     r._321 = -(L.mx * f.x + L.my * f.y + L.mz * f.z);
//     return r;
// }

// Application of motors




#endif // BASED__PROJECTIVE_GEOMETRY_HPP
