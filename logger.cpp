#include "logger.hpp"

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <errno.h>
#include <string.h>
#include <time.h>


bool logger::is(logger_type t)
{
    return (type & t) > 0;
}

void logger::flush()
{
    if (is(LOGGER__FILE))
    {
        char filename_buffer[512] = {};
        memcpy(filename_buffer, filename.data, filename.size);

        if (rotate_size > 0)
        {

            struct stat st;
            int fstat_result = stat(filename_buffer, &st);
            if (fstat_result < 0)
            {
                if (errno != ENOENT)
                {
                    fprintf(stderr, "Could not get stats on log file (errno: %d - \"%s\")\n", errno, strerror(errno));
                }
            }
            else
            {
                if (st.st_size > rotate_size)
                {
                    char filename_buffer_new[512] = {};
                    memcpy(filename_buffer_new, filename.data, filename.size);
                    filename_buffer_new[filename.size] = '.';
                    filename_buffer_new[filename.size + 1] = '1';

                    int rename_result = rename(filename_buffer, filename_buffer_new);
                    if (rename_result < 0)
                    {
                        fprintf(stderr, "Could not rename logger file to rotate (errno : %d - \"%s\")\n", errno, strerror(errno));
                    }
                }
            }
        }

        int fd = open(filename_buffer, O_CREAT | O_APPEND | O_RDWR, 0666);
        if (fd < 0)
        {
            fprintf(stderr, "Could not open log file (errno: %d - \"%s\")\n", errno, strerror(errno));
        }
        else
        {
            string_view ready_string = sb.get_string();
            isize bytes_written = write(fd, ready_string.data, ready_string.size);
            if (bytes_written < 0)
            {
                fprintf(stderr, "Error write logger file (errno: %d - \"%s\")\n", errno, strerror(errno));
            }
            sb.reset();
            close(fd);
        }
    }
}

void logger::log(struct code_location cl,
                 char const *fmt, ...)
{
    if (is(LOGGER__STREAM))
    {
        va_list args;
        va_start(args, fmt);
        dprintf(fd, "[%s:%d] ", cl.filename, cl.line);
        vdprintf(fd, fmt, args);
        dprintf(fd, "\n");
        va_end(args);
    }
    if (is(LOGGER__FILE))
    {
        va_list args;
        va_start(args, fmt);
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        sb.append("[%d-%02d-%02d %02d:%02d:%02d] ",
            tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
        sb.append(fmt, args);
        sb.append("\n");
        flush();
        va_end(args);
    }
}


