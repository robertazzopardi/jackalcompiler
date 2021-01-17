#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "lexer.h"

inline void printSequence(const Sequence seq)
{
    printf("\n");
    for (size_t i = 0; i < seq.count; i++)
        printf("%s ", seq.tokens[i].value);
    printf("\n");
}

inline int isOperator(const char o)
{
    return (o == MOD || o == DIV || o == MUL || o == ADD || o == SUB || o == EXP || o == LARR || o == RARR);
}

static inline void allocate(Sequence *seq)
{
    seq->count++;
    seq->tokens = realloc(seq->tokens, seq->count * sizeof(*seq->tokens));
}

void cleanUpSeq(Sequence seq)
{
    // while (seq.count--)
    //     free(seq.tokens[seq.count].value);
    free(seq.tokens);
}

char *trimwhitespace(char *str)
{
    char *end;

    // Trim leading space
    while (isspace((unsigned char)*str))
        str++;

    if (*str == 0) // All spaces?
        return str;

    // Trim trailing space
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end))
        end--;

    // Write new null terminator character
    end[1] = '\0';

    return str;
}

int isNumeric(const char *s)
{
    if (s == NULL || *s == '\0' || isspace(*s))
        return 0;
    char *p;
    strtod(s, &p);
    return *p == '\0';
}

Sequence lex(FileContents fileContents)
{
    Sequence seq = {.count = 0, .tokens = malloc(sizeof(*seq.tokens))};

    for (size_t i = 0; i < fileContents.linecount; i++)
    {
        char *line = fileContents.lines[i];

        int firstnum = 0;

        char *rawToken;

        // First rawToken
        rawToken = strtok(line, " ");

        // Loop other tokens
        while (rawToken != NULL)
        {
            // trim whitespace
            rawToken = trimwhitespace(rawToken);

            allocate(&seq);

            if (strcmp(rawToken, FUNC) == 0)
            {
                seq.tokens[seq.count - 1].value = rawToken;
                seq.tokens[seq.count - 1].attr = _funcDef;
            }
            else if (seq.tokens[seq.count - 2].attr == _funcDef)
            {
                seq.tokens[seq.count - 1].value = rawToken;
                seq.tokens[seq.count - 1].attr = _funcName;
            }
            else if (isNumeric(rawToken))
            {
                char *tmp = strtok(NULL, " ");

                if (firstnum == 0 && (seq.tokens[seq.count - 2].attr != _int || seq.tokens[seq.count - 2].attr != _float))
                {
                    seq.tokens[seq.count - 1].value = "(";
                    seq.tokens[seq.count - 1].attr = _leftBracket;

                    allocate(&seq);

                    firstnum = 1;
                }

                seq.tokens[seq.count - 1].value = rawToken;

                seq.tokens[seq.count - 1].attr = strchr(rawToken, DOT) ? _float : _int;

                if (tmp == NULL)
                {
                    allocate(&seq);

                    seq.tokens[seq.count - 1].value = ")";
                    seq.tokens[seq.count - 1].attr = _rightBracket;

                    firstnum = 0;
                }

                rawToken = tmp;
                continue;
            }
            else if (strcmp(rawToken, TYPEDEC) == 0)
            {
                seq.tokens[seq.count - 1].value = rawToken;
                seq.tokens[seq.count - 1].attr = _funcReturnType;
            }
            else if (seq.tokens[seq.count - 2].attr == _funcReturnType)
            {
                seq.tokens[seq.count - 1].value = rawToken;
                seq.tokens[seq.count - 1].attr = _type;
            }
            else if (isOperator(rawToken[strlen(rawToken) - 1]))
            {
                // printf("%s\n", rawToken);
                char c = rawToken[strlen(rawToken) - 1];

                seq.tokens[seq.count - 1].value = rawToken;

                if (c == SUB || c == ADD)
                {
                    seq.tokens[seq.count - 1].precedence = 2;
                    seq.tokens[seq.count - 1].associate = left_to_right;
                }
                else if (c == MUL || c == DIV || c == MOD)
                {
                    seq.tokens[seq.count - 1].precedence = 3;
                    seq.tokens[seq.count - 1].associate = left_to_right;
                }
                else if (c == LARR || c == RARR)
                {
                    seq.tokens[seq.count - 1].precedence = 1;
                    seq.tokens[seq.count - 1].associate = left_to_right;
                }
                else
                {
                    seq.tokens[seq.count - 1].precedence = 4;
                    seq.tokens[seq.count - 1].associate = right_to_left;
                }

                seq.tokens[seq.count - 1].attr = _operator;
            }

            else if (strcmp(rawToken, IF) == 0)
            {
                seq.tokens[seq.count - 1].value = rawToken;
                seq.tokens[seq.count - 1].attr = _if;
            }

            else if (strcmp(rawToken, ELIF) == 0)
            {
                seq.tokens[seq.count - 1].value = rawToken;
                seq.tokens[seq.count - 1].attr = _elif;
            }

            else if (strcmp(rawToken, ELSE) == 0)
            {
                seq.tokens[seq.count - 1].value = rawToken;
                seq.tokens[seq.count - 1].attr = _else;
            }

            // used for function calls for now
            else
            {
                seq.tokens[seq.count - 1].value = rawToken;
                seq.tokens[seq.count - 1].attr = _funcCall;
            }

            // Get next rawToken
            rawToken = strtok(NULL, " ");
        }
    }

    printSequence(seq);

    // cleanUpSeq(seq);
    return seq;
}
