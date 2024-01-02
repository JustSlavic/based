#ifndef BASED__STRING_BUILDER_H
#define BASED__STRING_BUILDER_H

#include "base.h"
#include "memory.h"


typedef struct string_builder {
    memory_block memory;
    usize used;
} string_builder;


void string_builder__append_string(string_builder *sb, char const *str);
void string_builder__append_format(string_builder *sb, char const *fmt, ...);
void string_builder__append_buffer(string_builder *sb, memory_block memory);
void string_builder__reset(string_builder *sb);
memory_block string_builder__get_string(string_builder *sb);


#endif // BASED__STRING_BUILDER_H
