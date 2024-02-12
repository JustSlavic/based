#include "ring_buffer.hpp"


template <typename T>
ring_buffer<T> ring_buffer<T>::allocate(memory_allocator allocator, uint32 count)
{
    ring_buffer<T> result;
    result.buffer = ALLOCATE_BUFFER_ALIGNED_(allocator, T, count);
    result.cursor = 0;
    result.count  = 0;
    return result;
}
