#include <code_formatting_program/formatter.h>

#include <code_formatting_program/syntax_parser.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Formatter formatter_init();

// getting the data
int formatter_get_src();
void formatter_get_output_location();

// conditions and cleanup
int continue_formate();
void formatter_destroy();

void formatter_run()
{
    formatter = formatter_init();

    // getting the src to be formatted
    int condition = X_FALSE;
    while (!condition)
    {
        if (formatter_get_src())
            condition = X_TRUE;
        else
            printf("\n\n");
    }

    // confirming that it is the right one 
    printf("\n\n========= src =========\n%s\n\n========= src =========\n[SRC_HTML_FORMATTER]: continue? (yes || no):\n[INPUT]: ", formatter.src);

    int result = continue_formate();
    if (!result)
    {
        formatter_destroy();
        formatter_run();
        return;
    }

    // getting the output location where the formatted version will be stored
    formatter_get_output_location();

    parser_set_highlight();

    formatter_destroy();

    printf("[SRC_HTML_FORMATTER]: success, exit program...");
    getchar();
}

int formatter_get_src()
{
    printf("[SRC_HTML_FORMATTER]: src to be formatted file location:\n[INPUT]: ");

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
        printf("[SRC_HTML_FORMATTER]: failed to find the path: %s\n", file_path);
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
        printf("[SRC_HTML_FORMATTER]: failed to read the file at the file path: %s\n", file_path);
        return X_FALSE;
    }

    buffer[length] = '\0';

    formatter.src = buffer;

    return X_TRUE;
}

void formatter_get_output_location()
{
    for (size_t i = 0; i < 2; i++)
    {
        if (!i)
            printf("[SRC_HTML_FORMATTER]: enter location of the new file:\n[INPUT]: ");
        else 
            printf("\n[SRC_HTML_FORMATTER]: enter name of the file:\n[INPUT]: ");

        // get user input
        char input[200];
        for (size_t i = 0; i < 200; i++)
            input[i] = '\0';

        fgets(input, 200, stdin);
        printf("\n");

        for (size_t i = 0; i < 200; i++)
        {
            if (input[i] == '\n')
            {
                input[i] = '\0';
                break;
            }
        }

        // store input
        size_t length = strlen(input);
        if (!i)
        {
            // setting the location 
            formatter.output_location = (char*)malloc(sizeof(char*) * length);
            strcpy(formatter.output_location, input);
        }
        else
        {
            // setting the file name
            formatter.file_name = (char*)malloc(sizeof(char*) * length);
            strcpy(formatter.file_name, input);
        }
    }
}   

int continue_formate()
{
    while (1)
    {
        char input[200];
        for (size_t i = 0; i < 200; i++)
            input[i] = '\0';

        fgets(input, 200, stdin);

        printf("\n");

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

        .output_location = NULL,
        .src = NULL
    };

    return formatter;
}

void formatter_destroy()
{
    if (formatter.output_location != NULL)
        free(formatter.output_location);
    if (formatter.src != NULL)
        free(formatter.src);
}


/* ================== PARSER ================== */


