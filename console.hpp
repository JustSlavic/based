#ifndef CONSOLE_HPP
#define CONSOLE_HPP

#include <base.h>
#if OS_LINUX || OS_MAC
#include <stdio.h>
#elif OS_WINDOWS
#endif

namespace console {

#if OS_WINDOWS
void print(...) {}
#elif OS_MAC || OS_LINUX
void print(char const *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
}
#endif

} // namespace console



#endif // CONSOLE_HPP
