#include "../platform.hpp"


namespace platform
{


memory_buffer allocate_pages(void *base, usize size)
{
    memory_buffer result;

    void *memory = VirtualAlloc(base_address, size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
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
    VirtualFree(buffer.data, buffer.size, MEM_RELEASE);
}

timepoint wall_clock::now()
{
    LARGE_INTEGER PerformanceCounter;
    QueryPerformanceCounter(&PerformanceCounter);
    timepoint result = { (uint64) PerformanceCounter.QuadPart * 1000000 / frequency() };
    return result;
}

int64 wall_clock::frequency()
{
    PERSIST int64 cache;
    if (cache == 0)
    {
        LARGE_INTEGER performance_counter_frequency;
        QueryPerformanceFrequency(&performance_counter_frequency);
        cache = performance_counter_frequency.QuadPart;
    }
    return cache;
}


} // namespace platform
