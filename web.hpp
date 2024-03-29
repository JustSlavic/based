#ifndef BASED__WEB_HPP
#define BASED__WEB_HPP

#include <base.h>


namespace web
{

int url_encode(memory_buffer from, memory_buffer to);
int url_decode(memory_buffer from, memory_buffer to);


} // namespace web

#endif // BASED__WEB_HPP
