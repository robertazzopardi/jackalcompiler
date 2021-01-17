/**
 * @file codegen.h
 * @author robertazzopardi (robertazzopardi@icloud.com)
 * @brief Interface of the llvm ir generation code
 * @version 0.1
 * @date 2021-01-16
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
#include <llvm-c/Transforms/IPO.h>
#include <llvm-c/Transforms/InstCombine.h>
#include <llvm-c/Transforms/AggressiveInstCombine.h>

#include "ast.h"
#include "filehandler.h"
#include "stack.h"

#define PRINT "print"

/**
 * @brief Entry of the code generation
 *
 * @param root
 */
void generateCode(Node *root, ProgramArgs paths);

/**
 * @brief parse the ast created from the parser
 *
 * @param node
 */
// void parseAst(const Node *node);

#endif
