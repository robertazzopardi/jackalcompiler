#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "filehandler.h"
#include "lexer.h"

int main(int argc, char **argv)
{
	FileContents filelines = readFile(argv[argc - 1]);

	Sequence seq = lex(filelines);

	// clean up file contents after lexing, the lines from the file are
	// not needed anymore
	cleanUpFileContents(filelines);

	// parse
	// code gen

	for (size_t i = 0; i < seq.count; i++)
		printf("%s ", seq.tokens[i].value);
	printf("\n");

	while (seq.count--)
		free(seq.tokens[seq.count].value);

	free(seq.tokens);

	return 0;
}
