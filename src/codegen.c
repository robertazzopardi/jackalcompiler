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

char *getBlockName(const char *funcName)
{
    char *blockName;
    blockName = malloc(strlen(funcName) * sizeof(*blockName));
    strcpy(blockName, funcName);
    strcat(blockName, "_block");
    return blockName;
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
    // char *blockName;
    // blockName = malloc(strlen(funcName) * sizeof(*blockName));
    // strcpy(blockName, funcName);
    // strcat(blockName, "_block");
    char *blockName = getBlockName(funcName);

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
 * @brief Add function to module
 *
 * @param builder
 * @param blockReference
 */
void commitFunction(const LLVMBuilderRef builder, const LLVMBasicBlockRef blockReference)
{
    LLVMPositionBuilderAtEnd(builder, blockReference);

    // LLVMBuildRetVoid(builder);
    LLVMBuildRet(builder, LLVMConstInt(LLVMInt32Type(), 0, LLVMRealPredicateFalse));
}

/**
 * @brief parse the ast created from the parser
 *
 * @param node
 */
void parseAst(Node *node, const LLVMModuleRef module)
{
    LLVMTypeRef *parameters = NULL;
    unsigned parameterLength = 0;
    LLVMTypeRef returnType;
    LLVMValueRef valueRef;
    LLVMBasicBlockRef blockReference;
    LLVMBuilderRef builder;

    char *blockName = NULL;

    // Loop start
    StackNode *st = NULL;
    push(&st, node);

    while (st)
    {
        Node *curr = top(st);
        pop(&st);
        //traverse current node first
        // printf("%s\n", curr->data.value);

        switch (curr->data.attr)
        {
        case _funcDef:
            // printf("%");
            // printf("%s\n", curr->data.value);

            // if next in tree
            // if there is a function made add to module
            if (builder != NULL)
            {
                // commitFunction(builder, blockReference);
            }
            break;

        case _funcName:
        {
            // printf("%s\n", curr->data.value);

            char *funcName = curr->data.value;

            if (curr->type == t_void)
            {
                returnType = LLVMFunctionType(LLVMVoidType(), NULL, 0, LLVMRealPredicateFalse);
                valueRef = LLVMAddFunction(module, funcName, returnType);

                blockName = getBlockName(funcName);

                builder = LLVMCreateBuilder();

                blockReference = LLVMAppendBasicBlock(valueRef, blockName);
                LLVMPositionBuilderAtEnd(builder, blockReference);

                //
                // define printf

                // LLVMTypeRef args[] = {LLVMInt8TypeInContext(LLVMGetModuleContext(module))};
                LLVMTypeRef args[] = {LLVMPointerType(LLVMInt8Type(), 0)};

                LLVMTypeRef printfType = LLVMFunctionType(LLVMInt32Type(), args, 1, LLVMRealPredicateTrue);

                LLVMAddFunction(module, "printf", printfType);

                /*Set up printf arguments*/
                LLVMValueRef fmtStr = LLVMBuildGlobalStringPtr(builder, "%d %d\n", "fmt");
                LLVMValueRef printArgs[] = {fmtStr, LLVMConstInt(LLVMInt32Type(), 20, LLVMRealPredicateFalse), LLVMConstInt(LLVMInt32Type(), 30, LLVMRealPredicateFalse)};

                /*We will be printing "20"*/
                LLVMBuildCall2(builder, printfType, LLVMGetNamedFunction(module, "printf"), printArgs, 3, "fmt");

                //
                //
                //
                //

                LLVMBuildRetVoid(builder);
            }

            // {LLVMVoidType()};
            // create llvm ir function
            // createFunction(module, curr->data.value, NULL, 0);

            break;
        }

        case _funcCall:
        {
            // char *funcCallName = curr->data.value;

            // LLVMValueRef tmp = LLVMBuildAdd(builder, LLVMGetParam(valueRef, 0), LLVMGetParam(valueRef, 1), "tmp");

            // LLVMBuildCall2(builder, LLVMVoidType(), valueRef, NULL, 0, "tmp");

            break;
        }
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

    if (builder != NULL)
    {
        // commitFunction(builder, blockReference);
    }

    free(st);
    free(blockName);
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

    /// Dump the IR we emited :)
    // if debug
    // LLVMDumpModule(module);

    /// Be clean!
    LLVMDisposeModule(module);
}

// llc -filetype=obj testfile.bc
// clang testfile.o

// llc -filetype=obj testfile.bc
// clang `llvm-config --cxxflags --ldflags --libs` testfile.o
