#include "acf.hpp"
#include "lexer.h"

// ====== impl ======

enum acf_kind
{
    ACF_INVALID,

    ACF_NULL,
    ACF_BOOL,
    ACF_INT,
    ACF_FLOAT,
    ACF_STRING,
    ACF_OBJECT,
    ACF_LIST,
};

struct acf_object
{
    acf_impl *vals[32];
    string_id keys[32];
    uint32 count;
};

struct acf_impl
{
    acf_kind kind;
    union
    {
        bool32      boolean;
        float64     floating;
        int64       integer;
        string_view string;
        acf_object *object;
    };
};





enum acf_token__kind
{
    ACF_TOKEN__EOF = 0,

    ACF_TOKEN__COMMA = ',',
    ACF_TOKEN__EQUAL = '=',
    ACF_TOKEN__SEMICOLON = ';',
    ACF_TOKEN__BRACE_OPEN = '{',
    ACF_TOKEN__BRACE_CLOSE = '}',
    ACF_TOKEN__BRACKET_OPEN = '[',
    ACF_TOKEN__BRACKET_CLOSE = ']',

    ACF_TOKEN__IDENTIFIER = 256,
    ACF_TOKEN__NULL,
    ACF_TOKEN__TRUE,
    ACF_TOKEN__FALSE,
    ACF_TOKEN__NUMLIT,
    ACF_TOKEN__STRING,
};
typedef enum acf_token__kind acf_token__kind;

struct acf__token
{
    acf_token__kind kind;
    int64 integer;
    string_view span;
};
typedef struct acf__token acf__token;

struct acf__lexer
{
    memory_allocator a;

    struct lexer l;

    acf__token current_token;
    bool32 current_token_ok;
};
typedef struct acf__lexer acf__lexer;

acf__token acf__get_token(acf__lexer *lexer)
{
    acf__token result = {};
    if (lexer->current_token_ok)
    {
        result = lexer->current_token;
    }
    else
    {
        consume_while(&lexer->l, is_ascii_whitespace);

        char c = get_char(&lexer->l);
        if (c == 0)
        {
            result.kind = ACF_TOKEN__EOF;
        }
        else if (is_valid_identifier_head(c))
        {
            if (eat_cstring(&lexer->l, "null"))
            {
                result.kind = ACF_TOKEN__NULL;
                result.span.data = get_pointer(&lexer->l) - 4;
                result.span.size = 4;
            }
            else if (eat_cstring(&lexer->l, "true"))
            {
                result.kind = ACF_TOKEN__TRUE;
                result.span.data = get_pointer(&lexer->l) - 4;
                result.span.size = 4;
            }
            else if (eat_cstring(&lexer->l, "false"))
            {
                result.kind = ACF_TOKEN__FALSE;
                result.span.data = get_pointer(&lexer->l) - 5;
                result.span.size = 5;
            }
            else
            {
                result.kind = ACF_TOKEN__IDENTIFIER;
                result.span.data = get_pointer(&lexer->l);
                result.span.size = consume_while(&lexer->l, is_valid_identifier_body);
            }
        }
        else if (is_ascii_digit(c))
        {
            int64 integral_part = 0;
            while (is_ascii_digit(c))
            {
                eat_char(&lexer->l);
                integral_part *= 10;
                integral_part += c - '0';
                c = get_char(&lexer->l);
            }
            result.kind = ACF_TOKEN__NUMLIT;
            result.integer = integral_part;
        }
        else if (c == '"')
        {
            eat_char(&lexer->l);
            result.kind = ACF_TOKEN__STRING;
            result.span.data = get_pointer(&lexer->l);
            result.span.size = consume_until(&lexer->l, is_ascii_double_quote);
            eat_char(&lexer->l);
        }
        else
        {
            result.kind = (enum acf_token__kind) c;
            eat_char(&lexer->l);
        }

        lexer->current_token = result;
        lexer->current_token_ok = true;
    }

    return result;
}

acf__token acf__eat_token(acf__lexer *lexer)
{
    acf__token result = acf__get_token(lexer);
    lexer->current_token_ok = false;
    return result;
}

// acf__parse_value (null | true | false | <numlit> | object | list)
// acf__parse_kv    (identifier '=' value ';'?)
// acf__parse       (kv pairs => object | comma separated values => list (if one value => return it))

acf acf__parse_value(acf__lexer *lexer, bool32 is_top_level);
acf::key_value_pair acf__parse_kv(acf__lexer *lexer);

acf acf__parse_value(acf__lexer *lexer, bool32 is_top_level)
{
    acf result = { NULL };

    acf__token t = acf__get_token(lexer);
    if (t.kind == ACF_TOKEN__NULL)
    {
        acf__eat_token(lexer);

        result = { ALLOCATE(lexer->a, acf_impl) };
        result.set_null();
    }
    else if (t.kind == ACF_TOKEN__TRUE || t.kind == ACF_TOKEN__FALSE)
    {
        acf__eat_token(lexer);

        result = { ALLOCATE(lexer->a, acf_impl) };
        result.set_bool(t.kind == ACF_TOKEN__TRUE ? true : false);
    }
    else if (t.kind == ACF_TOKEN__NUMLIT)
    {
        acf__eat_token(lexer);

        result = { ALLOCATE(lexer->a, acf_impl) };
        result.set_integer(t.integer);
    }
    else if (t.kind == ACF_TOKEN__STRING)
    {
        acf__eat_token(lexer);

        result = { ALLOCATE(lexer->a, acf_impl) };
        result.set_string(t.span);
    }
    else if ((t.kind == '{') || (t.kind == ACF_TOKEN__IDENTIFIER))
    {
        bool32 ok = false;
        bool32 ate_open_brace = false;
        if (t.kind == '{')
        {
            acf__eat_token(lexer);
            ok = true;
            ate_open_brace = true;
        }
        else // t.kind == ACF_TOKEN__IDENTIFIER
        {
            ok = is_top_level;
        }

        if (ok)
        {
            result = { ALLOCATE(lexer->a, acf_impl) };
            result.set_object();

            result.impl->object = ALLOCATE(lexer->a, acf_object);

            if (result.is_valid())
            {
                while (result.get_size() < ARRAY_COUNT(result.impl->object->keys))
                {
                    acf__token id = acf__get_token(lexer);
                    if (id.kind != ACF_TOKEN__IDENTIFIER) { break; };

                    acf::key_value_pair p = acf__parse_kv(lexer);
                    if (p.value.is_valid())
                    {
                        result.push(p.key, p.value);

                        acf__token semicolon = acf__get_token(lexer);
                        if (semicolon.kind == ';')
                        {
                            acf__eat_token(lexer);
                        }
                        else if (semicolon.kind == '}')
                        {
                            break;
                        }
                    }
                    else
                    {
                        break;
                    }
                }

                acf__token brace = acf__get_token(lexer);
                if (brace.kind == '}')
                {
                    if (ate_open_brace)
                    {
                        acf__eat_token(lexer);
                    }
                    else
                    {
                        // Report error
                    }
                }
                else
                {
                    if (ate_open_brace)
                    {
                        // Report error
                    }
                }
            }
        }
        else
        {
            // Report error
        }
    }
    else if (t.kind == '[')
    {
        // Parse list
        acf__eat_token(lexer);
        // ...
    }
    return result;
}

acf::key_value_pair acf__parse_kv(acf__lexer *lexer)
{
    acf::key_value_pair result = {};
    acf__token id = acf__get_token(lexer);
    if (id.kind == ACF_TOKEN__IDENTIFIER)
    {
        acf__eat_token(lexer);
        acf__token eq = acf__get_token(lexer);
        if (eq.kind == '=')
        {
            acf__eat_token(lexer);
            acf value = acf__parse_value(lexer, false);
            if (value.is_valid())
            {
                result.key = string_id::from(id.span);
                result.value = value;
            }
        }
    }
    return result;
}

acf acf::parse(memory_allocator a, memory_block buffer)
{
    acf result = {};
    acf__lexer lexer;
    lexer.a = a;
    lexer.l = make_lexer(buffer);
    lexer.current_token_ok = false;
    result = acf__parse_value(&lexer, true);

    return result;
}


// ==================== public interface ==================== //

bool32 acf::is_valid()    { return (impl && impl->kind != ACF_INVALID); }

bool32 acf::is_null()     { return (impl && impl->kind == ACF_NULL); }
bool32 acf::is_bool()     { return (impl && impl->kind == ACF_BOOL); }
bool32 acf::is_integer()  { return (impl && impl->kind == ACF_INT); }
bool32 acf::is_floating() { return (impl && impl->kind == ACF_FLOAT); }
bool32 acf::is_string()   { return (impl && impl->kind == ACF_STRING); }
bool32 acf::is_object()   { return (impl && impl->kind == ACF_OBJECT && impl->object != NULL); }
bool32 acf::is_list()     { return (impl && impl->kind == ACF_LIST); }

bool32      acf::get_bool()     { return (is_bool() ? impl->boolean : false); }
int64       acf::get_integer()  { return (is_integer() ? impl->integer : 0); }
float64     acf::get_floating() { return (is_floating() ? impl->floating : 0.0); }
string_view acf::get_string()   { return (is_string() ? impl->string : string_view{}); }

bool32      acf::get_bool(bool32 default_value)        { return (is_bool() ? impl->boolean : default_value); }
int64       acf::get_integer(int64 default_value)      { return (is_integer() ? impl->integer : default_value); }
float64     acf::get_floating(float64 default_value)   { return (is_floating() ? impl->floating : default_value); }
string_view acf::get_string(char const *default_value) { return (is_string() ? impl->string : string_view{ default_value, cstring__size_no0(default_value) }); }

acf acf::get_value(char const *cstr) { return get_value(string_id::from(cstr)); }
acf acf::get_value(key_t k)
{
    acf result = {};
    if (is_object())
    {
        for (usize i = 0; i < ARRAY_COUNT(impl->object->keys); i++)
        {
            if (impl->object->keys[i] == k)
            {
                result = acf{ impl->object->vals[i] };
                break;
            }
        }
    }
    return result;
}



uint32 acf::get_size()
{
    if (!is_valid()) return 0;
    if (is_object()) return impl->object->count;
    else return 1;
}

void acf::set_null()
{
    if (!is_valid()) { impl->kind = ACF_NULL; }
}

void acf::set_bool(bool32 value)
{
    if (is_bool() || !is_valid())
    {
        impl->kind = ACF_BOOL;
        impl->boolean = value;
    }
}

void acf::set_integer(int64 value)
{
    if (is_integer() || !is_valid())
    {
        impl->kind = ACF_INT,
        impl->integer = value;
    }
}

void acf::set_floating(float64 value)
{
    if (is_floating() || !is_valid())
    {
        impl->kind = ACF_FLOAT,
        impl->floating = value;
    }
}

void acf::set_string(string_view value)
{
    if (is_string() || !is_valid())
    {
        impl->kind = ACF_STRING,
        impl->string = value;
    }
}

void acf::set_object()
{
    if (is_object() || !is_valid())
    {
        impl->kind = ACF_OBJECT;
    }
}

void acf::push(string_id k, acf v)
{
    if (is_object())
    {
        auto index = impl->object->count;
        impl->object->keys[index] = k;
        impl->object->vals[index] = v.impl;
        impl->object->count += 1;
    }
}

acf::key_value_pair acf::pair_iterator::operator * () const
{
    key_value_pair result = {};
    if ((acf{ impl }).is_object())
    {
        if (index < impl->object->count)
        {
            result.key = impl->object->keys[index];
            result.value = acf{ impl->object->vals[index] };
        }
    }
    return result;
}

// acf::key_t acf::pair_iterator::key() const;
// acf::val_t acf::pair_iterator::value() const;

