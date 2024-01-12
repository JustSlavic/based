#ifndef BASED__CODE_LOCATION_H
#define BASED__CODE_LOCATION_H


struct code_location
{
    char const *function;
    char const *filename;
    int line;
};
typedef struct code_location code_location;


FORCE_INLINE
code_location make_code_location(char const *function, char const *filename, int line)
{
    code_location result;
    result.function = function;
    result.filename = filename;
    result.line = line;
    return result;
}

#define CL_HERE make_code_location(__FUNCTION__, __FILE__, __LINE__)


#endif // BASED__CODE_LOCATION_H
