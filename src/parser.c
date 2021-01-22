#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "parser.h"
#include "lexer.h"
#include "ast.h"
#include "stack.h"

size_t findIndex(const Token *a, const size_t size, const Attribute attribute, const size_t i)
{
    size_t index = i;

    while (index < size && a[index].attr != attribute)
        ++index;

    return (index == size ? -1 : index);
}

Node *evaluateExpression(Node *op, Node *e1, Node *e2)
{
    char o = op->data.value[0];

    char *fmt = GET_FORMAT(o);

    switch (o)
    {
    case ADD:
        sprintf(op->data.value, fmt, strtold(e1->data.value, NULL) + strtold(e2->data.value, NULL));
        break;
    case SUB:
        sprintf(op->data.value, fmt, strtold(e1->data.value, NULL) - strtold(e2->data.value, NULL));
        break;
    case MUL:
        sprintf(op->data.value, fmt, strtold(e1->data.value, NULL) * strtold(e2->data.value, NULL));
        break;
    case DIV:
        sprintf(op->data.value, fmt, strtold(e1->data.value, NULL) / strtold(e2->data.value, NULL));
        break;
    case MOD:
        sprintf(op->data.value, fmt, strtoll(e1->data.value, (char **)NULL, 10) % strtoll(e2->data.value, (char **)NULL, 10));
        break;
    case EXP:
        sprintf(op->data.value, fmt, pow(strtold(e1->data.value, NULL), strtold(e2->data.value, NULL)));
        break;
    case LARR:
        sprintf(op->data.value, fmt, strtold(e1->data.value, NULL) < strtold(e2->data.value, NULL));
        break;
    case RARR:
        sprintf(op->data.value, fmt, strtold(e1->data.value, NULL) > strtold(e2->data.value, NULL));
        break;
    default:
        printf("Error cannot evaluate expression %c\n", o);
        break;
    }

    op->data.attr = strchr(op->data.value, DOT) ? _float : _int;

    op->rightNode = NULL;
    op->leftNode = NULL;

    free(e1);
    free(e2);

    return op;
}

void addRemainingOperators(StackNode **operator_stack, StackNode **output_stack, size_t *output_size)
{
    while (*operator_stack)
    {
        if (*output_size > 1)
        {
            if ((*operator_stack)->data->data.attr == _operator &&
                (((*output_stack)->data->data.attr == _int || (*output_stack)->data->data.attr == _float) &&
                 ((*output_stack)->next->data->data.attr == _int || (*output_stack)->next->data->data.attr == _float)))
            {
                Node *op = pop(operator_stack);

                Node *e2 = pop(output_stack);
                Node *e1 = pop(output_stack);

                push(output_stack, evaluateExpression(op, e1, e2));

                freeTree(e1);
                freeTree(e2);

                *output_size -= 1;
            }
            else
            {

                Node *op = pop(operator_stack);

                Node *e2 = pop(output_stack);
                Node *e1 = pop(output_stack);

                if (e1 && e2)
                    push(output_stack, newNodeBoth(op->data, e1, e2));
                else if (!e1 && e2)
                    push(output_stack, newNodeLeft(op->data, e2));

                freeTree(op);

                output_size--;
            }
        }
        else
        {
            Node *op = pop(operator_stack);
            Node *n = pop(output_stack);

            push(output_stack, newNodeLeft(op->data, n));

            freeTree(op);
        }
    }
}

Node *shuntingYardPostFix(const Sequence seq, const size_t start, const size_t end)
{
    StackNode *operator_stack = NULL;
    StackNode *output_stack = NULL;

    size_t output_size = 0;

    for (size_t i = start; i < end; i++)
    {
        Token token = seq.tokens[i];

        switch (token.attr)
        {
        case _float:
        case _int:
            push(&output_stack, newNodeLeaf(token));
            output_size++;
            break;
        case _operator:
        case _leftBracket:
        case _rightBracket:

            if (token.attr != _leftBracket)
            {
                while (operator_stack && ((token.attr == _rightBracket && top(operator_stack)->data.attr != _leftBracket) ||
                                          (top(operator_stack)->data.precedence > token.precedence) ||
                                          ((top(operator_stack)->data.precedence == token.precedence) &&
                                           (token.associate == left_to_right))))
                {
                    Node *op = pop(&operator_stack);

                    Node *e2 = pop(&output_stack);
                    Node *e1 = pop(&output_stack);

                    push(&output_stack, evaluateExpression(op, e1, e2));

                    output_size--;
                }

                // If we popped until '(' because token is ')', toss both parens
                if (token.attr == _rightBracket)
                    freeTree(pop(&operator_stack));
            }

            // Everything except ')' --> stack
            if (token.attr != _rightBracket)
                push(&operator_stack, newNodeLeaf(token));

            break;

        default:
            break;
        }
    }

    addRemainingOperators(&operator_stack, &output_stack, &output_size);

    free(operator_stack);

    return pop(&output_stack);
}

void addConditionNode(Node **curr, Node **ifRoot, Token token)
{
    *ifRoot = *curr;
    if (*curr)
    {
        (*curr)->data = token;
        (*curr)->leftNode = calloc(1, sizeof(*(*curr)->leftNode));
        *curr = (*curr)->leftNode;

        Token condition;
        condition.value = CONDITION;
        condition.attr = _condition;

        (*curr)->data = condition;
    }
}

Node *parse(Sequence seq)
{
    printf("\n");

    Node *root = calloc(1, sizeof(*root));
    Node *curr = root;
    Node *ifRoot = NULL;

    for (size_t i = 0; i < seq.count; i++)
    {
        Token token = seq.tokens[i];

        switch (token.attr)
        {
        case _funcDef:
            curr = findRightmostElement(&root);
        case _funcCall:
            if (curr && curr->leftNode)
            {
                curr->rightNode = calloc(1, sizeof(*curr->rightNode));
                curr->rightNode->parent = curr;
                curr = curr->rightNode;
            }
            if (curr)
                curr->data = token;
            break;
        case _funcName:
            if (curr)
            {
                curr->leftNode = calloc(1, sizeof(*curr->leftNode));
                curr->leftNode->parent = curr;
                curr = curr->leftNode;
                curr->data = token;
            }
            break;
        case _type:
            if (strcmp(token.value, VOID) == 0 && curr)
                curr->type = t_void;

            if (curr)
            {
                curr->leftNode = calloc(1, sizeof(*curr->leftNode));
                curr->leftNode->parent = curr;
                curr = curr->leftNode;
            }
            break;
        case _if:
            addConditionNode(&curr, &ifRoot, token);
            break;
        case _elif:
        case _else:
            curr = ifRoot;
            if (curr)
            {
                curr->rightNode = calloc(1, sizeof(*curr->rightNode));
                curr->rightNode->parent = curr;
                // curr->rightNode->parent = ifRoot;

                curr = curr->rightNode;
                addConditionNode(&curr, &ifRoot, token);
            }
            break;
        case _leftBracket:
            if (curr)
            {
                size_t n = findIndex(seq.tokens, seq.count, _rightBracket, i);
                curr->leftNode = shuntingYardPostFix(seq, i, n + 1);
                i = n;
            }
            break;
        case _rightBracket:
        case _funcReturnType:
            break;
        case _int:
        case _float:
            if (curr)
                curr->leftNode = shuntingYardPostFix(seq, i, seq.count);
            break;
        default:
            break;
        }
    }

    print2d(root, 0);

    return root;
}
