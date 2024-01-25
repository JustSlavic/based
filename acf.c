#include "acf.h"
#include "lexer.h"


struct acf
{
    enum
    {
        ACF__NULL,
        ACF__BOOL,
        ACF__INT,
        ACF__FLOAT,
        ACF__STRING,
        ACF__OBJECT,
        ACF__LIST,
    } kind;
    union
    {
        bool32      boolean;
        float64     floating;
        int64       integer;
        string_view string_;
        struct
        {
            struct acf *vals[32];
            string_view keys[32];
            uint32      pair_count;
        };
    };
};
typedef struct acf acf;

bool32 acf__is_null(acf *t)
{
    bool32 result = (t && t->kind == ACF__NULL);
    return result;
}

bool32 acf__is_bool(acf *t)
{
    bool32 result = (t && t->kind == ACF__BOOL);
    return result;
}

bool32 acf__is_integer(acf *t)
{
    bool32 result = (t && t->kind == ACF__INT);
    return result;
}

bool32 acf__is_floating(acf *t)
{
    bool32 result = (t && t->kind == ACF__FLOAT);
    return result;
}

bool32 acf__is_string(acf *t)
{
    bool32 result = (t && t->kind == ACF__STRING);
    return result;
}

bool32 acf__is_object(acf *t)
{
    bool32 result = (t && t->kind == ACF__OBJECT);
    return result;
}

bool32 acf__is_list(acf *t)
{
    bool32 result = (t && t->kind == ACF__LIST);
    return result;
}

bool32 acf__get_bool(acf *t)
{
    bool32 result = 0;
    if (acf__is_bool(t))
    {
        result = t->boolean;
    }
    return result;
}

int64 acf__get_integer(acf *t)
{
    int64 result = 0;
    if (acf__is_integer(t))
    {
        result = t->integer;
    }
    return result;
}


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
        consume_while(&lexer->l, is_ascii_space);

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
                result.span.data = (char *) (lexer->l.buffer.memory + lexer->l.cursor - 4);
                result.span.size = 4;
            }
            else
            {
                result.kind = ACF_TOKEN__IDENTIFIER;
                result.span.data = (char *) (lexer->l.buffer.memory + lexer->l.cursor);
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

acf *acf__parse_value(acf__lexer *lexer, bool32 is_top_level);
acf *acf__parse_kv(acf__lexer *lexer, string_view *k);

acf *acf__parse_value(acf__lexer *lexer, bool32 is_top_level)
{
    acf *result = NULL;

    acf__token t = acf__get_token(lexer);
    if (t.kind == ACF_TOKEN__NULL)
    {
        result = ALLOCATE(lexer->a, acf);
        if (result)
        {
            acf__eat_token(lexer);
            result->kind = ACF__NULL;
        }
    }
    else if (t.kind == ACF_TOKEN__TRUE || t.kind == ACF_TOKEN__FALSE)
    {
        result = ALLOCATE(lexer->a, acf);
        if (result)
        {
            acf__eat_token(lexer);
            result->kind = ACF__BOOL;
            result->boolean = ACF_TOKEN__TRUE ? true : false;
        }
    }
    else if (t.kind == ACF_TOKEN__NUMLIT)
    {
        result = ALLOCATE(lexer->a, acf);
        if (result)
        {
            acf__eat_token(lexer);
            result->kind = ACF__INT;
            result->integer = t.integer;
        }
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
            // Parse kv pairs
            result = ALLOCATE(lexer->a, acf);
            result->kind = ACF__OBJECT;

            if (result)
            {
                while (result->pair_count < ARRAY_COUNT(result->keys))
                {
                    acf__token id = acf__get_token(lexer);
                    if (id.kind != ACF_TOKEN__IDENTIFIER) { break; };

                    string_view k;
                    acf *v = acf__parse_kv(lexer, &k);
                    if (v)
                    {
                        result->keys[result->pair_count] = k;
                        result->vals[result->pair_count] = v;
                        result->pair_count += 1;

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

acf *acf__parse_kv(acf__lexer *lexer, string_view *k)
{
    acf *result = NULL;
    acf__token id = acf__get_token(lexer);
    if (id.kind == ACF_TOKEN__IDENTIFIER)
    {
        acf__eat_token(lexer);
        acf__token eq = acf__get_token(lexer);
        if (eq.kind == '=')
        {
            acf__eat_token(lexer);
            result = acf__parse_value(lexer, false);
            if (result && k)
            {
                *k = id.span;
            }
        }
    }
    return result;
}

acf *acf__parse(memory_allocator a, memory_block buffer)
{
    acf *result = NULL;
    acf__lexer lexer = {
        .a = a,
        .l = make_lexer(buffer),
        .current_token_ok = false,
    };
    result = acf__parse_value(&lexer, true);

    return result;
}

acf__object_iterator acf__get_pairs(acf *t)
{
    acf__object_iterator result = {
        .object = t,
        .index = 0,
        .count = t->pair_count,
    };
    return result;
}

bool32 acf__next_pair(acf__object_iterator *it)
{
    bool32 result = false;
    if (it->index < it->count)
    {
        it->index += 1;
        result = true;
    }
    return result;
}

bool32 acf__get_key(acf__object_iterator *it, string_view *k)
{
    bool32 result = false;
    if (it->index < it->count)
    {
        *k = it->object->keys[it->index];
        result = true;
    }
    return result;
}

bool32 acf__get_val(acf__object_iterator *it, acf **v)
{
    bool32 result = false;
    if (it->index < it->count)
    {
        *v = it->object->vals[it->index];
        result = true;
    }
    return result;
}
