#ifndef BASED__MEMORY_H
#define BASED__MEMORY_H

#include "base.h"
#include <string.h>

FORCE_INLINE
usize get_padding(void *pointer, usize alignment)
{
    // @todo: check if we can delete final '& (alignment - 1)'
    usize result = (alignment - ((usize)pointer & (alignment - 1))) & (alignment - 1);
    return result;
}

FORCE_INLINE
byte *align_pointer(void *pointer, usize alignment)
{
    byte *result = (byte *) pointer + get_padding(pointer, alignment);
    return result;
}

namespace cstring
{


FORCE_INLINE
usize size_no0(char const *cstr)
{
    usize result = 0;
    while (*cstr++) result += 1;
    return result;
}

FORCE_INLINE
usize size_with0(char const *cstr)
{
    usize result = size_no0(cstr) + 1;
    return result;
}


} // cstring

#endif // BASED__MEMORY_H
