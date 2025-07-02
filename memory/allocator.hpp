#ifndef BASED__MEMORY_ALLOCATOR_H
#define BASED__MEMORY_ALLOCATOR_H

#include <base.h>
#include <array.hpp>
#include <code_location.h>
#include <memory/buffer.hpp>



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

    void *opaque;

    static memory_allocator make_arena(memory_buffer, memory_allocator *parent = NULL);
    memory_allocator allocate_arena(usize size);

    static memory_allocator make_pool(memory_buffer buffer, uint32 chunk_size);
    memory_allocator allocate_pool(usize buffer_size, uint32 chunk_size);

#if DEBUG
    template <typename T>
    T *allocate();

    memory_buffer allocate_buffer_(usize size, code_location cl);
    memory_buffer allocate_buffer_(usize size, usize alignment, code_location cl);

    memory_buffer allocate_buffer(usize size, code_location cl);
    memory_buffer allocate_buffer(usize size, usize alignment, code_location cl);

    template <typename T>
    array<T> allocate_array(usize count, code_location cl);
    string allocate_string(usize count);

    void print_debug();
    void silence_debug();
#else
    template <typename T>
    T *allocate();

    memory_buffer allocate_buffer_(usize size);
    memory_buffer allocate_buffer_(usize size, usize alignment);

    memory_buffer allocate_buffer(usize size);
    memory_buffer allocate_buffer(usize size, usize alignment);

    template <typename T>
    array<T> allocate_array(usize count);
    string allocate_string(usize count);
#endif

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

#if DEBUG

#define ALLOCATE_BUFFER_(A, S) (A).allocate_buffer_(S, CL_HERE)
#define ALLOCATE_ALIGNED_BUFFER_(A, S, B) (A).allocate_buffer_(S, B, CL_HERE)

#define ALLOCATE_BUFFER(A, S) (A).allocate_buffer(S, CL_HERE)
#define ALLOCATE_ALIGNED_BUFFER(A, S, B) (A).allocate_buffer(S, B, CL_HERE)

template <typename T>
T *memory_allocator::allocate()
{
    memory_buffer buffer = allocate_buffer(sizeof(T), alignof(T), CL_HERE);
    return (T *) buffer.data;
}

template <typename T>
array<T> memory_allocator::allocate_array(usize count, code_location cl)
{
    array<T> result = make_array<T>(allocate_buffer(count * sizeof(T), cl));
    return result;
}

#define ALLOCATE_ARRAY(A, T, N) (A).allocate_array<T>(N, CL_HERE)

#else // DEBUG

#define ALLOCATE_BUFFER_(A, S) (A).allocate_buffer_(S);
#define ALLOCATE_ALIGNED_BUFFER_(A, S, B) (A).allocate_buffer_(S, B);

#define ALLOCATE_BUFFER(A, S) (A).allocate_buffer(S);
#define ALLOCATE_ALIGNED_BUFFER(A, S, B) (A).allocate_buffer(S, B);

template <typename T>
T *memory_allocator::allocate()
{
    memory_buffer buffer = allocate_buffer(sizeof(T), alignof(T));
    return (T *) buffer.data;
}

template <typename T>
array<T> memory_allocator::allocate_array(usize count)
{
    array<T> result = make_array<T>(allocate_buffer(count * sizeof(T)));
    return result;
}

string memory_allocator::allocate_string(usize size)
{
    string result = make_array<char>(allocate_buffer(size));
    return result;
}

#define ALLOCATE_ARRAY(A, T, N) A.allocate_array<T>(N)

#endif // DEBUG

template <typename T>
void memory_allocator::deallocate(T *t)
{
    deallocate(t, sizeof(T));
}


#endif // BASED__MEMORY_ALLOCATOR_H
