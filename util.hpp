#ifndef BASED__UTIL_HPP
#define BASED__UTIL_HPP

#include "base.h"


memory_buffer load_file(memory_allocator allocator, char const *filename);
int load_file(byte *buffer, usize buffer_size, char const *filename);

int to_int(char const *s, usize size);

bool is_eof(char c);
bool is_newline(char c);
bool is_crlf(char c);
bool is_ascii_space(char c);
bool is_ascii_whitespace(char c);
bool is_ascii_alpha(char c);
bool is_ascii_digit(char c);
bool is_valid_identifier_head(char c);
bool is_valid_identifier_body(char c);
bool is_ascii_double_quote(char c);


#endif // BASED__UTIL_HPP
