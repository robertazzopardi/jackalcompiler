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

#ifndef __CODEGEN__
#define __CODEGEN__

typedef struct Node Node;

typedef struct ProgramArgs ProgramArgs;

/**
 * @brief Entry of the code generation
 *
 * @param root
 */
void generateCode(Node *root, ProgramArgs *paths);

/**
 * @brief parse the ast created from the parser
 *
 * @param node
 */
// void parseAst(const Node *node);

#endif
