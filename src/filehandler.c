#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/filehandler.h"

char *filepath;
char *folderpath;
char *filename;

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

void parseArgs(char **argv)
{
    int i = 0;

    while (strstr(argv[i], EXT_SRC) == NULL)
        i++;

    if (i == 0)
    {
        printf("file not found");
        exit(0);
    }

    filepath = argv[i];

    // get folder path
    char *e1 = strrchr(filepath, SEP);
    int index = (int)(e1 - filepath) + 1;

    folderpath = malloc(index * sizeof(*folderpath));
    strncpy(folderpath, filepath, index);
    folderpath[index] = '\0';

    // get file name
    char *e2 = strrchr(filepath, DOT);
    int index2 = (int)(e2 - filepath);

    filename = malloc(index2 - 1 * sizeof(*filename));
    strncpy(filename, filepath + index, index2 - index);
    filename[index2] = '\0';

    printf("\n%s %s %s\n", filepath, folderpath, filename);
}

void writeLLVMIR(const LLVMModuleRef mod)
{
    char destPath[string_ln(filename) + string_ln(folderpath) + string_ln(EXT_BC)];
    strcpy(destPath, folderpath);
    strcat(destPath, filename);
    strcat(destPath, EXT_BC);

    if (remove(destPath) == 0)
    {
        // printf("File deleted successfully");/
    }
    else
    {
        // printf("Error: unable to delete the file");
    }

    char *errorMessage = "error message";
    if (LLVMPrintModuleToFile(mod, destPath, &errorMessage) != 0)
    {
        fprintf(stderr, "error writing to file, skipping\n");
    }

    // llc -filetype=obj testfile.bc
    // clang testfile.o
}

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

FileContents readFile()
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

void freePathVars()
{
    free(filename);
    free(folderpath);
    // free(filepath);
}

void cleanUpFileContents(FileContents lines)
{
    while (lines.linecount--)
        free(lines.lines[lines.linecount]);
    free(lines.lines);
}
