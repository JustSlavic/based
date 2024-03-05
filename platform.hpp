#ifndef PLATFORM_HPP
#define PLATFORM_HPP

#include "base.h"
#include "memory_buffer.hpp"


namespace platform
{


memory_buffer allocate_pages(void *base, usize size);
memory_buffer allocate_pages(usize size);
int deallocate_pages(memory_buffer);


} // namespace platform


#endif // PLATFORM_HPP
