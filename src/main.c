/**
 * @file main.c
 * @author Robert Azzopardi-Yashi (you@domain.com)
 * @brief main driver file for the jackal compiler
 * @version 0.1
 * @date 2021-01-13
 *
 * @copyright Copyright (c) 2021
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/filehandler.h"
#include "../include/lexer.h"
#include "../include/parser.h"
#include "../include/ast.h"
#include "../include/codegen.h"

/**
 * @brief
 *
 * Start of program
 *
 * @param argc
 * @param argv
 * @return int
 */
int main(int argc, char **argv)
{
	parseArgs(argv);

	// start complination
	FileContents filelines = readFile();

	Sequence seq = lex(filelines);

	printSequence(seq);

	cleanUpFileContents(filelines);

	Node *ast = parse(seq);

	// print2d(ast, 0);
	generateCode(ast);

	freeTree(ast);

	while (seq.count--)
		free(seq.tokens[seq.count].value);
	free(seq.tokens);

	freePathVars();

	return 0;
}
