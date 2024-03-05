#ifndef PLATFORM_HPP
#define PLATFORM_HPP

#include "base.h"
#include "memory/buffer.hpp"
#include "memory/allocator.hpp"
#include "time.hpp"


namespace platform
{

[[nodiscard]] memory_buffer allocate_pages(void *base, usize size);
[[nodiscard]] memory_buffer allocate_pages(usize size);
int deallocate_pages(memory_buffer);

namespace wall_clock
{
    [[nodiscard]] int64 frequency();
    [[nodiscard]] timepoint now();
} // namespace wall_clock

int load_file(char const *filename, memory_buffer buffer);
int load_file(char const *filename, memory_allocator *a, memory_buffer *buffer);


} // namespace platform


#endif // PLATFORM_HPP
