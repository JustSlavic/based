#include "platform.hpp"
#include <sys/mman.h>


namespace platform
{


memory_buffer allocate_pages(void *base, usize size)
{
    memory_buffer result = {};
    void *memory = mmap(base, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (memory != NULL)
    {
        result.data = (byte *) memory;
        result.size = size;
    }
    return result;
}

memory_buffer allocate_pages(usize size)
{
    return allocate_pages(0, size);
}

int deallocate_pages(memory_buffer buffer)
{
    int ec = munmap(buffer.data, buffer.size);
    return ec;
}


} // namespace platform
