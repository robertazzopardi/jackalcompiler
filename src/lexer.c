#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
// #include <math.h>

#include "lexer.h"

int isNumeric(const char *s)
{
    if (s == NULL || *s == '\0' || isspace(*s))
        return 0;
    char *p;
    strtod(s, &p);
    return *p == '\0';
}

int isOperator(const char *s)
{
    if (s == NULL || *s == '\0' || isspace(*s) || strlen(s) > 1)
        return 0;

    return s[0] == ADD || s[0] == SUB || s[0] == MUL || s[0] == DIV || s[0] == MOD || s[0] == EXP;
}

char findLastOperaror(Token *tokens, size_t count)
{
    for (size_t i = count - 1; i > 0; i--)
    {
        if (isOperator(tokens[i].value))
        {
            // printf("%s\n", tokens[i].value);
            return tokens[i].value[0];
        }
    }
    return 0;
}

void allocate(Sequence *seq, int *sum)
{
    // if (isNumeric(seq->tokens[seq->count - 1].value) || isOperator(seq->tokens[seq->count - 1].value))
    // {
    //     if (*sum == 0)
    //     {
    //         *sum = atoi(seq->tokens[seq->count - 1].value);
    //     }
    //     else
    //     {
    //         // printf("%c\n", findLastOperaror(seq->tokens, seq->count));
    //         switch (findLastOperaror(seq->tokens, seq->count))
    //         {
    //         case ADD:
    //             *sum += atoi(seq->tokens[seq->count - 1].value);
    //             break;
    //         case SUB:
    //             *sum -= atoi(seq->tokens[seq->count - 1].value);
    //             break;
    //         case MUL:
    //             *sum *= atoi(seq->tokens[seq->count - 1].value);
    //             break;
    //         case DIV:
    //             *sum /= atoi(seq->tokens[seq->count - 1].value);
    //             break;
    //         case MOD:
    //             *sum %= atoi(seq->tokens[seq->count - 1].value);
    //             break;
    //         case EXP:
    //             *sum = pow(*sum, atoi(seq->tokens[seq->count - 1].value));
    //             break;

    //         case 0:
    //             break;

    //         default:
    //             break;
    //         }
    //     }
    // }
    // else
    // {
    //     // printf("%d\n", *sum);
    //     // printf("%s\n", seq->tokens[seq->count - 1].value);
    // }
    // printf("%d\n", *sum);

    seq->count++;
    seq->tokens = realloc(seq->tokens, (seq->count + 1) * sizeof(*seq->tokens));

    seq->tokens[seq->count - 1].value = NULL;
    seq->tokens[seq->count].value = NULL;

    seq->tokens[seq->count - 1].attr = 0;
    seq->tokens[seq->count].attr = 0;
}

void append(char **dest, char *c)
{
    if (*dest)
    {
        *dest = realloc(*dest, strlen(*dest) + 2);
        strncat(*dest, c, 1);
    }
    else
    {
        *dest = calloc(1, 2);

        char t[2];
        t[0] = c[0];
        t[1] = '\0';
        strcpy(*dest, t);
    }
}

Sequence lex(FileContents filecontents)
{
    Sequence seq;
    seq.tokens = calloc(1, sizeof(*seq.tokens));

    seq.count = 0;

    for (size_t i = 0; i < filecontents.linecount; i++)
    {
        char *line = filecontents.lines[i];

        seq.count++;

        int expressionSum = 0;

        for (size_t j = 0; j < strlen(line); j++)
        {
            switch (line[j])
            {
            case SUB:
            case ADD:
            case MUL:
            case DIV:
            case MOD:
            case EXP:
                allocate(&seq, &expressionSum);

                seq.tokens[seq.count - 1].attr = _operator;

                append(&seq.tokens[seq.count - 1].value, &line[j]);
                if (isdigit(line[j + 1]) > 0 && line[j] != SUB)
                    allocate(&seq, &expressionSum);

                break;
            case LPR:
                allocate(&seq, &expressionSum);

                seq.tokens[seq.count - 1].attr = _leftBracket;

                append(&seq.tokens[seq.count - 1].value, &line[j]);
                break;
            case RPR:
                allocate(&seq, &expressionSum);
                seq.tokens[seq.count - 1].attr = _rightBracket;

                append(&seq.tokens[seq.count - 1].value, &line[j]);
                break;
            case DOT:
                seq.tokens[seq.count - 1].attr = _float;
            case '0' ... '9':
                if (j > 0)
                    if (line[j - 1] == SPA || line[j - 1] == LPR)
                        allocate(&seq, &expressionSum);

                if (!seq.tokens[seq.count - 1].attr || seq.tokens[seq.count - 1].attr == _operator)
                    seq.tokens[seq.count - 1].attr = _int;
            case 'A' ... 'Z':
            case 'a' ... 'z':
                append(&seq.tokens[seq.count - 1].value, &line[j]);
                if (!seq.tokens[seq.count - 1].attr)
                    seq.tokens[seq.count - 1].attr = _func;
                break;
            default:
                break;
            }
        }
    }

    return seq;
}
