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

typedef struct Token Token;

typedef struct FileContents FileContents;

typedef struct Sequence Sequence;

/**
 * @brief Built In types
 *
 */
enum Type { t_int, t_float, t_void };

/**
 * @brief Token Attributes
 *
 */
typedef enum {
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

/**
 * @brief Operator Associate
 *
 */
typedef enum { none, left_to_right, right_to_left } Associates;

/**
 * @brief Token
 *
 */
struct Token {
    char *value;
    Attribute attr;
    int precedence;
    Associates associate;
};

/**
 * @brief Array of tokens and array length
 *
 */
struct Sequence {
    Token *tokens;
    int count;
};

/**
 * @brief Print the sequence in a line
 *
 * @param seq
 */
void printSequence(const Sequence *seq);

/**
 * @brief Free the allocated sequence memory
 *
 * @param seq
 */
void cleanUpSeq(Sequence seq);

/**
 * @brief Tokenize the program file and return a sequence of the tokens
 *
 * @param fileContents
 * @return Sequence
 */
Sequence lex(FileContents *fileContents);

/**
 * @brief Check if a character is an operator
 *
 * @param o
 * @return int
 */
static inline int isOperator(const char o) {
    return (o == MOD || o == DIV || o == MUL || o == ADD || o == SUB ||
            o == EXP || o == LARR || o == RARR);
}

#endif
