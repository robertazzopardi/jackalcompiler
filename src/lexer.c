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

void addNumber(Sequence *seq, unsigned *j, const char *line)
{
    unsigned curr = *j;

    while (isdigit(line[curr + 1]) || line[curr + 1] == DOT)
        curr++;

    sprintf(seq->tokens[seq->count - 1].value, "%.*s", curr - *j + 1, line + *j);

    seq->tokens[seq->count - 1].attr = strchr(seq->tokens[seq->count - 1].value, DOT) ? _float : _int;

    *j = curr;
}

void addOperator(Sequence *seq, const char c, const char *line, unsigned *j)
{
    if (c == SUB && isdigit(line[*j + 1]))
    {
        addNumber(seq, j, line);
    }
    else if (c == SUB && line[*j + 1] == RARR)
    {
        seq->tokens[seq->count - 1].value[0] = line[*j];
        seq->tokens[seq->count - 1].value[1] = line[*j + 1];
        seq->tokens[seq->count - 1].attr = _funcReturnType;

        *j += 2;
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
        else if (c == LARR || c == RARR)
        {
            seq->tokens[seq->count - 1].precedence = 6;
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

static inline void addPlaceHolder(Sequence seq, char *placeHolder, unsigned *j, unsigned index, Attribute attr)
{
    strcpy(seq.tokens[seq.count - 1].value, placeHolder);

    seq.tokens[seq.count - 1].attr = attr;

    *j = index;
}

void addString(Sequence *seq, const char *line, unsigned *j, const size_t len)
{
    char *placeHolder = NULL;

    unsigned indexOfSpace = (strchr(line + *j, SPA) - (line + *j));
    unsigned indexOfLeftBracket = (strchr(line + *j, LPR) - (line + *j));

    if (indexOfSpace < indexOfLeftBracket)
        placeHolder = malloc(indexOfSpace * sizeof(*placeHolder));
    else if (indexOfLeftBracket < indexOfSpace)
        placeHolder = malloc(indexOfLeftBracket * sizeof(*placeHolder));
    else
        placeHolder = malloc((len - *j) * sizeof(*placeHolder));

    unsigned wordIndex = 0;

    for (unsigned index = *j; index < len; index++)
    {
        placeHolder[wordIndex++] = line[index];

        if (isalpha(line[index + 1]) || line[index + 1] == LPR)
            continue;

        if (strcmp(placeHolder, FUNC) == 0)
        {
            addPlaceHolder(*seq, placeHolder, j, index, _funcDef);

            break;
        }
        else if (strcmp(placeHolder, IF) == 0)
        {
            addPlaceHolder(*seq, placeHolder, j, index, _if);

            break;
        }
        else if (seq->tokens[seq->count - 1].attr == _funcDef && isspace(line[index + 1]))
        {
            addPlaceHolder(*seq, placeHolder, j, index, _funcName);

            break;
        }
        else if (strcmp(placeHolder, VOID) == 0)
        {
            addPlaceHolder(*seq, placeHolder, j, index, _type);

            break;
        }
        else if (line[index + 1] == SPA && line[index + 2] != SUB)
        {
            addPlaceHolder(*seq, placeHolder, j, index, _funcCall);

            break;
        }
        else if (line[index + 1] == SPA && line[index + 2] == SUB && line[index + 3] == RARR)
        {
            addPlaceHolder(*seq, placeHolder, j, index, _funcName);

            break;
        }
        else if (line[index] == LPR)
        {
            // sprintf(seq->tokens[seq->count - 1].value, "%.*s", index - *j, line + *j);
            placeHolder[strlen(placeHolder) - 1] = ESC;

            addPlaceHolder(*seq, placeHolder, j, index - 1, _funcCall);

            break;
        }
        else
        {
            printf("Undefined char %c\n", line[index]);
        }
    }
}

void addBracket(Sequence *seq, const char c)
{
    sprintf(seq->tokens[seq->count - 1].value, "%c", c);

    if (c == CMM)
        seq->tokens[seq->count - 1].attr = _comma;
    else
        seq->tokens[seq->count - 1].attr = c == LPR ? _leftBracket : _rightBracket;
}

static inline void allocate(Sequence *seq)
{
    seq->count++;
    seq->tokens = realloc(seq->tokens, seq->count * sizeof(*seq->tokens));
}

void reSetValue(Sequence *seq, const char paren, const Attribute attr)
{
    seq->tokens[seq->count - 1].value[0] = paren;
    seq->tokens[seq->count - 1].value[1] = ESC;
    seq->tokens[seq->count - 1].precedence = 0;
    seq->tokens[seq->count - 1].attr = attr;
    seq->tokens[seq->count - 1].associate = 0;
}

void parseLine(Sequence *seq, const char *line)
{
    const size_t len = strlen(line);

    unsigned firstNumber = 0;
    unsigned secondNumber = 0;

    for (unsigned j = 0; j < len; j++)
    {
        const char c = line[j];

        if (isspace(c))
            continue;

        allocate(seq);

        if (isOperator(c))
            addOperator(seq, c, line, &j);
        else if (isalpha(c))
            addString(seq, line, &j, len);
        else if (c == CMM || c == LPR || c == RPR)
            addBracket(seq, c);
        else if (c == DOT || isdigit(c))
            addNumber(seq, &j, line);
        else
            printf("undefined char found in line %c\n", c);

        if (strncmp(line, FUNC, strlen(FUNC)) != 0 &&
            seq->tokens[seq->count - 1].attr != _type &&
            seq->tokens[seq->count - 1].attr != _rightBracket)
        {
            if (seq->count > 1 && firstNumber == 0 &&
                seq->tokens[seq->count - 1].attr != _funcCall &&
                (seq->tokens[seq->count - 1].attr == _int ||
                 seq->tokens[seq->count - 1].attr == _float ||
                 seq->tokens[seq->count - 1].attr == _leftBracket))
            {
                firstNumber = seq->count - 1;

                Token valueBeforeNumber = seq->tokens[seq->count - 1];

                reSetValue(seq, LPR, _leftBracket);

                allocate(seq);

                seq->tokens[seq->count - 1] = valueBeforeNumber;
            }
        }

        if (firstNumber != 0 && secondNumber == 0)
            secondNumber = seq->count - 1;
    }

    if (firstNumber != 0 && secondNumber != 0)
    {
        allocate(seq);

        reSetValue(seq, RPR, _rightBracket);
    }
}

Sequence lex(FileContents filecontents)
{
    Sequence seq = {.count = 0, .tokens = malloc(sizeof(*seq.tokens))};

    for (size_t i = 0; i < filecontents.linecount; i++)
    {
        const char *line = filecontents.lines[i];

        parseLine(&seq, line);
    }

    return seq;
}
