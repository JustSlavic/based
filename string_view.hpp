#ifndef BASED__STRING_VIEW
#define BASED__STRING_VIEW

#include "base.h"


struct string_view
{
    char const *data;
    usize size;
};


FORCE_INLINE
bool32 operator == (string_view s1, string_view s2)
{
    bool32 is_equal = (s1.size == s2.size);
    for (int i = 0; is_equal && i < s1.size; i++)
    {
        is_equal = (s1.data[i] == s2.data[i]);
    }
    return is_equal;
}


#endif // BASED__STRING_VIEW
