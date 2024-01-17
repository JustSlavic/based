#ifndef BASED__ARRAY_H
#define BASED__ARRAY_H

#include <base.h>


struct array_header
{
    uint32 count;
    uint32 capacity;
};

#define array(TYPE) TYPE *
#define array_push(ARRAY, VALUE) \
    do { \
        struct array_header header__ = ((struct array_header *)(ARRAY)) - 1; \
        if (header__->count < header__->capacity) { \
            (ARRAY)[header__->count++] = (VALUE); \
        } \
    } while (0)

// #define ALLOCATE_ARRAY_(ALLOCATOR, TYPE, CAPACITY)
//     do {
//         memory_block blk = ALLOCATE_BUFFER(ALLOCATOR, )
//     } while (0)


#endif // BASED__ARRAY_H
