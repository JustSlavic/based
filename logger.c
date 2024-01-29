#include "logger.h"

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <errno.h>
#include <string.h>
#include <time.h>


void logger__flush(struct logger *logger)
{
    if (logger->type == LOGGER__FILE)
    {
        if (logger->rotate_size > 0)
        {
            struct stat st;
            int fstat_result = stat(logger->filename, &st);
            if (fstat_result < 0)
            {
                if (errno != ENOENT)
                {
                    fprintf(stderr, "Could not get stats on log file (errno: %d - \"%s\")\n", errno, strerror(errno));
                }
            }
            else
            {
                if (st.st_size > logger->rotate_size)
                {
                    char new_filename[512];
                    memory__set(new_filename, 0, sizeof(new_filename));
                    memory__copy(new_filename, logger->filename, array_count(logger->filename) - 1);
                    memory__copy(new_filename + array_count(logger->filename) - 1, ".1", 2);

                    int rename_result = rename(logger->filename, new_filename);
                    if (rename_result < 0)
                    {
                        fprintf(stderr, "Could not rename logger file to rotate (errno : %d - \"%s\")\n", errno, strerror(errno));
                    }
                }
            }
        }

        int fd = open(logger->filename, O_CREAT | O_APPEND | O_RDWR, 0666);
        if (fd < 0)
        {
            fprintf(stderr, "Could not open log file (errno: %d - \"%s\")\n", errno, strerror(errno));
        }
        else
        {
            memory_block string_to_write = logger->sb.get_string();
            isize bytes_written = write(fd, string_to_write.memory, string_to_write.size);
            if (bytes_written < 0)
            {
                fprintf(stderr, "Error write logger file (errno: %d - \"%s\")\n", errno, strerror(errno));
            }
            logger->sb.reset();
            close(fd);
        }
    }
}

void logger__log(struct logger *logger,
                 struct code_location cl,
                 char const *fmt, ...)
{
    // @todo: make 2 independent settings: send to stream / file
    //                                     print [filename:line] / print [date time]
    va_list args;
    va_start(args, fmt);
    if (logger->type == LOGGER__STREAM)
    {
        dprintf(logger->fd, "[%s:%d] ", cl.filename, cl.line);
        vdprintf(logger->fd, fmt, args);
        dprintf(logger->fd, "\n");
    }
    else if (logger->type == LOGGER__FILE)
    {
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        logger->sb.append("[%d-%02d-%02d %02d:%02d:%02d] ",
            tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
        logger->sb.append(fmt, args);
        logger->sb.append("\n");
        logger__flush(logger);
    }
    va_end(args);
}
