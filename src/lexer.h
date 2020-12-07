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
#define ESC '\0'

#define DOT '.'
#define CMM ','
#define RARR '>'
#define LARR '<'

#define FUNC "func"
#define VOID "void"
#define MAIN "main"
#define IF "if"
#define ELIF "elif"
#define ELSE "else"
#define CONDITION "condition"

typedef enum
{
    _none,
    _leftBracket,
    _rightBracket,
    _int,
    _float,
    _var,
    _type,
    _funcCall,
    _operator,
    _comma,
    _funcReturnType,
    _funcDef,
    _funcName,
    _if,
    _elif,
    _else,
    _condition
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
    // union
    // {
    //     float f;
    //     double d;
    //     int i;
    //     char *s;
    //     char c;
    // } value;
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

void printSequence(const Sequence seq);

#endif
