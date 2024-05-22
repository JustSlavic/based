#ifndef BASED__RING_BUFFER_HPP
#define BASED__RING_BUFFER_HPP

#include "base.h"
#include "memory_allocator.h"


template <typename T>
struct ring_buffer
{
    memory_buffer buffer;
    uint32 size;
    uint32 capacity;

    int64 it_begin;
    int64 it_end;

    static ring_buffer from(void *data, usize size)
    {
        
    }

    static ring_buffer from(memory_buffer buffer)
    {
        return ring_buffer<T>::from(buffer.data, buffer.size);
    }

    void push_back(T&& t)
    {
        if ((end - begin) < capacity) {

        }
    }

    void push_front(T&& t)
    {
        if ((end - begin) < capacity) {
            f
    }
};


#endif // BASED__RING_BUFFER_HPP
