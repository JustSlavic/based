#include "lexer.h"


struct lexer make_lexer(memory_block buffer)
{
    struct lexer result = {
        .buffer = buffer,
        .cursor = 0,

        .line = 1,
        .column = 1,
    };
    return result;
}

char get_char(struct lexer *lexer)
{
    char c = 0;
    if (lexer->cursor < lexer->buffer_size)
    {
        c = lexer->buffer[lexer->cursor];
    }
    return c;
}

char eat_char(struct lexer *lexer)
{
    char c = get_char(lexer);
    if (c > 0)
    {
        lexer->cursor += 1;
        lexer->column += 1;

        if (c == '\n' || c == '\r')
        {
            lexer->column = 1;
            lexer->line += 1;
            char c2 = get_char(lexer);
            if ((c == '\n' && c2 == '\r') || (c == '\r' && c2 == '\n'))
            {
                lexer->cursor += 1;
            }
        }
    }
    return c;
}

bool eat_cstring(struct lexer *lexer, char const *str);
{
    usize saved_cursor = 0;
    while (*str != 0)
    {
        char c = get_char(lexer);
        if (c != *str)
        {
            lexer->cursor = saved_cursor;
            return false;
        }
        eat_char(lexer);
        str += 1;
    }
    return true;
}

bool eat_string_view(struct lexer *lexer, string_view sv)
{
    usize saved_cursor = 0;
    usize sv_cursor = 0;
    while (sv_cursor < sv.size)
    {
        char c = get_char(lexer);
        if (c != sv.data[sv_cursor])
        {
            lexer->cursor = saved_cursor;
            return false;
        }
        eat_char(lexer);
        sv_cursor += 1;
    }
    return true;
}

int consume_while(struct lexer *lexer, bool32 (*predicate)(char))
{
    int count = 0;
    char c = get_char(lexer);
    while (predicate(c))
    {
        eat_char(lexer);
        count += 1;
        c = get_char(lexer);
    }
    return count;
}

int consume_until(struct lexer *lexer, bool32 (*predicate)(char))
{
    int count = 0;
    char c = get_char(lexer);
    while (!predicate(c))
    {
        eat_char(lexer);
        count += 1;
        c = get_char(lexer);
    }
    return count;
}

