#ifndef BASED__DESERIALIZER_HPP
#define BASED__DESERIALIZER_HPP

#include "base.h"


struct deserializer
{
    memory_buffer buffer;
    usize cursor;

    static deserializer from(memory_buffer buffer)
    {
        deserializer result;
        result.buffer = buffer;
        result.cursor = 0;
        return result;
    }

    template <typename T>
    int get(T *);
};


template <typename T>
int deserializer::get(T *t)
{
    int ec = 0;
    if (cursor + sizeof(T) <= buffer.size)
    {
        memcpy(t, buffer.data + cursor, sizeof(T));
        cursor += sizeof(T);
    }
    else
    {
        ec = -1;
    }
    return ec;
}


#endif // BASED__DESERIALIZER_HPP
