#ifndef BASED__TIME_HPP
#define BASED__TIME_HPP

#include "base.h"


struct timepoint
{
    uint64 us;
};

struct duration
{
    uint64 us;
};

float64 get_seconds(timepoint t)
{
    float64 result = t.us / 1000000.0;
    return result;
}

float64 get_seconds(duration d)
{
    float64 result = d.us / 1000000.0;
    return result;
}

INLINE duration operator - (timepoint t1, timepoint t2)
{
    ASSERT(t2.us < t1.us);
    duration result = { t1.us - t2.us };
    return result;
}

INLINE timepoint operator + (timepoint t, duration d)
{
    timepoint result = { t.us + d.us };
    return result;
}

INLINE duration operator + (duration d1, duration d2)
{
    duration result = { d1.us + d2.us };
    return result;
}

INLINE duration operator - (duration d1, duration d2)
{
    duration result = { d1.us - d2.us };
    return result;
}

INLINE bool32 operator < (duration d1, duration d2)
{
    bool32 result = (d1.us < d2.us);
    return result;
}


#endif // BASED__TIME_HPP
