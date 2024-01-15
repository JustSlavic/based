#ifndef BASED__MEMORY_ALLOCATOR_H
#define BASED__MEMORY_ALLOCATOR_H

#include "base.h"
#include "memory.h"

#define ALLOCATE_TYPE_IN_BLOCK_(ALLOCATOR, TYPE) memory_allocator__allocate_(ALLOCATOR, sizeof(TYPE), alignof(TYPE))
#define ALLOCATE_TYPE_IN_BLOCK(ALLOCATOR, TYPE) memory_allocator__allocate(ALLOCATOR, sizeof(TYPE), alignof(TYPE))
#define ALLOCATE_(ALLOCATOR, TYPE) (TYPE *) ALLOCATE_TYPE_IN_BLOCK_(ALLOCATOR, TYPE).memory
#define ALLOCATE(ALLOCATOR, TYPE) (TYPE *) ALLOCATE_TYPE_IN_BLOCK(ALLOCATOR, TYPE).memory
#define DEALLOCATE(ALLOCATOR, POINTER) memory_allocator__deallocate(ALLOCATOR, POINTER)
#define DEALLOCATE_BLOCK(ALLOCATOR, BLOCK) memory_allocator__deallocate(ALLOCATOR, BLOCK)
#define REALLOCATE(ALLOCATOR, BLOCK, NEW_SIZE) memory_allocator__reallocate(ALLOCATOR, BLOCK, NEW_SIZE)

#define ALLOCATE_BUFFER_(ALLOCATOR, SIZE) memory_allocator__allocate_(ALLOCATOR, SIZE, 1)
#define ALLOCATE_BUFFER(ALLOCATOR, SIZE) memory_allocator__allocate(ALLOCATOR, SIZE, 1)

#define ALLOCATE_BUFFER_ALIGNED_(ALLOCATOR, SIZE, ALIGNMENT) memory_allocator__allocate_(ALLOCATOR, SIZE, ALIGNMENT)
#define ALLOCATE_BUFFER_ALIGNED(ALLOCATOR, SIZE, ALIGNMENT) memory_allocator__allocate(ALLOCATOR, SIZE, ALIGNMENT)

#define ALLOCATE_COPY(ALLOCATOR, BLOCK) memory_allocator__allocate_copy(ALLOCATOR, BLOCK)


typedef struct memory_allocator_t *memory_allocator;

memory_allocator make_memory_arena     (memory_block);
memory_allocator allocate_memory_arena (memory_allocator, usize);
void             memory_arena__reset   (memory_allocator a);

// @todo: memory_stack
// @todo: memory_pool
// @todo: memory_heap

memory_allocator mallocator(void);

memory_block memory_allocator__allocate_     (memory_allocator a, usize size, usize alignment);
memory_block memory_allocator__allocate      (memory_allocator a, usize size, usize alignment);
void         memory_allocator__deallocate    (memory_allocator a, memory_block block);
memory_block memory_allocator__reallocate    (memory_allocator a, memory_block block, usize new_size);
memory_block memory_allocator__allocate_copy (memory_allocator a, memory_block block);

struct memory_allocator__report
{
    usize size;
    usize used;
};

struct memory_allocator__report memory_allocator__report(memory_allocator a);


#endif // BASED__MEMORY_ALLOCATOR_H
