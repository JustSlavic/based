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
    if (lexer->cursor < lexer->buffer.size)
    {
        c = lexer->buffer.memory[lexer->cursor];
        if (c <= 0) c = 0;
    }
    return c;
}

char eat_char(struct lexer *lexer)
{
    char c1 = get_char(lexer);
    if (c1 > 0)
    {
        lexer->cursor += 1;
        lexer->column += 1;

        if (c1 == '\r' || c1 == '\n')
        {
            lexer->column = 1;
            lexer->line += 1;

            char c2 = get_char(lexer);
            if (c1 == '\r' && c2 == '\n')
            {
                lexer->cursor += 1;
            }

            c1 = '\n'; // @note: In case if I want to check it outside
        }
    }
    return c1;
}

int32 eat_newline(struct lexer *lexer)
{
    int32 result = 0;
    if (get_char(lexer) == '\r')
    {
        eat_char(lexer);
        result += 1;
    }
    if (get_char(lexer) == '\n')
    {
        eat_char(lexer);
        result += 1;
    }
    return result;
}

char *get_pointer(struct lexer *lexer)
{
    char *result = (char *) (lexer->buffer.memory + lexer->cursor);
    return result;
}

bool eat_cstring(struct lexer *lexer, char const *str)
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
    while (c <= 0 && !predicate(c))
    {
        eat_char(lexer);
        count += 1;
        c = get_char(lexer);
    }
    return count;
}


FORCE_INLINE bool32 is_ascii_space(char c) { return (c == ' '); }
FORCE_INLINE bool32 is_ascii_whitespace(char c) { return (c == ' ') || (c == '\t') || (c == '\r') || (c == '\n'); }
FORCE_INLINE bool32 is_ascii_alpha(char c) { return ((c >= 'A') && (c <= 'Z')) || ((c >= 'a') && (c <= 'z')); }
FORCE_INLINE bool32 is_ascii_digit(char c) { return (c >= '0') && (c <= '9'); }
FORCE_INLINE bool32 is_valid_identifier_head(char c) { return (c == '_') || is_ascii_alpha(c); }
FORCE_INLINE bool32 is_valid_identifier_body(char c) { return (c == '_') || is_ascii_alpha(c) || is_ascii_digit(c); }
