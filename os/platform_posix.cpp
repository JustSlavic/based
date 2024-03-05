#include "../platform.hpp"

#include <sys/mman.h>
#include <time.h>


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

int64 wall_clock::frequency()
{
    PERSIST int64 cache;
    if (cache == 0)
    {
        struct timespec ts = {};
        int ec = clock_getres(CLOCK_MONOTONIC, &ts);
        if (ec == -1)
        {
            // @todo: process the system error, I could not run the game when there's undefined clock resolution
        }
        else if (ts.tv_sec > 0)
        {
            // @todo: process the error, because I could not run the game when the monotonic clock is so slow
        }
        else
        {
            cache = 1000000000ll / (int64) ts.tv_nsec;
        }
    }
    return cache;
}

timepoint wall_clock::now()
{
    timepoint result = {};

    struct timespec ts = {};
    int ec = clock_gettime(CLOCK_MONOTONIC, &ts);
    if (ec < 0)
    {
        // @todo: process the error, because I could not run the game when the monotonic clock is not ticking
    }
    else
    {
        result.us = (uint64) ts.tv_sec * 1000000000ll + ts.tv_nsec;
    }

    return result;
}



} // namespace platform
