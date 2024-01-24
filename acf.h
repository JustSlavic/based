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


#endif // BASED__ACF_H
