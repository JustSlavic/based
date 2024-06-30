#ifndef BASED__SERIALIZER_HPP
#define BASED__SERIALIZER_HPP

#include "base.h"


struct serializer
{
    byte *data;
    usize size;
    usize capacity;

    static serializer from(void *buffer, usize buffer_size);
    int push(void *buffer, usize buffer_size);
};

serializer serializer::from(void *buffer, usize buffer_size)
{
    serializer result;
    result.data = (byte *) buffer;
    result.capacity = buffer_size;
    result.size = 0;
    return result;
}

int serializer::push(void *buffer, usize buffer_size)
{
    if (size + buffer_size > capacity)
        return -1;

    memcpy(data + size, buffer, buffer_size);
    size += buffer_size;

    return 0;
}


#endif // BASED__SERIALIZER_HPP
