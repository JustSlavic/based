#ifndef BASED__UTIL_HPP
#define BASED__UTIL_HPP

#include "base.h"


int to_int(char const *s, usize size);
uint8 from_hex(char c);

bool is_eof(char c);
bool is_newline(char c);
bool is_crlf(char c);
bool is_ascii_space(char c);
bool is_ascii_whitespace(char c);
bool is_ascii_alpha(char c);
bool is_ascii_capital_alpha(char c);
bool is_ascii_lowercase_alpha(char c);
bool is_ascii_digit(char c);
bool is_ascii_hex(char c);
bool is_valid_identifier_head(char c);
bool is_valid_identifier_body(char c);
bool is_ascii_double_quote(char c);


#endif // BASED__UTIL_HPP
