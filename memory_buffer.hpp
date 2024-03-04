#ifndef BASED__MEMORY_BUFFER_HPP
#define BASED__MEMORY_BUFFER_HPP

#include "base.h"
#include "memory.hpp"


struct memory_buffer
{
    usize size;
    byte *data;

    operator bool()
    {
        return (size != 0) && (data != NULL);
    }

    FORCE_INLINE
    static memory_buffer make()
    {
        memory_buffer result;
        result.size = 0;
        result.data = NULL;
        return result;
    }

    FORCE_INLINE
    static memory_buffer from(char const *s)
    {
        memory_buffer result;
        result.size = cstring::size_no0(s);
        result.data = (byte *) s;
        return result;
    }

    FORCE_INLINE
    static memory_buffer from(void *d, usize n)
    {
        memory_buffer result;
        result.size = n;
        result.data = (byte *) d;
        return result;
    }

    FORCE_INLINE
    static memory_buffer from(byte *d, usize n)
    {
        memory_buffer result;
        result.size = n;
        result.data = (byte *) d;
        return result;
    }

    FORCE_INLINE
    static memory_buffer from(char *d, usize n)
    {
        memory_buffer result;
        result.size = n;
        result.data = (byte *) d;
        return result;
    }

};


#endif // BASED__MEMORY_BUFFER_HPP
