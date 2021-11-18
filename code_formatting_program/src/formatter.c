#include <code_formatting_program/formatter.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

    // stores the original src
    char* src;
    size_t src_len;

    char* output_location;

    char* new_src;
    size_t new_len;
} Formatter;

Formatter formatter;

Formatter formatter_init();

int formatter_get_src();
void formatter_get_output_location();

int continue_formate();
void formatter_destroy();
void formatter_run()
{
    formatter = formatter_init();

    int condition = X_FALSE;
    while (!condition)
    {
        if (formatter_get_src())
            condition = X_TRUE;
        else
            printf("\n\n");
    }

    printf("\n\n======= src =======\n\n%s\n\n======= src =======\ncontinue? (yes || no): ", formatter.src);
    int result = continue_formate();
    if (!result)
    {
        formatter_destroy();
        formatter_run();
        return;
    }

    formatter_get_output_location();

}

int formatter_get_src()
{
    printf("src to be formatted file location:\n");

    // getting the file's location from user input
    char file_path[200];
    for (size_t i = 0; i < 200; i++)
        file_path[i] = '\0';

    fgets(file_path, 200, stdin);
    for (size_t i = 0; i < 200; i++)
    {
        if (file_path[i] == '\n')
        {
            file_path[i] = '\0';
            break;
        }
    }

    // getting the contents from the file
    FILE* file = fopen(file_path, "rb");
    if (file == NULL)
    {
        printf("failed to find the path: %s\n", file_path);
        return X_FALSE;
    }

    fseek(file, 0, SEEK_END);
    size_t length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* buffer = (char*)malloc(sizeof(char*) * (length + 1));
    size_t read_length = fread(buffer, 1, length + 1, file);
    if (read_length != length)
    {
        free(buffer);
        printf("failed to read the file at the file path: %s\n", file_path);
        return X_FALSE;
    }

    buffer[length] = '\0';

    formatter.src = buffer;
    formatter.src_len = length;

    return X_TRUE;
}

void formatter_get_output_location()
{
    
}

int continue_formate()
{
    while (1)
    {
        char input[200];
        for (size_t i = 0; i < 200; i++)
            input[i] = '\0';

        fgets(input, 200, stdin);

        if (strcmp(input, "yes\n") == 0)
            return X_TRUE;
        else if (strcmp(input, "no\n") == 0)
            return X_FALSE;
    }
}

Formatter formatter_init()
{
    Formatter formatter = (Formatter) {
        .language = 0,

        .new_len = 0,
        .new_src = NULL,

        .output_location = NULL,

        .src_len = 0,
        .src = NULL
    };

    return formatter;
}

void formatter_destroy()
{
    if (formatter.new_src != NULL)
        free(formatter.new_src);
    if (formatter.output_location != NULL)
        free(formatter.output_location);
    if (formatter.src != NULL)
        free(formatter.src);
}

