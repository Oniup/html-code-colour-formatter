#ifndef CODE_FORMATTER_H
#define CODE_FORMATTER_H

#define LANGUAGE_C      0
#define LANGUAGE_CPP    1
#define LANGUAGE_CSHARP 2
#define LANGUAGE_JAVA   3

#define LANGUAGE_PHP    4
#define LANGUAGE_HTML   5

#define X_TRUE  1
#define X_FALSE 0

typedef struct Formatter
{
    // defines the language that is being formatted
    int language; // TODO: add this in later, for now just make it work with .c files

    char* file_name;

    // stores the original src
    char* src;

    char* output_location;
} Formatter;

Formatter formatter;

void formatter_run();
int continue_formate();


#endif 