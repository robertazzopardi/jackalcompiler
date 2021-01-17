/**
 * @file codegen.c
 * @author robertazzopardi (robertazzopardi@icloud.com)
 * @brief Implementation to generate llvm code
 * @version 0.1
 * @date 2021-01-17
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "codegen.h"

int string_ln(char *p) /* p=&str[0] */
{
    int count = 0;
    while (*p != '\0')
    {
        count++;
        p++;
    }
    return count;
}

void writeLLVMIR(const LLVMModuleRef mod, ProgramArgs paths)
{
    char destPath[string_ln(paths.filename) + string_ln(paths.folderpath) + string_ln(EXT_BC)];
    strcpy(destPath, paths.folderpath);
    strcat(destPath, paths.filename);
    strcat(destPath, EXT_BC);

    if (remove(destPath) == 0)
    {
        // printf("File deleted successfully");/
    }
    else
    {
        // printf("Error: unable to delete the file");
    }

    char *errors = 0;
    if (LLVMPrintModuleToFile(mod, destPath, &errors) != 0)
    {
        fprintf(stderr, "error writing to file, skipping\n");
    }
    LLVMDisposeMessage(errors);

    // llc -filetype=obj testfile.bc
    // clang testfile.o

    // llc - filetype = obj testfile.bc
    //                      clang `llvm -
    //                  config-- cxxflags-- ldflags-- libs` testfile.o
}

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

static inline LLVMValueRef getArgValue(Token token)
{
    LLVMValueRef value = NULL;
    switch (token.attr)
    {
    case _int:
        value = LLVMConstInt(LLVMInt32Type(), strtol(token.value, (char **)NULL, 10), LLVMRealPredicateFalse);
        break;

    case _float:
        value = LLVMConstRealOfString(LLVMFloatType(), token.value);
        break;

    default:
        break;
    }
    return value;
}

static inline char *getFormatValue(Attribute attr)
{
    char *value = NULL;
    switch (attr)
    {
    case _int:
        value = "%d\n";
        break;

    case _float:
        value = "%.6g\n";
        break;

    default:
        break;
    }
    return value;
}

static inline char *getTypeName(Attribute attr)
{
    char *value = NULL;
    switch (attr)
    {
    case _int:
        value = "int_fmt";
        break;

    case _float:
        value = "float_fmt";
        break;

    default:
        break;
    }
    return value;
}

// static inline int isFunctionDefined(const LLVMModuleRef module, const char *funcName)
// {
//     return LLVMGetNamedFunction(module, funcName) != NULL;
// }

// static inline int isGlobalDefined(const LLVMModuleRef module, const char *name)
// {
//     return LLVMGetNamedGlobal(module, name) != NULL;
// }

void callPrintf(const LLVMModuleRef module, const LLVMBuilderRef builder, Token token)
{
    char *fmtName = getTypeName(token.attr);

    // Define printf function
    LLVMTypeRef args[] = {LLVMPointerType(LLVMInt8Type(), 0)};
    LLVMTypeRef printfType = LLVMFunctionType(LLVMInt32Type(), args, 1, LLVMRealPredicateTrue);

    // Check if declared
    // if (isFunctionDefined(module, "printf") == 0)
    LLVMAddFunction(module, "printf", printfType);

    // Set format
    LLVMValueRef fmtStr = LLVMBuildGlobalStringPtr(builder, getFormatValue(token.attr), fmtName);

    // char *fmt = getFormatValue(token.attr);

    // Set arguments to pass to orintf
    LLVMValueRef printArgs[] = {
        fmtStr,
        getArgValue(token),
    };

    // Add the call to the builder
    LLVMBuildCall2(builder, printfType, LLVMGetNamedFunction(module, "printf"), printArgs, 2, fmtName);
}

/**
 * @brief parse the ast created from the parser
 *
 * @param node
 */
void astToLLVMIR(Node *node, const LLVMModuleRef module)
{
    // LLVMTypeRef *parameters = NULL;
    // unsigned parameterLength = 0;
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

        switch (curr->data.attr)
        {
        case _funcDef:
            // if next in tree
            // if there is a function made add to module
            if (builder != NULL)
            {
                LLVMBuildRetVoid(builder);
            }
            break;

        case _funcName:
        {
            char *funcName = curr->data.value;

            if (curr->type == t_void)
            {
                returnType = LLVMFunctionType(LLVMVoidType(), NULL, 0, LLVMRealPredicateFalse);
                valueRef = LLVMAddFunction(module, funcName, returnType);

                blockName = getBlockName(funcName);

                builder = LLVMCreateBuilder();

                blockReference = LLVMAppendBasicBlock(valueRef, blockName);
                LLVMPositionBuilderAtEnd(builder, blockReference);
            }

            break;
        }

        case _funcCall:
        {
            if (strcmp(curr->data.value, PRINT) == 0)
            {
                callPrintf(module, builder, curr->leftNode->data);
            }

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
        LLVMBuildRetVoid(builder);
        // commitFunction(builder, blockReference);
    }

    free(st);
    free(blockName);
}

void runLLVMPasses(const LLVMModuleRef module)
{
    LLVMPassManagerRef passManager = LLVMCreatePassManager();

    LLVMAddConstantMergePass(passManager);
    LLVMAddGlobalOptimizerPass(passManager);
    LLVMAddInstructionCombiningPass(passManager);
    LLVMAddIPConstantPropagationPass(passManager);
    LLVMAddAggressiveInstCombinerPass(passManager);
    LLVMAddMergeFunctionsPass(passManager);

    if (LLVMRunPassManager(passManager, module) == 1)
    {
        printf("output modified by passes\n");
    }
}

/**
 * @brief Entry method to generate the llvm ir code
 *
 * @param root
 */
void generateCode(Node *root, ProgramArgs paths)
{
    // Check if there is actually code in the file
    if (!root->leftNode && !root->rightNode)
    {
        printf("No source code found could not compile\n");
        exit(0);
    }

    // create module
    LLVMModuleRef module = createModule(paths.filename);

    // create llvm ir function
    astToLLVMIR(root, module);
    printf("\n");

    // Analyse module
    verifyModule(module);

    // Optimise with passes
    runLLVMPasses(module);

    // write to file
    writeLLVMIR(module, paths);

    /// Be clean!
    LLVMDisposeModule(module);
}

// llc -filetype=obj testfile.bc
// clang testfile.o

// llc -filetype=obj testfile.bc
// clang `llvm-config --cxxflags --ldflags --libs` testfile.o
