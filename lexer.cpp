#include "lexer.hpp"


lexer lexer::from(void *data, usize size)
{
    lexer result;
    result.buffer = memory_buffer::from(data, size);
    result.cursor = 0;
    result.line = 1;
    result.column = 1;
    return result;
}

char lexer::get_char(int lookup)
{
    char c = 0;
    if ((cursor + lookup) < buffer.size)
    {
        c = buffer.data[cursor + lookup];
        if (c <= 0) c = 0;
    }
    return c;
}

char lexer::eat_char()
{
    char c = get_char();
    if (c != 0) cursor += 1;
    if (c == '\r')
    {
        column = 1;
    }
    else if (c == '\n')
    {
        column = 1;
        line += 1;
    }
    return c;
}

void lexer::eat_crlf()
{
    char r = get_char(0);
    char n = get_char(1);
    if (r == '\r' && n == '\n')
    {
        eat_char();
        eat_char();
    }
}

char const *lexer::get_remaining_input()
{
    char const *result = (char const *) (buffer.data + cursor);
    return result;
}

int lexer::eat_string(char const *cstr)
{
    int length = 0;
    char const *input = get_remaining_input();
    while (*input && *cstr && *input == *cstr)
    {
        cstr += 1;
        input += 1;
        length += 1;
    }
    if (*cstr == 0)
    {
        cursor += length;
        return length;
    }
    return 0;
}

int lexer::eat_string(char const *cstr, usize size)
{
    char const *input = get_remaining_input();
    while (size-->0 && *input && *cstr && *input == *cstr)
    {
        cstr += 1;
        input += 1;
    }
    if (size == 0)
    {
        cursor += size;
        return size;
    }
    return 0;
}

int lexer::consume_while(predicate_t *predicate)
{
    int count = 0;
    char c = get_char();
    while (c > 0 && predicate(c))
    {
        eat_char();
        count += 1;
        c = get_char();
    }
    return count;
}

int lexer::consume_until(predicate_t *predicate)
{
    int count = 0;
    char c = get_char();
    while (c > 0 && !predicate(c))
    {
        eat_char();
        count += 1;
        c = get_char();
    }
    return count;
}
