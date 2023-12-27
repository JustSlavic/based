#ifndef BASED__TRANSFORM_HPP
#define BASED__TRANSFORM_HPP

#include "base.h"


struct transform
{
    union
    {
        struct
        {
            float32 _11, _12, _13;
            float32 _21, _22, _23;
            float32 _31, _32, _33;
            float32 _41, _42, _43;
        };
        struct
        {
            matrix3 matrix;
            vector3 displacement;
        };
        struct
        {
            vector3 sx; // Vector Ox in the parent (old, previous) coordinated
            vector3 sy; // Vector Oy
            vector3 sz; // Vector Oz
            vector3 sd; // Displacement
        };
        struct
        {
            vector3 _1, _2, _3, _4;
        };
        float32 e[4][3];
        vector3 s[4];
    };
};

FORCE_INLINE transform transform__identity()
{
    transform tm = {};
    tm._11 = tm._22 = tm._33 = 1.f;
    return tm;
}

float32 determinant(transform tm)
{
    float32 result = determinant(tm.matrix);
    return result;
}

transform inverse(transform tm)
{
    transform result = {};

    float32 det = determinant(tm.matrix);
    if (!near_zero(det))
    {
        result.matrix = (1.0f / det) * adjoint(tm.matrix);

        float32 m_32_43_33_42 = (tm._32 * tm._43 - tm._33 * tm._42);
        float32 m_31_43_33_41 = (tm._31 * tm._43 - tm._33 * tm._41);
        float32 m_31_42_32_41 = (tm._31 * tm._42 - tm._32 * tm._41);
        float32 m_12_23_13_22 = (tm._12 * tm._23 - tm._13 * tm._22);
        float32 m_11_23_13_21 = (tm._11 * tm._23 - tm._13 * tm._21);
        float32 m_11_22_12_21 = (tm._11 * tm._22 - tm._12 * tm._21);

        result._41 = (- tm._21 * m_32_43_33_42 + tm._22 * m_31_43_33_41 - tm._23 * m_31_42_32_41) / det;
        result._42 = (  tm._11 * m_32_43_33_42 - tm._12 * m_31_43_33_41 + tm._13 * m_31_42_32_41) / det;
        result._43 = (- tm._41 * m_12_23_13_22 + tm._42 * m_11_23_13_21 - tm._43 * m_11_22_12_21) / det;
    }
    return result;
}

FORCE_INLINE matrix4 transform__to_matrix4(transform tm)
{
    matrix4 result;
    result._1 = V4(tm._1, 0);
    result._2 = V4(tm._2, 0);
    result._3 = V4(tm._3, 0);
    result._4 = V4(tm._4, 1);
    transpose(result);
    return result;
}

/*
                  | 11 12 13 0 |
    | x y z 1 | * | 21 22 23 0 | = | (11x + 21y + 31z + 41),
                  | 31 32 33 0 |     (12x + 22y + 32z + 42),
                  | 41 42 43 1 |     (13x + 23y + 33z + 43) |
*/
FORCE_INLINE vector3 transform_point(transform tm, vector3 v)
{

    vector3 result;
    result._e1 = tm._11*v._e1 + tm._21*v._e2 + tm._31*v._e3 + tm._41;
    result._e2 = tm._12*v._e1 + tm._22*v._e2 + tm._32*v._e3 + tm._42;
    result._e3 = tm._13*v._e1 + tm._23*v._e2 + tm._33*v._e3 + tm._43;
    return result;
}
FORCE_INLINE vector4 transform_point(transform tm, vector4 v)
{
    vector4 result;
    result._1 = tm._11*v._1 + tm._21*v._2 + tm._31*v._3 + tm._41*v._4;
    result._2 = tm._12*v._1 + tm._22*v._2 + tm._32*v._3 + tm._42*v._4;
    result._3 = tm._13*v._1 + tm._23*v._2 + tm._33*v._3 + tm._43*v._4;
    result._4 = 1.f;
    return result;
}
/*
                  | 11 12 13 0 |
    | x y z 0 | * | 21 22 23 0 | = | (11x + 21y + 31z),
                  | 31 32 33 0 |     (12x + 22y + 32z),
                  | 41 42 43 1 |     (13x + 23y + 33z) |
*/
FORCE_INLINE vector3 transform_vector(transform tm, vector3 v)
{

    vector3 result;
    result._e1 = tm._11*v._e1 + tm._21*v._e2 + tm._31*v._e3;
    result._e2 = tm._12*v._e1 + tm._22*v._e2 + tm._32*v._e3;
    result._e3 = tm._13*v._e1 + tm._23*v._e2 + tm._33*v._e3;
    return result;
}
FORCE_INLINE vector4 transform_vector(transform tm, vector4 v)
{
    vector4 result;
    result._1 = tm._11*v._1 + tm._21*v._2 + tm._31*v._3;
    result._2 = tm._12*v._1 + tm._22*v._2 + tm._32*v._3;
    result._3 = tm._13*v._1 + tm._23*v._2 + tm._33*v._3;
    result._4 = 0.f;
    return result;
}

/*
    F = first
    S = second

    The transform struct is row-major storage for matrix, so
    multiplication performed in left-to-right order.
    We just pretend that operator is (S*F)*vec for consistency
    with the matrix4 interface (which is natural from math class).

    | f11 f12 f13 0 |   | s11 s12 s13 0 |
    | f21 f22 f23 0 | * | s21 s22 s23 0 | =
    | f31 f32 f33 0 |   | s31 s32 s33 0 |
    | f41 f42 f43 1 |   | s41 s42 s43 1 |
*/
FORCE_INLINE transform operator * (transform s, transform f)
{
    transform result;

    result._11 = f._11*s._11 + f._12*s._21 + f._13*s._31;
    result._12 = f._11*s._12 + f._12*s._22 + f._13*s._32;
    result._13 = f._11*s._13 + f._12*s._23 + f._13*s._33;

    result._21 = f._21*s._11 + f._22*s._21 + f._23*s._31;
    result._22 = f._21*s._12 + f._22*s._22 + f._23*s._32;
    result._23 = f._21*s._13 + f._22*s._23 + f._23*s._33;

    result._31 = f._31*s._11 + f._32*s._21 + f._33*s._31;
    result._32 = f._31*s._12 + f._32*s._22 + f._33*s._32;
    result._33 = f._31*s._13 + f._32*s._23 + f._33*s._33;

    result._41 = f._41*s._11 + f._42*s._21 + f._43*s._31 + s._41;
    result._42 = f._41*s._12 + f._42*s._22 + f._43*s._32 + s._42;
    result._43 = f._41*s._13 + f._42*s._23 + f._43*s._33 + s._43;

    return result;
}

FORCE_INLINE transform transform__scale_x(float32 sx)
{
    transform tm = transform__identity();
    tm._11 = sx;
    return tm;
}
FORCE_INLINE transform transform__scale_y(float32 sy)
{
    transform tm = transform__identity();
    tm._22 = sy;
    return tm;
}
FORCE_INLINE transform transform__scale_z(float32 sz)
{
    transform tm = transform__identity();
    tm._33 = sz;
    return tm;
}
FORCE_INLINE transform transform__scale(vector3 s)
{
    transform tm = transform__identity();
    tm._11 = s.x;
    tm._22 = s.y;
    tm._33 = s.z;
    return tm;
}
FORCE_INLINE transform transform__translate_x(float32 tx)
{
    transform tm = transform__identity();
    tm._41 = tx;
    return tm;
}
FORCE_INLINE transform transform__translate_y(float32 ty)
{
    transform tm = transform__identity();
    tm._42 = ty;
    return tm;
}
FORCE_INLINE transform transform__translate_z(float32 tz)
{
    transform tm = transform__identity();
    tm._43 = tz;
    return tm;
}
FORCE_INLINE transform transform__translate(vector3 t)
{
    transform tm = transform__identity();
    tm._41 = t.x;
    tm._42 = t.y;
    tm._43 = t.z;
    return tm;
}
FORCE_INLINE transform transform__rotate_x(float32 rx)
{
    //  1  0  0  0
    //  0  c -s  0
    //  0  s  c  0
    //  0  0  0  1
    transform tm = transform__identity();
    auto c = math::cos(rx);
    auto s = math::sin(rx);
    tm._22 = c; tm._23 = -s;
    tm._32 = s; tm._33 =  c;
    return tm;
}
FORCE_INLINE transform transform__rotate_y(float32 ry)
{
    //  c  0 -s  0
    //  0  1  0  0
    //  s  0  c  0
    //  0  0  0  1
    transform tm = transform__identity();
    auto c = math::cos(ry);
    auto s = math::sin(ry);
    tm._11 = c; tm._13 = -s;
    tm._31 = s; tm._33 =  c;
    return tm;
}
FORCE_INLINE transform transform__rotate_z(float32 rz)
{
    //  c -s  0  0
    //  s  c  0  0
    //  0  0  1  0
    //  0  0  0  1
    transform tm = transform__identity();
    auto c = math::cos(rz);
    auto s = math::sin(rz);
    tm._11 = c; tm._12 = -s;
    tm._21 = s; tm._22 =  c;
    return tm;
}
// FORCE_INLINE transform transform__rotate(vector3 r)
// {
//     transform tm = transform__identity();

//     return tm;
// }


#endif // BASED__TRANSFORM_HPP
