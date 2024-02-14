#include "fs.hpp"

#include <fcntl.h>


file file::open(char const *filename)
{
    file result;
    result.fd = ::open(filename, O_RDONLY, 0);
    return result;
}

void file::close()
{
    if (fd > 0) ::close(fd);
}

isize file::size()
{
    isize result = 0;
    struct stat st;
    int ec = fstat(fd, &st);
    if (ec >= 0) result = st.st_size;
    return result;
}

isize file::read(byte *buffer, usize size)
{
    isize result = ::read(fd, buffer, size);
    return result;
}
