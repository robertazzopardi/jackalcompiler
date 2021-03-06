/**
 * @file main.c
 * @author robertazzopardi (robertazzopardi@icloud.com)
 * @brief Main driver file for the jackal compiler
 * @version 0.1
 * @date 2021-01-17
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "ast.h"
#include "codegen.h"
#include "filehandler.h"
#include "lexer.h"
#include "parser.h"
#include <stdlib.h>

int main(int argc, char **argv) {
    ProgramArgs args = parseArgs(argc, argv);

    // start of complination

    FileContents filelines = readFile(args.filepath);

    Sequence seq = lex(&filelines);

    Node *ast = parse(&seq);

    generateCode(ast, &args);

    // Clean up
    cleanUpFileContents(filelines);
    freeTree(ast);
    cleanUpSeq(seq);
    freePathVars(args);

    return 0;
}
