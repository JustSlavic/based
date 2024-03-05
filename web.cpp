#include "web.hpp"

static char to_hex_table[16] = {
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
};
static int8 from_hex_table[128] = {
   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    0,  1,  2,  3,  4,  5,  6,  7,  8,  9, -1, -1, -1, -1, -1, -1,
   -1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1,
   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
   -1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1,
   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
};

int web::url_encode(memory_buffer from, memory_buffer to)
{
    uint32 cursor_from = 0;
    uint32 cursor_to = 0;

    while ((cursor_from < from.size) && (cursor_to < to.size))
    {
        char c = from.data[cursor_from++];
        if (('A' <= c && c <= 'Z') ||
            ('a' <= c && c <= 'z') ||
            ('0' <= c && c <= '9') ||
            c == '-' ||
            c == '_' ||
            c == '.' ||
            c == '~')
        {
            to.data[cursor_to++] = c;
        }
        else if (c == ' ')
        {
            to.data[cursor_to++] = '+';
        }
        else
        {
            if (cursor_to + 3 < to.size)
            {
                to.data[cursor_to++] = '%';

                int n1 = (c >> 4) & 0xf;
                to.data[cursor_to++] = (n1 < 16) ? to_hex_table[n1] : '%';

                int n2 = (c & 0xf);
                to.data[cursor_to++] = (n2 < 16) ? to_hex_table[n2] : '%';
            }
        }
    }
    return cursor_to;
}

int web::url_decode(memory_buffer from, memory_buffer to)
{
    uint32 cursor_from = 0;
    uint32 cursor_to = 0;

    while ((cursor_from < from.size) && (cursor_to < to.size))
    {
        char c = from.data[cursor_from++];
        if (c == '%')
        {
            if (cursor_from + 2 <= from.size)
            {
                char c1 = from.data[cursor_from++];
                char c2 = from.data[cursor_from++];

                int n1 = from_hex_table[(int)c1];
                int n2 = from_hex_table[(int)c2];

                if (n1 < 0 || n2 < 0) break;

                to.data[cursor_to++] = (n1 << 4) | n2;
            }
        }
        else if (c == '+')
        {
            to.data[cursor_to++] = ' ';
        }
        else if (('A' <= c && c <= 'Z') ||
                 ('a' <= c && c <= 'z') ||
                 ('0' <= c && c <= '9') ||
                 c == '-' ||
                 c == '_' ||
                 c == '.' ||
                 c == '~')
        {
            to.data[cursor_to++] = c;
        }
        else
        {
            break;
        }
    }

    return cursor_to;
}

