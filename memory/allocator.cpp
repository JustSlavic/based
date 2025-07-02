#include "allocator.hpp"

#include "memory.h"

#include <stdlib.h>
#include <string.h>


#define MINIMAL_MEMORY_PAGE_SIZE KILOBYTES(100)


/* ============= MEMORY ARENA ============= */

struct memory_arena
{
    memory_allocator *parent;
    usize size;
    usize used;

    bool32 print_debug;
};

#if DEBUG
memory_buffer memory_arena__allocate_(memory_arena *a, usize size, usize alignment, code_location cl)
#else
memory_buffer memory_arena__allocate_(memory_arena *a, usize size, usize alignment)
#endif
{
    memory_buffer result = {};

    byte *data = (byte *) (a + 1);

    usize padding = get_padding(data + a->used, alignment);
    if (a->used + padding + size <= a->size)
    {
        result.data = data + a->used + padding;
        result.size = size;

        a->used += (size + padding);
    }

    if (a->print_debug)
        printf("allocated %10llu bytes at 0x%p at %s:%d (%s)\n", result.size, result.data,
            cl.filename, cl.line, cl.function);

    return result;
}

void memory_arena__reset(memory_arena *a)
{
    a->used = 0;
}

/* ============================== MEMORY STACK =================================== */
// @todo
/* ============================== MEMORY POOL ==================================== */
// @todo
/* ============================== MEMORY HEAP ==================================== */
// @todo
/* ============================= MEMORY MALLOC =================================== */

memory_allocator mallocator()
{
    memory_allocator result;
    result.kind = memory_allocator::MALLOC;
    result.opaque = NULL;
    return result;
}

memory_buffer mallocator__allocate_(usize size, usize alignment)
{
    memory_buffer result;
    result.data = (byte *) malloc(size);
    result.size = size;
    return result;
}

void mallocator__deallocate(void *p)
{
    free(p);
}

/* =============================================================================== */

memory_allocator memory_allocator::make_arena(memory_buffer memory, memory_allocator *parent)
{
    ASSERT(memory.size > (sizeof(memory_arena) + MINIMAL_MEMORY_PAGE_SIZE));

    memory_allocator result = {};
    {
        usize padding = get_padding(memory.data, alignof(memory_arena));
        auto *arena = (memory_arena *) (memory.data + padding);
        arena->parent = parent;
        arena->size = memory.size - (sizeof(memory_arena) + padding);
        arena->used = 0;

        result.kind = memory_allocator::ARENA;
        result.opaque = arena;
    }
    return result;
}

memory_allocator memory_allocator::allocate_arena(usize size)
{
    memory_allocator result = {};

#if DEBUG
    auto memory = allocate_buffer_(size, alignof(memory_allocator), CL_HERE);
#else
    auto memory = allocate_buffer_(size, alignof(memory_allocator));
#endif
    if (memory.data)
    {
        result = make_arena(memory, this);
    }
    return result;
}

#if DEBUG
memory_buffer memory_allocator::allocate_buffer_(usize size, usize alignment, code_location cl)
#else
memory_buffer memory_allocator::allocate_buffer_(usize size, usize alignment)
#endif
{
    memory_buffer result = {};
    switch (kind)
    {
        case ARENA:
        {
#if DEBUG
            result = memory_arena__allocate_((memory_arena *) opaque, size, alignment, cl);
#else
            result = memory_arena__allocate_((memory_arena *) opaque, size, alignment);
#endif
        }
        break;

        case MALLOC:
        {
            result = mallocator__allocate_(size, alignment);
        }
        break;

        default:
            ASSERT_FAIL("Unknown kind of allocator");
    }
    return result;
}

#if DEBUG
memory_buffer memory_allocator::allocate_buffer_(usize size, code_location cl)
{
    memory_buffer result = allocate_buffer_(size, 8, cl);
    return result;
}
#else
memory_buffer memory_allocator::allocate_buffer_(usize size)
{
    memory_buffer result = allocate_buffer_(size, 8);
    return result;
}
#endif

#if DEBUG
memory_buffer memory_allocator::allocate_buffer(usize size, usize alignment, code_location cl)
{
    memory_buffer result = allocate_buffer_(size, alignment, cl);
    memset(result.data, 0, result.size);
    return result;
}
#else
memory_buffer memory_allocator::allocate_buffer(usize size, usize alignment)
{
    memory_buffer result = allocate_buffer_(size, alignment);
    memset(result.data, 0, result.size);
    return result;
}
#endif

#if DEBUG
memory_buffer memory_allocator::allocate_buffer(usize size, code_location cl)
{
    memory_buffer result = allocate_buffer_(size, 8, cl);
    memset(result.data, 0, result.size);
    return result;
}
#else
memory_buffer memory_allocator::allocate_buffer(usize size)
{
    memory_buffer result = allocate_buffer_(size, 8);
    memset(result.data, 0, result.size);
    return result;
}
#endif

memory_buffer memory_allocator::allocate_copy(void *pointer, usize size)
{
    memory_buffer result = allocate_buffer_(size, CL_HERE);
    memcpy(result.data, pointer, size);
    return result;
}

void memory_allocator::deallocate(void *p, usize size)
{
    switch (kind)
    {
        case ARENA:
        break;

        case MALLOC:
            mallocator__deallocate(p);
        break;

        default:
            ASSERT_FAIL("Unknown kind of allocator");
    }
}

void memory_allocator::deallocate(memory_buffer buffer)
{
    deallocate(buffer.data, buffer.size);
}

memory_buffer memory_allocator::reallocate(memory_buffer, usize new_size)
{
    NOT_IMPLEMENTED();
    return memory_buffer{};
}

void memory_allocator::reset()
{
    switch (kind)
    {
        case ARENA:
        {
            memory_arena__reset((memory_arena *) opaque);
        }
        break;

        case MALLOC:
        break;

        default:
            ASSERT_FAIL("memory_allocator::reset() - unknown kind of allocator");
    }
}

void memory_allocator::print_debug()
{
    if (kind == ARENA)
    {
        auto *arena = (memory_arena *) opaque;
        arena->print_debug = true;
    }
}
void memory_allocator::silence_debug()
{
    if (kind == ARENA)
    {
        auto *arena = (memory_arena *) opaque;
        arena->print_debug = false;
    }
}

memory_allocator::report memory_allocator::get_report()
{
    report result = {};

    switch (kind)
    {
        case ARENA:
        {
            auto *arena = (memory_arena *) opaque;
            result.kind = ARENA;
            result.size = arena->size;
            result.used = arena->used;
        }
        break;

        case MALLOC:
            result.size = 0;
            result.used = 0;
        break;

        default:
            ASSERT_FAIL("memory_allocator::get_report() - unknown kind of allocator");
    }
    return result;
}
