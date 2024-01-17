#ifndef BASED__LEXER_H
#define BASED__LEXER_H

#include "base.h"
#include "memory.h"
#include "string_view.h"


struct lexer
{
    memory_block buffer;
    usize cursor;

    uint32 line;
    uint32 column;
};

struct lexer make_lexer(memory_block buffer);

char get_char(struct lexer *);
char eat_char(struct lexer *);

bool eat_cstring(struct lexer *, char const *);
bool eat_string_view(struct lexer *, string_view sv);

int consume_while(struct lexer *lexer, bool32 (*predicate)(char));
int consume_until(struct lexer *lexer, bool32 (*predicate)(char));

// Predicates

FORCE_INLINE bool32 is_ascii_space(char c) { return (c == ' '); }
FORCE_INLINE bool32 is_ascii_whitespace(char c) { return (c == ' ') || (c == '\t') || (c == '\r') || (c == '\n'); }
FORCE_INLINE bool32 is_ascii_alpha(char c) { return ((c >= 'A') && (c <= 'Z')) || ((c >= 'a') && (c <= 'z')); }
FORCE_INLINE bool32 is_ascii_digit(char c) { return (c >= '0') && (c <= '9'); }
FORCE_INLINE bool32 is_valid_identifier_head(char c) { return (c == '_') || is_ascii_alpha(c); }
FORCE_INLINE bool32 is_valid_identifier_body(char c) { return (c == '_') || is_ascii_alpha(c) || is_ascii_digit(c); }


#endif // BASED__LEXER_H
