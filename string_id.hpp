#ifndef BASED__STRING_ID_H
#define BASED__STRING_ID_H

#include <base.h>
#include <memory_allocator.h>
#include <string_view.hpp>


struct string_id
{
    uint32 id;

    struct storage
    {
        memory_allocator allocator;

        char const *table[1024];
        usize       sizes[1024];
        uint64      hashes[1024];
    };

    static storage initialize(memory_allocator);

    static string_id from(char const *cstring);
    static string_id from(string_view sv);

    static string_id from(storage *s, char const *cstring);
    static string_id from(storage *s, string_view sv);

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
