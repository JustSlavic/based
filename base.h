#ifndef BASED__BASE_HPP
#define BASED__BASE_HPP

#if defined(_MSC_VER)
    #define COMPILER_MSVC 1
#elif defined(__GNUC__) && !defined(__clang__)
    #define COMPILER_GNU 1
#elif defined(__clang__)
    #define COMPILER_CLANG 1
#elif defined(__MINGW32__)
    #define COMPILER_MINGW 1
#endif

#if defined(_WIN32) || defined(_WIN64)
    #define OS_WINDOWS 1
#elif defined(__linux__) || defined(__linux) || defined(linux)
    #define OS_LINUX 1
#elif defined(__APPLE__) || defined(__MACH__)
    #define OS_MAC 1
#elif defined(__FreeBSD__)
    #define OS_FREEBSD 1
#elif defined(__unix__) || defined(__unix) || defined(unix)
    // Nothing yet
#endif

// Detect the 32-bit or 64-bit architecture
#if defined(__arm__) || defined(__i386__)
#define ARCH_32BIT 1
#endif
#if defined(__aarch64__) || defined(__x86_64__)
#define ARCH_64BIT 1
#endif

#if COMPILER_MSVC
    typedef          __int8  int8;
    typedef          __int16 int16;
    typedef          __int32 int32;
    typedef          __int64 int64;
    typedef unsigned __int8  uint8;
    typedef unsigned __int16 uint16;
    typedef unsigned __int32 uint32;
    typedef unsigned __int64 uint64;
    typedef float            float32;
    typedef double           float64;

    #if DEBUG
        #define DEBUG_BREAK __debugbreak
        // #if defined(__arm__) && !defined(__thumb__) // ARM
        // #define DEBUG_BREAK() { __asm__ volatile(".inst 0xe7f001f0"); } (void)0
        // #elif defined(__aarch64__) && defined(__APPLE__) // Apple
        // #define DEBUG_BREAK() { __builtin_debugtrap(); } (void)0
        // #elif defined(__aarch64__)
        // #define DEBUG_BREAK() { __asm__ volatile(".inst 0xd4200000"); } (void)0
        // #endif
    #else
        #define DEBUG_BREAK
    #endif // DEBUG


    #define FORCE_INLINE __forceinline
    #define DLL_EXPORT __declspec(dllexport)
    #define DEBUG_CYCLE_COUNT __rdtsc

    #define RELEASE_COM(PTR) do { if ((PTR)) { (PTR)->Release(); } (PTR) = NULL; } while(0)
#endif // COMPILER_MSVC

#if COMPILER_GNU
    typedef   signed char        int8;
    typedef   signed short       int16;
    typedef   signed int         int32;
    typedef unsigned char        uint8;
    typedef unsigned short       uint16;
    typedef unsigned int         uint32;
    typedef float                float32;
    typedef double               float64;

    #define FORCE_INLINE         __attribute__((always_inline)) inline
    // #define DLL_EXPORT           __attribute__((dllexport))
    #define DLL_EXPORT           extern

    #ifndef INT8_MIN
        #define INT8_MIN             0x80
        #define INT16_MIN            0x8000
        #define INT32_MIN            0x80000000
        #define INT8_MAX             0x7F
        #define INT16_MAX            0x7FFF
        #define INT32_MAX            0x7FFFFFFF
        #define UINT8_MIN            0
        #define UINT16_MIN           0
        #define UINT32_MIN           0
        #define UINT8_MAX            0xFF
        #define UINT16_MAX           0xFFFF
        #define UINT32_MAX           0xFFFFFFFF
    #endif // INT8_MIN

    #ifndef va_start
        typedef __builtin_va_list va_list;
        #define va_start __builtin_va_start
        #define va_arg   __builtin_va_arg
        #define va_end   __builtin_va_end
    #endif // va_start

    #if ARCH_64BIT
        typedef   signed long long   int64;
        typedef unsigned long long   uint64;
    #endif // ARCH_64BIT

    #if DEBUG
        #define DEBUG_BREAK __builtin_trap
    #else
        #define DEBUG_BREAK
    #endif // DEBUG
#endif // COMPILER_GNU

#if COMPILER_CLANG
    typedef   signed char        int8;
    typedef   signed short       int16;
    typedef   signed int         int32;
    typedef unsigned char        uint8;
    typedef unsigned short       uint16;
    typedef unsigned int         uint32;
    typedef float                float32;
    typedef double               float64;

    #define FORCE_INLINE         __attribute__((always_inline)) inline
    #define DLL_EXPORT           extern

    #if DEBUG
        #define DEBUG_BREAK __builtin_trap
    #else
        #define DEBUG_BREAK
    #endif // DEBUG

    #ifndef va_start
        typedef __builtin_va_list va_list;
        #define va_start __builtin_va_start
        #define va_arg   __builtin_va_arg
        #define va_end   __builtin_va_end
    #endif

    #define DO_PRAGMA(S) _Pragma(#S)
    #define SUPRESS_WARNING_PUSH(OPT) \
        DO_PRAGMA(clang diagnostic push); \
        DO_PRAGMA(clang diagnostic ignored OPT);
    #define SUPRESS_WARNING_POP() DO_PRAGMA(clang diagnostic pop)
#endif // COMPILER_CLANG

#if ARCH_32BIT
    typedef int32                isize;
    typedef uint32               usize;
    typedef int32                intptr;
    typedef uint32               uintptr;
    #define SIZE_MAX             INT32_MAX
    #define SIZE_MIN             INT32_MiN
    #define USIZE_MAX            UINT32_MAX
    #define USIZE_MIN            UINT32_MIN
#endif // ARCH_32BIT

#if ARCH_64BIT
    typedef   signed long long   int64;
    typedef unsigned long long   uint64;
    typedef int64                isize;
    typedef uint64               usize;
    typedef int64                intptr;
    typedef uint64               uintptr;
    #ifndef INT64_MAX
        #define INT64_MAX            0x7FFFFFFFFFFFFFFF
        #define INT64_MIN            0x8000000000000000
        #define UINT64_MIN           0
        #define UINT64_MAX           0xFFFFFFFFFFFFFFFF
        #define SIZE_MAX             INT64_MAX
        #define SIZE_MIN             INT64_MiN
        #define USIZE_MAX            UINT64_MAX
        #define USIZE_MIN            UINT64_MIN
    #endif
#endif // ARCH_64BIT


typedef uint8                byte;
typedef uint32               bool32;

#ifndef __cplusplus // C
    #define static_assert(VAR, MSG) _Static_assert((VAR), MSG)
    #define alignof(TYPE) _Alignof(TYPE)
    typedef uint8                bool;
    #ifndef true
        #define true 1
    #endif
    #ifndef false
        #define false 0
    #endif
#endif // C

#define VA_ARGS(...) , ##__VA_ARGS__

#ifndef NULL
    #define NULL 0
#endif

#define EPSILON    1e-5f

#define FUNCTION   static
#define PERSIST    static
#define GLOBAL     static
#define INLINE     inline

#ifdef DEBUG
    #define ASSERT(COND)          do {if (COND) {} else { DEBUG_BREAK(); }} while(false)
    #define ASSERT_MSG(COND, ...) do {if (COND) {} else { \
        console::print("%s", __VA_ARGS__); DEBUG_BREAK(); }} while(false)
#else
    #define ASSERT(COND)          (void)(0)
    #define ASSERT_MSG(COND, ...) (void)(0)
#endif // DEBUG

#define ASSERT_FAIL(...) ASSERT_MSG(false, __VA_ARGS__)
#define UNREACHABLE() ASSERT_FAIL("Unreachable code path!")
#define NOT_IMPLEMENTED() ASSERT_FAIL("This code path is not implemented yet!")

#define ARRAY_COUNT(ARRAY) (sizeof(ARRAY) / sizeof(ARRAY[0]))
#define STRINGIFY_(X) #X
#define STRINGIFY(X) STRINGIFY_(X)
#define UNUSED(X) (void) (X)
#define MACRO_EXPAND(X) X
#define MACRO_OVERLOAD_1(_1, NAME, ...) NAME
#define MACRO_OVERLOAD_2(_1, _2, NAME, ...) NAME
#define MACRO_OVERLOAD_3(_1, _2, _3, NAME, ...) NAME
#define MACRO_OVERLOAD_4(_1, _2, _3, _4, NAME, ...) NAME
#define KILOBYTES(N) (1024ull * (N))
#define MEGABYTES(N) (1024ull * KILOBYTES(N))
#define GIGABYTES(N) (1024ull * MEGABYTES(N))
#define TERABYTES(N) (1024ull * GIGABYTES(N))
#define KILOBYTES_FROM_BYTES(BYTES) ((BYTES) / 1024.0f)
#define MEGABYTES_FROM_BYTES(BYTES) (KILOBYTES_FROM_BYTES(BYTES) / 1024.0f)
#define GIGABYTES_FROM_BYTES(BYTES) (MEGABYTES_FROM_BYTES(BYTES) / 1024.0f)
#define TERABYTES_FROM_BYTES(BYTES) (GIGABYTES_FROM_BYTES(BYTES) / 1024.0f)
#define TOGGLE(X) (X) = !(X)

#define GLSL(...) #__VA_ARGS__
#define HLSL(...) #__VA_ARGS__
#define WGSL(...) #__VA_ARGS__


#endif // BASED__BASE_HPP
