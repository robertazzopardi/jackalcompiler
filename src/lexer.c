#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"

void addMissingBrackets(Sequence *seq, const size_t len, unsigned skips)
{
    seq->tokens = realloc(seq->tokens, (seq->count + 2) * sizeof(*seq->tokens));

    Token tmp[len - skips];

    unsigned l = 0;
    while (l < seq->count - len + skips)
        l++;

    unsigned count = 0;
    for (size_t i = l; i < seq->count; i++)
        tmp[count++] = seq->tokens[i];

    strcpy(seq->tokens[l].value, "(");
    seq->tokens[l].attr = _leftBracket;

    unsigned t = 0;
    while (l < seq->count + 1)
    {
        seq->tokens[l + 1] = tmp[t++];
        l++;
    }

    strcpy(seq->tokens[l].value, ")");
    seq->tokens[l].attr = _rightBracket;

    seq->count += 2;
}

void addNumber(Sequence *seq, unsigned *j, const char *line)
{
    unsigned curr = *j;

    while (isdigit(line[curr + 1]) || line[curr + 1] == DOT)
        curr++;

    sprintf(seq->tokens[seq->count - 1].value, "%.*s", curr - *j + 1, line + *j);

    seq->tokens[seq->count - 1].attr = strchr(seq->tokens[seq->count - 1].value, DOT) ? _float : _int;

    *j = curr;
}

int isOperator(const char o)
{
    return (o == MOD || o == DIV || o == MUL || o == ADD || o == SUB || o == EXP);
}

void parseLine(Sequence *seq, const char *line, size_t *prev)
{
    const size_t len = strlen(line);
    printf("%s\n", line);

    unsigned skips = 0;

    for (unsigned j = 0; j < len; j++)
    {
        const char c = line[j];

        if (isOperator(c))
        {
            seq->count++;
            seq->tokens = realloc(seq->tokens, seq->count * sizeof(*seq->tokens));

            if (c == SUB && isdigit(line[j + 1]))
            {
                addNumber(seq, &j, line);
            }
            else
            {
                sprintf(seq->tokens[seq->count - 1].value, "%c", c);

                // set precedence
                if (c == SUB || c == ADD)
                {
                    seq->tokens[seq->count - 1].precedence = 2;
                    seq->tokens[seq->count - 1].associate = left_to_right;
                }
                else if (c == MUL || c == DIV || c == MOD)
                {
                    seq->tokens[seq->count - 1].precedence = 3;
                    seq->tokens[seq->count - 1].associate = left_to_right;
                }
                else
                {
                    seq->tokens[seq->count - 1].precedence = 4;
                    seq->tokens[seq->count - 1].associate = right_to_left;
                }

                seq->tokens[seq->count - 1].attr = _operator;
            }
        }
        else if (isalpha(c))
        {
            for (unsigned index = j; index < len; index++)
            {
                if (line[index] == LPR)
                {
                    seq->count++;
                    seq->tokens = realloc(seq->tokens, seq->count * sizeof(*seq->tokens));
                    sprintf(seq->tokens[seq->count - 1].value, "%.*s", index - j, line + j);

                    seq->tokens[seq->count - 1].attr = _func;

                    j = index - 1;
                    break;
                }
            }
        }
        else if (c == CMM || c == LPR || c == RPR)
        {
            seq->count++;
            seq->tokens = realloc(seq->tokens, seq->count * sizeof(*seq->tokens));
            sprintf(seq->tokens[seq->count - 1].value, "%c", c);

            if (c == CMM)
                seq->tokens[seq->count - 1].attr = _comma;
            else
                seq->tokens[seq->count - 1].attr = c == LPR ? _leftBracket : _rightBracket;
        }
        else if (c == DOT || isdigit(c))
        {
            seq->count++;
            seq->tokens = realloc(seq->tokens, seq->count * sizeof(*seq->tokens));

            addNumber(seq, &j, line);
        }
        else if (c == SPA)
        {
            skips++;
        }
        else
        {
            printf("undefined char found in line %c\n", c);
        }
    }

    if (seq->tokens[*prev].value[0] != LPR && seq->tokens[seq->count - 1].value[0] != RPR)
        addMissingBrackets(seq, len, skips);
}

Sequence lex(FileContents filecontents)
{
    Sequence seq;
    seq.tokens = malloc(sizeof(*seq.tokens));
    seq.count = 0;

    printf("\n");

    size_t prev = 0;

    for (size_t i = 0; i < filecontents.linecount; i++)
    {
        const char *line = filecontents.lines[i];

        parseLine(&seq, line, &prev);

        prev = seq.count;
    }

    return seq;
}
