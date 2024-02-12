#ifndef BASED__FS_HPP
#define BASED__FS_HPP

#include "base.h"


struct file
{
    int fd;

    static file open(char const *filename);

    isize size();
    isize read(byte *buffer, usize size);
};


#endif // BASED__FS_HPP
