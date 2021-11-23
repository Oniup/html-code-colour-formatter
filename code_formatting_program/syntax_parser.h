#ifndef SYNTAX_PARSER_H
#define SYNTAX_PARSER_H

/* ======= syntax key words ======= */

#define SYX_NOTHING     -1

#define SYX_VOID         0
#define SYX_STRUCT       1

#define SYX_TYPEDEF      2
#define SYX_SIGNED       3
#define SYX_UNSIGNED     4

#define SYX_SHORT        5
#define SYX_CHAR         6
#define SYX_INT          7
#define SYX_LONG         8
#define SYX_FLOAT        9
#define SYX_DOUBLE       10

#define SYX_FUNCTION     11
#define SYX_NEW_LINE     12
#define SYX_SPACE        13
#define SYX_DATA_TYPE    14
#define SYX_NUMBER       22
#define SYX_GREATER      15
#define SYX_LESS         16
#define SYX_QUOTE        17
#define SYX_FULL_STOP    18
#define SYX_MINUS        20
#define SYX_OPEN_BRACKET 21
#define SYX_COMA         23
#define SYX_SEMICOLON    24
#define SYX_ASTERISK     25

/* ======= syntax key words ======= */

static const int span_close_length = 6;
static const char* span_close = "</span>";

static const int sxy_datatype_length = 44;
static const char* syx_datatype[] = {
    "short",
    "char",
    "int",
    "long",
    "float",
    "double",
    "unsigned",
    "signed",

    "size_t",
    "uint8_t",
    "uint16_t",
    "uint32_t",
    "uint64_t",

    "short*",
    "char*",
    "int*",
    "long*",
    "float*",
    "double*",
    "unsigned*",
    "signed*",
    "void*",

    "size_t*",
    "uint8_t*",
    "uint16_t*",
    "uint32_t*",
    "uint64_t*",

    "if",
    "else",
    "for",
    "while",
    "continue",
    "break",
    "return",

    "const",
    "typedef",
    "void",
    "struct",
    "static",
    
    "#include",
    "#define",
    "#ifndef",
    "#endif",
    "#ifdef",
};

void parser_set_highlight();

#endif 