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


#endif // BASED__HASH_HPP
