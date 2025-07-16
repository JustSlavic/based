#ifndef CONSOLE_HPP
#define CONSOLE_HPP

#include <base.h>
#if OS_LINUX || OS_MAC
#include <stdio.h>
#elif OS_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <stdarg.h>
#endif

namespace console {

#if OS_WINDOWS
void print(char const *fmt, ...) {
    char buffer[1024] = {};
    va_list args;
    va_start(args, fmt);
    vsprintf(buffer, fmt, args);
    va_end(args);
    OutputDebugStringA(buffer);
}
#elif OS_MAC || OS_LINUX
void print(char const *fmt, ...)
{
    char buffer[1024] = {};
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, 1023, fmt, args);
    va_end(args);
    printf("%s", buffer);
}
#endif

} // namespace console



#endif // CONSOLE_HPP
