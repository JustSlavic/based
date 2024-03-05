#ifndef PLATFORM_HPP
#define PLATFORM_HPP

#include "base.h"
#include "memory_buffer.hpp"
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


} // namespace platform


#endif // PLATFORM_HPP
