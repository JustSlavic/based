#include "web.hpp"
#include "util.hpp"


char to_hex_table[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
int web::url_encode(memory_buffer from, memory_buffer to)
{
    int cursor = 0;
    for (usize index = 0; index < from.size; index++)
    {
        char c = from.data[index];
        if (is_ascii_alpha(c) ||
            is_ascii_digit(c) ||
            c == '-' ||
            c == '_' ||
            c == '.' ||
            c == '~')
        {
            to.data[cursor++] = c;
        }
        else
        {
            to.data[cursor++] = '%';
            int n1 = (c >> 4);
            int n2 = (c & 0xf);
            to.data[cursor++] = (n1 < 16) ? to_hex_table[n1] : '?';
            to.data[cursor++] = (n2 < 16) ? to_hex_table[n2] : '?';
        }
    }
    return cursor;
}

int web::url_decode(memory_buffer from, memory_buffer to)
{
    int cursor = 0;
    usize index = 0;
    while (index < from.size)
    {
        char c = from.data[index++];
        if (c == '%')
        {
            char n1 = from.data[index++];
            char n2 = from.data[index++];
            if (is_ascii_hex(n1) && is_ascii_hex(n2))
            {
                to.data[cursor++] = (from_hex(n1) << 4) | from_hex(n2);
            }
            else
            {
                break;
            }
        }
        else if (c == '+')
        {
            to.data[cursor++] = ' ';
        }
        else
        {
            to.data[cursor++] = c;
        }
    }

    return cursor;
}
