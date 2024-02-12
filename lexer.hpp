#ifndef BASED__LEXER_H
#define BASED__LEXER_H

#include "base.h"
#include "memory.h"
#include "string_view.hpp"


struct lexer
{
    typedef bool predicate_t(char);

    memory_buffer buffer;
    usize cursor;

    uint32 line;
    uint32 column;

    static lexer from(void *, usize);

    char get_char(int lookup = 0);
    char eat_char();

    void eat_crlf();

    char const *get_remaining_input();

    int eat_string(char const *);
    int eat_string(char const *, usize);

    int consume_while(predicate_t *p);
    int consume_until(predicate_t *p);
};


#endif // BASED__LEXER_H
