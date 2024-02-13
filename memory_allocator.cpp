#include "memory_allocator.hpp"
#include "memory_bucket.hpp"


#define MINIMAL_MEMORY_PAGE_SIZE MEGABYTES(1)


struct memory_arena
{
    memory_allocator parent;
    memory_bucket    buffer;
};

memory_buffer memory_arena__allocate_ (memory_arena *a, usize size, usize alignment);
void          memory_arena__reset     (memory_arena *a);


// @todo: memory_stack

struct memory_pool
{
    memory_allocator parent;

    memory_buffer buffer;
    uint32 chunk_size;

    void *free_list;
};

memory_buffer memory_pool__allocate_ (memory_pool *a, usize size, usize alignment);
void          memory_pool__deallocate(memory_pool *a, void *p);
void          memory_pool__reset     (memory_pool *a);

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
    auto arena_buffer = allocate_buffer_(size);
    return make_arena(arena_buffer);
}

memory_allocator memory_allocator::make_pool(memory_buffer memory, usize chunk_size)
{
    memory_allocator result = { POOL };

    auto buffer = memory_bucket::from(memory);
    buffer.used = memory__get_padding(buffer.data, alignof(memory_pool));

    auto *pool_impl = (memory_pool *) (buffer.data + buffer.used);
    buffer.used += sizeof(memory_pool);

    void *first_chunk = NULL;
    void *chunk = NULL;

    // Allocate first chunk
    {
        buffer.used += memory__get_padding(buffer.data + buffer.used, alignof(void *));
        first_chunk = (void *) (buffer.data + buffer.used);
        chunk = first_chunk;
        buffer.used += chunk_size;
    }

    while ((buffer.used + 8) + chunk_size < buffer.size)
    {
        buffer.used += memory__get_padding(buffer.data + buffer.used, alignof(void *));
        *(void **) chunk = (void *) (buffer.data + buffer.used);
        chunk = (void *) (buffer.data + buffer.used);
        buffer.used += chunk_size;
    }

    pool_impl->buffer = memory;
    pool_impl->chunk_size = chunk_size;
    pool_impl->free_list = first_chunk;

    result.impl = (memory_allocator_impl *) pool_impl;
    return result;
}

memory_allocator memory_allocator::allocate_pool(usize size, usize chunk_size)
{
    auto pool_buffer = allocate_buffer_(size);
    return make_pool(pool_buffer, chunk_size);
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

        case POOL:
        {
            result = memory_pool__allocate_((memory_pool *) impl, size, alignment);
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

        case POOL:
            memory_pool__deallocate((memory_pool *) impl, p);
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

// ======================= memory_pool ======================= //

memory_buffer memory_pool__allocate_(memory_pool *pool, usize, usize alignment)
{
    memory_buffer result = {};
    if (pool->free_list)
    {
        result.data = (byte *) pool->free_list;
        result.size = pool->chunk_size;

        pool->free_list = *(void **) pool->free_list;
    }

    return result;
}

void memory_pool__deallocate(memory_pool *a, void *p)
{
    *(void **) p = a->free_list;
    a->free_list = p;
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
