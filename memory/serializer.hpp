#ifndef BASED__SERIALIZER_HPP
#define BASED__SERIALIZER_HPP

#include "base.h"


struct serializer
{
    byte *data;
    usize size;
    usize capacity;

    static serializer from(void *memory, usize memory_size);
    static serializer from(memory_buffer memory);
    int push(void *memory, usize memory_size);
    template <typename T> int push(T t);
    template <typename T> int push(T *t);

    void reset();
};

serializer serializer::from(void *memory, usize memory_size)
{
    serializer result;
    result.data = (byte *) memory;
    result.size = 0;
    result.capacity = memory_size;
    return result;
}

serializer serializer::from(memory_buffer memory)
{
    serializer result = serializer::from(memory.data, memory.size);
    return result;
}

int serializer::push(void *memory, usize memory_size)
{
    if (size + memory_size > capacity) return 0;

    memcpy(data + size, memory, memory_size);
    size += memory_size;
    return memory_size;
}

template <typename T>
int serializer::push(T t)
{
    push(&t, sizeof(T));
}

template <typename T>
int serializer::push(T *t)
{
    push(t, sizeof(T));
}

void serializer::reset()
{
    size = 0;
}


#endif // BASED__SERIALIZER_HPP
