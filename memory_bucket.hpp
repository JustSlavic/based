#ifndef BASED__MEMORY_BUCKET_HPP
#define BASED__MEMORY_BUCKET_HPP

#include "base.h"
#include "memory_buffer.h"


struct memory_bucket
{
    usize used;
    usize size;
    byte *data;

    static memory_bucket from(void *buffer, usize size);
    static memory_bucket from(memory_buffer buffer);
    memory_buffer get_buffer();
    memory_buffer get_free();
    memory_buffer get_used();

    int append(char const *fmt, ...);
    int append(char const *fmt, va_list args);
};



#endif // BASED__MEMORY_BUCKET_HPP
