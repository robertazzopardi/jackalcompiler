#ifndef _STACK_
#define _STACK_

#include <limits.h>

#include "ast.h"

typedef struct stack StackNode;
typedef struct stack
{
    Node *data;
    StackNode *next;
} StackNode;

StackNode *newNode(Node *data);
int isEmpty(StackNode *root);
void push(StackNode **root, Node *data);
Node *pop(StackNode **root);
Node *top(StackNode *root);

#endif
