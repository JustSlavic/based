#ifndef BASED__COMPLEX_HPP
#define BASED__COMPLEX_HPP

#include "base.h"
#include "vector2.hpp"


struct complex
{
    union
    {
        struct { float32 re, im; };
        struct { float32 _1, _e12; };
        float32 e[2];
    };
};

FORCE_INLINE complex & operator += (complex & a, complex b) { a.re += b.re; a.im += b.im; return a; }
FORCE_INLINE complex & operator -= (complex & a, complex b) { a.re -= b.re; a.im -= b.im; return a; }
FORCE_INLINE complex & operator *= (complex & a, float32 c) { a.re *= c;    a.im *= c;    return a; }
FORCE_INLINE complex & operator *= (complex & a, complex b) { a.re = a.re * b.re - a.im * b.im; a.im = a.re * b.im + a.im * b.re; return a; }

FORCE_INLINE bool operator == (complex a, complex b) { bool r = (a.re == b.re) && (a.im == b.im); return r; }
FORCE_INLINE bool operator != (complex a, complex b) { bool r = !(a == b); return r; }

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
    r.re = (a._e1 * b._e1) + (a._e2 * b._e2);
    r.im = (a._e1 * b._e2) - (a._e2 * b._e1);
    return r;
}

FORCE_INLINE complex to_complex(vector2 v) { complex r; r.re = v.x; r.im = v.y; return r; }
FORCE_INLINE vector2 to_vector2(complex a) { vector2 r; r.x = a.re; r.y = a.im; return r; }

FORCE_INLINE void    conjugate(complex & a) { a.im = -a.im; }
FORCE_INLINE complex conjugated(complex a) { conjugate(a); return a; }


#endif // BASED__COMPLEX_HPP
