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
    _leftBracket,
    _rightBracket,
    _int,
    _float,
    _var,
    _type,
    _func,
    _operator,
    _comma
} Attribute;

typedef enum
{
    none,
    left_to_right,
    right_to_left
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
void addMissingBrackets(Sequence *seq, const size_t len, unsigned skips);
void addNumber(Sequence *seq, unsigned *j, const char *line);
void parseLine(Sequence *seq, const char *line, size_t *prev);
int isOperator(const char o);
void printSequence(const Sequence seq);

#endif
