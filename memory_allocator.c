#include "memory_allocator.h"

#if OS_LINUX
#include <malloc.h>
#endif

#define MINIMAL_MEMORY_PAGE_SIZE MEGABYTES(1)


typedef enum memory_allocator_type {
    MEMORY_ALLOCATOR_NONE,
    MEMORY_ALLOCATOR_ARENA,
    MEMORY_ALLOCATOR_STACK,
    MEMORY_ALLOCATOR_POOL,
    MEMORY_ALLOCATOR_HEAP,
    MEMORY_ALLOCATOR_MALLOC,
} memory_allocator_type;


typedef struct memory_arena {
    memory_allocator_type type;
    memory_allocator parent;

    byte *memory;
    usize size;
    usize used;
} memory_arena;

memory_allocator make_memory_arena       (memory_block);
memory_allocator allocate_memory_arena   (memory_allocator, usize);
memory_block     memory_arena__allocate_ (memory_allocator a, usize size, usize alignment);
void             memory_arena__reset     (memory_allocator a);


// @todo: memory_stack
// @todo: memory_pool
// @todo: memory_heap


typedef struct memory_mallocator {
    memory_allocator_type type;
} memory_mallocator;

static memory_mallocator mallocator_instance;

memory_allocator mallocator();
memory_block     mallocator__allocate_  (memory_allocator a, usize size, usize alignment);
void             mallocator__deallocate (memory_allocator a, memory_block);


memory_block memory_allocator__allocate_(memory_allocator a, usize size, usize alignment);
memory_block memory_allocator__allocate (memory_allocator a, usize size, usize alignment);
void         memory_allocator__deallocate(memory_allocator a, memory_block block);
memory_block memory_allocator__reallocate(memory_allocator a, memory_block block, usize new_size);



// memory_arena

memory_allocator make_memory_arena(memory_block block)
{
    memory_allocator result = NULL;
    if (block.size >= MINIMAL_MEMORY_PAGE_SIZE)
    {
        usize padding = memory__get_padding(block.memory, alignof(memory_arena));
        byte *memory_for_allocator = block.memory + padding;

        memory_arena *arena = (memory_arena *) memory_for_allocator;
        arena->type = MEMORY_ALLOCATOR_ARENA;
        arena->parent = NULL;
        arena->memory = memory_for_allocator + sizeof(memory_arena);
        arena->size = block.size - padding - sizeof(memory_arena);
        arena->used = 0;

        result = (memory_allocator) arena;
    }
    return result;
}

memory_allocator allocate_memory_arena(memory_allocator parent, usize requested_arena_size)
{
    memory_allocator result = NULL;

    memory_block memory_for_allocator = ALLOCATE_BUFFER_ALIGNED_(parent, requested_arena_size + sizeof(memory_arena), alignof(memory_arena));
    if (memory_for_allocator.memory != NULL)
    {
        memory_arena *arena = (memory_arena *) memory_for_allocator.memory;
        arena->type = MEMORY_ALLOCATOR_ARENA;
        arena->parent = parent;
        arena->memory = memory_for_allocator.memory + sizeof(memory_arena);
        arena->size = memory_for_allocator.size - sizeof(memory_arena);
        arena->used = 0;

        result = (memory_allocator) arena;
    }
    return result;
}

memory_block memory_arena__allocate_(memory_allocator a, usize size, usize alignment)
{
    memory_arena *arena = (memory_arena *) a;
    memory_block result = memory__empty_block();

    byte *pointer = arena->memory + arena->used;
    usize padding = memory__get_padding(pointer, alignment);

    if ((arena->used + padding + size) <= arena->size)
    {
        result.memory = arena->memory + arena->used + padding;
        result.size = size;
        arena->used += (size + padding);
    }
    else
    {
        ASSERT_FAIL("Out of memory!"); // @todo: report error or allocate more memory and try again
    }

    return result;
}

void memory_arena__reset(memory_allocator a)
{
    memory_allocator_type type = *(memory_allocator_type *) a;
    if (type == MEMORY_ALLOCATOR_ARENA)
    {
        memory_arena *arena = (memory_arena *) a;
        arena->used = 0;
    }
}

// mallocator

memory_allocator mallocator()
{
    mallocator_instance.type = MEMORY_ALLOCATOR_MALLOC;
    return (memory_allocator) &mallocator_instance;
}

memory_block mallocator__allocate_(memory_allocator a, usize size, usize alignment)
{
    memory_block result;
    result.memory = (byte *) malloc(size);
    result.size   = size;
    return result;
}

void mallocator__deallocate(memory_allocator a, memory_block block)
{
    memory_allocator_type type = *(memory_allocator_type *) a;
    if (type == MEMORY_ALLOCATOR_MALLOC)
    {
        free(block.memory);
    }
}


// memory_allocator

struct memory_block memory_allocator__allocate_(memory_allocator a, usize size, usize alignment)
{
    struct memory_block result;
    memory_allocator_type type = *(memory_allocator_type *) a;
    switch (type)
    {
        case MEMORY_ALLOCATOR_ARENA:
            result = memory_arena__allocate_(a, size, alignment);
            break;
        case MEMORY_ALLOCATOR_MALLOC:
            result = mallocator__allocate_(a, size, alignment);
            break;
        case MEMORY_ALLOCATOR_STACK:
        case MEMORY_ALLOCATOR_POOL:
        case MEMORY_ALLOCATOR_HEAP:
        default:
            result = memory__empty_block();
            ASSERT_FAIL();
    }
    return result;
}

struct memory_block memory_allocator__allocate(memory_allocator a, usize size, usize alignment)
{
    memory_block result = memory_allocator__allocate_(a, size, alignment);
    if (result.memory)
    {
        memory__set(result.memory, 0, result.size);
    }
    return result;
}

void memory_allocator__deallocate(memory_allocator a, memory_block block)
{
    memory_allocator_type type = *(memory_allocator_type *) a;
    switch (type)
    {
        case MEMORY_ALLOCATOR_ARENA:
            break; // @note: do nothing for memory arena
        case MEMORY_ALLOCATOR_STACK:
        case MEMORY_ALLOCATOR_POOL:
        case MEMORY_ALLOCATOR_HEAP:
            ASSERT_FAIL();
            break;
        case MEMORY_ALLOCATOR_MALLOC:
            mallocator__deallocate(a, block);
            break;
        default:
            ASSERT_FAIL();
    }
}

memory_block memory_allocator__reallocate(memory_allocator a, memory_block block, usize new_size)
{
    ASSERT_FAIL("Not implemented!");
    memory_block result = memory__empty_block();
    return result;
}

memory_block memory_allocator__allocate_copy(memory_allocator a, memory_block block)
{
    memory_block result = ALLOCATE_BUFFER_ALIGNED_(a, block.size, 8);
    memory__copy(result.memory, block.memory, block.size);
    return result;
}

struct memory_allocator__report memory_allocator__report(memory_allocator a)
{
    struct memory_allocator__report report;

    memory_allocator_type type = *(memory_allocator_type *) a;
    switch (type)
    {
        case MEMORY_ALLOCATOR_ARENA:
        {
            memory_arena *arena = (memory_arena *) a;
            report.size = arena->size;
            report.used = arena->used;
        }
        break;

        case MEMORY_ALLOCATOR_STACK:
        case MEMORY_ALLOCATOR_POOL:
        case MEMORY_ALLOCATOR_HEAP:

        case MEMORY_ALLOCATOR_MALLOC:
        {
#if OS_LINUX
            struct mallinfo2 info = mallinfo2();
            report.size = info.uordblks;
            report.used = report.size - info.fordblks;
#else
            report.size = 0;
            report.used = 0;
#endif
        }
        break;

        default:
            memory__set(&report, 0, sizeof(report));
            ASSERT_FAIL();
    }

    return report;
}


