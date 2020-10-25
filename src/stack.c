#include <stdio.h>
#include <stdlib.h>

#include "stack.h"

StackNode *newNode(Node *data)
{
    StackNode *stackNode = (StackNode *)malloc(sizeof(StackNode));
    stackNode->data = data;
    stackNode->next = NULL;
    return stackNode;
}

int isEmpty(StackNode *root)
{
    return !root;
}

void push(StackNode **root, Node *data)
{
    StackNode *stackNode = newNode(data);
    stackNode->next = *root;
    *root = stackNode;

    printf("%s pushed to stack\n", data->data.value);
}

Node *pop(StackNode **root)
{
    if (isEmpty(*root))
        return NULL;

    StackNode *temp = *root;
    *root = (*root)->next;
    Node *popped = temp->data;

    free(temp);

    return popped;
}

Node *top(StackNode *root)
{
    if (isEmpty(root))
        return NULL;
    return root->data;
}
