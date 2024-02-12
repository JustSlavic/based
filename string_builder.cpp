#include "string_builder.hpp"
#include "memory.h"


string_builder string_builder::from(memory_buffer buffer)
{
    string_builder result;
    result.buffer = memory_bucket::from(buffer);
    return result;
}

int string_builder::append(char const *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    auto free_memory = buffer.get_free();
    int copied_bytes = vsnprintf((char *) free_memory.data, free_memory.size, fmt, args);
    if (copied_bytes > 0) buffer.used += copied_bytes;

    va_end(args);

    return copied_bytes;
}

int string_builder::append(char const *fmt, va_list args)
{
    auto free_memory = buffer.get_free();
    int copied_bytes = vsnprintf((char *) free_memory.data, free_memory.size, fmt, args);
    if (copied_bytes > 0) buffer.used += copied_bytes;

    return copied_bytes;
}

// int string_builder::append(memory_buffer )
// {
//     if (used + str.size < buffer.size)
//     {
//         memory__copy(buffer.memory + used, str.memory, str.size);
//         used += str.size;
//         return str.size;
//     }
//     return 0;
// }

void string_builder::reset() {
    buffer.used = 0;
}

string_view string_builder::get_string() {
    string_view result;
    result.data = (char const *) buffer.data;
    result.size = buffer.used;
    return result;
}

