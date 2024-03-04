#include "memory_allocator.hpp"

#include "memory.h"

#include <stdlib.h>
#include <string.h>


#define MINIMAL_MEMORY_PAGE_SIZE MEGABYTES(1)


struct memory_arena
{
    memory_allocator *parent;
    usize size;
    usize used;
    byte *data;
};

memory_buffer memory_arena__allocate_ (memory_arena *a, usize size, usize alignment);
void          memory_arena__reset     (memory_arena *a);


// @todo: memory_stack

struct memory_pool
{
    memory_allocator *parent;

    memory_buffer buffer;
    uint32 chunk_size;
    uint32 chunk_count;
    uint32 chunks_used;

    void *free_list;
};

memory_buffer memory_pool__allocate_ (memory_pool *a, usize size, usize alignment);
void          memory_pool__deallocate(memory_pool *a, void *p);
void          memory_pool__reset     (memory_pool *a);

// struct memory_heap
// {
//     #define HEAP_HEADER_OFFSET_MASK   (((usize) -1) >> 1)
//     #define HEAP_HEADER_OCCUPIED_MASK (((usize)  1) << (8*sizeof(usize) - 1))

//     using header = usize;
//     using footer = usize;
//     using guard  = usize;

//     static usize get_offset(usize *data)
//     {
//         usize result = (*data) & HEAP_HEADER_OFFSET_MASK;
//         return result;
//     }

//     static void set_offset(usize *data, usize chunk_size)
//     {
//         (*data) = ((*data) & HEAP_HEADER_OCCUPIED_MASK) | (chunk_size & HEAP_HEADER_OFFSET_MASK);
//     }

//     static bool32 is_occupied(usize *data)
//     {
//         bool result = (*data) & HEAP_HEADER_OCCUPIED_MASK;
//         return result;
//     }

//     static void set_occupied(usize *data, bool occupied)
//     {
//         (*data) = ((*data) & HEAP_HEADER_OFFSET_MASK) | (((usize) occupied) << (8*sizeof(usize) - 1));
//     }

//     #undef HEAP_HEADER_OFFSET_MASK
//     #undef HEAP_HEADER_OCCUPIED_MASK

//     header *first_header;
//     footer *last_footer;

//     guard *first_guard;
//     guard *last_guard;
// };

memory_allocator *mallocator();
memory_buffer     mallocator__allocate_  (usize size, usize alignment);
void              mallocator__deallocate (void *p);

// ============================================================================= //

memory_allocator memory_allocator::make_arena(memory_buffer memory)
{
    memory_allocator result = { ARENA };
    if (sizeof(opaque) >= sizeof(memory_arena))
    {
        memset(result.opaque, 0, sizeof(opaque));

        auto *arena = (memory_arena *) result.opaque;
        arena->parent = NULL;
        arena->size = memory.size;
        arena->used = 0;
        arena->data = memory.data;
    }
    return result;
}

memory_allocator memory_allocator::allocate_arena(usize size)
{
    memory_allocator result = {};
    auto arena_buffer = allocate_buffer_(size, alignof(memory_allocator));
    if (arena_buffer)
    {
        result = make_arena(arena_buffer);
        auto *arena = (memory_arena *) result.opaque;
        arena->parent = this;
    }
    return result;
}

// ============================================================================= //

memory_allocator memory_allocator::make_pool(memory_buffer memory, uint32 chunk_size)
{
    memory_allocator result = { POOL };

    ASSERT(get_padding(memory.data, alignof(void *)) == 0);

    // Chunk size is rounded up to the closest 8-byte boundary

    chunk_size += (alignof(void *) - (chunk_size & alignof(void *)));

    // Assuming the memory.data is on the 8-byte boundary, and
    // chunk size `mod` 8 == 0, I can safely allocate all chunks
    if (sizeof(opaque) >= sizeof(memory_pool))
    {
        memset(result.opaque, 0, sizeof(opaque));

        auto *pool = (memory_pool *) result.opaque;

        uint32 chunk_count = 0;

        byte *first  = memory.data;
        byte *cursor = memory.data;

        while (true)
        {
            if (memory.size - (chunk_count * chunk_size) < chunk_size)
                break;

            byte *next = cursor + chunk_size;

            *(byte **) cursor = next;
            cursor = next;
            chunk_count += 1;
        }

        pool->parent = NULL;
        pool->buffer = memory;

        pool->chunk_size = chunk_size;
        pool->chunk_count = chunk_count;
        pool->chunks_used = 0;

        pool->free_list = first;
    }

    return result;
}

memory_allocator memory_allocator::allocate_pool(usize size, uint32 chunk_size)
{
    memory_allocator result = {};
    auto pool_buffer = allocate_buffer_(size, alignof(memory_allocator));
    if (pool_buffer)
    {
        result = make_pool(pool_buffer, chunk_size);
        auto *pool = (memory_pool *) result.opaque;
        pool->parent = this;
    }
    return result;
}

// ============================================================================= //

// void initialize_memory_heap(allocator *a, void *memory, usize size)
// {
//     auto *heap = (memory_heap *) a;

//     heap->type = allocator::heap;
//     heap->memory = memory;
//     heap->size = size;
//     heap->used = 0;

//     ASSERT(size > (2*sizeof(memory_heap::guard) + sizeof(memory_heap::header) + sizeof(memory_heap::footer)));

//     usize first_guard_padding = get_padding(memory, alignof(memory_heap::guard));
//     heap->first_guard = (memory_heap::guard *) ((byte *) memory + first_guard_padding);

//     byte *last_guard_address = ((byte *) memory + size - sizeof(memory_heap::guard));
//     usize last_guard_padding = get_padding(last_guard_address, alignof(memory_heap::guard));
//     last_guard_address -= (sizeof(memory_heap::guard) - last_guard_padding);
//     heap->last_guard = (memory_heap::guard *) last_guard_address;

//     heap->used += sizeof(memory_heap::guard) * 2 + last_guard_padding;

//     *heap->first_guard = 0;
//     *heap->last_guard = 0;

//     uintptr first_header_address = (uintptr) ((byte *) heap->first_guard + sizeof(memory_heap::guard));
//     usize offset = (uintptr) last_guard_address - first_header_address;

//     heap->first_header = (memory_heap::header *) first_header_address;
//     memory_heap::set_occupied(heap->first_header, false);
//     memory_heap::set_offset(heap->first_header, offset);

//     heap->last_footer = (memory_heap::footer *) ((byte *) last_guard_address - sizeof(memory_heap::footer));
//     memory_heap::set_occupied(heap->last_footer, false);
//     memory_heap::set_offset(heap->last_footer, offset);

//     heap->used += sizeof(memory_heap::header) + sizeof(memory_heap::footer);
// }

// void initialize_memory_heap(allocator *a, memory_block block)
// {
//     initialize_memory_heap(a, block.memory, block.size);
// }

// void heap_reset(allocator *a)
// {
//     initialize_memory_heap(a, a->memory, a->size);
// }

// void *heap_allocate(allocator *a, usize requested_size, usize alignment)
// {
//     auto *heap = (memory_heap *) a;
//     void *result = NULL;

//     // The minimal allocation size is 8 bytes (have to be for alignment purposes)
//     if (requested_size < sizeof(usize))
//         requested_size = sizeof(usize);

//     auto *header = heap->first_header;
//     while (1)
//     {
//         if (header == heap->last_guard) break;

//         bool32 is_occupied = memory_heap::is_occupied(header);
//         usize offset = memory_heap::get_offset(header);
//         usize chunk_size = offset - 2*sizeof(memory_heap::guard);

//         // [header][memory][footer][guard or offset]
//         // | <--    offset    ---> |
//         //         |      | <- chunk_size

//         if (!is_occupied && (chunk_size >= requested_size))
//         {
//             if (chunk_size >= (requested_size + 4*sizeof(memory_heap::guard)))
//             {
//                 // [header].................................................................[footer]
//                 //         [allocated memory][new footer][new header]...the rest of chunk...
//                 //
//                 // The rest of chunk is at minimum sizeof(memory_heap::guard);
//                 // + also one additional 'sizeof' for padding, so minimal allocation size is 8 bytes,

//                 auto *new_footer = (memory_heap::footer *) ((byte *) header + sizeof(memory_heap::header) + requested_size);
//                 usize new_footer_padding = get_padding(new_footer, alignof(memory_heap::footer));
//                 new_footer = (memory_heap::footer *) ((byte *) new_footer + new_footer_padding);
//                 auto *new_header = (memory_heap::header *) ((byte *) new_footer + sizeof(memory_heap::footer));

//                 usize offset_1 = requested_size + new_footer_padding + sizeof(memory_heap::footer) + sizeof(memory_heap::header);
//                 usize offset_2 = offset - offset_1;

//                 memory_heap::set_offset(header, offset_1);
//                 memory_heap::set_occupied(header, true);
//                 *new_footer = *header;

//                 auto *old_footer = (memory_heap::footer *) ((byte *) header + offset - sizeof(memory_heap::footer));

//                 memory_heap::set_offset(new_header, offset_2);
//                 memory_heap::set_occupied(new_header, false);
//                 *old_footer = *new_header;

//                 result = (byte *) header + sizeof(memory_heap::header);

//                 heap->used += requested_size + new_footer_padding + sizeof(memory_heap::footer) + sizeof(memory_heap::header);
//             }
//             else
//             {
//                 // Allocate in this chunk without dividing it more
//                 auto *footer = (memory_heap::footer *) (header + offset - sizeof(memory_heap::footer));

//                 memory_heap::set_occupied(header, true);
//                 memory_heap::set_occupied(footer, true);

//                 result = (byte *) header + sizeof(memory_heap::header);

//                 heap->used += requested_size;
//             }

//             break;
//         }

//         header = (memory_heap::header *) ((byte *) header + offset);
//     }


//     return result;
// }

// void heap_deallocate(allocator *a, void *memory)
// {
//     auto *heap = (memory_heap *) a;
//     auto *header = (memory_heap::header *) ((byte *) memory - sizeof(memory_heap::header));
//     usize offset = memory_heap::get_offset(header);
//     auto *footer = (memory_heap::footer *) ((byte *) header + offset - sizeof(memory_heap::footer));
//     heap->used -= (offset - sizeof(memory_heap::header) - sizeof(memory_heap::footer));

//     auto *left_footer = (memory_heap::footer *) ((byte *) header - sizeof(memory_heap::footer));
//     if (*left_footer != 0)
//     {
//         if (!memory_heap::is_occupied(left_footer))
//         {
//             usize left_offset = memory_heap::get_offset(left_footer);
//             auto *left_header = (memory_heap::header *) ((byte *) left_footer - left_offset + sizeof(memory_heap::header));
//             header = left_header;

//             offset = left_offset + offset;
//             memory_heap::set_offset(header, offset);
//             memory_heap::set_offset(footer, offset);

//             heap->used -= (sizeof(memory_heap::footer) + sizeof(memory_heap::header));
//         }
//     }

//     auto *right_header = (memory_heap::header *) ((byte *) footer + sizeof(memory_heap::header));
//     if (*right_header != 0)
//     {
//         if (!memory_heap::is_occupied(right_header))
//         {
//             usize right_offset = memory_heap::get_offset(right_header);
//             auto *right_footer = (memory_heap::footer *) ((byte *) right_header + right_offset - sizeof(memory_heap::footer));
//             footer = right_footer;

//             offset = offset + right_offset;
//             memory_heap::set_offset(header, offset);
//             memory_heap::set_offset(footer, offset);

//             heap->used -= (sizeof(memory_heap::footer) + sizeof(memory_heap::header));
//         }
//     }

//     memory_heap::set_occupied(header, false);
//     memory_heap::set_occupied(footer, false);
// }

// @todo
// void *reallocate(allocator *a, void *memory, usize size);

// ============================================================================= //

template <typename T>
T *memory_allocator::allocate()
{
    memory_buffer buffer = allocate_buffer(sizeof(T), alignof(T));
    return (T *) buffer.data;
}

memory_buffer memory_allocator::allocate_buffer_(usize size, usize alignment)
{
    memory_buffer result = {};
    switch (kind)
    {
        case ARENA:
        {
            result = memory_arena__allocate_((memory_arena *) opaque, size, alignment);
        }
        break;

        case POOL:
        {
            result = memory_pool__allocate_((memory_pool *) opaque, size, alignment);
        }
        break;

        case MALLOC:
        {
            result = mallocator__allocate_(size, alignment);
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
    memset(result.data, 0, result.size);
    return result;
}

memory_buffer memory_allocator::allocate_buffer(usize size)
{
    memory_buffer result = allocate_buffer_(size, 8);
    memset(result.data, 0, result.size);
    return result;
}

memory_buffer memory_allocator::allocate_copy(void *pointer, usize size)
{
    memory_buffer result = allocate_buffer_(size);
    memcpy(result.data, pointer, size);
    return result;
}

void memory_allocator::deallocate(void *p, usize size)
{
    switch (kind)
    {
        case ARENA:
        break;

        case POOL:
            memory_pool__deallocate((memory_pool *) opaque, p);
        break;

        case MALLOC:
            mallocator__deallocate(p);
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
            memory_arena__reset((memory_arena *) opaque);
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

        case POOL:
        {
            auto *pool = (memory_pool *) opaque;
            result.kind = POOL;
            result.size = pool->buffer.size;
            result.used = pool->chunk_size * pool->chunks_used;
            result.chunks_used = pool->chunks_used;
            result.chunk_size = pool->chunk_size;
            result.chunk_count = pool->chunk_count;
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

    usize padding = get_padding(arena->data + arena->used, alignment);
    if (arena->used + padding + size <= arena->size)
    {
        result.data = arena->data + arena->used + padding;
        result.size = size;

        arena->used += (size + padding);
    }

    return result;
}

void memory_arena__reset(memory_arena *arena)
{
    arena->used = 0;
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
        pool->chunks_used += 1;
    }

    return result;
}

void memory_pool__deallocate(memory_pool *a, void *p)
{
    *(void **) p = a->free_list;
    a->free_list = p;
    a->chunks_used -= 1;
}

// ======================= mallocator ======================= //

memory_allocator *mallocator()
{
    static memory_allocator result;
    result.kind = memory_allocator::MALLOC;
    return &result;
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
