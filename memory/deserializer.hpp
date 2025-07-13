#ifndef BASED__DESERIALIZER_HPP
#define BASED__DESERIALIZER_HPP

#include "base.h"


struct deserializer
{
    byte *data;
    usize size;
    usize cursor;

    static deserializer from(void *memory, usize memory_size);
    static deserializer from(memory_buffer memory);
    memory_buffer get_memory_buffer(usize memory_size);
    template <typename T> T get();
    template <typename T> int get(T *t);
};

deserializer deserializer::from(void *memory, usize memory_size)
{
    deserializer result;
    result.data = (byte *) memory;
    result.size = size;
    result.cursor = 0;
    return result;
}

deserializer deserializer::from(memory_buffer memory)
{
    deserializer result = deserializer::from(memory.data, memory.size);
    return result;
}

memory_buffer deserializer::get_memory_buffer(usize memory_size)
{
    memory_buffer result = {};
    if (cursor + memory_size <= size)
    {
        result.data = data + cursor;
        result.size = memory_size;
        cursor += memory_size;
    }
    return result;
}

template <typename T>
T deserializer::get()
{
    ASSERT(cursor + sizeof(T) <= size);
    void *p = data + cursor;
    cursor += sizeof(T);
    return *(T *) p;
}

template <typename T>
int deserializer::get(T *t)
{
    ASSERT(cursor + sizeof(T) <= size);
    *t = *(T *) (data + cursor);
    return sizeof(T);
}


#endif // BASED__DESERIALIZER_HPP
