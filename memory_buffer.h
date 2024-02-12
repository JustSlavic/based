#ifndef BASED__MEMORY_BUFFER_HPP
#define BASED__MEMORY_BUFFER_HPP

#include "base.h"


struct memory_buffer
{
    usize size;
    byte *data;

    FORCE_INLINE
    static memory_buffer make()
    {
        memory_buffer result;
        result.size = 0;
        result.data = NULL;
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
