#ifndef LOGGER_H
#define LOGGER_H

#include <base.h>
#include <code_location.h>


struct logger
{
    char const *filename;
    string_builder sb;
};

#if DEBUG

void logger__log(struct logger *logger, struct code_location cl, char const *fmt, ...);
void logger__log_untrusted(struct logger *logger, struct code_location cl, char const *buffer, usize size);
#define LOG(FORMAT, ...) logger__log(logger, CL_HERE, (FORMAT) VA_ARGS(__VA_ARGS__))
#define LOG_UNTRUSTED(BUFFER, SIZE) logger__log_untrusted(logger, CL_HERE, (char const *)(BUFFER), (SIZE))

#else

void logger__log(struct logger *logger, char const *fmt, ...);
void logger__log_untrusted(struct logger *logger, char const *buffer, usize size);
#define LOG(FORMAT, ...) logger__log(logger, (FORMAT) VA_ARGS(__VA_ARGS__))
#define LOG_UNTRUSTED(BUFFER, SIZE) logger__log_untrusted(logger, (char const *)(BUFFER), (SIZE))

#endif

void logger__flush(struct logger *logger);


#endif // LOGGER_H
