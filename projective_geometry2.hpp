#ifndef BASED__PROJECTIVE_GEOMETRY2_HPP
#define BASED__PROJECTIVE_GEOMETRY2_HPP

#include "base.h"
#include "vector2.hpp"


struct line2
{
    union
    {
        struct { float32 a, b, c; }; // Ax + By + C = 0
        struct { float32 _e1, _e2, _e0; };
        struct { vector2 normal; float32 c_; };
    };
};


static constexpr line2 Ox { 0.f, 1.f, 0.f };
static constexpr line2 Oy { 1.f, 0.f, 0.f };
static constexpr line2 InfLine { 0.f, 0.f, 1.f };


struct point2
{
    union
    {
        struct { float32 x, y, w; };
        struct { float32 _e20, _e01, _e12; };
        struct { vector2 vector; float32 w_; };
    };
};


static constexpr point2 Origin { 0.f, 0.f, 1.f };


struct motor2
{
    union
    {
        struct { float32 _1, _e20, _e01, _e12; };
        struct { float32 pad1; float32 x, y, w; };
        struct { float32 pad2; point2 point2; };
    };
};


struct flector2
{
    union
    {
        struct { float32 _e1, _e2, _e0, _e120; };
        struct { float32 a, b, c, f; };
        struct { line2 line2; float32 pad2; };
    };
};


line2 make_line(float32 a, float32 b, float32 c)
{
    line2 result;
    result._e1 = a;
    result._e2 = b;
    result._e0 = c;
    return result;
}

point2 make_point(float32 x, float32 y)
{
    point2 result;
    result.w = 1.f;
    result.x = x;
    result.y = y;
    return result;
}

line2 dual(point2 p)
{
    line2 result;
    result._e1 = p._e20;
    result._e2 = p._e01;
    result._e0 = p._e12;
    return result;
}

point2 dual(line2 l)
{
    point2 result;
    result._e20 = l._e1;
    result._e01 = l._e2;
    result._e12 = l._e0;
    return result;
}

vector2 to_vector2(point2 p)
{
    vector2 result = p.vector;
    return result;
}

point2 to_point(motor2 m)
{
    point2 result = m.point2;
    return result;
}

line2 to_line(flector2 f)
{
    line2 result = f.line2;
    return result;
}

// =====================================================

line2 operator - (line2 l1)
{
    line2 result;
    result.a = -l1.a;
    result.b = -l1.b;
    result.c = -l1.c;
    return result;
}

line2 operator + (line2 l, vector2 v)
{
    line2 result;
    result.a = l.a;
    result.b = l.b;
    result.c = l.c - l.a * v.x - l.b * v.y;
    return result;
}

point2 operator + (point2 p, vector2 v)
{
    point2 result;
    result.x = p.x + v.x * p.w;
    result.y = p.y + v.y * p.w;
    result.w = p.w;
    return result;
}

point2 operator + (vector2 v, point2 p)
{
    point2 result = p + v;
    return result;
}

motor2 operator * (line2 l1, line2 l2)
{
    // (a1e1 + b1e2 + c1e0)(a2e1 + b2e2 + c2e0) =
    // (a1a2 + b1b2) _1  +
    // (b1c2 - c1b2) e20 +
    // (c1a2 - a1c2) e01 +
    // (a1b2 - b1a2) e12
    motor2 result;
    result._1   = (l1.a * l2.a + l1.b * l2.b);
    result._e20 = (l1.b * l2.c - l1.c * l2.b);
    result._e01 = (l1.c * l2.a - l1.a * l2.c);
    result._e12 = (l1.a * l2.b - l1.b * l2.a);
    return result;
}

flector2 operator * (line2 l, point2 p)
{
    // (ae1 + be2 + ce0)(xe20 + ye01 + we12) =
    // (-bw) e1 +
    // ( aw) e2 +
    // (bx - ay) e0 +
    // (ax + by + cw) e012
    flector2 result;
    result._e1   = -l.b * p.w;
    result._e2   =  l.a * p.w;
    result._e0   =  l.b * p.x - l.a * p.y;
    result._e120 =  l.a * p.x + l.b * p.y + l.c * p.w;
    return result;
}

flector2 operator * (motor2 m, line2 l)
{
    // (t + we12 + xe20 + ye01)(ae1 + be2 + ce0) =
    // (ta + wb) e1 +
    // (tb - wa) e2 +
    // (tc - xb + ya) e0 +
    // (wc + xa + yb) e012
    flector2 result;
    result._e1 = (m._1 * l._e1 + m._e12 * l._e2);
    result._e2 = (m._1 * l._e2 - m._e12 * l._e1);
    result._e0 = (m._1 * l._e0 - m._e20 * l._e2 + m._e01 * l._e1);
    result._e120 = (m._e12 * l._e0 + m._e20 * l._e1 + m._e01 * l._e2);
    return result;
}

motor2 operator * (motor2 m, point2 p)
{
    // (t + x1 e20 + y1 e01 + w1 e12)(x2 e20 + y2 e01 + w2 e12) =
    // (-w1 w2) _1_ +
    // (t x2 - y1 w2 + w1 y2) e20 +
    // (t y2 + x1 w2 - w1 x2) e01 +
    // (t w2) e12
    motor2 result;
    result._1   = -m.w * p.w;
    result._e20 =  m._1 * p.x - m.y * p.w + m.w * p.y;
    result._e01 =  m._1 * p.y + m.x * p.w - m.w * p.x;
    result._e12 =  m._1 * p.w;
    return result;
}

motor2 operator * (motor2 m1, motor2 m2)
{
    // (t1 + x1 e20 + y1 e01 + w1 e12)(t2 + x2 e20 + y2 e01 + w2 e12)
    // (t1 t2 - w1 w2) _1_ +
    // (t1 x2 + x1 t2 - y1 w2 + w1 y2) e20 +
    // (t1 y2 + x1 w2 + y1 t2 - w1 x2) e01 +
    // (t1 w2 + w1 t2) e12
    motor2 result;
    result._1   = m1._1 * m2._1 - m1.w * m2.w;
    result._e20 = m1._1 * m2.x  + m1.x * m2._1 - m1.y * m2.w  + m1.w * m2.y;
    result._e01 = m1._1 * m2.y  + m1.x * m2.w  + m1.y * m2._1 - m1.w * m2.x;
    result._e12 = m1._1 * m2.w  + m1.w * m2._1;
    return result;
}

motor2 operator * (flector2 f, line2 l)
{
    // (a1 e1 + b1 e2 + c1 e0 + f e012)(a2 e1 + b2 e2 + c2 e0) =
    // (a1a2 + b1b2) _1_ +
    // (b1c2 - c1b2 + f a2) e20 +
    // (c1a2 - a1c2 + f b2) e01 +
    // (a1b2 - b1a2) e12
    motor2 result;
    result._1   = f.a * l.a + f.b * l.b;
    result._e20 = f.b * l.c - f.c * l.b + f.f * l.a;
    result._e01 = f.c * l.a - f.a * l.c + f.f * l.b;
    result._e12 = f.a * l.b - f.b * l.a;
    return result;
}

flector2 operator * (flector2 f, motor2 m)
{
    // (a e1 + b e2 + c e0 + f e012) (t + x e20 + y e01 + w e12) =
    // (at - bs) e1 +
    // (as + bt) e2 +
    // (bx - ay + ct - ds) e0 +
    // (ax + by + cs + dt) e012
    flector2 result;
    result._e1   = (f._e1 * m._1   - f._e2 * m._e12);
    result._e2   = (f._e1 * m._e12 + f._e2 * m._1);
    result._e0   = (f._e2 * m._e20 - f._e1 * m._e01 + f._e0 * m._1   - f._e120 * m._e12);
    result._e120 = (f._e1 * m._e20 + f._e2 * m._e01 + f._e0 * m._e12 + f._e120 * m._1);
    return result;
}

float32 inner(line2 l1, line2 l2)
{
    // @note inner(line2, line2) is the same as inner product of their normal vectors:
    // (a1e1 + b1e2 + c1e0) * (a2e1 + b2e2 + c2e0) =
    // (a1a2 + b1b2)
    float32 result = l1.a * l2.a + l1.b * l2.b;
    return result;
}

line2 inner(line2 l, point2 p)
{
    // @note: gonna take only 1-blade
    // (ae1 + be2 + ce0)(we1e2 + xe20 + ye01) =
    // (-bw) e1 +
    // ( aw) e2 +
    // (bx - ay) e0
    line2 result;
    result._e1 = -l.b * p.w;
    result._e2 =  l.a * p.w;
    result._e0 =  l.b * p.x - l.a * p.y;
    return result;
}

line2 inner(point2 p, line2 l)
{
    line2 result = -inner(l, p);
    return result;
}

point2 outer(line2 l1, line2 l2)
{
    // (a1e1 + b1e2 + c1e0) /\ (a2e1 + b2e2 + c2e0) =
    // (a1b2 - a2b1) e12 +
    // (b1c2 - c1b2) e20 +
    // (c1a2 - a1c2) e01
    point2 result;
    result.x = l1.b * l2.c - l1.c * l2.b;
    result.y = l1.c * l2.a - l1.a * l2.c;
    result.w = l1.a * l2.b - l2.a * l1.b;
    return result;
}

line2 join(point2 p1, point2 p2)
{
    line2 result = dual(outer(dual(p1), dual(p2)));
    return result;
}

void conjugate(motor2 & m)
{
    m._e12 = -m._e12;
    m._e20 = -m._e20;
    m._e01 = -m._e01;
}

motor2 conjugated(motor2 m)
{
    conjugate(m);
    return m;
}


#endif // BASED__PROJECTIVE_GEOMETRY2_HPP
