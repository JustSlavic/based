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
    if (logger__is(logger, LOGGER__FILE))
    {
        char filename[512] = {};
        memory__copy(filename, logger->filename.data, logger->filename.size);

        if (logger->rotate_size > 0)
        {

            struct stat st;
            int fstat_result = stat(filename, &st);
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
                    char new_filename[512] = {};
                    memory__copy(new_filename, logger->filename.data, logger->filename.size);
                    new_filename[logger->filename.size] = '.';
                    new_filename[logger->filename.size + 1] = '1';

                    int rename_result = rename(filename, new_filename);
                    if (rename_result < 0)
                    {
                        fprintf(stderr, "Could not rename logger file to rotate (errno : %d - \"%s\")\n", errno, strerror(errno));
                    }
                }
            }
        }

        int fd = open(filename, O_CREAT | O_APPEND | O_RDWR, 0666);
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
    if (logger__is(logger, LOGGER__STREAM))
    {
        va_list args;
        va_start(args, fmt);
        dprintf(logger->fd, "[%s:%d] ", cl.filename, cl.line);
        vdprintf(logger->fd, fmt, args);
        dprintf(logger->fd, "\n");
        va_end(args);
    }
    if (logger__is(logger, LOGGER__FILE))
    {
        va_list args;
        va_start(args, fmt);
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        logger->sb.append("[%d-%02d-%02d %02d:%02d:%02d] ",
            tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
        logger->sb.append(fmt, args);
        logger->sb.append("\n");
        logger__flush(logger);
        va_end(args);
    }
}

bool logger__is(struct logger *logger, logger_type type)
{
    return (logger->type & type) > 0;
}

