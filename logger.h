#ifndef LOGGER_H
#define LOGGER_H

#include "base.h"
#include "string_view.hpp"
#include "string_builder.hpp"
#include "code_location.h"


enum logger_type
{
    LOGGER__NONE    = 0,
    LOGGER__STREAM  = 0x1,
    LOGGER__FILE    = 0x2,
};

struct logger
{
    string_builder sb;
    
    uint32 type;
    int fd;
    string_view filename;
    isize rotate_size;
};

#define LOGGER(WHERE_TO_FIND_IT) struct logger *logger = (WHERE_TO_FIND_IT)->logger

void logger__flush(struct logger *logger);
void logger__log(struct logger *logger, struct code_location cl, char const *fmt, ...);
bool logger__is(struct logger *logger, logger_type type);
#define LOG(FORMAT, ...) logger__log(logger, CL_HERE, (FORMAT), ##__VA_ARGS__)


#endif // LOGGER_H
