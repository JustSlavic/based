#ifndef BASED__CODE_LOCATION_H
#define BASED__CODE_LOCATION_H


struct code_location
{
    char const *function;
    char const *filename;
    int line;
};

#define CL_HERE (struct code_location){ .function = __FUNCTION__, .filename = __FILE__, .line = __LINE__, }


#endif // BASED__CODE_LOCATION_H
