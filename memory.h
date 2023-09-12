#ifndef BASED__MEMORY_H
#define BASED__MEMORY_H

#include "base.h"


typedef struct memory_block
{
    byte *memory;
    usize size;
} memory_block;

FORCE_INLINE
memory_block memory__advance_block(memory_block *block, usize bytes)
{
    if (bytes > block->size)
        bytes = block->size;

    memory_block result;
    result.memory = block->memory;
    result.size = bytes;

    block->memory = block->memory + bytes;
    block->size = block->size - bytes;

    return result;
}

FORCE_INLINE
byte *memory__set(void *destination, int value, usize count)
{
    byte *d = (byte *) destination;
    while (count--) *d++ = (byte) value;
    return (byte *) destination;
}

FORCE_INLINE
byte *memory__copy(void *destination, void const *source, usize count)
{
    byte *d = (byte *) destination;
    byte *s = (byte *) source;
    while (count--) *d++ = *s++;
    return (byte *) destination;
}

FORCE_INLINE
memory_block memory__empty_block()
{
    memory_block result;
    memory__set(&result, 0, sizeof(memory_block));
    return result;
}

FORCE_INLINE
usize memory__get_padding(void *pointer, usize alignment)
{
    // @todo: check if we can delete final '& (alignment - 1)'
    usize result = (alignment - ((usize)pointer & (alignment - 1))) & (alignment - 1);
    return result;
}

FORCE_INLINE
byte *memory__align_pointer(void *pointer, usize alignment)
{
    byte *result = (byte *) pointer + memory__get_padding(pointer, alignment);
    return result;
}


#endif // BASED__MEMORY_H
