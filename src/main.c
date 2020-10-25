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

	// clean up file contents after lexing
	cleanUpFileContents(filelines);

	// parser
	parse(seq);

	while (seq.count--)
		free(seq.tokens[seq.count].value);
	free(seq.tokens);

	// code gen

	return 0;
}
