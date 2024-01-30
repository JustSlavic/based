#include "string_id.hpp"


struct string_id__storage
{
    memory_allocator allocator;

    char const *table[1024];
    usize       sizes[1024];
    uint64      hashes[1024];
};

GLOBAL struct string_id__storage string_id__storage_instance;


void string_id::initialize(memory_allocator a)
{
    memory__set(&string_id__storage_instance, 0, sizeof(string_id__storage));
    string_id__storage_instance.allocator = a;
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

string_id string_id__from_buffer(char const *buffer, usize size)
{
    string_id result = {};

    uint64 hash = string_id__hash(buffer, size);

    int32 index = -1;
    for (usize offset = 0; offset < ARRAY_COUNT(string_id__storage_instance.table); offset++)
    {
        uint64 i = (hash + offset) % ARRAY_COUNT(string_id__storage_instance.table);
        if ((string_id__storage_instance.hashes[i] == 0) ||
            (string_id__storage_instance.hashes[i] == hash))
        {
            index = i;
            break;
        }
    }

    if (index < 0)
    {
        ASSERT_FAIL("Hash table is full!!! Make better hash table!");
    }
    else
    {
        if (string_id__storage_instance.table[index] == NULL)
        {
            string_id__storage_instance.table[index] = (char const *) ALLOCATE_BUFFER(string_id__storage_instance.allocator, size + 1).memory;
            string_id__storage_instance.sizes[index] = size;
            string_id__storage_instance.hashes[index] = hash;

            memory__copy((void *) string_id__storage_instance.table[index], buffer, size);
        }

        result.id = index;
    }

    return result;
}

string_id string_id::from(char const *cstr)
{
    usize size = cstring__size_no0(cstr);
    return string_id__from_buffer(cstr, size);
}

string_id string_id::from(string_view sv)
{
    return string_id__from_buffer(sv.data, sv.size);
}

char const *string_id::get_cstring()
{
    char const *result = string_id__storage_instance.table[id];
    return result;
}
 
string_view string_id::get_string_view()
{
    string_view result;
    result.data = string_id__storage_instance.table[id];
    result.size = string_id__storage_instance.sizes[id];
    return result;
}
