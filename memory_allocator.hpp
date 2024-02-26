#ifndef BASED__MEMORY_ALLOCATOR_H
#define BASED__MEMORY_ALLOCATOR_H

#include "base.h"
#include "memory_buffer.hpp"
#include "array.hpp"


struct memory_allocator_impl;
struct memory_allocator
{
    enum memory_allocator_kind
    {
        NONE,
        ARENA,
        STACK,
        POOL,
        HEAP,
        MALLOC,
    } kind;

    memory_allocator_impl *impl;

    static memory_allocator make_arena(memory_buffer);
    memory_allocator allocate_arena(usize size);

    static memory_allocator make_pool(memory_buffer buffer, uint32 chunk_size);
    memory_allocator allocate_pool(usize buffer_size, uint32 chunk_size);

    template <typename T>
    T *allocate();

    memory_buffer allocate_buffer_(usize size);
    memory_buffer allocate_buffer_(usize size, usize alignment);

    memory_buffer allocate_buffer(usize size);
    memory_buffer allocate_buffer(usize size, usize alignment);

    template <typename T>
    array<T> allocate_array(usize count);

    template <typename T>
    array<T> allocate_array_open(usize count);

    memory_buffer allocate_copy(void *, usize);

    template <typename T>
    void deallocate(T *);
    void deallocate(void *, usize);
    void deallocate(memory_buffer);

    memory_buffer reallocate(memory_buffer, usize new_size);

    void reset();

    struct report
    {
        memory_allocator_kind kind;
        usize size;
        usize used;
        usize chunks_used;
        usize chunk_size;
        usize chunk_count;
    };

    report get_report();
};

memory_allocator mallocator();


struct memory_allocator__report memory_allocator__report(memory_allocator a);


template <typename T>
array<T> memory_allocator::allocate_array(usize count)
{
    auto buffer = allocate_buffer_(sizeof(T) * count, alignof(T));

    array<T> result;
    result.data_ = (T *) buffer.data;
    result.size_ = 0;
    result.capacity_ = count;
    return result;
}

template <typename T>
array<T> memory_allocator::allocate_array_open(usize count)
{
    auto result = allocate_array<T>(count);
    result.resize(count);
    return result;
}


#endif // BASED__MEMORY_ALLOCATOR_H
