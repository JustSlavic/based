#include "memory_allocator.hpp"
#include "memory_bucket.hpp"


#define MINIMAL_MEMORY_PAGE_SIZE MEGABYTES(1)


struct memory_arena {
    memory_allocator parent;
    memory_bucket    buffer;
};

memory_buffer memory_arena__allocate_ (memory_arena *a, usize size, usize alignment);
void          memory_arena__reset     (memory_arena *a);


// @todo: memory_stack
// @todo: memory_pool
// @todo: memory_heap


struct mallocator_t {};
static mallocator_t mallocator_instance;

memory_allocator mallocator();
memory_buffer    mallocator__allocate_  (mallocator_t *a, usize size, usize alignment);
void             mallocator__deallocate (mallocator_t *a, void *p);

// ======================================================== //

memory_allocator memory_allocator::make_arena(memory_buffer memory)
{
    memory_allocator result = { ARENA };
    if (memory.size > sizeof(memory_arena))
    {
        auto buffer = memory_bucket::from(memory);
        buffer.used = memory__get_padding(buffer.data, alignof(memory_arena));

        auto *arena_impl = (memory_arena *) (buffer.data + buffer.used);
        buffer.used += sizeof(memory_arena);

        arena_impl->parent.kind = NONE;
        arena_impl->parent.impl = NULL;
        arena_impl->buffer = buffer;

        result.impl = (memory_allocator_impl *) arena_impl;
    }
    return result;
}

memory_allocator memory_allocator::allocate_arena(usize size)
{
    memory_allocator result = { ARENA };
    auto arena_impl = allocate<memory_arena>();
    if (arena_impl)
    {
        auto arena_buffer = allocate_buffer_(size);
        arena_impl->buffer = memory_bucket::from(arena_buffer);
        arena_impl->parent.kind = kind;
        arena_impl->parent.impl = impl;

        result.impl = (memory_allocator_impl *) arena_impl;
    }
    return result;
}

template <typename T>
T *memory_allocator::allocate()
{
    memory_buffer buffer = allocate_buffer(sizeof(T), alignof(T));
    return (T *) buffer.data;
}

memory_buffer memory_allocator::allocate_buffer_(usize size, usize alignment)
{
    memory_buffer result;
    switch (kind)
    {
        case ARENA:
        {
            result = memory_arena__allocate_((memory_arena *) impl, size, alignment);
        }
        break;

        case MALLOC:
        {
            result = mallocator__allocate_((mallocator_t *) impl, size, alignment);
        }
        break;

        default:
            ASSERT_FAIL();
    }
    return result;
}

memory_buffer memory_allocator::allocate_buffer_(usize size)
{
    memory_buffer result = allocate_buffer_(size, 8);
    return result;
}

memory_buffer memory_allocator::allocate_buffer(usize size, usize alignment)
{
    memory_buffer result = allocate_buffer_(size, alignment);
    memory__set(result.data, 0, result.size);
    return result;
}

memory_buffer memory_allocator::allocate_buffer(usize size)
{
    memory_buffer result = allocate_buffer_(size, 8);
    memory__set(result.data, 0, result.size);
    return result;
}

memory_buffer memory_allocator::allocate_copy(void *pointer, usize size)
{
    memory_buffer result = allocate_buffer_(size);
    memory__copy(result.data, pointer, size);
    return result;
}

void memory_allocator::deallocate(void *p, usize size)
{
    switch (kind)
    {
        case ARENA:
        break;

        case MALLOC:
            mallocator__deallocate((mallocator_t *) impl, p);
        break;

        default:
            ASSERT_FAIL();
    }
}

void memory_allocator::deallocate(memory_buffer buffer)
{
    deallocate(buffer.data, buffer.size);
}

template <typename T>
void memory_allocator::deallocate(T *t)
{
    deallocate(t, sizeof(T));
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
            memory_arena__reset((memory_arena *) impl);
        }
        break;

        case MALLOC:
        break;

        default:
            ASSERT_FAIL();
    }
}

memory_allocator::report memory_allocator::get_report()
{
    report result;

    switch (kind)
    {
        case ARENA:
        {
            auto *arena = (memory_arena *) impl;
            result.size = arena->buffer.size;
            result.used = arena->buffer.used;
        }
        break;

        case MALLOC:
            result.size = 0;
            result.used = 0;
        break;

        default:
            ASSERT_FAIL();
    }
    return result;
}

// ======================= memory_arena ======================= //

memory_buffer memory_arena__allocate_(memory_arena *arena, usize size, usize alignment)
{
    memory_buffer result = {};

    auto free_buffer = arena->buffer.get_free();
    usize padding = memory__get_padding(free_buffer.data, alignment);

    if ((padding + size) <= free_buffer.size)
    {
        result.data = free_buffer.data + padding;
        result.size = size;

        arena->buffer.used += (size + padding);
    }

    return result;
}

void memory_arena__reset(memory_arena *arena)
{
    arena->buffer.used = 0;
}

// ======================= mallocator ======================= //

memory_allocator mallocator()
{
    memory_allocator result;
    result.kind = memory_allocator::MALLOC;
    result.impl = (memory_allocator_impl *) &mallocator_instance;
    return result;
}

memory_buffer mallocator__allocate_(mallocator_t *a, usize size, usize alignment)
{
    memory_buffer result;
    result.data = (byte *) malloc(size);
    result.size = size;
    return result;
}

void mallocator__deallocate(mallocator_t *a, void *p)
{
    free(p);
}
