#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "filehandler.h"
#include "lexer.h"
#include "parser.h"
#include "ast.h"

int main(int argc, char **argv)
{
	FileContents filelines = readFile(argv[argc - 1]);

	Sequence seq = lex(filelines);

	printSequence(seq);

	cleanUpFileContents(filelines);

	Node *tree = parse(seq);

	freeTree(tree);

	free(seq.tokens);

	// Token t = {.value = "if", .attr};

	// Node *n = newNodeBoth();

	// print2d(n, 0);

	return 0;
}
