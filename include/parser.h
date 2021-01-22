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

/**
 * @brief Convert the sequence of tokens to an Abstract Syntax Tree
 *
 * @param seq
 * @return Node*
 */
Node *parse(Sequence seq);

#endif
