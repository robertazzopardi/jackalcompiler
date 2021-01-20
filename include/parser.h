/**
 * @file parser.h
 * @author robertazzopardi (robertazzopardi@icloud.com)
 * @brief Parser Declaration
 * @version 0.1
 * @date 2021-01-20
 *
 * @copyright Copyright (c) 2021
 *
 */

#ifndef _PARSER_
#define _PARSER_

#include "lexer.h"
#include "ast.h"

#define EVAL(res, e1, op, e2) \
    snprintf(res, 10, "%.6g", strtof(e1, NULL) op strtof(e2, NULL))

/**
 * @brief Convert the sequence of tokens to an Abstract Syntax Tree
 *
 * @param seq
 * @return Node*
 */
Node *parse(Sequence seq);

#endif
