#include "filehandler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief determine the path seperators
 *
 */
#ifdef _WIN32
#define SEP '\\'
#else
#define SEP '/'
#endif

#define LINE_BUFFER 80
#define ESC '\0'
#define EXT_SRC ".jackal"
#define EXT_BC ".bc"
#define DOT '.'

/* needs rework */
ProgramArgs parseArgs(int argc, char **argv) {
    ProgramArgs paths = {0};

    for (int i = 0; i < argc; i++) {
        // Break when .jackal extension is found
        if (strstr(argv[i], EXT_SRC) != NULL) {
            paths.filepath = argv[i];
            break;
        }
    }

    if (paths.filepath == NULL) {
        printf("file not found\n");
        exit(0);
    }

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

    return paths;
}

void parseFileLines(char line[], FileContents *allLines, FILE *inputFile) {
    while (fgets(line, LINE_BUFFER, inputFile) != 0) {
        line[strcspn(line, "\r\n")] = ESC;

        char *result = strstr(line, "//");
        size_t position = result - line;

        if (position < strlen(line))
            line[position] = ESC;

        if (strlen(line) > 0) {
            printf("%s\n", line);

            allLines->lines =
                realloc(allLines->lines,
                        (allLines->linecount + 1) * sizeof(*allLines->lines));
            allLines->lines[allLines->linecount++] = strdup(line);
        }
    }
}

FileContents readFile(const char *filepath) {
    printf("\n");

    // open the file
    FILE *inputFile = fopen(filepath, "r");
    if (!inputFile) {
        printf("Can't open file: %s\n", filepath);
        exit(EXIT_FAILURE);
    }

    char line[LINE_BUFFER];

    FileContents allLines = {.linecount = 0, .lines = NULL};

    parseFileLines(line, &allLines, inputFile);

    fclose(inputFile);

    return allLines;
}

void freePathVars(ProgramArgs paths) {
    free(paths.filename);
    free(paths.folderpath);
    // free(paths.filepath);
}

void cleanUpFileContents(FileContents lines) {
    while (lines.linecount--)
        free(lines.lines[lines.linecount]);
    free(lines.lines);
}
