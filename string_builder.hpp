#ifndef BASED__STRING_BUILDER_HPP
#define BASED__STRING_BUILDER_HPP

#include "base.h"
#include "memory.h"


struct string_builder {
    memory_block buffer;
    usize used;

    void append(char const *fmt, ...);
    void append(char const *fmt, va_list args);
    void append(memory_block memory);
    void reset();
    memory_block get_string();
};

string_builder make_string_builder(memory_block blk);



#endif // BASED__STRING_BUILDER_H
