#ifndef BASED__STRING_ID_H
#define BASED__STRING_ID_H

#include <base.h>
#include <memory/memory.hpp>
#include <memory/allocator.hpp>
#include <string_view.hpp>


#define STRID_STORAGE_CAPACITY 1024

struct string_id
{
    uint32 id;

    struct storage
    {
        memory_allocator allocator;

        string_view strings[STRID_STORAGE_CAPACITY];
        uint64      hashes[STRID_STORAGE_CAPACITY];
    };

    static storage initialize(memory_allocator);

    static string_id from(char const *cstring);
    static string_id from(char const *cstring, usize size);
    static string_id from(string_view s);

    static string_id from(storage *sg, char const *cstring);
    static string_id from(storage *sg, char const *cstring, usize size);
    static string_id from(storage *sg, string_view s);

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
