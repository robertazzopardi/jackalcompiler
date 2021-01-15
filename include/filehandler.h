/**
 * @file filehandler.h
 * @author Robert Azzopardi-Yashi (you@domain.com)
 * @brief Interface to handle file system interactions
 * @version 0.1
 * @date 2021-01-13
 *
 * @copyright Copyright (c) 2021
 *
 */

#ifndef _FILE_HANDLER_
#define _FILE_HANDLER_

/**
 * @brief determine the path seperators
 *
 */
#ifdef _WIN32
#define SEP '\\'
#else
#define SEP '/'
#endif

#include <llvm-c/Core.h>

#define LINE_BUFFER 80

/**
 * @brief file extensions and file args
 *
 */
#define EXT_SRC ".lambda"
#define EXT_BC ".bc"
#define DOT '.'

/**
 * @brief struct to hold the files lines and the count
 *
 */
typedef struct
{
    char **lines;
    size_t linecount;
} FileContents;

/**
 * @brief global file path
 *
 */
extern char *filepath;

/**
 * @brief global folder path
 *
 */
extern char *folderpath;

/**
 * @brief global file name
 *
 */
extern char *filename;

/**
 * @brief read the files contents
 *
 * @return FileContents
 */
FileContents readFile();

/**
 * @brief parse the command line args
 *
 * @param argc
 * @param argv
 */
void parseArgs(int argc, char **argv);

/**
 * @brief write the bit code from llvm to a file
 *
 * @param mod
 */
void writeLLVMIR(const LLVMModuleRef mod);

/**
 * @brief clean up the lines read from the file
 *
 * @param lines
 */
void cleanUpFileContents(FileContents lines);

/**
 * @brief free the global string for the path variables
 *
 */
void freePathVars();

#endif
