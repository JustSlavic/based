#ifndef BASED__PROJECTIVE_GEOMETRY3_HPP
#define BASED__PROJECTIVE_GEOMETRY3_HPP

#include "base.h"
#include "vector3.hpp"


struct plane3
{
    // Ax + By + Cz + D = 0
    // Ae1 + Be2 + Ce3 + De0
    union
    {
        struct { float32 a, b, c, d; };
        struct { float32 _e1, _e2, _e3, _e0; };
        struct { vector3 normal; float32 w; };
    };
};

struct line3
{
    union
    {
        struct { float32 _e23, _e31, _e12, _e01, _e02, _e03; };
        struct { vector3 direction; vector3 moment; };
    };
};

struct point3
{
    union
    {
        struct { float32 _e203, _e301, _e102, _e123; };
        struct { float32 x, y, z, w; };
        struct { vector3 vector; float32 pad1; };
    };
};


plane3 make_plane3(float32 a, float32 b, float32 c, float32 d)
{
    plane3 result;
    result._e1 = a;
    result._e2 = b;
    result._e3 = c;
    result._e0 = d;
    return result;
}

line3 make_line3(vector3 direction, vector3 point_on_line)
{
    line3 result;
    result.direction = direction;
    result.moment = cross(point_on_line, point_on_line + direction);
    return result;
}

point3 make_point3(float32 x, float32 y, float32 z)
{
    point3 result;
    result.x = x;
    result.y = y;
    result.z = z;
    result.w = 1.f;
    return result;
}

line3 outer(plane3 p1, plane3 p2)
{
    // (a1e1 + b1e2 + c1e3 + d1e0) /\ (a2e1 + b2e2 + c2e3 + d2e0)
    // (a1b2 - b1a2) e12 +
    // (b1c2 - c1b2) e23 +
    // (c1a2 - a1c2) e31 +
    // (d1a2 - a1d2) e01 +
    // (d1b2 - b1d2) e02 +
    // (d1c2 - c1d2) e03

    line3 result;
    result._e23 = p1.b * p2.c - p1.c * p2.b;
    result._e31 = p1.c * p2.a - p1.a * p2.c;
    result._e12 = p1.a * p2.b - p1.b * p2.a;
    result._e01 = p1.d * p2.a - p1.a * p2.d;
    result._e02 = p1.d * p2.b - p1.b * p2.d;
    result._e03 = p1.d * p2.c - p1.c * p2.d;
    return result;
}

point3 outer(plane3 p, line3 l)
{
    // (Ae1 + Be2 + Ce3 + De0) /\ (d1e23 + d2e31 + d3e12 + m1e01 + m2e02 + m3e03)
    // (Ad1 + Bd2 + Cd3) e123
    // (Dd1 + Cm2 - Bm3) e230
    // (Dd2 + Am3 - Cm1) e310
    // (Dd3 + Bm1 - Am2) e120

    point3 result;
    result._e203 = p._e2 * l._e03 - p._e0 * l._e23 - p._e3 * l._e02;
    result._e301 = p._e3 * l._e01 - p._e0 * l._e31 - p._e1 * l._e03;
    result._e102 = p._e1 * l._e02 - p._e0 * l._e12 - p._e2 * l._e01;
    result._e123 = p._e1 * l._e23 + p._e2 * l._e31 + p._e3 * l._e12;
    return result;
}

point3 outer(line3 l, plane3 p)
{
    point3 result = outer(p, l);
    return result;
}

float32 inner(plane3 p1, plane3 p2)
{
    // (A1e1 + B1e2 + C1e3 + D1e0) * (A2e1 + B2e2 + B2e3 + D2e0)
    float32 result = p1.a * p2.a + p1.b * p2.b + p1.c * p2.c;
    return result;
}

float32 inner(line3 l1, line3 l2)
{
    float32 result = 0.f;
    return result;
}

float32 inner(point3 pt1, point3 pt2)
{
    float32 result = 0.f;
    return result;
}

plane3 inner(plane3 p, line3 l)
{
    // (Ae1 + Be2 + Ce3 + De0) * (d1e23 + d2e31 + d3e12 + m1e01 + m2e02 + m3e03)
    //            + Bd1 e3   - Cd1 e2
    // - Ad2 e3              + Cd2 e1
    //   Ad3 e2   - Bd3 e1
    // - Am1 e0
    //            - Bm2 e0
    //                       - Cm3 e0
    plane3 result;
    result._e1 = p._e3 * l._e31 - p._e2 * l._e12;
    result._e2 = p._e1 * l._e12 - p._e3 * l._e23;
    result._e3 = p._e2 * l._e23 - p._e1 * l._e31;
    result._e0 = - p._e1 * l._e01 - p._e2 * l._e02 - p._e3 * l._e03;
    return result;
}

plane3 inner(line3 l, plane3 p)
{
    // (d1e23 + d2e31 + d3e12 + m1e01 + m2e02 + m3e03) * (Ae1 + Be2 + Ce3 + De0)
    //              d2A e3   - d3A e2   + m1A e0
    // - d1B e3              + d3B e1              + m2B e0
    //   d1C e2   - d2C e1                                    + m3C e0
    plane3 result = inner(p, l);
    result._e1 = -result._e1;
    result._e2 = -result._e2;
    result._e3 = -result._e3;
    result._e0 = -result._e0;
    return result;
}

plane3 inner(line3 l, point3 pt)
{
    // (d1e23 + d2e31 + d3e12 + m1e01 + m2e02 + m3e03) * (x e203 + y e301 + z e102 + w e123)
    //  + d1x e0
    //           + d2y e0
    //                    + d3z e0
    //  - d1w e1 - d2w e2 - d3w e3
    plane3 result;
    result._e1 = - l._e23 * pt._e123;
    result._e2 = - l._e31 * pt._e123;
    result._e3 = - l._e12 * pt._e123;
    result._e0 = l._e23 * pt._e203 + l._e31 * pt._e301 + l._e12 * pt._e102;
    return result;
}

plane3 inner(point3 pt, line3 l)
{
    plane3 result = inner(l, pt);
    return result;
}

line3 inner(plane3 p, point3 pt)
{
    // (Ae1 + Be2 + Ce3 + De0) * (x e203 + y e301 + z e102 + w e123)
    //            + Bx e03   - Cx e02
    // - Ay e03              + Cy e01
    //   Az e02   - Bz e01
    //   Aw e23   + Bw e31   + Cw e12
    line3 result;
    result._e01 = p._e3 * pt._e301 - p._e2 * pt._e102;
    result._e02 = p._e1 * pt._e102 - p._e3 * pt._e203;
    result._e03 = p._e2 * pt._e203 - p._e1 * pt._e301;
    result._e23 = p._e1 * pt._e123;
    result._e31 = p._e2 * pt._e123;
    result._e12 = p._e3 * pt._e123;
    return result;
}

line3 inner(point3 pt, plane3 p)
{
    line3 result = inner(p, pt);
    return result;
}

point3 dual(plane3 p)
{
    point3 result;
    result._e203 = p._e1;
    result._e301 = p._e2;
    result._e102 = p._e3;
    result._e123 = p._e0;
    return result;
}

line3 dual(line3 l)
{
    line3 result;
    result._e01 = l._e23;
    result._e02 = l._e31;
    result._e03 = l._e12;
    result._e23 = l._e01;
    result._e31 = l._e02;
    result._e12 = l._e03;
    return result;
}

plane3 dual(point3 p)
{
    plane3 result;
    result._e1 = p._e203;
    result._e2 = p._e301;
    result._e3 = p._e102;
    result._e0 = p._e123;
    return result;
}

line3 join(point3 pt1, point3 pt2)
{
    line3 result = dual(outer(dual(pt1), dual(pt2)));
    return result;
}

plane3 join(point3 pt, line3 l)
{
    plane3 result = dual(outer(dual(pt), dual(l)));
    return result;
}

plane3 join(line3 l, point3 pt)
{
    plane3 result = dual(outer(dual(l), dual(pt)));
    return result;
}

line3 project(line3 l, plane3 p)
{
    line3 result = outer(inner(p, l), p);
    return result;
}

plane3 project(plane3 p, line3 l)
{
    plane3 result = inner(inner(l, p), l);
    return result;
}

line3 project(line3 l, point3 pt)
{
    line3 result = inner(inner(pt, l), pt);
    return result;
}

point3 project(point3 pt, line3 l)
{
    point3 result = outer(inner(l, pt), l);
    return result;
}

point3 project(point3 pt, plane3 p)
{
    point3 result = outer(inner(p, pt), p);
    return result;
}

plane3 project(plane3 p, point3 pt)
{
    plane3 result = inner(inner(pt, p), pt);
    return result;
}


#endif // BASED__PROJECTIVE_GEOMETRY3_HPP
