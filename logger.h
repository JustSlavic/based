#ifndef LOGGER_H
#define LOGGER_H

#include "base.h"
#include "string_builder.h"
#include "code_location.h"
#include "array.h"


struct logger
{
    string_builder sb;
    enum
    {
        LOGGER__NONE,
        LOGGER__STREAM,
        LOGGER__FILE,
    } type;
    union
    {
        int fd;
        struct
        {
            array(char) filename;
            usize rotate_size;
        };
    };
};

#define LOGGER(WHERE_TO_FIND_IT) struct logger *logger = (WHERE_TO_FIND_IT)->logger

void logger__flush(struct logger *logger);
void logger__log(struct logger *logger, struct code_location cl, char const *fmt, ...);
#define LOG(FORMAT, ...) logger__log(logger, CL_HERE, (FORMAT), ##__VA_ARGS__)


#endif // LOGGER_H
