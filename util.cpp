#include "util.hpp"

memory_buffer load_file(memory_allocator allocator, char const *filename)
{
    memory_buffer result = {};

    int fd = open(filename, O_RDONLY, 0);
    if (fd < 0)
    {}
    else
    {
        struct stat st;
        int ec = fstat(fd, &st);
        if (ec < 0)
        {}
        else
        {
            memory_buffer block = allocator.allocate_buffer(st.st_size + 1);
            if (block.data != NULL)
            {
                uint32 bytes_read = read(fd, block.data, st.st_size);
                if (bytes_read < st.st_size)
                {
                    allocator.deallocate(block.data, block.size);
                }
                else
                {
                    result = block;
                }
            }
        }
        close(fd);
    }

    return result;
}

// int load_file(byte *buffer, usize buffer_size, char const *filename)
// {
//     memory_buffer result = {};

//     int fd = open(filename, O_RDONLY, 0);
//     if (fd < 0)
//     {}
//     else
//     {
//         struct stat st;
//         int ec = fstat(fd, &st);
//         if (ec < 0)
//         {}
//         else
//         {
//             if (buffer_size >= (st.st_size + 1))
//             {
//                 uint32 bytes_read = read(fd, block.data, st.st_size);
//                 if (bytes_read < st.st_size)
//                 {
//                     allocator.deallocate(block.data, block.size);
//                 }
//                 else
//                 {
//                     result = block;
//                 }
//             }
//         }
//         close(fd);
//     }

//     return result;
// }

int to_int(char const *s, usize size)
{
    int n = 0;
    while ((size-->0) && is_ascii_digit(*s))
    {
        n *= 10;
        n += (*s++ - '0');
    }
    return n;
}

FORCE_INLINE bool is_eof(char c) { return (c == 0); }
FORCE_INLINE bool is_newline(char c) { return (c == '\n'); }
FORCE_INLINE bool is_crlf(char c) { return (c == '\r') || (c == '\n'); }
FORCE_INLINE bool is_ascii_space(char c) { return (c == ' '); }
FORCE_INLINE bool is_ascii_whitespace(char c) { return (c == ' ') || (c == '\t') || (c == '\r') || (c == '\n'); }
FORCE_INLINE bool is_ascii_alpha(char c) { return ((c >= 'A') && (c <= 'Z')) || ((c >= 'a') && (c <= 'z')); }
FORCE_INLINE bool is_ascii_digit(char c) { return (c >= '0') && (c <= '9'); }
FORCE_INLINE bool is_valid_identifier_head(char c) { return (c == '_') || is_ascii_alpha(c); }
FORCE_INLINE bool is_valid_identifier_body(char c) { return (c == '_') || is_ascii_alpha(c) || is_ascii_digit(c); }
FORCE_INLINE bool is_ascii_double_quote(char c) { return (c == '"'); }
