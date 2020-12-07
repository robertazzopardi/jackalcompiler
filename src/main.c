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

	Node *ast = parse(seq);

	print2d(ast, 0);

	freeTree(ast);

	while (seq.count--)
		free(seq.tokens[seq.count].value);
	free(seq.tokens);

	return 0;
}
