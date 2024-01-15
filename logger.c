#include "logger.h"

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <errno.h>
#include <string.h>
#include <time.h>


void logger__log(struct logger *logger,
#if DEBUG
                 struct code_location cl,
#endif
                 char const *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
#if DEBUG
    printf("[%s:%d] ", cl.filename, cl.line);
    vprintf(fmt, args);
#else
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    string_builder__append_format(&logger->sb, "[%d-%02d-%02d %02d:%02d:%02d] ", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    string_builder__append_format_va_list(&logger->sb, fmt, args);
#endif
    va_end(args);
}

void logger__flush_file(struct logger *logger, int fd)
{
    memory_block string_to_write = string_builder__get_string(&logger->sb);
    isize bytes_written = write(fd, string_to_write.memory, string_to_write.size);
    if (bytes_written < 0)
    {
        fprintf(stderr, "Error write logger file (errno: %d - \"%s\")\n", errno, strerror(errno));
    }
    string_builder__reset(&logger->sb);
}

void logger__flush_filename(struct logger *logger, char const *filename, usize rotate_size)
{
    int fd = open(filename, O_NOFOLLOW | O_CREAT | O_APPEND | O_RDWR, 0666);
    if (fd < 0)
    {
        fprintf(stderr, "Could not open log file (errno: %d - \"%s\")\n", errno, strerror(errno));
        return;
    }

    if (rotate_size > 0)
    {
        struct stat st;
        int fstat_result = fstat(fd, &st);
        if (fstat_result < 0)
        {
            fprintf(stderr, "Could not get stats on log file (errno: %d - \"%s\")\n", errno, strerror(errno));
            return;
        }

        if (st.st_size > rotate_size)
        {
            close(fd);

            char new_name_buffer[512];
            memory__set(new_name_buffer, 0, sizeof(new_name_buffer));
            memory__copy(new_name_buffer, filename, cstring__size_no0(filename));
            memory__copy(new_name_buffer + cstring__size_no0(filename), ".1", 2);

            int rename_result = rename(filename, new_name_buffer);
            if (rename_result < 0)
            {
                fprintf(stderr, "Could not rename logger file to rotate (errno : %d - \"%s\")\n", errno, strerror(errno));
                return;
            }

            fd = open(filename, O_NOFOLLOW | O_CREAT | O_TRUNC | O_WRONLY, 0666);
            if (fd < 0)
            {
                fprintf(stderr, "Could not open new logger file (errno : %d - \"%s\")\n", errno, strerror(errno));
                return;
            }
        }
    }

    logger__flush_file(logger, fd);
}