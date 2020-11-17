#
# 'make'        build executable file test files
# 'make clean'  removes all .o and executable files
#

# define the C compiler to use
CC = 		gcc

# define any compile-time flags
CFLAGS	:= -Wall -Wextra

# define library paths in addition to /usr/lib
#   if I wanted to include libraries not in /usr/lib I'd specify
#   their path using -Lpath, something like:
LFLAGS 		:= -L/usr/local/lib

# define BIN directory
BIN			:= bin

# define source directory
SRC			:= src

# define name format
TESTNAME := test_

#define testing files
FILES = filehandler lexer

TESTFILES = $(TESTNAME)filehandler $(TESTNAME)lexer

ifeq ($(OS),Windows_NT)
MAIN	:= .exe
SOURCEDIRS	:= $(SRC)
FIXPATH = $(subst /,\,$1)
RM			:= del /q /f
MD	:= mkdir
else
MAIN	:=
SOURCEDIRS	:= $(shell find $(SRC) -type d)
FIXPATH = $1
RM = rm -f
MD	:= mkdir -p
endif

all: $(BIN) $(FILES)

$(BIN):
	$(MD) $(BIN)

$(FILES): %: $(SRC)/$(TESTNAME)%.c
	$(CC) $(CFLAGS) $(SRC)/$(TESTNAME)$@.c ../$(SRC)/$@.o $(LFLAGS) -lunity -o $(BIN)/$(TESTNAME)$@

.PHONY: clean
clean:
	rm $(BIN)/*

run: $(TESTFILES)

$(TESTFILES): %:
	./$(BIN)/$@
	@echo ""
