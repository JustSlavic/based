#include "memory_bucket.hpp"

#include <stdio.h>


memory_bucket memory_bucket::from(void *buffer, usize size)
{
    memory_bucket result;
    result.used = 0;
    result.size = size;
    result.data = (byte *) buffer;
    return result;
}

memory_bucket memory_bucket::from(memory_buffer buffer)
{
    memory_bucket result;
    result.used = 0;
    result.size = buffer.size;
    result.data = buffer.data;
    return result;
}

memory_buffer memory_bucket::get_buffer()
{
    memory_buffer result;
    result.size = size;
    result.data = data;
    return result;
}

memory_buffer memory_bucket::get_free()
{
    memory_buffer result;
    result.size = size - used;
    result.data = data + used;
    return result;
}

memory_buffer memory_bucket::get_used()
{
    memory_buffer result;
    result.size = used;
    result.data = data;
    return result;
}

int memory_bucket::append(char const *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    auto free_memory = get_free();
    int copied_bytes = vsnprintf((char *) free_memory.data, free_memory.size, fmt, args);
    if (copied_bytes > 0) used += copied_bytes;

    va_end(args);

    return copied_bytes;
}

int memory_bucket::append(char const *fmt, va_list args)
{
    auto free_memory = get_free();
    int copied_bytes = vsnprintf((char *) free_memory.data, free_memory.size, fmt, args);
    if (copied_bytes > 0) used += copied_bytes;

    return copied_bytes;
}

void memory_bucket::append(void *buffer, usize n)
{
    auto free_memory = get_free();
    memcpy(free_memory.data, buffer, n);
    used += n;
}
