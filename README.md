# Based library

This library is the collection of structs and functions useful to me.
Ignore this library, if those function are not useful to you.

![Tests passed](https://github.com/JustSlavic/based_test/actions/workflows/c-cpp.yml/badge.svg)

**@todo:**
1. `serializer` is the struct that would allow to push into memory_buffer objects, like integers, strings, other memory buffers, etc.;
2. `deserializer` is the struct that would allow to sweep through memory_buffer object, while interpreting the memory in it as objects, like integers, strings, other memory buffers, etc.;
3. Remove platform-dependent code from root of the library, it should only be accecable through `platform.hpp` and `os/platform_xxxx.cpp`;
4. Ring buffer;
5. Good enough, but simple hash table;
