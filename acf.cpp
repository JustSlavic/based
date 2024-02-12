#include "acf.hpp"
#include "lexer.hpp"

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

#define ACF_MAX_SIZE 32

struct acf_object
{
    acf_impl *vals[ACF_MAX_SIZE];
    string_id keys[ACF_MAX_SIZE];
    uint32 count;
};

struct acf_list
{
    acf_impl *vals[ACF_MAX_SIZE];
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
        acf_list   *list;
    };
};





enum acf_token_kind
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

struct acf_token
{
    acf_token_kind kind;
    int64 integer;
    string_view span;
};

struct acf_parser : lexer
{
    memory_allocator allocator;

    acf_token current_token;
    bool is_current_token_ok;

    static acf_parser from(memory_allocator, memory_buffer);

    acf_token get_token();
    acf_token eat_token();

    acf parse_value(bool32 is_top_level);
    acf::key_value_pair parse_kv();
};

acf_parser acf_parser::from(memory_allocator a, memory_buffer buffer)
{
    acf_parser result;
    *(lexer *) &result = lexer::from(buffer.data, buffer.size);
    result.allocator = a;
    result.is_current_token_ok = false;
    return result;
}

acf_token acf_parser::get_token()
{
    if (is_current_token_ok) { return current_token; }

    consume_while(is_ascii_whitespace);

    acf_token result = {};
    char c = get_char();
    if (c == 0)
    {
        result.kind = ACF_TOKEN__EOF;
    }
    else if (is_valid_identifier_head(c))
    {
        if (eat_string("null"))
        {
            result.kind = ACF_TOKEN__NULL;
            result.span.data = get_remaining_input() - 4;
            result.span.size = 4;
        }
        else if (eat_string("true"))
        {
            result.kind = ACF_TOKEN__TRUE;
            result.span.data = get_remaining_input() - 4;
            result.span.size = 4;
        }
        else if (eat_string("false"))
        {
            result.kind = ACF_TOKEN__FALSE;
            result.span.data = get_remaining_input() - 5;
            result.span.size = 5;
        }
        else
        {
            result.kind = ACF_TOKEN__IDENTIFIER;
            result.span.data = get_remaining_input();
            result.span.size = consume_while(is_valid_identifier_body);
        }
    }
    else if (is_ascii_digit(c))
    {
        int64 integral_part = 0;
        while (is_ascii_digit(c))
        {
            eat_char();
            integral_part *= 10;
            integral_part += c - '0';
            c = get_char();
        }
        result.kind = ACF_TOKEN__NUMLIT;
        result.integer = integral_part;
    }
    else if (c == '"')
    {
        eat_char();
        result.kind = ACF_TOKEN__STRING;
        result.span.data = get_remaining_input();
        result.span.size = consume_until(is_ascii_double_quote);
        eat_char();
    }
    else
    {
        result.kind = (enum acf_token_kind) c;
        eat_char();
    }

    current_token = result;
    is_current_token_ok = true;
    return result;
}

acf_token acf_parser::eat_token()
{
    acf_token result = get_token();
    is_current_token_ok = false;
    return result;
}

acf acf_parser::parse_value(bool32 is_top_level)
{
    acf result = {};

    auto t = get_token();
    if (t.kind == ACF_TOKEN__NULL)
    {
        eat_token();

        result.impl = allocator.allocate<acf_impl>();
        result.set_null();
    }
    else if (t.kind == ACF_TOKEN__TRUE || t.kind == ACF_TOKEN__FALSE)
    {
        eat_token();

        result.impl = allocator.allocate<acf_impl>();
        result.set_bool(t.kind == ACF_TOKEN__TRUE ? true : false);
    }
    else if (t.kind == ACF_TOKEN__NUMLIT)
    {
        eat_token();

        result.impl = allocator.allocate<acf_impl>();
        result.set_integer(t.integer);
    }
    else if (t.kind == ACF_TOKEN__STRING)
    {
        eat_token();

        result.impl = allocator.allocate<acf_impl>();
        result.set_string(t.span);
    }
    else if ((t.kind == '{') || (t.kind == ACF_TOKEN__IDENTIFIER))
    {
        bool32 ok = false;
        bool32 ate_open_brace = false;
        if (t.kind == '{')
        {
            eat_token();
            ok = true;
            ate_open_brace = true;
        }
        else // t.kind == ACF_TOKEN__IDENTIFIER
        {
            ok = is_top_level;
        }

        if (ok)
        {
            result.impl = allocator.allocate<acf_impl>();
            result.set_object();

            result.impl->object = allocator.allocate<acf_object>();

            if (result.is_valid())
            {
                while (result.get_size() < ACF_MAX_SIZE)
                {
                    acf_token id = get_token();
                    if (id.kind != ACF_TOKEN__IDENTIFIER) { break; };

                    auto kv = parse_kv();
                    if (kv.value.is_valid())
                    {
                        result.push(kv.key, kv.value);

                        acf_token semicolon = get_token();
                        if (semicolon.kind == ';')
                        {
                            eat_token();
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

                acf_token brace = get_token();
                if (brace.kind == '}')
                {
                    if (ate_open_brace)
                    {
                        eat_token();
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
        eat_token();

        result.impl = allocator.allocate<acf_impl>();
        result.set_list();

        result.impl->list = allocator.allocate<acf_list>();

        if (result.is_valid())
        {
            while (result.get_size() < ARRAY_COUNT(result.impl->list->vals))
            {
                acf item = parse_value(false);
                if (item.is_valid())
                {
                    result.push(item);

                    auto comma = get_token();
                    if (comma.kind == ',')
                    {
                        eat_token();
                    }
                    else if (comma.kind == ']')
                    {
                        break;
                    }
                }
                else
                {
                    break;
                }
            }

            auto bracket = get_token();
            if (bracket.kind == ']')
            {
                eat_token();
            }
            else
            {
                // Report error
            }
        }
    }
    return result;
}

acf::key_value_pair acf_parser::parse_kv()
{
    acf::key_value_pair result = {};

    auto id = get_token();
    if (id.kind == ACF_TOKEN__IDENTIFIER)
    {
        eat_token();
        acf_token eq = get_token();
        if (eq.kind == '=')
        {
            eat_token();
            acf value = parse_value(false);
            if (value.is_valid())
            {
                result.key = string_id::from(id.span.data, id.span.size);
                result.value = value;
            }
        }
    }
    return result;
}

acf acf::parse(memory_allocator a, memory_buffer buffer)
{
    acf_parser parser = acf_parser::from(a, buffer);
    acf result = parser.parse_value(true);

    auto eof = parser.get_token();
    if (eof.kind != ACF_TOKEN__EOF)
    {
        {
            acf list_value;
            list_value.impl = a.allocate<acf_impl>();
            list_value.set_list();

            list_value.impl->list = a.allocate<acf_list>();

            list_value.push(result);
            result = list_value;
        }

        if (eof.kind == ',')
        {
            parser.eat_token();
        }

        while (result.get_size() < ACF_MAX_SIZE)
        {
            acf item = parser.parse_value(false);
            if (item.is_valid())
            {
                result.push(item);

                auto comma = parser.get_token();
                if (comma.kind == ',')
                {
                    parser.eat_token();
                }
                else if (comma.kind == ']' || comma.kind == ACF_TOKEN__EOF)
                {
                    break;
                }
            }
            else
            {
                break;
            }
        }
    }

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

bool32      acf::to_bool()     { return (is_bool() ? impl->boolean : false); }
int64       acf::to_integer()  { return (is_integer() ? impl->integer : 0); }
float64     acf::to_floating() { return (is_floating() ? impl->floating : 0.0); }
string_view acf::to_string()   { return (is_string() ? impl->string : string_view{}); }

bool32      acf::to_bool(bool32 default_value)        { return (is_bool() ? impl->boolean : default_value); }
int64       acf::to_integer(int64 default_value)      { return (is_integer() ? impl->integer : default_value); }
float64     acf::to_floating(float64 default_value)   { return (is_floating() ? impl->floating : default_value); }
string_view acf::to_string(char const *default_value) { return (is_string() ? impl->string : string_view{ default_value, cstring__size_no0(default_value) }); }

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
    if (is_list()) return impl->list->count;
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

void acf::set_list()
{
    if (is_list() || !is_valid())
    {
        impl->kind = ACF_LIST;
    }
}

void acf::push(key_t k, val_t v)
{
    if (is_object())
    {
        auto index = impl->object->count;
        impl->object->keys[index] = k;
        impl->object->vals[index] = v.impl;
        impl->object->count += 1;
    }
}

void acf::push(val_t v)
{
    if (is_list())
    {
        auto index = impl->list->count;
        impl->list->vals[index] = v.impl;
        impl->list->count += 1;
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

acf::val_t acf::value_iterator::operator * () const
{
    val_t result = {};
    if ((acf{ impl }).is_list())
    {
        if (index < impl->list->count)
        {
            result.impl = impl->list->vals[index];
        }
    }
    return result;
}

