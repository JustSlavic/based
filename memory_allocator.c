#include "memory_allocator.h"


//
// The memory pages immediately filled with
// +--------------+------------------+--------
// | memory_block | memory_allocator |   ...
// +--------------+------------------+--------
//
// So in the code you would only have 'memory_allocator__system' and 'memory_allocator *'
//
// @note: as the first iteration of the idea, allocators will crash when 'out of memory'
// error happens, but in the future it should ask parent for more memory, or at least
// have some way to decide what to do.
// e.g. struct memory_allocation__policy { ... };
//


#define MINIMAL_MEMORY_BLOCK_SIZE MEGABYTES(1)


typedef enum memory_allocator__type
{
    MEMORY_ALLOCATOR_NONE,
    MEMORY_ALLOCATOR_ARENA,
    MEMORY_ALLOCATOR_STACK,
    MEMORY_ALLOCATOR_POOL,
    MEMORY_ALLOCATOR_HEAP,
} memory_allocator__type;


typedef struct memory_allocator__arena
{
    enum memory_allocator__type type;
    memory_allocator parent;

    byte *memory;
    usize size;
    usize used;
} memory_allocator__arena;


memory_allocator memory_allocator__create_arena_from_memory_block(memory_block block)
{
    memory_block block_for_allocator = memory__advance_block(&block, sizeof(memory_allocator__arena));

    memory_allocator__arena *arena = NULL;
    if (block_for_allocator.size == sizeof(memory_allocator__arena))
    {
        arena = (memory_allocator__arena *) block_for_allocator.memory;

        arena->type = MEMORY_ALLOCATOR_ARENA;
        arena->parent = NULL;
        arena->memory = block.memory;
        arena->size = block.size;
        arena->used = 0;
    }

    return (memory_allocator) arena;
}


memory_allocator memory_allocator__create_arena(memory_allocator parent, usize size)
{
    memory_block block = ALLOCATE_BUFFER_(parent, size + sizeof(memory_allocator__arena));
    memory_allocator result = memory_allocator__create_arena_from_memory_block(block);
    return result;
}


memory_block memory_allocator__allocate_copy(memory_allocator a, memory_block block)
{
    memory_block result = ALLOCATE_BUFFER_ALIGNED_(a, block.size, 8);
    memory__copy(result.memory, block.memory, block.size);
    return result;
}


memory_block memory_allocator__arena_allocate_(memory_allocator a, usize size, usize alignment)
{
    memory_allocator__arena *arena = (memory_allocator__arena *) a;
    struct memory_block result = memory__empty_block();

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


void memory_allocator__arena_reset(memory_allocator a)
{
    memory_allocator__arena *arena = (memory_allocator__arena *) a;
    arena->used = 0;
}


void memory_allocator__initialize_stack(memory_allocator parent, memory_allocator a)
{
    ASSERT_FAIL("Not implemented!");
}

void memory_allocator__initialize_pool(memory_allocator parent, memory_allocator a)
{
    ASSERT_FAIL("Not implemented!");
}

void memory_allocator__initialize_heap(memory_allocator parent, memory_allocator a)
{
    ASSERT_FAIL("Not implemented!");
}


struct memory_block memory_allocator__allocate_(memory_allocator a, usize size, usize alignment)
{
    struct memory_block result;
    enum memory_allocator__type type = *(enum memory_allocator__type *) a;
    switch (type)
    {
        case MEMORY_ALLOCATOR_ARENA: result = memory_allocator__arena_allocate_(a, size, alignment);
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
    struct memory_block result = memory_allocator__allocate_(a, size, alignment);
    if (result.memory)
        memory__set(result.memory, 0, result.size);
    return result;
}

void memory_allocator__deallocate(memory_allocator a, memory_block block)
{
    memory_allocator__type type = *(memory_allocator__type *) a;
    switch (type)
    {
        case MEMORY_ALLOCATOR_ARENA:
            // memory_allocator__arena_deallocate(a, block);
            break;
        case MEMORY_ALLOCATOR_STACK:
        case MEMORY_ALLOCATOR_POOL:
        case MEMORY_ALLOCATOR_HEAP:
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

void memory_allocator__reset(memory_allocator a)
{
    memory_allocator__type type = *(memory_allocator__type *) a;
    switch (type)
    {
        case MEMORY_ALLOCATOR_ARENA:
            memory_allocator__arena_reset(a);
            break;
        case MEMORY_ALLOCATOR_STACK:
        case MEMORY_ALLOCATOR_POOL:
        case MEMORY_ALLOCATOR_HEAP:
            ASSERT_FAIL("Not implemented!");
            break;
        default:
            ASSERT_FAIL();
    }
}
