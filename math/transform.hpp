#ifndef BASED__TRANSFORM_HPP
#define BASED__TRANSFORM_HPP

#include "base.h"
#include "vector3.hpp"
#include "matrix3.hpp"


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
    };

    FORCE_INLINE static transform identity () { transform tm = {}; tm._11 = tm._22 = tm._33 = 1.f; return tm; }

    FORCE_INLINE static transform scale_x (float32 sx) { transform tm = identity(); tm._11 = sx; return tm; }
    FORCE_INLINE static transform scale_y (float32 sy) { transform tm = identity(); tm._22 = sy; return tm; }
    FORCE_INLINE static transform scale_z (float32 sz) { transform tm = identity(); tm._33 = sz; return tm; }
    FORCE_INLINE static transform scale   (float32 sx, float32 sy, float32 sz) { transform tm = identity(); tm._11 = sx; tm._22 = sy; tm._33 = sz; return tm; }
    FORCE_INLINE static transform scale   (vector3 s)  { return scale(s.x, s.y, s.z); }
    FORCE_INLINE static transform scale   (float32 s)  { return scale(s, s, s); }

    FORCE_INLINE static transform translate_x (float32 tx) { transform tm = identity(); tm._41 = tx; return tm; }
    FORCE_INLINE static transform translate_y (float32 ty) { transform tm = identity(); tm._42 = ty; return tm; }
    FORCE_INLINE static transform translate_z (float32 tz) { transform tm = identity(); tm._43 = tz; return tm; }
    FORCE_INLINE static transform translate   (float32 tx, float32 ty, float32 tz) { transform tm = identity(); tm._41 = tx; tm._42 = ty; tm._43 = tz; return tm; }
    FORCE_INLINE static transform translate   (vector3 t)  { return translate(t.x, t.y, t.z); }
    FORCE_INLINE static transform translate   (float32 t)  { return translate(t, t, t); }

    FORCE_INLINE static transform rotate_x (float32 rx) { transform tm = identity(); auto c = cosf(rx); auto s = sinf(rx); tm._22 = tm._33 = c; tm._23 = -(tm._32 = s); return tm; }
    FORCE_INLINE static transform rotate_y (float32 ry) { transform tm = identity(); auto c = cosf(ry); auto s = sinf(ry); tm._11 = tm._33 = c; tm._13 = -(tm._31 = s); return tm; }
    FORCE_INLINE static transform rotate_z (float32 rz) { transform tm = identity(); auto c = cosf(rz); auto s = sinf(rz); tm._11 = tm._22 = c; tm._12 = -(tm._21 = s); return tm; }
};

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

FORCE_INLINE matrix4 to_matrix4(transform tm)
{
    matrix4 result;
    result._1 = V4(tm._1, 0);
    result._2 = V4(tm._2, 0);
    result._3 = V4(tm._3, 0);
    result._4 = V4(tm._4, 1);
    transpose(result);
    return result;
}

FORCE_INLINE vector3 transform_point(transform tm, vector3 v)
{
    vector3 result;
    result.x = tm._11*v.x + tm._21*v.y + tm._31*v.z + tm._41;
    result.y = tm._12*v.x + tm._22*v.y + tm._32*v.z + tm._42;
    result.z = tm._13*v.x + tm._23*v.y + tm._33*v.z + tm._43;
    return result;
}

FORCE_INLINE vector4 transform_point(transform tm, vector4 v)
{
    vector4 result;
    result.x = tm._11*v.x + tm._21*v.y + tm._31*v.z + tm._41*v.w;
    result.y = tm._12*v.x + tm._22*v.y + tm._32*v.z + tm._42*v.w;
    result.z = tm._13*v.x + tm._23*v.y + tm._33*v.z + tm._43*v.w;
    result.w = 1.f;
    return result;
}

FORCE_INLINE vector3 transform_vector(transform tm, vector3 v)
{

    vector3 result;
    result.x = tm._11*v.x + tm._21*v.y + tm._31*v.z;
    result.y = tm._12*v.x + tm._22*v.y + tm._32*v.z;
    result.z = tm._13*v.x + tm._23*v.y + tm._33*v.z;
    return result;
}

FORCE_INLINE vector4 transform_vector(transform tm, vector4 v)
{
    vector4 result;
    result.x = tm._11*v.x + tm._21*v.y + tm._31*v.z;
    result.y = tm._12*v.x + tm._22*v.y + tm._32*v.z;
    result.z = tm._13*v.x + tm._23*v.y + tm._33*v.z;
    result.w = 0.f;
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


#endif // BASED__TRANSFORM_HPP
