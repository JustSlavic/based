#include "string_id.hpp"


GLOBAL struct string_id::storage string_id__storage_instance;


string_id::storage string_id::initialize(memory_allocator a)
{
    memory__set(&string_id__storage_instance, 0, sizeof(string_id::storage));
    string_id__storage_instance.allocator = a;

    return string_id__storage_instance;
}

uint64 string_id__hash(char const *buffer, usize size)
{
    uint32 primes[32] = {
          2,   3,  5,  7, 11,  13,  17,  19,  23,  29,
         31,  37, 41, 43, 47,  53,  59,  61,  67,  71,
         73,  79, 83, 89, 97, 101, 103, 107, 109, 113,
        127, 131,
    };

    uint64 hash = 0;
    for (uint32 index = 0; index < size; index++)
    {
        hash += buffer[index] * primes[index % ARRAY_COUNT(primes)];
    }
    return hash;
}

string_id string_id::from(storage *sg, char const *buffer, usize size)
{
    string_id result = {};

    uint64 hash = string_id__hash(buffer, size);

    int32 index = -1;
    for (usize offset = 0; offset < STRID_STORAGE_CAPACITY; offset++)
    {
        uint64 i = (hash + offset) % STRID_STORAGE_CAPACITY;
        if ((sg->hashes[i] == 0) ||
            (sg->hashes[i] == hash))
        {
            index = (int32) i;
            break;
        }
    }

    if (index < 0)
    {
        ASSERT_FAIL("Hash table is full!!! Make better hash table!");
    }
    else
    {
        if (sg->strings[index].data == NULL)
        {
            auto string_buffer = sg->allocator.allocate_buffer(size + 1);
            memory__copy(string_buffer.data, buffer, size);

            sg->strings[index] = string_view::from(string_buffer.data, string_buffer.size);
            sg->hashes[index]  = hash;
        }

        result.id = index;
    }

    return result;
}

string_id string_id::from(char const *buffer, usize size)
{
    return from(&string_id__storage_instance, buffer, size);
}

string_id string_id::from(storage *sg, char const *cstr)
{
    usize size = cstring__size_no0(cstr);
    return string_id::from(sg, cstr, size);
}

string_id string_id::from(storage *sg, string_view s)
{
    return string_id::from(sg, s.data, s.size);
}

string_id string_id::from(char const *cstr)
{
    usize size = cstring__size_no0(cstr);
    return string_id::from(cstr, size);
}

string_id string_id::from(string_view s)
{
    return string_id::from(s.data, s.size);
}

char const *string_id::get_cstring()
{
    char const *result = string_id__storage_instance.strings[id].data;
    return result;
}
 
string_view string_id::get_string_view()
{
    string_view result = string_id__storage_instance.strings[id];
    return result;
}
