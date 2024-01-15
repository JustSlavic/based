#ifndef LOGGER_H
#define LOGGER_H

#include "base.h"
#include "string_builder.h"
#include "code_location.h"


struct logger
{
    string_builder sb;
};

#define LOGGER(WHERE_TO_FIND_IT) struct logger *logger = (WHERE_TO_FIND_IT)->logger

#if DEBUG
void logger__log(struct logger *logger, struct code_location cl, char const *fmt, ...);
#define LOG(FORMAT, ...) logger__log(logger, CL_HERE, (FORMAT), ##__VA_ARGS__)
#else
void logger__log(struct logger *logger, char const *fmt, ...);
#define LOG(FORMAT, ...) logger__log(logger, (FORMAT), ##__VA_ARGS__)
#endif

void logger__flush_filename(struct logger *logger, char const *filename, usize rotate_size);
void logger__flush_file(struct logger *logger, int fd);


#endif // LOGGER_H
