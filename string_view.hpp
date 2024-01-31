#ifndef BASED__STRING_VIEW
#define BASED__STRING_VIEW

#include "base.h"


struct string_view
{
    char const *data;
    usize size;

    bool32 is_empty() const { return (data == 0) || (size == 0); }
};


FORCE_INLINE
bool32 operator == (string_view s1, string_view s2)
{
    bool32 is_equal = (s1.size == s2.size);
    for (usize i = 0; is_equal && i < s1.size; i++)
    {
        is_equal = (s1.data[i] == s2.data[i]);
    }
    return is_equal;
}

FORCE_INLINE
bool32 operator == (string_view s1, char const *s2)
{
    // @todo: make it better
    return (s1 == (string_view{ s2, cstring__size_no0(s2) }));
}


#endif // BASED__STRING_VIEW
