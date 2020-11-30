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
    return (o == MOD || o == DIV || o == MUL || o == ADD || o == SUB || o == EXP);
}

void addMissingBrackets(Sequence *seq, const size_t len, unsigned skips)
{
    seq->tokens = (Token *)realloc(seq->tokens, (seq->count + 2) * sizeof(Token));

    // create temp array to hold the previous line from the sequence
    Token tmp[len - skips];

    // count to the end of the last line
    unsigned l = 0;
    while (l < seq->count - len + skips)
        l++;

    // put the last lines tokens in the temp araray
    unsigned count = 0;
    for (size_t i = l; i < seq->count; i++)
        tmp[count++] = seq->tokens[i];

    // set the next token to left bracket
    strcpy(seq->tokens[l].value, "(");
    seq->tokens[l].attr = _leftBracket;

    // re add tokens from temp array to the main sequence
    unsigned t = 0;
    while (l < seq->count + 1)
    {
        seq->tokens[l + 1] = tmp[t++];
        l++;
    }

    // set last token to right bracket
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

void parseLine(Sequence *seq, const char *line, const size_t *prev)
{
    const size_t len = strlen(line);
    // printf("%s\n", line);

    unsigned skips = 0;

    unsigned firstNumber = 0;
    unsigned secondNumber = 0;

    for (unsigned j = 0; j < len; j++)
    {
        const char c = line[j];

        if (isOperator(c))
        {
            seq->count++;
            seq->tokens = (Token *)realloc(seq->tokens, seq->count * sizeof(Token));

            if (c == SUB && isdigit(line[j + 1]))
            {
                addNumber(seq, &j, line);
            }
            else if (c == SUB && line[j + 1] == LARR)
            {
                seq->tokens[seq->count - 1].value[0] = line[j];
                seq->tokens[seq->count - 1].value[1] = line[j + 1];
                seq->tokens[seq->count - 1].attr = _returnType;
                // printf("%s\n", seq->tokens[seq->count - 1].value);
                // j++;
                j += 2;
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
            seq->count++;
            seq->tokens = (Token *)realloc(seq->tokens, seq->count * sizeof(Token));

            char *placeHolder;

            unsigned indexOfSpace = (strchr(line + j, SPA) - (line + j));
            unsigned indexOfLeftBracket = (strchr(line + j, LPR) - (line + j));

            if (indexOfSpace < indexOfLeftBracket)
                placeHolder = (char *)malloc(indexOfSpace * sizeof(char));
            else if (indexOfLeftBracket < indexOfSpace)
                placeHolder = (char *)malloc(indexOfLeftBracket * sizeof(char));

            unsigned wordIndex = 0;

            // give the function name the type function and then check for main later

            for (unsigned index = j; index < len; index++)
            {
                placeHolder[wordIndex++] = line[index];

                if (isalpha(line[index + 1]) || line[index + 1] == LPR)
                    continue;

                if (strcmp(placeHolder, FUNC) == 0)
                {
                    strcpy(seq->tokens[seq->count - 1].value, placeHolder);

                    seq->tokens[seq->count - 1].attr = _funcDef;

                    j = index;
                    break;
                }
                // else if (strcmp(seq->tokens[seq->count - 1].value, FUNC) == 0 && isspace(line[index + 1]))
                else if (seq->tokens[seq->count - 1].attr == _funcDef && isspace(line[index + 1]))
                {
                    strcpy(seq->tokens[seq->count - 1].value, placeHolder);

                    seq->tokens[seq->count - 1].attr = _funcName;

                    j = index;
                    break;
                }
                else if (strcmp(placeHolder, VOID) == 0)
                {
                    strcpy(seq->tokens[seq->count - 1].value, placeHolder);

                    seq->tokens[seq->count - 1].attr = _type;

                    j = index;
                    break;
                }
                else if (line[index + 1] == SPA && line[index + 2] != SUB)
                {
                    strcpy(seq->tokens[seq->count - 1].value, placeHolder);

                    seq->tokens[seq->count - 1].attr = _func;

                    j = index;
                    break;
                }
                else if (line[index + 1] == SPA && line[index + 2] == SUB && line[index + 3] == LARR)
                {
                    strcpy(seq->tokens[seq->count - 1].value, placeHolder);

                    seq->tokens[seq->count - 1].attr = _funcName;

                    j = index;
                    break;
                }
                else if (line[index] == LPR)
                {
                    sprintf(seq->tokens[seq->count - 1].value, "%.*s", index - j, line + j);

                    seq->tokens[seq->count - 1].attr = _func;

                    j = index - 1;
                    break;
                }
                else
                {
                    printf("Undefined char %c\n", line[index]);
                }
            }
        }
        else if (c == CMM || c == LPR || c == RPR)
        {
            seq->count++;
            seq->tokens = (Token *)realloc(seq->tokens, seq->count * sizeof(Token));
            sprintf(seq->tokens[seq->count - 1].value, "%c", c);

            if (c == CMM)
                seq->tokens[seq->count - 1].attr = _comma;
            else
                seq->tokens[seq->count - 1].attr = c == LPR ? _leftBracket : _rightBracket;
        }
        else if (c == DOT || isdigit(c))
        {
            seq->count++;
            seq->tokens = (Token *)realloc(seq->tokens, seq->count * sizeof(Token));

            addNumber(seq, &j, line);
        }
        else if (isspace(c))
        {
            skips++;
        }
        else
        {
            printf("undefined char found in line %c\n", c);
        }

        if (seq->count > 1 && firstNumber == 0 && seq->tokens[seq->count - 1].attr != _func &&
            (seq->tokens[seq->count - 1].attr == _int || seq->tokens[seq->count - 1].attr == _float ||
             seq->tokens[seq->count - 1].attr == _leftBracket || seq->tokens[seq->count - 2].attr != _leftBracket))
        {

            firstNumber = seq->count - 1;
            Token tmp1 = seq->tokens[seq->count - 1];
            seq->tokens[seq->count - 1].value[0] = '(';
            seq->tokens[seq->count - 1].value[1] = '\0';
            seq->tokens[seq->count - 1].precedence = 0;
            seq->tokens[seq->count - 1].attr = _leftBracket;
            seq->tokens[seq->count - 1].associate = 0;

            seq->count++;
            seq->tokens = (Token *)realloc(seq->tokens, seq->count * sizeof(Token));
            seq->tokens[seq->count - 1] = tmp1;
        }
        if (firstNumber != 0 && secondNumber == 0)
        // if (firstNumber != 0)
        {
            secondNumber = seq->count - 1;
        }

        // if (len - 1 == j)
        // {
        //     printf("");
        // }

        printf("");
    }

    // if (seq->tokens[*prev].value[0] != LPR && seq->tokens[seq->count - 1].value[0] != RPR &&
    //     (seq->tokens[seq->count - 1].attr == _int || seq->tokens[seq->count - 1].attr == _float))
    //     addMissingBrackets(seq, len, skips);

    if (firstNumber != 0 && secondNumber != 0)
    {
        seq->count++;
        seq->tokens = (Token *)realloc(seq->tokens, seq->count * sizeof(Token));

        seq->tokens[seq->count - 1].value[0] = ')';
        seq->tokens[seq->count - 1].value[1] = '\0';
        seq->tokens[seq->count - 1].precedence = 0;
        seq->tokens[seq->count - 1].attr = _rightBracket;
        seq->tokens[seq->count - 1].associate = 0;
    }
}

Sequence lex(FileContents filecontents)
{
    Sequence seq = {.count = 0, .tokens = (Token *)malloc(sizeof(Token))};

    // printf("\n");

    size_t prev = 0;

    for (size_t i = 0; i < filecontents.linecount; i++)
    {
        const char *line = filecontents.lines[i];

        parseLine(&seq, line, &prev);

        prev = seq.count;
    }

    return seq;
}
