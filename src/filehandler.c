#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "filehandler.h"

// char *filepath;
// char *folderpath;
// char *filename;

ProgramArgs parseArgs(int argc, char **argv)
{
    int i = 0;

    // while (strstr(argv[i], EXT_SRC) == NULL)
    //     i++;
    for (i = 0; i < argc; i++)
    {
        if (strstr(argv[i], EXT_SRC) != NULL)
        {
            break;
        }
    }

    if (i == 0)
    {
        printf("file not found");
        exit(0);
    }

    ProgramArgs paths;

    paths.filepath = argv[i];

    // get folder path
    char *e1 = strrchr(paths.filepath, SEP);
    int index = (int)(e1 - paths.filepath) + 1;

    paths.folderpath = malloc(index * sizeof(*paths.folderpath));
    strncpy(paths.folderpath, paths.filepath, index);
    paths.folderpath[index] = ESC;

    // get file name
    char *e2 = strrchr(paths.filepath, DOT);
    int index2 = (int)(e2 - paths.filepath);

    paths.filename = malloc(index2 - 1 * sizeof(*paths.filename));
    strncpy(paths.filename, paths.filepath + index, index2 - index);
    paths.filename[index2] = ESC;

    // printf("\n%s %s %s\n", paths.filepath, paths.folderpath, paths.filename);

    return paths;
}

void parseFileLines(char line[], FileContents *allLines, FILE *inputFile)
{
    while (fgets(line, LINE_BUFFER, inputFile) != 0)
    {
        line[strcspn(line, "\r\n")] = ESC;

        char *result = strstr(line, "//");
        size_t position = result - line;

        if (position < strlen(line))
            line[position] = ESC;

        if (strlen(line) > 0)
        {
            printf("%s\n", line);

            allLines->lines = realloc(allLines->lines, (allLines->linecount + 1) * sizeof(*allLines->lines));
            allLines->lines[allLines->linecount++] = strdup(line);
        }
    }
}

FileContents readFile(const char *filepath)
{
    printf("\n");

    // open the file
    FILE *inputFile = fopen(filepath, "r");
    if (!inputFile)
    {
        printf("Can't open file: %s\n", filepath);
        exit(EXIT_FAILURE);
    }

    char line[LINE_BUFFER];

    FileContents allLines = {.linecount = 0, .lines = NULL};

    parseFileLines(line, &allLines, inputFile);

    fclose(inputFile);

    return allLines;
}

// void freePathVars(ProgramArgs paths)
// {
//     // free(paths.filename);
//     // free(paths.folderpath);
//     // free(filepath);
// }

void cleanUpFileContents(FileContents lines)
{
    while (lines.linecount--)
        free(lines.lines[lines.linecount]);
    free(lines.lines);
}
