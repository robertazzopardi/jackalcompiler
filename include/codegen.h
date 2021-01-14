/**
 * @file codegen.h
 * @author Robert Azzopardi-Yashi (you@domain.com)
 * @brief Interface of the llvm ir generation code
 * @version 0.1
 * @date 2021-01-13
 *
 * @copyright Copyright (c) 2021
 *
 */

#ifndef _CODEGEN_
#define _CODEGEN_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <llvm-c/Core.h>
#include <llvm-c/Analysis.h>

#include "ast.h"
#include "filehandler.h"
#include "stack.h"

/**
 * @brief Entry of the code generation
 *
 * @param root
 */
void generateCode(Node *root);

/**
 * @brief parse the ast created from the parser
 *
 * @param node
 */
// void parseAst(const Node *node);

#endif
