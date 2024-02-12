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

    static ring_buffer allocate(memory_allocator allocator, uint32 count);
};


#endif // BASED__RING_BUFFER_HPP
