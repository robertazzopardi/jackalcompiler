#ifndef _FILE_HANDLER_
#define _FILE_HANDLER_

#define LINE_BUFFER 256

typedef struct
{
    char **lines;
    size_t linecount;
} FileContents;

FileContents readFile(const char *file);
void writeAssemblyFile(const char *content);
void cleanUpFileContents(FileContents lines);

#endif