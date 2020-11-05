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

void push(StackNode **root, Node *data)
{
    StackNode *stackNode = newNode(data);
    stackNode->next = *root;
    *root = stackNode;

    // printf("%s pushed to stack\n", data->data.value);
}

Node *pop(StackNode **root)
{
    if (IS_EMPTY(*root))
        return NULL;

    StackNode *temp = *root;
    *root = (*root)->next;
    Node *popped = temp->data;

    free(temp);

    // printf("%s popped from stack\n", popped->data.value);

    return popped;
}

Node *top(StackNode *root)
{
    if (IS_EMPTY(root))
        return NULL;
    return root->data;
}

void printStack(StackNode *root)
{
    StackNode *n = root;
    while (n)
    {
        printf("%s ", n->data->data.value);
        n = n->next;
    }
    printf("\n");
}
