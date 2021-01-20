/**
 * @file lexer.h
 * @author robertazzopardi (robertazzopardi@icloud.com)
 * @brief Declaration of the lexer class
 * @version 0.1
 * @date 2021-01-19
 *
 * @copyright Copyright (c) 2021
 *
 */

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

#define RET_TYPE "->"

#define FUNC "func"
#define VOID "void"
#define MAIN "main"
#define IF "if"
#define ELIF "elif"
#define ELSE "else"
#define CONDITION "condition"

#define GET_FORMAT(o) ((o == MOD || o == LARR || o == RARR) ? "%d" : "%.6g")

typedef enum
{
    t_int,
    t_float,
    t_void
} Type;

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
    Attribute attr;
    int precedence;
    Associates associate;
} Token;

typedef struct
{
    Token *tokens;
    size_t count;
} Sequence;

int isOperator(const char o);

void printSequence(const Sequence seq);

void cleanUpSeq(Sequence seq);

Sequence lex(FileContents fileContents);

#endif
