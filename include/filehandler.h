/**
 * @file filehandler.h
 * @author robertazzopardi (robertazzopardi@icloud.com)
 * @brief Interface to handle file system interactions
 * @version 0.1
 * @date 2021-01-16
 *
 * @copyright Copyright (c) 2021
 *
 */

#ifndef _FILE_HANDLER_
#define _FILE_HANDLER_

typedef struct ProgramArgs ProgramArgs;

typedef struct FileContents FileContents;

/**
 * @brief Holds the program flags
 *
 */
struct ProgramArgs {
    char *filepath;
    char *filename;
    char *folderpath;
};

/**
 * @brief Struct to hold the files lines and the count
 *
 */
struct FileContents {
    char **lines;
    int linecount;
};

/**
 * @brief Read the files contents
 *
 * @param filepath
 * @return FileContents
 */
FileContents readFile(const char *filepath);

/**
 * @brief Parse the command line args
 *
 * @param argc
 * @param argv
 */
ProgramArgs parseArgs(int argc, char **argv);

/**
 * @brief Write the bit code from llvm to a file
 *
 * @param mod
 * @param paths
 */
// void writeLLVMIR(const LLVMModuleRef mod, ProgramArgs paths);

/**
 * @brief clean up the lines read from the file
 *
 * @param lines
 */
void cleanUpFileContents(FileContents lines);

/**
 * @brief Used to free up some program args
 *
 * @param paths
 */
void freePathVars(ProgramArgs paths);

#endif
