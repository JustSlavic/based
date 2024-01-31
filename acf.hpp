#ifndef BASED__ACF_H
#define BASED__ACF_H

#include "base.h"
#include "memory_allocator.h"
#include "string_view.hpp"
#include "string_id.hpp"


struct acf_impl;
struct acf
{
    typedef string_id key_t;
    typedef acf       val_t;

    acf_impl *impl;

    bool32 is_valid();

    bool32 is_null();
    bool32 is_bool();
    bool32 is_integer();
    bool32 is_floating();
    bool32 is_string();
    bool32 is_object();
    bool32 is_list();

    bool32      to_bool();
    int64       to_integer();
    float64     to_floating();
    string_view to_string();
    bool32      to_bool(bool32 default_value);
    int64       to_integer(int64 default_value);
    float64     to_floating(float64 default_value);
    string_view to_string(char const *default_value);
    
    acf get_value(char const *cstr);
    acf get_value(key_t k);

    uint32 get_size();

    void set_null();
    void set_bool(bool32 value);
    void set_integer(int64 value);
    void set_floating(float64 value);
    void set_string(string_view value);
    void set_object();
    void set_list();

    void push(key_t k, val_t v);
    void push(val_t v);

    struct key_value_pair;
    struct pair_iterator;
    struct pair_iterator_proxy;
    struct value_iterator;
    struct value_iterator_proxy;

    pair_iterator_proxy pairs();
    value_iterator_proxy values();

    static acf parse(memory_allocator a, memory_block buffer);
};

struct acf::key_value_pair
{
    key_t key;
    val_t value;
};

struct acf::pair_iterator
{
    acf_impl *impl;
    uint32 index;

    pair_iterator& operator ++ () { index += 1; return *this; }
    pair_iterator  operator ++ (int) { pair_iterator old = *this; index += 1; return old; }

    pair_iterator& operator -- () { index -= 1; return *this; }
    pair_iterator  operator -- (int) { pair_iterator old = *this; index -= 1; return old; }

    bool operator == (const pair_iterator& other) const { return (impl == other.impl) && (index == other.index); }
    bool operator != (const pair_iterator& other) const { return !(*this == other); }

    key_value_pair operator * () const;
    key_t key() const;
    val_t value() const;
};

struct acf::pair_iterator_proxy
{
    acf_impl *impl;

    pair_iterator begin() { pair_iterator it; it.impl = impl; it.index = 0; return it; }
    pair_iterator end()   { pair_iterator it; it.impl = impl; it.index = (acf{ impl }).get_size(); return it; }
};

struct acf::value_iterator
{
    acf_impl *impl;
    uint32 index;

    value_iterator& operator ++ () { index += 1; return *this; }
    value_iterator  operator ++ (int) { value_iterator old = *this; index += 1; return old; }

    value_iterator& operator -- () { index -= 1; return *this; }
    value_iterator  operator -- (int) { value_iterator old = *this; index -= 1; return old; }

    bool operator == (const value_iterator& other) const { return (impl == other.impl) && (index == other.index); }
    bool operator != (const value_iterator& other) const { return !(*this == other); }

    val_t operator * () const;
};

struct acf::value_iterator_proxy
{
    acf_impl *impl;

    value_iterator begin() { value_iterator it; it.impl = impl; it.index = 0; return it; }
    value_iterator end()   { value_iterator it; it.impl = impl; it.index = (acf{ impl }).get_size(); return it; }
};

acf::pair_iterator_proxy acf::pairs() { return acf::pair_iterator_proxy{ impl }; }
acf::value_iterator_proxy acf::values() { return acf::value_iterator_proxy{ impl }; }

#endif // BASED__ACF_H
