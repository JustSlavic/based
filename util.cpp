#include "util.hpp"

int to_int(char const *s, usize size)
{
    int n = 0;
    while ((size-->0) && is_ascii_digit(*s))
    {
        n *= 10;
        n += (*s++ - '0');
    }
    return n;
}

uint8 from_hex(char c)
{
    uint8 n = is_ascii_digit(c) ? (c - '0') :
              is_ascii_capital_alpha(c) ? (c - 'A' + 10) :
              is_ascii_lowercase_alpha(c) ? (c - 'a' + 10) :
              0;
    return n;
}

FORCE_INLINE bool is_eof(char c) { return (c == 0); }
FORCE_INLINE bool is_newline(char c) { return (c == '\n'); }
FORCE_INLINE bool is_crlf(char c) { return (c == '\r') || (c == '\n'); }
FORCE_INLINE bool is_ascii_space(char c) { return (c == ' '); }
FORCE_INLINE bool is_ascii_whitespace(char c) { return (c == ' ') || (c == '\t') || (c == '\r') || (c == '\n'); }
FORCE_INLINE bool is_ascii_alpha(char c) { return is_ascii_capital_alpha(c) || is_ascii_lowercase_alpha(c); }
FORCE_INLINE bool is_ascii_lowercase_alpha(char c) { return (c >= 'a') && (c <= 'z'); }
FORCE_INLINE bool is_ascii_capital_alpha(char c) { return (c >= 'A') && (c <= 'Z'); }
FORCE_INLINE bool is_ascii_digit(char c) { return (c >= '0') && (c <= '9'); }
FORCE_INLINE bool is_ascii_hex(char c) { return is_ascii_digit(c) || (('a' <= c) && (c <= 'f')) || (('A' <= c) && (c <= 'F')); }
FORCE_INLINE bool is_valid_identifier_head(char c) { return (c == '_') || is_ascii_alpha(c); }
FORCE_INLINE bool is_valid_identifier_body(char c) { return (c == '_') || is_ascii_alpha(c) || is_ascii_digit(c); }
FORCE_INLINE bool is_ascii_double_quote(char c) { return (c == '"'); }
