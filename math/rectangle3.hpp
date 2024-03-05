#ifndef BASED__AABB_HPP
#define BASED__AABB_HPP

#include <base.h>


struct rectangle3
{
    union
    {
        struct
        {
            vector3 min;
            vector3 max;
        };
        struct
        {
            float32 min_x, min_y, min_z;
            float32 max_x, max_y, max_z;
        };
    };

    static rectangle3 from_center_radius(vector3 center, float32 rx, float32 ry, float32 rz)
    {
        rectangle3 result;
        result.min = center - V3(rx, ry, rz);
        result.max = center + V3(rx, ry, rz);
        return result;
    }
};



#endif // BASED__AABB_HPP
