#ifndef BASED__STRING_VIEW
#define BASED__STRING_VIEW

#include "base.h"


struct string_view {
    char const *data;
    usize size;
};
typedef struct string_view string_view;


#endif // BASED__STRING_VIEW
