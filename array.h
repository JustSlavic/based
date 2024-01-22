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
        struct array_header header__##__LINE__ = ((struct array_header *)(ARRAY)) - 1; \
        if (header__##__LINE__->count < header__##__LINE__->capacity) { \
            (ARRAY)[header__##__LINE__->count++] = (VALUE); \
        } \
    } while (0)
#define array_count(ARRAY) ((((struct array_header *)(ARRAY)) - 1)->count)
#define array_capacity(ARRAY) ((((struct array_header *)(ARRAY)) - 2)->capacity)

#define ALLOCATE_ARRAY_(ALLOCATOR, TYPE, CAPACITY) \
    ({ memory_block blk__##__LINE__ = ALLOCATE_BUFFER_ALIGNED_(ALLOCATOR, \
                                                               sizeof(struct array_header) + sizeof(TYPE)*(CAPACITY), \
                                                               alignof(struct array_header)); \
       TYPE *retval = blk__##__LINE__.memory + 8; \
       array_count(retval) = 0; \
       array_capacity(retval) = (CAPACITY); \
       retval; \
    })
    // ((TYPE *) (((byte *)()) + sizeof(struct array_header)))
#define ALLOCATE_ARRAY(ALLOCATOR, TYPE, CAPACITY) \
    ({ memory_block blk__##__LINE__ = ALLOCATE_BUFFER_ALIGNED(ALLOCATOR, \
                                                              sizeof(struct array_header) + sizeof(TYPE)*(CAPACITY), \
                                                              alignof(struct array_header)); \
       TYPE *retval = (TYPE *) blk__##__LINE__.memory + 8; \
       array_count(retval) = 0; \
       array_capacity(retval) = (CAPACITY); \
       retval; \
    })


#endif // BASED__ARRAY_H
