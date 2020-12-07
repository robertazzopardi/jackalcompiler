#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "filehandler.h"

void parseFileLines(char line[], FileContents *allLines, FILE *inputFile)
{
    while (fgets(line, LINE_BUFFER, inputFile) != 0)
    {
        line[strcspn(line, "\r\n")] = '\0';

        char *result = strstr(line, "//");
        size_t position = result - line;

        if (position < strlen(line))
            line[position] = '\0';

        if (strlen(line) > 0)
        {
            printf("%s\n", line);

            allLines->lines = realloc(allLines->lines, (allLines->linecount + 1) * sizeof(*allLines->lines));
            allLines->lines[allLines->linecount++] = strdup(line);
        }
    }
}

FileContents readFile(const char *file)
{
    printf("\n");

    // open the file
    FILE *inputFile = fopen(file, "r");
    if (!inputFile)
    {
        printf("Can't open file: %s\n", file);
        exit(EXIT_FAILURE);
    }

    char line[LINE_BUFFER];

    FileContents allLines = {.linecount = 0, .lines = NULL};

    parseFileLines(line, &allLines, inputFile);

    fclose(inputFile);

    return allLines;
}

// void writeFile(const char *content) {}

void cleanUpFileContents(FileContents lines)
{
    while (lines.linecount--)
        free(lines.lines[lines.linecount]);
    free(lines.lines);
}
