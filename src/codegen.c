/**
 * @file codegen.c
 * @author Robert Azzopardi (you@domain.com)
 * @brief Implementation to generate llvm code
 * @version 0.1
 * @date 2021-01-13
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "../include/codegen.h"

/**
 * @brief Create a Module object
 *
 * @param moduleName
 * @return LLVMModuleRef
 */
LLVMModuleRef createModule(const char *moduleName)
{
    return LLVMModuleCreateWithName(moduleName);
}

/**
 * @brief Create a Function object
 *
 * @param module
 * @param funcName
 */
void createFunction(const LLVMModuleRef module, const char *funcName, LLVMTypeRef *paramTypes, const unsigned paramLen)
{

    // LLVMTypeRef param_types[] = {LLVMInt32Type(), LLVMInt32Type()};

    // LLVMTypeRef ret_type = LLVMFunctionType(LLVMInt32Type(), paramTypes, paramLen, 0);
    LLVMTypeRef ret_type = LLVMFunctionType(LLVMVoidType(), paramTypes, paramLen, 0);

    LLVMValueRef sum = LLVMAddFunction(module, funcName, ret_type);

    // create the function block
    char *blockName;
    blockName = malloc(strlen(funcName) * sizeof(*blockName));
    strcpy(blockName, funcName);
    strcat(blockName, "_block");

    LLVMBasicBlockRef entry = LLVMAppendBasicBlock(sum, blockName);

    // Instruction builders
    LLVMBuilderRef builder = LLVMCreateBuilder();
    LLVMPositionBuilderAtEnd(builder, entry);

    // LLVM IR instruction
    // LLVMValueRef tmp = LLVMBuildAdd(builder, LLVMGetParam(sum, 0), LLVMGetParam(sum, 1), "tmp");
    // LLVMBuildRet(builder, tmp);
    LLVMBuildRetVoid(builder);
}

/**
 * @brief validates a module through LLVM
 *
 * @param module
 */
void verifyModule(const LLVMModuleRef module)
{
    char *error = NULL;
    LLVMVerifyModule(module, LLVMAbortProcessAction, &error);
    LLVMDisposeMessage(error);
}

/**
 * @brief parse the ast created from the parser
 *
 * @param node
 */
void parseAst(Node *node, const LLVMModuleRef module)
{
    StackNode *st = NULL;
    push(&st, node);

    while (st)
    {
        Node *curr = top(st);
        pop(&st);
        //traverse current node first
        printf("%s\n", curr->data.value);

        switch (curr->data.attr)
        {
        case _funcDef:
            // if next in tree
            // if there is a function made add to module
            break;

        case _funcName:
        {
            // {LLVMVoidType()};
            // create llvm ir function
            createFunction(module, curr->data.value, NULL, 0);
        }
        break;

        default:
            break;
        }

        //put right subtree first
        if (curr->rightNode)
            push(&st, curr->rightNode);
        //put left subtree then as it will
        //stay in top for next iteration
        if (curr->leftNode)
            push(&st, curr->leftNode);
    }

    free(st);
}

/**
 * @brief Entry method to generate the llvm ir code
 *
 * @param root
 */
void generateCode(Node *root)
{
    // Check if there is actually code in the file
    if (!root->leftNode && !root->rightNode)
    {
        printf("No source code found could not compile\n");
        exit(0);
    }

    // create module
    LLVMModuleRef module = createModule(filename);

    // create llvm ir function
    // createFunction(module, "main");
    parseAst(root, module);
    printf("\n");

    // Analyse module
    verifyModule(module);

    // write to file
    writeLLVMIR(module);
}
