#ifndef BASED__INTEGER_H
#define BASED__INTEGER_H

#include <base.h>


#define truncate_to_int32(x) (int32) (x)
#define truncate_to_uint32(x) (uint32) (x)


int16 int16__change_endianness(int16 n)
{
    int16 result = ((n & 0xFF00) >> 8) |
                   ((n & 0x00FF) << 8);
    return result;
}

uint16 uint16__change_endianness(uint16 n)
{
    uint16 result = ((n & 0xFF00) >> 8) |
                    ((n & 0x00FF) << 8);
    return result;
}

int32 int32__change_endianness(int32 n)
{
    int32 result = ((n & 0xFF000000) >> 24) |
                   ((n & 0x00FF0000) >>  8) |
                   ((n & 0x0000FF00) <<  8) |
                   ((n & 0x000000FF) << 24);
    return result;
}

uint32 uint32__change_endianness(uint32 n)
{
    uint32 result = ((n & 0xFF000000) >> 24) |
                    ((n & 0x00FF0000) >>  8) |
                    ((n & 0x0000FF00) <<  8) |
                    ((n & 0x000000FF) << 24);
    return result;
}

int64 int64__change_endianness(int64 n)
{
    int64 result = ((n & 0xFF00000000000000) >> 56) |
                   ((n & 0x00FF000000000000) >> 40) |
                   ((n & 0x0000FF0000000000) >> 24) |
                   ((n & 0x000000FF00000000) >>  8) |
                   ((n & 0x00000000FF000000) <<  8) |
                   ((n & 0x0000000000FF0000) << 24) |
                   ((n & 0x000000000000FF00) << 40) |
                   ((n & 0x00000000000000FF) << 56);
    return result;
}

uint64 uint64__change_endianness(uint64 n)
{
    uint64 result = ((n & 0xFF00000000000000) >> 56) |
                    ((n & 0x00FF000000000000) >> 40) |
                    ((n & 0x0000FF0000000000) >> 24) |
                    ((n & 0x000000FF00000000) >>  8) |
                    ((n & 0x00000000FF000000) <<  8) |
                    ((n & 0x0000000000FF0000) << 24) |
                    ((n & 0x000000000000FF00) << 40) |
                    ((n & 0x00000000000000FF) << 56);
    return result;
}


#endif // BASED__INTEGER_H
