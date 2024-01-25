#ifndef BASED__ACF_H
#define BASED__ACF_H

#include "base.h"
#include "memory_allocator.h"
#include "string_view.h"


typedef struct acf acf;

acf *acf__parse(memory_allocator a, memory_block buffer);

bool32 acf__is_null(acf *);
bool32 acf__is_bool(acf *);
bool32 acf__is_integer(acf *);
bool32 acf__is_floating(acf *);
bool32 acf__is_string(acf *);
bool32 acf__is_object(acf *);
bool32 acf__is_list(acf *);

bool32 acf__get_bool(acf *t);
int64 acf__get_integer(acf *t);

struct acf__object_iterator
{
    acf *object;
    uint32 index;
    uint32 count;
};
typedef struct acf__object_iterator acf__object_iterator;

acf__object_iterator acf__get_pairs(acf *t);
bool32 acf__next_pair(acf__object_iterator *it);
bool32 acf__get_key(acf__object_iterator *it, string_view *k);
bool32 acf__get_val(acf__object_iterator *it, acf **v);

struct acf__list_iterator
{
    acf *list;
    uint32 index;
};
typedef struct acf__list_iterator acf__list_iterator;

acf__list_iterator acf__get_list(acf *t);
bool32 acf__next(acf__list_iterator *it);


#endif // BASED__ACF_H
