#include "string_builder.h"
#include "memory.h"


void string_builder__append_string(string_builder *sb, char const *str)
{
    usize string_size = cstring__size_no0(str);
    if (sb->used + string_size < sb->memory.size)
    {
        memory__copy(sb->memory.memory + sb->used, str, string_size);
        sb->used += string_size;
    }
}

void string_builder__append_format(string_builder *sb, char const *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    int copied_bytes = vsprintf((char *) sb->memory.memory + sb->used, fmt, args);
    va_end(args);

    if (copied_bytes > 0)
    {
        sb->used += copied_bytes;
    }
}

void string_builder__append_buffer(string_builder *sb, memory_block str)
{
    if (sb->used + str.size < sb->memory.size)
    {
        memory__copy(sb->memory.memory + sb->used, str.memory, str.size);
        sb->used += str.size;
    }
}

void string_builder__reset(string_builder *sb)
{
    sb->used = 0;
}

memory_block string_builder__get_string(string_builder *sb)
{
    memory_block result;
    result.memory = sb->memory.memory;
    result.size = sb->used;
    return result;
}

