#ifndef _LEXER_
#define _LEXER_

#include "filehandler.h"

#define ADD '+'
#define SUB '-'
#define MUL '*'
#define DIV '/'
#define MOD '%'
#define EXP '^'
#define SPA ' '
#define RPR ')'
#define LPR '('
#define NLI '\n'
#define DOT '.'
#define CMM ','

// #define ALLOCATE_MEM(type, seq)                                                  \
//     ({                                                                           \
//         seq.count++;                                                             \
//         seq.tokens = realloc(seq.tokens, (seq.count + 1) * sizeof(*seq.tokens)); \
//         seq.tokens[seq.count - 1].d_type##type = NULL;                           \
//         seq.tokens[seq.count].d_type##type = NULL;                               \
//         seq.tokens[seq.count - 1].attr = 0;                                      \
//         seq.tokens[seq.count].attr = 0;                                          \
//     })

typedef enum
{
    _leftBracket = 1,
    _rightBracket = 2,
    _int = 3,
    _float = 4,
    _var = 5,
    _type = 6,
    _func = 7,
    _operator = 8,
    _comma = 9
} Attribute;

typedef enum
{
    none,
    left_to_right,
    right_to_left
} Associates;

typedef struct
{
    char *value;
    Attribute attr;
} Token;

// typedef struct
// {
//     union type
//     {
//         char *s;
//         char c;
//         int i;
//         float f;
//         double d;
//     } d_type;
//     Attribute attr;
// } Token;

typedef struct
{
    Token *tokens;
    size_t count;
} Sequence;

Sequence lex(FileContents filecontents);
void appendChar(Token *token, char line, size_t i);
void allocate(Sequence *seq);
// int isOperator(const char *s);
// int isNumeric(const char *s);
// char findLastOperaror(Token *tokens, size_t count);

#endif
