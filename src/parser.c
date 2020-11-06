#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "parser.h"
#include "lexer.h"
#include "ast.h"
#include "stack.h"

void parse(Sequence seq)
{
    for (size_t i = 0; i < seq.count; i++)
        printf("%s ", seq.tokens[i].value);
    printf("\n");

    Node *n = shuntingYardPostFix(seq);

    // printPostOrder(n);

    print2d(n, 0);
    printf("\n");

    freeTree(n);
}

Node *shuntingYardPostFix(const Sequence seq)
{
    StackNode *operator_stack = NULL;
    StackNode *output_stack = NULL;
    StackNode *main_stack = NULL;
    size_t output_size = 0;

    int bracket_count = 0;

    for (size_t i = 0; i < seq.count; i++)
    {
        Token token = seq.tokens[i];

        if (token.attr == _float || token.attr == _int)
        {
            push(&output_stack, newNodeLeaf(token));
            output_size++;
        }
        else if (token.attr == _func)
        {
            push(&operator_stack, newNodeLeaf(token));
        }
        else if (token.attr == _comma)
        {
            push(&operator_stack, newNodeLeaf(token));
        }
        else if (token.attr == _operator || token.attr == _leftBracket || token.attr == _rightBracket)
        {
            if (token.attr == _leftBracket)
                bracket_count++;
            if (token.attr == _rightBracket)
                bracket_count--;

            if (token.attr != _leftBracket)
            {
                while (!IS_EMPTY(operator_stack) && ((token.attr == _rightBracket && top(operator_stack)->data.attr != _leftBracket) ||
                                                     (getPrecedence(top(operator_stack)->data) > getPrecedence(token)) ||
                                                     ((getPrecedence(top(operator_stack)->data) == getPrecedence(token)) &&
                                                      (getAssociates(token) == left_to_right))))
                {
                    Node *op = pop(&operator_stack);

                    Node *e2 = pop(&output_stack);
                    Node *e1 = pop(&output_stack);

                    if (op->data.attr == _operator &&
                        ((e1->data.attr == _int || e1->data.attr == _float) &&
                         (e2->data.attr == _int || e2->data.attr == _float)))
                    {
                        evaluateExpression(&op, e1, e2);

                        push(&output_stack, op);
                    }

                    else if ((operator_stack && e1 && e2) && operator_stack->data->data.attr == _comma && op->data.attr == _func && (e1->data.attr == _comma || e2->data.attr == _comma))
                    {
                        while (operator_stack->data->data.attr != _func)
                            pop(&operator_stack);

                        Node *func = pop(&operator_stack);

                        if (e1->data.attr == _comma && e2->data.attr != _comma)
                        {
                            op = newNodeBoth(op->data, e1->leftNode, e1->rightNode);
                            push(&output_stack, newNodeBoth(func->data, op, e2));
                        }
                        else if (e1->data.attr != _comma && e2->data.attr == _comma)
                        {
                            op = newNodeBoth(op->data, e2->leftNode, e2->rightNode);
                            push(&output_stack, newNodeBoth(func->data, e1, op));
                        }
                    }

                    else if (!operator_stack && op->data.attr == _func && e1 && e1->data.attr == _func)
                    {
                        push(&output_stack, newNodeLeft(op->data, e1));
                    }

                    else if (!operator_stack && op->data.attr == _func && e2 && e2->data.attr == _func)
                    {
                        push(&output_stack, newNodeLeft(op->data, e2));
                    }

                    else if (operator_stack->data->data.attr == _operator && operator_stack->next->data->data.attr != _func)
                    {
                        Node *opop = pop(&operator_stack);
                        Node *val = pop(&output_stack);

                        evaluateExpression(&opop, val, e1);

                        push(&output_stack, newNodeBoth(op->data, opop, e2));
                    }

                    else if (operator_stack->data->data.attr == _func && e1->data.attr == _comma && e2->data.attr != _comma)
                    {
                        Node *func = pop(&operator_stack);
                        e1 = newNodeBoth(func->data, e1->leftNode, e1->rightNode);
                        push(&output_stack, newNodeBoth(op->data, e1, e2));
                    }
                    else if (operator_stack->data->data.attr == _func && e1->data.attr != _comma && e2->data.attr == _comma)
                    {
                        Node *func = pop(&operator_stack);
                        e2 = newNodeBoth(func->data, e2->leftNode, e2->rightNode);
                        push(&output_stack, newNodeBoth(op->data, e1, e2));
                    }

                    else if (operator_stack->data->data.attr == _operator && operator_stack->next->data->data.attr == _func)
                    {
                        Node *opop = pop(&operator_stack);
                        Node *func = pop(&operator_stack);

                        op->leftNode = e1->leftNode;
                        op->rightNode = e1->rightNode;

                        func->leftNode = e2->leftNode;
                        func->rightNode = e2->rightNode;

                        push(&output_stack, newNodeBoth(opop->data, op, func));
                    }

                    else if (op->data.attr == _func && e2->data.attr == _func)
                    {
                        push(&output_stack, newNodeLeft(op->data, e2));
                    }

                    else if (operator_stack->data->data.attr == _leftBracket && op->data.attr == _func &&
                             e1 && !e2 && e1->data.attr == _comma)
                    {
                        op = newNodeBoth(op->data, e1->leftNode, e1->rightNode);
                        push(&output_stack, op);
                    }
                    else if (operator_stack->data->data.attr == _leftBracket && op->data.attr == _func &&
                             !e1 && e2 && e2->data.attr == _comma)
                    {
                        op = newNodeBoth(op->data, e2->leftNode, e2->rightNode);
                        push(&output_stack, op);
                    }

                    else if (operator_stack->data->data.attr == _leftBracket && op->data.attr == _func &&
                             e1 && !e2)
                    {
                        push(&output_stack, newNodeLeft(op->data, e1));
                    }
                    else if (operator_stack->data->data.attr == _leftBracket && op->data.attr == _func &&
                             !e1 && e2)
                    {
                        push(&output_stack, newNodeLeft(op->data, e2));
                    }

                    else if (op->data.attr == _func && e1 && !e2)
                    {
                        op->leftNode = e1->leftNode;
                        op->rightNode = e1->rightNode;

                        push(&output_stack, op);
                    }
                    else if (op->data.attr == _func && !e1 && e2)
                    {
                        op->leftNode = e2->leftNode;
                        op->rightNode = e2->rightNode;

                        push(&output_stack, op);
                    }
                    else if (op->data.attr == _func && e1 && e2)
                    {
                        op->leftNode = e1;
                        op->rightNode = e2;

                        push(&output_stack, op);
                    }

                    else if (op->data.attr == _func && e1->data.attr == _comma && e2->data.attr == _comma)
                    {
                        Node *opop = pop(&operator_stack);
                        Node *val = pop(&operator_stack);

                        push(&output_stack, newNodeBoth(opop->data, newNodeBoth(op->data, e1->leftNode, e1->rightNode), newNodeBoth(val->data, e2->leftNode, e2->rightNode)));
                    }

                    else if ((output_stack && output_stack->next) && operator_stack->data->data.attr == _comma &&
                             ((output_stack->data->data.attr == _int || output_stack->next->data->data.attr == _float) ||
                              (output_stack->data->data.attr == _float || output_stack->next->data->data.attr == _float)))
                    {
                        push(&output_stack, newNodeBoth(op->data, e1, e2));
                    }

                    else if (operator_stack->data->data.attr == _leftBracket)
                    {
                        if (e1 && e2)
                        {
                            if (e1->data.attr != _comma && e2->data.attr == _comma)
                                push(&output_stack, newNodeBoth(op->data, e2, e1));
                            else
                                push(&output_stack, newNodeBoth(op->data, e1, e2));
                        }
                        else if (e1 && !e2)
                            push(&output_stack, newNodeLeft(op->data, e1));
                        else if (!e1 && e2)
                            push(&output_stack, newNodeLeft(op->data, e2));
                    }

                    else if (op->data.attr == _comma && e1->data.attr == _comma && e2->data.attr != _comma)
                    {
                        op->leftNode = e1;
                        op->rightNode = e2;

                        push(&output_stack, op);
                    }
                    else if (op->data.attr == _comma && e1->data.attr != _comma && e2->data.attr == _comma)
                    {
                        op->leftNode = e2;
                        op->rightNode = e1;

                        push(&output_stack, op);
                    }

                    else
                    {
                        printf("EXCEPTION");
                    }

                    output_size--;
                }

                // If we popped until '(' because token is ')', toss both parens
                if (token.attr == _rightBracket)
                    freeTree(pop(&operator_stack));
            }

            // Everything except ')' --> stack
            if (token.attr != _rightBracket)
                push(&operator_stack, newNodeLeaf(token));

            // clean up and push to the main stack
            if (bracket_count == 0)
            {
                addRemainingOperators(&operator_stack, &output_stack, &output_size);
                push(&main_stack, pop(&output_stack));
            }
        }
        else
        {
        }
    }

    // addRemainingOperators(&operator_stack, &output_stack, &output_size);
    // push(&main_stack, pop(&output_stack));

    // make final tree from main stack
    while (main_stack && main_stack->next)
    {
        Node *top = pop(&main_stack);
        Node *next = pop(&main_stack);

        Node *right = findLastElement(&top);

        right->rightNode = next;

        push(&main_stack, top);
    }

    free(operator_stack);
    free(output_stack);
    return pop(&main_stack);
}

void addRemainingOperators(StackNode **operator_stack, StackNode **output_stack, size_t *output_size)
{
    while (!IS_EMPTY(*operator_stack))
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

                evaluateExpression(&op, e1, e2);

                push(output_stack, op);

                free(e1);
                free(e2);

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

                free(op);

                output_size--;
            }
        }
        else
        {
            Node *op = pop(operator_stack);
            Node *n = pop(output_stack);

            push(output_stack, newNodeLeft(op->data, n));

            free(op);
        }
    }
}

// void evaluateExpression(StackNode **operator_stack, StackNode **output_stack, size_t *output_size)
void evaluateExpression(Node **op, Node *e1, Node *e2)
{
    (*op)->data.value = realloc((*op)->data.value, sizeof(char *));

    switch ((*op)->data.value[0])
    {
    case ADD:
        snprintf((*op)->data.value, 10, "%.6g", atof(e1->data.value) + atof(e2->data.value));
        break;
    case SUB:
        snprintf((*op)->data.value, 10, "%.6g", atof(e1->data.value) - atof(e2->data.value));
        break;
    case MUL:
        snprintf((*op)->data.value, 10, "%.6g", atof(e1->data.value) * atof(e2->data.value));
        break;
    case DIV:
        snprintf((*op)->data.value, 10, "%.6g", atof(e1->data.value) / atof(e2->data.value));
        break;
    case MOD:
        snprintf((*op)->data.value, 10, "%d", atoi(e1->data.value) % atoi(e2->data.value));
        break;
    case EXP:
        snprintf((*op)->data.value, 10, "%.6g", pow(atof(e1->data.value), atof(e2->data.value)));
        break;
    default:
        break;
    }

    (*op)->data.attr = strchr((*op)->data.value, DOT) ? _float : _int;

    freeTree(e1);
    freeTree(e2);
}

int getPrecedence(Token token)
{
    if (strlen(token.value) > 1)
        return 0;

    int p = 0;

    switch (token.value[0])
    {
    case ADD:
    case SUB:
        p = 2;
        break;
    case DIV:
    case MUL:
    case MOD:
        p = 3;
        break;
    case EXP:
        p = 4;
        break;
    default:
        break;
    }
    return p;
}

Associates getAssociates(Token token)
{
    if (strlen(token.value) > 1)
        return 0;

    Associates a = 0;
    switch (token.value[0])
    {
    case ADD:
    case SUB:
    case DIV:
    case MUL:
    case MOD:
        a = left_to_right;
        break;
    case EXP:
        a = right_to_left;
        break;
    default:
        break;
    }
    return a;
}
