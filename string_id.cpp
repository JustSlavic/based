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

string_id string_id__from_buffer(string_id::storage *s, char const *buffer, usize size)
{
    string_id result = {};

    uint64 hash = string_id__hash(buffer, size);

    int32 index = -1;
    for (usize offset = 0; offset < ARRAY_COUNT(s->table); offset++)
    {
        uint64 i = (hash + offset) % ARRAY_COUNT(s->table);
        if ((s->hashes[i] == 0) ||
            (s->hashes[i] == hash))
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
        if (s->table[index] == NULL)
        {
            s->table[index] = (char const *) ALLOCATE_BUFFER(s->allocator, size + 1).memory;
            s->sizes[index] = size;
            s->hashes[index] = hash;

            memory__copy((void *) s->table[index], buffer, size);
        }

        result.id = index;
    }

    return result;
}

string_id string_id::from(storage *s, char const *cstr)
{
    usize size = cstring__size_no0(cstr);
    return string_id__from_buffer(s, cstr, size);
}

string_id string_id::from(storage *s, string_view sv)
{
    return string_id__from_buffer(s, sv.data, sv.size);
}

string_id string_id::from(char const *cstr)
{
    usize size = cstring__size_no0(cstr);
    return string_id__from_buffer(&string_id__storage_instance, cstr, size);
}

string_id string_id::from(string_view sv)
{
    return string_id__from_buffer(&string_id__storage_instance, sv.data, sv.size);
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
