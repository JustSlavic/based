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
    bool32 should_rename = rotate_size > 0;
    bool32 renamed = false;
    if (should_rename)
    {
        struct stat st;
        int fstat_result = stat(filename, &st);
        if (fstat_result < 0)
        {
            fprintf(stderr, "Could not get stats on log file (errno: %d - \"%s\")\n", errno, strerror(errno));
        }
        else
        {
            if (st.st_size > rotate_size)
            {
                char new_filename[512];
                memory__set(new_filename, 0, sizeof(new_filename));
                memory__copy(new_filename, filename, cstring__size_no0(filename));
                memory__copy(new_filename + cstring__size_no0(filename), ".1", 2);

                int rename_result = rename(filename, new_filename);
                if (rename_result < 0)
                {
                    fprintf(stderr, "Could not rename logger file to rotate (errno : %d - \"%s\")\n", errno, strerror(errno));
                }
                else
                {
                    renamed = true;
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
        bool32 ok = true;
        if (should_rename && !renamed)
        {
            int truncate_result = ftruncate(fd, 0);
            if (truncate_result < 0)
            {
                fprintf(stderr, "Could not truncate log file (errno: %d - \"%s\")\n", errno, strerror(errno));
            }
            else
            {
                ok = false;
            }
        }

        if (ok) logger__flush_file(logger, fd);
        close(fd);
    }
}
