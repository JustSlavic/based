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
int32 eat_newline(struct lexer *);

char *get_pointer(struct lexer *);

bool eat_cstring(struct lexer *, char const *);
bool eat_string_view(struct lexer *, string_view sv);

int consume_while(struct lexer *lexer, bool32 (*predicate)(char));
int consume_until(struct lexer *lexer, bool32 (*predicate)(char));

// Predicates

bool32 is_ascii_space(char c);
bool32 is_ascii_whitespace(char c);
bool32 is_ascii_alpha(char c);
bool32 is_ascii_digit(char c);
bool32 is_valid_identifier_head(char c);
bool32 is_valid_identifier_body(char c);


#endif // BASED__LEXER_H
