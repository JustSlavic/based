#ifndef BASED__HASH_HPP
#define BASED__HASH_HPP

#include <base.h>


FORCE_INLINE uint32 hash_djb2(char const *cstr)
{
    // http://www.cse.yorku.ca/~oz/hash.html
    uint32 h = 5381;
    int c;
    while ((c = *cstr++))
        h = ((h << 5) + h) + c; /* h * 33 + c */
    return h;
}

FORCE_INLINE uint32 hash_djb2(byte *data, usize size)
{
    // http://www.cse.yorku.ca/~oz/hash.html
    uint32 h = 5381;
    for (usize i = 0; i < size; i++)
    {
        int c = data[i];
        h = ((h << 5) + h) + c; /* h * 33 + c */
    }
    return h;
}


#endif // BASED__HASH_HPP
