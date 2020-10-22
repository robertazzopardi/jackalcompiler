#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "lexer.h"

void allocate(Sequence *seq)
{
    seq->count++;
    seq->tokens = realloc(seq->tokens, (seq->count + 1) * sizeof(*seq->tokens));

    seq->tokens[seq->count - 1].value = NULL;
    seq->tokens[seq->count].value = NULL;
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
                allocate(&seq);
                append(&seq.tokens[seq.count - 1].value, &line[j]);
                if (isdigit(line[j + 1]) > 0 && line[j] != SUB)
                    allocate(&seq);
                break;
            case LPR:
            case RPR:
                allocate(&seq);
                append(&seq.tokens[seq.count - 1].value, &line[j]);
                break;
            case DOT:
            case '0' ... '9':
                if (line[j - 1] == SPA || line[j - 1] == LPR)
                    allocate(&seq);
            case 'A' ... 'Z':
            case 'a' ... 'z':
                append(&seq.tokens[seq.count - 1].value, &line[j]);
                break;
            default:
                break;
            }
        }
    }

    return seq;
}
