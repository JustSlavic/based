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

    static duration seconds(uint64 t)
    {
        duration result;
        result.us = (t * 1000000);
        return result;
    }
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
    ASSERT(t2.us <= t1.us);
    duration result = { t1.us - t2.us };
    return result;
}

INLINE timepoint operator + (timepoint t, duration d)
{
    timepoint result = { t.us + d.us };
    return result;
}

FORCE_INLINE bool operator <  (timepoint t1, timepoint t2) { return (t1.us <  t2.us); }
FORCE_INLINE bool operator <= (timepoint t1, timepoint t2) { return (t1.us <= t2.us); }
FORCE_INLINE bool operator == (timepoint t1, timepoint t2) { return (t1.us == t2.us); }
FORCE_INLINE bool operator != (timepoint t1, timepoint t2) { return (t1.us != t2.us); }
FORCE_INLINE bool operator >= (timepoint t1, timepoint t2) { return (t1.us >= t2.us); }
FORCE_INLINE bool operator  > (timepoint t1, timepoint t2) { return (t1.us  > t2.us); }

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

INLINE bool operator < (duration d1, duration d2)
{
    bool32 result = (d1.us < d2.us);
    return result;
}


#endif // BASED__TIME_HPP
