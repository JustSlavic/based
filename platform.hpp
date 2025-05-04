#ifndef PLATFORM_HPP
#define PLATFORM_HPP

#include "base.h"
#include "memory/buffer.hpp"
#include "memory/allocator.hpp"
#include "time.hpp"


namespace platform
{

struct window;

[[nodiscard]] memory_buffer allocate_pages(void *base, usize size);
[[nodiscard]] memory_buffer allocate_pages(usize size);
int deallocate_pages(memory_buffer);

[[nodiscard]] int32 get_monitor_refresh_rate_hz(window *window);
[[nodiscard]] void get_monitor_resolution(window *window, int32 *width, int32 *height);

namespace clock
{

[[nodiscard]] int64 frequency();
[[nodiscard]] timepoint now();

} // namespace clock

int load_file(char const *filename, memory_buffer buffer);
memory_buffer load_file(char const *filename, memory_allocator *a);

} // namespace platform


#endif // PLATFORM_HPP
