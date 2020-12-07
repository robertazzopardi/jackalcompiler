#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "parser.h"
#include "lexer.h"
#include "ast.h"
#include "stack.h"

size_t FindIndex(const Token *a, size_t size, Attribute attribute, const size_t i)
{
    size_t index = i;

    while (index < size && a[index].attr != attribute)
        ++index;

    return (index == size ? -1 : index);
}

void eval(Node *op, Node *result, Node *e1, Node *e2)
{
    switch (op->data.value[0])
    {
    case ADD:
        snprintf(result->data.value, 10, "%.6g", strtof(e1->data.value, NULL) + strtof(e2->data.value, NULL));
        break;
    case SUB:
        snprintf(result->data.value, 10, "%.6g", strtof(e1->data.value, NULL) - strtof(e2->data.value, NULL));
        break;
    case MUL:
        snprintf(result->data.value, 10, "%.6g", strtof(e1->data.value, NULL) * strtof(e2->data.value, NULL));
        break;
    case DIV:
        snprintf(result->data.value, 10, "%.6g", strtof(e1->data.value, NULL) / strtof(e2->data.value, NULL));
        break;
    case MOD:
        snprintf(result->data.value, 10, "%d", atoi(e1->data.value) % atoi(e2->data.value));
        break;
    case EXP:
        snprintf(result->data.value, 10, "%.6g", pow(strtof(e1->data.value, NULL), strtof(e2->data.value, NULL)));
        break;
    case LARR:
        snprintf(result->data.value, 10, "%d", strtof(e1->data.value, NULL) < strtof(e2->data.value, NULL));
        break;
    case RARR:
        snprintf(result->data.value, 10, "%d", strtof(e1->data.value, NULL) > strtof(e2->data.value, NULL));
        break;
    default:
        printf("Error cannot evaluate expression\n");
        break;
    }
}

Node *evaluateExpression(Node *op, Node *e1, Node *e2)
{
    Node *result = malloc(sizeof(*result));
    result->data.value = malloc(sizeof(*result->data.value));

    eval(op, result, e1, e2);

    result->data.attr = strchr(result->data.value, DOT) ? _float : _int;

    free(op);
    free(e1);
    free(e2);

    result->rightNode = NULL;
    result->leftNode = NULL;

    return result;
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

    size_t bracket_count = 0;

    for (size_t i = start; i < end; i++)
    {
        Token token = seq.tokens[i];

        if (token.attr == _float || token.attr == _int)
        {
            push(&output_stack, newNodeLeaf(token));
            output_size++;
        }
        else if (token.attr == _funcCall || token.attr == _funcDef || token.attr == _funcName)
        {
            push(&operator_stack, newNodeLeaf(token));
        }
        else if (token.attr == _funcReturnType)
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
                while (operator_stack && ((token.attr == _rightBracket && top(operator_stack)->data.attr != _leftBracket) ||
                                          (top(operator_stack)->data.precedence > token.precedence) ||
                                          ((top(operator_stack)->data.precedence == token.precedence) &&
                                           (token.associate == left_to_right))))
                {
                    Node *op = pop(&operator_stack);

                    Node *e2 = pop(&output_stack);
                    Node *e1 = pop(&output_stack);

                    if (op->data.attr == _operator &&
                        (e1->data.attr == _int || e1->data.attr == _float) &&
                        (e2->data.attr == _int || e2->data.attr == _float))
                    {

                        push(&output_stack, evaluateExpression(op, e1, e2));
                    }

                    else if ((operator_stack && e1 && e2) &&
                             operator_stack->data->data.attr == _comma &&
                             op->data.attr == _funcCall &&
                             (e1->data.attr == _comma || e2->data.attr == _comma))
                    {
                        while (operator_stack->data->data.attr != _funcCall)
                            pop(&operator_stack);

                        Node *func = pop(&operator_stack);

                        if (e1->data.attr == _comma &&
                            e2->data.attr != _comma)
                        {
                            op = newNodeBoth(op->data, e1->leftNode, e1->rightNode);
                            push(&output_stack, newNodeBoth(func->data, op, e2));
                        }
                        else if (e1->data.attr != _comma &&
                                 e2->data.attr == _comma)
                        {
                            op = newNodeBoth(op->data, e2->leftNode, e2->rightNode);
                            push(&output_stack, newNodeBoth(func->data, e1, op));
                        }

                        freeTree(func);
                    }
                    else if (!operator_stack &&
                             op->data.attr == _funcCall &&
                             e1 &&
                             e1->data.attr == _funcCall)
                    {
                        push(&output_stack, newNodeLeft(op->data, e1));
                    }

                    else if (!operator_stack &&
                             op->data.attr == _funcCall &&
                             e2 &&
                             e2->data.attr == _funcCall)
                    {
                        push(&output_stack, newNodeLeft(op->data, e2));
                    }

                    else if (operator_stack->data->data.attr == _operator &&
                             operator_stack->next->data->data.attr != _funcCall)
                    {
                        Node *opop = pop(&operator_stack);
                        Node *val = pop(&output_stack);

                        opop = evaluateExpression(opop, val, e1);

                        push(&output_stack, newNodeBoth(op->data, opop, e2));
                    }

                    else if (operator_stack->data->data.attr == _funcCall &&
                             e1->data.attr == _comma &&
                             e2->data.attr != _comma)
                    {
                        Node *func = pop(&operator_stack);
                        e1 = newNodeBoth(func->data, e1->leftNode, e1->rightNode);
                        push(&output_stack, newNodeBoth(op->data, e1, e2));
                    }
                    else if (operator_stack->data->data.attr == _funcCall &&
                             e1->data.attr != _comma &&
                             e2->data.attr == _comma)
                    {
                        Node *func = pop(&operator_stack);
                        e2 = newNodeBoth(func->data, e2->leftNode, e2->rightNode);
                        push(&output_stack, newNodeBoth(op->data, e1, e2));
                    }

                    else if (operator_stack->data->data.attr == _operator &&
                             operator_stack->next->data->data.attr == _funcCall)
                    {
                        Node *opop = pop(&operator_stack);
                        Node *func = pop(&operator_stack);

                        op->leftNode = e1->leftNode;
                        op->rightNode = e1->rightNode;

                        func->leftNode = e2->leftNode;
                        func->rightNode = e2->rightNode;

                        push(&output_stack, newNodeBoth(opop->data, op, func));

                        freeTree(opop);
                        // free(func);
                    }

                    else if (op->data.attr == _funcCall &&
                             e2->data.attr == _funcCall)
                    {
                        push(&output_stack, newNodeLeft(op->data, e2));
                    }

                    else if (operator_stack->data->data.attr == _leftBracket &&
                             op->data.attr == _funcCall &&
                             e1 &&
                             !e2 &&
                             e1->data.attr == _comma)
                    {
                        op = newNodeBoth(op->data, e1->leftNode, e1->rightNode);
                        push(&output_stack, op);
                    }
                    else if (operator_stack->data->data.attr == _leftBracket &&
                             op->data.attr == _funcCall &&
                             !e1 &&
                             e2 &&
                             e2->data.attr == _comma)
                    {
                        op = newNodeBoth(op->data, e2->leftNode, e2->rightNode);
                        push(&output_stack, op);
                    }

                    else if (operator_stack->data->data.attr == _leftBracket &&
                             op->data.attr == _funcCall &&
                             e1 &&
                             !e2)
                    {
                        push(&output_stack, newNodeLeft(op->data, e1));
                    }
                    else if (operator_stack->data->data.attr == _leftBracket &&
                             op->data.attr == _funcCall &&
                             !e1 && e2)
                    {
                        push(&output_stack, newNodeLeft(op->data, e2));
                    }

                    else if (op->data.attr == _funcCall &&
                             e1 &&
                             !e2)
                    {
                        op->leftNode = e1->leftNode;
                        op->rightNode = e1->rightNode;

                        push(&output_stack, op);
                    }
                    else if (op->data.attr == _funcCall &&
                             !e1 &&
                             e2)
                    {
                        op->leftNode = e2->leftNode;
                        op->rightNode = e2->rightNode;

                        push(&output_stack, op);
                    }
                    else if (op->data.attr == _funcCall &&
                             e1 &&
                             e2)
                    {
                        op->leftNode = e1;
                        op->rightNode = e2;

                        push(&output_stack, op);
                    }

                    else if (op->data.attr == _funcCall &&
                             e1->data.attr == _comma &&
                             e2->data.attr == _comma)
                    {
                        Node *opop = pop(&operator_stack);
                        Node *val = pop(&operator_stack);

                        push(&output_stack, newNodeBoth(opop->data, newNodeBoth(op->data, e1->leftNode, e1->rightNode),
                                                        newNodeBoth(val->data, e2->leftNode, e2->rightNode)));
                    }

                    else if (output_stack &&
                             output_stack->next &&
                             operator_stack->data->data.attr == _comma &&
                             ((output_stack->data->data.attr == _int || output_stack->next->data->data.attr == _float) ||
                              (output_stack->data->data.attr == _float || output_stack->next->data->data.attr == _float)))
                    {
                        push(&output_stack, newNodeBoth(op->data, e1, e2));
                    }

                    else if (operator_stack->data->data.attr == _leftBracket)
                    {
                        if (e1 && e2)
                        {
                            if (e1->data.attr != _comma &&
                                e2->data.attr == _comma)
                                push(&output_stack, newNodeBoth(op->data, e2, e1));
                            else
                                push(&output_stack, newNodeBoth(op->data, e1, e2));
                        }
                        else if (e1 && !e2)
                            push(&output_stack, newNodeLeft(op->data, e1));
                        else if (!e1 && e2)
                            push(&output_stack, newNodeLeft(op->data, e2));
                    }

                    else if (op->data.attr == _comma &&
                             e1->data.attr == _comma &&
                             e2->data.attr != _comma)
                    {
                        op->leftNode = e1;
                        op->rightNode = e2;

                        push(&output_stack, op);
                    }
                    else if (op->data.attr == _comma &&
                             e1->data.attr != _comma &&
                             e2->data.attr == _comma)
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
            }
        }
        else
        {
        }
    }

    free(operator_stack);
    // free(output_stack);
    return pop(&output_stack);
}

static inline void addNodeRight(Node **curr, Node **ifRoot, Token token)
{
    *ifRoot = *curr;
    (*curr)->data = token;
    (*curr)->leftNode = calloc(1, sizeof(*(*curr)->leftNode));
    *curr = (*curr)->leftNode;

    Token condition;
    condition.value = CONDITION;
    condition.attr = _condition;

    (*curr)->data = condition;
}

Node *parse(Sequence seq)
{
    printf("\n");

    Node *root = calloc(1, sizeof(*root));
    Node *curr = root;
    Node *ifRoot;

    for (size_t i = 0; i < seq.count; i++)
    {
        Token token = seq.tokens[i];

        switch (token.attr)
        {
        case _funcDef:
            curr = findLastElement(&root);
        case _funcCall:
            if (curr->leftNode)
            {
                curr->rightNode = calloc(1, sizeof(*curr->rightNode));
                curr = curr->rightNode;
            }
            curr->data = token;
            break;
        case _funcName:
            curr->leftNode = calloc(1, sizeof(*curr->leftNode));
            curr = curr->leftNode;
            curr->data = token;
            break;
        case _type:
            curr->rightNode->data = token;
            curr->leftNode = calloc(1, sizeof(*curr->leftNode));
            curr = curr->leftNode;
            break;

        case _if:
            addNodeRight(&curr, &ifRoot, token);
            break;
        case _elif:
        case _else:
            curr = ifRoot;
            curr->rightNode = calloc(1, sizeof(*curr->rightNode));
            curr = curr->rightNode;
            addNodeRight(&curr, &ifRoot, token);
            break;
        case _funcReturnType:
            curr->rightNode = calloc(1, sizeof(*curr->rightNode));
            break;
        case _leftBracket:
        {
            size_t n = FindIndex(seq.tokens, seq.count, _rightBracket, i);
            curr->leftNode = shuntingYardPostFix(seq, i, n + 1);
            i = n;
        }
        break;
        case _rightBracket:
            break;
        case _int:
        case _float:
            curr->leftNode = shuntingYardPostFix(seq, i, seq.count);
            break;

        default:
            break;
        }
    }

    return root;
}
