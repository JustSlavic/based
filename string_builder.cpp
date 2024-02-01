#include "string_builder.hpp"
#include "memory.h"


string_builder make_string_builder(memory_block blk)
{
    string_builder result;
    result.buffer = blk;
    result.used = 0;
    return result;
}

int string_builder::append(char const *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    char *cursor = (char *) buffer.memory + used;
    usize buffer_size = buffer.size - used;
    int copied_bytes = vsnprintf(cursor, buffer_size, fmt, args);
    va_end(args);

    if (copied_bytes > 0)
    {
        used += copied_bytes;
    }

    return copied_bytes;
}

int string_builder::append(char const *fmt, va_list args)
{
    char *cursor = (char *) buffer.memory + used;
    usize buffer_size = buffer.size - used;
    int copied_bytes = vsnprintf(cursor, buffer_size, fmt, args);

    if (copied_bytes > 0)
    {
        used += copied_bytes;
    }

    return copied_bytes;
}

int string_builder::append(memory_block str)
{
    if (used + str.size < buffer.size)
    {
        memory__copy(buffer.memory + used, str.memory, str.size);
        used += str.size;
        return str.size;
    }
    return 0;
}

void string_builder::reset() {
    used = 0;
}

memory_block string_builder::get_string() {
    memory_block result;
    result.memory = buffer.memory;
    result.size = used;
    return result;
}

