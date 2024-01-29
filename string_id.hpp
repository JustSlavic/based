#ifndef BASED__STRING_ID_H
#define BASED__STRING_ID_H

#include <base.h>


struct string_id
{
    uint32 id;

    static void initialize(memory_allocator);

    static string_id from(char const *cstring);
    static string_id from(string_view sv);
    char const *get_cstring();
    string_view get_string_view();
};


FORCE_INLINE
bool32 operator == (string_id id1, string_id id2)
{
    bool32 result = (id1.id == id2.id);
    return result;
}



#endif // BASED__STRING_ID_H
