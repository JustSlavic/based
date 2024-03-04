#ifndef BASED__RING_BUFFER_HPP
#define BASED__RING_BUFFER_HPP

#include "base.h"
#include "memory_allocator.h"


template <typename T>
struct ring_buffer
{
    memory_block buffer;
    uint32 cursor;   // Next position where you can write data
    uint32 count;    // Number of elements you can read from the buffer looking back
    uint32 capacity; // Maximum number of elements that could be in the buffer

    static ring_buffer from(void *buffer, usize size)
    {
        ring_buffer<T> result;
        result.buffer.data = (byte *) buffer;
        result.buffer.size = size;
        result.cursor = 0;
        result.count = 0;
        result.capacity = size / sizeof(T);

        return result;
    }
};


#endif // BASED__RING_BUFFER_HPP
