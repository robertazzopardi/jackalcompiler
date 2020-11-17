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
    none = 0,
    left_to_right = 1,
    right_to_left = 2
} Associates;

typedef struct
{
    char value[31];
    Attribute attr;
    int precedence;
    Associates associate;
} Token;

typedef struct
{
    Token *tokens;
    size_t count;
} Sequence;

Sequence lex(FileContents filecontents);
int isOperator(const char o);

#endif
