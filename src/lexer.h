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
    lparen,
    rparen,
    integer,
    floatpt,
    var,
    type,
    func,
    op
} Attribute;

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
void allocate(Sequence *seq);
void freeTokens();

#endif