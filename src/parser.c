#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "lexer.h"
#include "ast.h"
#include "stack.h"

void parse(Sequence seq)
{
    for (size_t i = 0; i < seq.count; i++)
        // printf("%u %s\n", seq.tokens[i].attr, seq.tokens[i].value);
        printf("%s ", seq.tokens[i].value);
    printf("\n");

    Node *n = shuntingYardPostFix(seq);

    printPostOrder(n);
    printf("\n");

    freeTree(n);
}

Node *shuntingYardPostFix(Sequence seq)
{
    Token temp;
    int start = 0;
    int end = seq.count - 1;
    while (start < end)
    {
        temp = seq.tokens[start];
        seq.tokens[start] = seq.tokens[end];
        seq.tokens[end] = temp;
        start++;
        end--;
    }

    StackNode *operator_stack = NULL;
    StackNode *output_stack = NULL;
    size_t output_size = 0;

    // for (size_t i = 0; i < seq.count - 1; i++)
    while (seq.count--)
    {
        Token token = seq.tokens[seq.count];
        // Token token = seq.tokens[i];

        if (token.attr == _float || token.attr == _int)
        {
            push(&output_stack, newNodeLeaf(token));
            output_size++;
        }
        else if (token.attr == _func)
        {
            push(&operator_stack, newNodeLeaf(token));
            // if (output_size > 1)
            // {
            //     Node *e = pop(&output_stack);
            //     output_size--;

            //     push(&output_stack, newNodeRight(token, e));
            //     output_size++;
            // }
            // else
            // {
            //     push(&output_stack, newNodeLeaf(token));
            //     output_size++;
            // }
        }
        else if (token.attr == _operator || token.attr == _leftBracket || token.attr == _rightBracket)
        {
            if (token.attr != _leftBracket)
            {
                while (!isEmpty(operator_stack) && ((token.attr == _rightBracket && top(operator_stack)->data.attr != _leftBracket) || (getPrecedence(top(operator_stack)) > getPrecedence(newNodeLeaf(token))) || ((getPrecedence(top(operator_stack)) == getPrecedence(newNodeLeaf(token))) && (getAssociates(newNodeLeaf(token)) == left_to_right))))
                {
                    Node *op = pop(&operator_stack);

                    Node *e2 = pop(&output_stack);
                    output_size--;

                    Node *e1 = pop(&output_stack);
                    output_size--;

                    push(&output_stack, newNodeBoth(op->data, e1, e2));
                    output_size++;
                }

                // If we popped until '(' because token is ')', toss both parens
                if (token.attr == _rightBracket)
                {
                    freeTree(pop(&operator_stack));
                }
            }

            // Everything except ')' --> stack
            if (token.attr != _rightBracket)
            {
                push(&operator_stack, newNodeLeaf(token));
            }
        }
        else
        {
        }
    }

    while (!isEmpty(operator_stack))
    {
        if (output_size > 1)
        {
            Node *op = pop(&operator_stack);

            Node *e2 = pop(&output_stack);
            output_size--;

            Node *e1 = pop(&output_stack);
            output_size--;

            push(&output_stack, newNodeBoth(op->data, e1, e2));
            output_size++;
        }
        else
        {
            Node *op = pop(&operator_stack);

            Node *n = pop(&output_stack);
            output_size--;

            push(&output_stack, newNodeRight(op->data, n));
            output_size++;
        }
    }

    // 3 4 2 * 1 5 − 2 3 ^ ^ / +
    return pop(&output_stack);
}

int getPrecedence(Node *node)
{
    if (!node)
    {
        return 0;
    }

    if (!node->data.value)
    {
        return 0;
    }

    if (strlen(node->data.value) > 1)
    {
        return 0;
    }

    int p = 0;
    switch (node->data.value[0])
    {
    case ADD:
        p = 2;
        break;
    case SUB:
        p = 2;
        break;
    case DIV:
        p = 3;
        break;
    case MUL:
        p = 3;
        break;
    case EXP:
        p = 4;
        break;
    case MOD:
        p = 3;
        break;
    default:
        break;
    }
    return p;
}

Associates getAssociates(Node *node)
{
    if (!node)
    {
        return 0;
    }

    if (!node->data.value)
    {
        return 0;
    }

    if (strlen(node->data.value) > 1)
    {
        return 0;
    }

    Associates a = 0;
    switch (node->data.value[0])
    {
    case ADD:
        a = left_to_right;
        break;
    case SUB:
        a = left_to_right;
        break;
    case DIV:
        a = left_to_right;
        break;
    case MUL:
        a = left_to_right;
        break;
    case EXP:
        a = right_to_left;
        break;
    case MOD:
        a = left_to_right;
        break;
    default:
        break;
    }
    return a;
}
