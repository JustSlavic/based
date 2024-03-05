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


int load_file(char const *filename, memory_buffer buffer)
{
    int Result = -1;

    HANDLE FileHandle = CreateFileA(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (FileHandle != INVALID_HANDLE_VALUE)
    {
        DWORD BytesRead;
        BOOL ReadFileResult = ReadFile(FileHandle, buffer.data, (DWORD) buffer.size, &BytesRead, NULL);
        if (ReadFileResult == TRUE)
        {
            Result = BytesRead;
        }

        CloseHandle(FileHandle);
    }

    return Result;
}

memory_buffer load_file(char const *filename, memory_allocator *a)
{
    memory_buffer Result = {}

    HANDLE FileHandle = CreateFileA(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (FileHandle != INVALID_HANDLE_VALUE)
    {
        LARGE_INTEGER FileSize;
        BOOL GetSizeResult = GetFileSizeEx(FileHandle, &FileSize);
        if (GetSizeResult == TRUE)
        {
            Result = a->allocate_buffer(FileSize.QuadPart)
            if (Result)
            {
                DWORD BytesRead;
                BOOL ReadFileResult = ReadFile(FileHandle, Result.data, (DWORD) FileSize.QuadPart, &BytesRead, NULL);
                if (ReadFileResult == FALSE)
                {
                    a->deallocate(Result);
                    memset(&Result, 0, sizeof(memory_block));
                }
            }
        }
        CloseHandle(FileHandle);
    }

    return Result;
}


} // namespace platform
