#
# 'make'        build executable file 'main'
# 'make clean'  removes all .o and executable files
#

# define the C compiler to use
CC = 		clang
# CC = 		gcc

CPP = 		clang++

# define any compile-time flags
CFLAGS	:= -Wall -Wextra -g

CFLAGS 	+= `llvm-config --cflags`

# LDFLAGS := `llvm-config --cxxflags --ldflags --libs core executionengine jit interpreter analysis native bitwriter --system-libs`
LDFLAGS := `llvm-config --cxxflags --ldflags --libs core analysis native bitwriter --system-libs`


# define library paths in addition to /usr/lib
#   if I wanted to include libraries not in /usr/lib I'd specify
#   their path using -Lpath, something like:
LFLAGS 		:= -L/usr/local/lib

# define BIN directory
BIN			:= bin

# define source directory
SRC			:= src

# define include directory
INCLUDE		:= include

# define lib directory
LIB			:= lib

# define test directory
TESTS 		:= test

# define test command line arguements
TESTARGS := -run -asm ./examples/testfile.lambda

ifeq ($(OS),Windows_NT)
MAIN	:= main.exe
SOURCEDIRS	:= $(SRC)
INCLUDEDIRS	:= $(INCLUDE)
LIBDIRS		:= $(LIB)
FIXPATH = $(subst /,\,$1)
RM			:= del /q /f
MD	:= mkdir
else
MAIN	:= main
SOURCEDIRS	:= $(shell find $(SRC) -type d)
INCLUDEDIRS	:= $(shell find $(INCLUDE) -type d)
LIBDIRS		:= $(shell find $(LIB) -type d)
FIXPATH = $1
RM = rm -f
MD	:= mkdir -p
endif

# define any directories containing header files other than /usr/include
INCLUDES	:= $(patsubst %,-I%, $(INCLUDEDIRS:%/=%))

# define the C libs
LIBS		:= $(patsubst %,-L%, $(LIBDIRS:%/=%))

# define the C source files
SOURCES		:= $(wildcard $(patsubst %,%/*.c, $(SOURCEDIRS)))

# define the C object files
OBJECTS		:= $(SOURCES:.c=.o)

#
# The following part of the makefile is generic; it can be used to
# build any executable just by changing the definitions above and by
# deleting dependencies appended to the file from 'make depend'
#

OUTPUTMAIN	:= $(call FIXPATH,$(BIN)/$(MAIN))

all: $(BIN) $(MAIN)

$(BIN):
	$(MD) $(BIN)

$(MAIN): $(OBJECTS)
	$(CPP) $(CFLAGS) $(INCLUDES) $(LDFLAGS) -o $(OUTPUTMAIN) $(OBJECTS) $(LFLAGS) $(LIBS)
# $(CC) $(CFLAGS) $(INCLUDES) $(LDFLAGS) -o $(OUTPUTMAIN) $(OBJECTS) $(LFLAGS) $(LIBS)
# $(CPP) --analyze $(CFLAGS) $(INCLUDES) $(LDFLAGS) -o $(OUTPUTMAIN) $(OBJECTS) $(LFLAGS) $(LIBS)

analyse:
	scan-build make

# this is a suffix replacement rule for building .o's from .c's
# it uses automatic variables $<: the name of the prerequisite of
# the rule(a .c file) and $@: the name of the target of the rule (a .o file)
# (see the gnu make manual section about automatic variables)
.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

.PHONY: clean
clean:
	$(RM) $(OUTPUTMAIN)
	$(RM) $(call FIXPATH,$(OBJECTS))

run: all
	./$(BIN)/$(MAIN) $(TESTARGS)

#  valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./bin/main ./examples/testfile.lambda

# cppcheck command for all source files
check: clean all
	cppcheck -f --enable=all --inconclusive --check-library --debug-warnings --suppress=missingIncludeSystem --check-config ./$(SRC)
# cppcheck --enable=all --suppress=missingIncludeSystem --check-config ./$(SRC)


# valgrind command to check memory
valgrind:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(BIN)/$(MAIN) $(TESTARGS)

checkvalgrind: check valgrind
