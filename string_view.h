#ifndef BASED__STRING_VIEW
#define BASED__STRING_VIEW


typedef struct string_view
{
    char const *data;
    usize size;
} string_view;


#endif // BASED__STRING_VIEW
