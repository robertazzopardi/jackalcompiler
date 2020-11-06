#ifndef _STACK_
#define _STACK_

#include <limits.h>

#include "ast.h"

#define IS_EMPTY(stack) ({ !stack; })

typedef struct stack StackNode;

typedef struct stack
{
    Node *data;
    StackNode *next;
} StackNode;

StackNode *newNode(Node *data);

void push(StackNode **root, Node *data);
Node *pop(StackNode **root);
Node *top(StackNode *root);
// void printStack(StackNode *root);

#endif
