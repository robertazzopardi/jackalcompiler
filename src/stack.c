#include <stdio.h>
#include <stdlib.h>

#include "stack.h"

StackNode *newNode(Node *data)
{
    StackNode *stackNode = malloc(sizeof(*stackNode));
    stackNode->data = data;
    stackNode->next = NULL;
    return stackNode;
}

void push(StackNode **root, Node *data)
{
    StackNode *stackNode = newNode(data);
    stackNode->next = *root;
    *root = stackNode;
}

Node *pop(StackNode **root)
{
    if (!*root)
        return NULL;

    StackNode *temp = *root;
    *root = (*root)->next;
    Node *popped = temp->data;

    free(temp);

    return popped;
}

Node *top(StackNode *root)
{
    if (!root)
        return NULL;
    return root->data;
}
