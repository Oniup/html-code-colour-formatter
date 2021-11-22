#include <code_formatting_program/syntax_parser.h>
#include <code_formatting_program/formatter.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int parsing_string = X_FALSE;
static int parsing_parent = X_FALSE;
static int parsing_number_count = 0;

void parser_parse(FILE* out);
void parser_file_exists(const char* file_path);
void parser_reset(char* current_key, size_t* current_size);

char* parser_get_insert(char* current_key, size_t current_size);
int parser_check_numbers(char* insert, size_t current_size);

void parse_data_holder(FILE* out, char* current_key, size_t* current_size, char* last_key, size_t* last_size, int custom);
void parse_string(FILE* out, char* current_key, size_t* current_size, char* last_key, size_t* last_size);
void parse_parent(FILE* out, char* current_key, size_t* current_size, char* last_key, size_t* last_size, int custom, size_t* i);
void parse_function(FILE* out, char* current_key, size_t* current_size, char* last_key, size_t* last_size);

void parser_set_highlight()
{
    size_t location_length = strlen(formatter.output_location) + strlen(formatter.file_name) + 5;
    char* location = (char*)malloc(sizeof(char*) * location_length);
    strcpy(location, formatter.output_location);
    strcat(location, formatter.file_name);
    strcat(location, ".html");

    // checking whether it exists and asking if user wants to replace it
    parser_file_exists(location);

    // create output file
    FILE* out = NULL;
    out = fopen(location, "w");
    if (out == NULL)
    {
        printf("[SRC_HTML_FORMATTER]: failed to create %s\n", location);
        getchar();
        exit(-1);
    }

    fprintf(out, "<div class=\"container code\">\n<pre>");
    parser_parse(out);
    fprintf(out, "\n</pre>\n</div>");

    fclose(out);
    free(location);
}

void parser_parse(FILE* out)
{
    size_t out_length = strlen(formatter.src);
    if (out_length < 1)
    {
        printf("\n[SRC_HTML_FORMATTER]: there is nothing in the file");
        return;
    }

    // used to store the current keyword for parsing
    char current_key[100];
    size_t current_size = 0;
    
    memset(current_key, '\0', 100);

    // is set to NULL if there is no need for it, only has memory when there could be a custom data type
    char* last_key = NULL;
    size_t last_size = 0;

    // parse file
    for (size_t i = 0; i < out_length; i++)
    {
        if (formatter.src[i] == '\r')
            continue;
        
        current_key[current_size] = formatter.src[i];
        
        // find if the word is complete
        int failed = X_FALSE;
        switch (current_key[current_size])
        {
        // checking when the end of the keyword is or line (no colouring)
        case '\n':
            if (!parsing_string)
            {
                parse_data_holder(out, current_key, &current_size, last_key, &last_size, SYX_NEW_LINE);
                continue;
            }
            break;
        case '>':
            parse_data_holder(out, current_key, &current_size, last_key, &last_size, SYX_GREATER);
            continue;
        case '<':
            parse_data_holder(out, current_key, &current_size, last_key, &last_size, SYX_LESS);
            continue;
        case ' ':
            parse_data_holder(out, current_key, &current_size, last_key, &last_size, SYX_NOTHING);
            continue;

        // checking if there is a string
        case '\"':
        case '\'':
            if (!parsing_parent)
            {
                parse_string(out, current_key, &current_size, last_key, &last_size);
                continue;
            }
            break;

        // checking for a function or macro
        case '(':
            parse_function(out, current_key, &current_size, last_key, &last_size);
            continue;
            break;

        // checking for parented data in structs
        case '-':
            if (!parsing_string)
            {
                parse_parent(out, current_key, &current_size, last_key, &last_size, SYX_MINUS, &i);
                continue;
            }
            break;
        case '.':
            if (!parsing_string)
            {
                parse_parent(out, current_key, &current_size, last_key, &last_size, SYX_NOTHING, &i);
                continue;
            }
            break;
        }

        // counting the amount of numbers
        if (current_key[current_size] < 58 && current_key[current_size] > 47)
            parsing_number_count++;
        else
            switch (current_key[current_size])
            {
            case 'f':
            case '.':
            case ',':
            case ';':
            case '\n':
            case ')':
                parsing_number_count++;
                break;
            }

        current_size++;
    }

    getchar();
}

char* parser_get_insert(char* current_key, size_t current_size)
{
    if (current_size > 0)
    {
        char* insert = (char*)malloc(sizeof(char*) * current_size);
        insert[current_size] = '\0';
        strncpy(insert, current_key, current_size);

        return insert;
    }
    else
        return NULL;
}

void parse_data_holder(FILE* out, char* current_key, size_t* current_size, char* last_key, size_t* last_size, int custom)
{
    static char** custom_stack = NULL;
    static int custom_stack_size = 0;

    char* insert = parser_get_insert(current_key, *current_size);
    
    switch (custom)
    {
    case SYX_NEW_LINE:
        if (insert != NULL)
        {
            int is_number = parser_check_numbers(current_key, *current_size);
            if (is_number == SYX_NOTHING)
                fprintf(out, "%s\n", insert);
            else 
            {
                if (insert[*current_size - 1] == ';')
                {
                    // removing the , and adding it outside of the span container
                    char* temp = (char*)malloc(sizeof(char*) * (*current_size - 1));
                    temp[*current_size - 1] = '\0';
                    strncpy(temp, insert, *current_size - 1);
                    printf("temp: %s\n", temp);

                    fprintf(out, "<span class=\"c n\">%s%s;\n", temp, span_close);

                    free(temp);
                }
            }
        }
        else    
            fprintf(out, "\n");
        break;
        
    case SYX_GREATER:
        if (insert != NULL)
        {
            if (parsing_parent)
            {
                fprintf(out, "%s%s&lt;", insert, span_close);
                parsing_parent = X_FALSE;
            }
            else
                fprintf(out, "%s&lt;", insert);
        }
        else    
            fprintf(out, "&lt;");
        break;

    case SYX_LESS:
        if (insert != NULL)
        {
            if (parsing_parent)
            {
                fprintf(out, "%s%s&gt;", insert, span_close);
                parsing_parent = X_FALSE;
            }
            else
                fprintf(out, "%s&gt;", insert);
        }
        else    
            fprintf(out, "&gt;");
        break;

    case SYX_NOTHING:
        if (insert != NULL)
        {
            if (parsing_parent)
            {
                if (insert != NULL)
                {
                    char* temp = (char*)malloc(sizeof(char*) * (*current_size - 1));
                    temp[*current_size - 1] = '\0';
                    strncpy(temp, insert, *current_size - 1);

                    if (insert[*current_size - 1] == ';')
                        fprintf(out, "%s%s; ", temp, span_close);
                    else if (insert[*current_size - 1] == ')')
                        fprintf(out, "%s%s) ", temp, span_close);
                    else if (insert[*current_key - 1] == ',')
                        fprintf(out, "%s%s, ", temp, span_close);
                    else
                        fprintf(out, "%s%s ", insert, span_close);

                    free(temp);
                }
                else
                    fprintf(out, "%s ", span_close);

                parsing_parent = X_FALSE;
                break;
            }

            int type = SYX_NOTHING;
            if (type == SYX_NOTHING)
            {
                // checking whether it is a c data type
                for (size_t i = 0; i < sxy_datatype_length; i++)
                {
                    if (strcmp(insert, syx_datatype[i]) == 0)
                    {
                        type = SYX_DATA_TYPE;
                        break;
                    }
                }
            }

            if (type == SYX_NOTHING)
                type = parser_check_numbers(current_key, *current_size);

            if (type == SYX_DATA_TYPE)
                fprintf(out, "<span class=\"c dt\">%s%s ", insert, span_close);
            else if (type == SYX_NUMBER)
            {
                if (insert[*current_size - 1] == ',' || insert[*current_size - 1] == ';')
                {
                    // removing the , and adding it outside of the span container
                    char* temp = (char*)malloc(sizeof(char*) * (*current_size - 1));
                    temp[*current_size - 1] = '\0';
                    strncpy(temp, insert, *current_size - 1);

                    if (insert[*current_size - 1] == ',')
                        fprintf(out, "<span class=\"c n\">%s%s, ", temp, span_close);
                    else if (insert[*current_size - 1] == ';')
                        fprintf(out, "<span class=\"c n\">%s%s; ", temp, span_close);

                    free(temp);
                }
                else
                    fprintf(out, "<span class=\"c n\">%s%s ", insert, span_close);
            }
            else
                fprintf(out, "%s ", insert);
        }
        else
            fprintf(out, " ");
        break;
    }

    if (insert != NULL)
        free(insert);

    parser_reset(current_key, current_size);
}

void parse_string(FILE* out, char* current_key, size_t* current_size, char* last_key, size_t* last_size)
{
    char* insert = parser_get_insert(current_key, *current_size);
    if (insert != NULL)
    {
        fprintf(out, "%s\"</span>", insert);
        parsing_string = X_FALSE;
    }
    else
    {
        parsing_string = X_TRUE;
        fprintf(out, "<span class=\"c s\">\"");
    }
    
    if (insert != NULL)
        free(insert);

    parser_reset(current_key, current_size);
}

void parse_parent(FILE* out, char* current_key, size_t* current_size, char* last_key, size_t* last_size, int custom, size_t* i)
{
    char* insert = parser_get_insert(current_key, *current_size);

    int is_numbers = parser_check_numbers(insert, *current_size);
    if (is_numbers == SYX_NOTHING)
    {
        if (!parsing_parent)
        {
            if (current_key[*current_size] == '.')
            {
                if (insert != NULL)
                {
                    parsing_parent = X_TRUE;
                    fprintf(out, "%s.<span class=\"c p\">", insert);
                }
                else
                    fprintf(out, ".");
            }
            else if (current_key[*current_size] == '-')
            {
                if (insert != NULL)
                {   
                    if (formatter.src[*i + 1] == '>')
                    {
                        parsing_parent = X_TRUE;
                        fprintf(out, "%s-&gt;<span class=\"c p\">", insert);
                        *i = *i + 1;
                    }
                    else 
                        fprintf(out, "%s-", insert);
                }
                else
                    fprintf(out, "-");
            }
        }
        else if (parsing_parent)
        {
            if (current_key[*current_size] == '.')
                fprintf(out, "%s%s.", insert, span_close);
            else if (current_key[*current_size] == '-')
            {
                if (formatter.src[*i + 1] == '>')
                {
                    *i = *i + 1;

                    fprintf(out, "%s%s-&gt;<span class=\"c p\">", insert, span_close);

                    if (insert != NULL)
                        free(insert);

                    parser_reset(current_key, current_size);

                    return;
                }
                else
                    fprintf(out, "%s%s-", insert, span_close);
            }
            
            parsing_parent = X_FALSE;
        }

        parser_reset(current_key, current_size);
    }
    else
    {
        *current_size = *current_size + 1;
        parsing_number_count++;
    }

    if (insert != NULL)
        free(insert);
}

void parse_function(FILE* out, char* current_key, size_t* current_size, char* last_key, size_t* last_size)
{
    char* insert = parser_get_insert(current_key, *current_size);
    if (!parsing_parent)
    {
        if (insert != NULL)
            fprintf(out, "<span class=\"c f\">%s%s(", insert, span_close);
        else
            fprintf(out, "(");
    }
    else
    {
        if (insert != NULL)
            fprintf(out, "%s%s(", insert, span_close);    
        else
            fprintf(out, "%s(", span_close);
    }
    
    if (insert != NULL)
        free(insert);

    parser_reset(current_key, current_size);
}

int parser_check_numbers(char* insert, size_t current_size)
{
    if (parsing_string)
        return SYX_NOTHING;

    if (parsing_number_count == current_size)
    {
        int number_count = 0;
        // check if they are all valid for being considered a number 
        for (size_t i = 0; i < current_size; i++)
        {
            if (insert[i] == '.')
                continue;
            else if (insert[i] == 'f')
                continue;
            else if (insert[i] == ',')
                continue;
            else if (insert[i] == ';')
                continue;
            else if (insert[i] == '\n')
                continue;
            else if (insert[i] == ')')
                continue;
            else if (insert[i] > 57 || insert[i] < 48)
                return SYX_NOTHING;
            else 
                number_count++;
        }

        if (number_count == 0)
            return SYX_NOTHING;
    }
    else
        return SYX_NOTHING;

    return SYX_NUMBER;
}

void parser_reset(char* current_key, size_t* current_size)
{
    memset(current_key, '\0', 100);
    *current_size = 0;
    parsing_number_count = 0;
}

void parser_file_exists(const char* file_path)
{
    // checking if the file exists
    int exists = X_FALSE;
    FILE* file = NULL;
    if (file = fopen(file_path, "r"))
    {
        fclose(file);
        exists = X_TRUE;
    }
    else
        exists = X_FALSE;

    if (exists)
    {
        // give user choices whether they want to replace it
        printf("[SRC_HTML_FORMATTER]: %s already exists, do you want to replace it? (yes || no):\n[INPUT]: ", file_path);
        int result = continue_formate();
        if (result)
        {
            result = remove(file_path);
            if (!result)
            {
                printf("[SRC_HTML_FORMATTER]: \ndeleted %s... replacing it with a new %s\ncontinue...", file_path, file_path);
                getchar();
                printf("\n");
            }
            else
            {
                printf("[SRC_HTML_FORMATTER]: something when wrong when replacing the index file\n");
                getchar();
                exit(-1);
            }
        }
        else 
        {
            printf("[SRC_HTML_FORMATTER]: exiting program...");
            getchar();
            exit(1);
        }
    }
}
