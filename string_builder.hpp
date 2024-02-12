#ifndef BASED__STRING_BUILDER_HPP
#define BASED__STRING_BUILDER_HPP

#include "base.h"
#include "memory_bucket.hpp"
#include "string_view.hpp"


struct string_builder {
    memory_bucket buffer;

    static string_builder from(memory_buffer);

    int append(char const *fmt, ...);
    int append(char const *fmt, va_list args);
    // int append(memory_buffer buffer);
    void reset();
    string_view get_string();
};


#endif // BASED__STRING_BUILDER_H
