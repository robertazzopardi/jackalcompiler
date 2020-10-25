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

typedef enum
{
    _leftBracket = 1,
    _rightBracket = 2,
    _int = 3,
    _float = 4,
    _var = 5,
    _type = 6,
    _func = 7,
    _operator = 8
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

typedef struct
{
    Token *tokens;
    size_t count;
} Sequence;

Sequence lex(FileContents filecontents);
void appendChar(Token *token, char line, size_t i);
void allocate(Sequence *seq, int *sum);
int isOperator(const char *s);
int isNumeric(const char *s);
char findLastOperaror(Token *tokens, size_t count);

#endif
