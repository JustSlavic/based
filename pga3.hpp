#ifndef BASED__PGA3_HPP
#define BASED__PGA3_HPP

#include "g3.hpp"



struct plane3
{
    // Ax + By + Cz + D = 0
    // Ae1 + Be2 + Ce3 + De0 = 0
    union
    {
        struct { float32 _e1, _e2, _e3, _e0; };
        struct { vector3 normal; float32 w; };
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




#endif // BASED__PGA3_HPP
