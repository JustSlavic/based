#ifndef BASED__STRING_VIEW
#define BASED__STRING_VIEW

#include "base.h"


struct string_view {
    char const *data;
    usize size;
};
typedef struct string_view string_view;

FORCE_INLINE int sv__compare(string_view sv1, string_view sv2);


#endif // BASED__STRING_VIEW
