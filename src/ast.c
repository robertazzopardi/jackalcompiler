#include <stdio.h>
#include <stdlib.h>

#include "ast.h"

void printPostOrder(Node *node)
{
    if (!node)
        return;

    printPostOrder(node->leftNode);
    printPostOrder(node->rightNode);

    printf("%s ", node->data.value);
}

void freeTree(Node *node)
{
    if (!node)
        return;

    freeTree(node->leftNode);
    freeTree(node->rightNode);

    free(node);
}

Node *newNodeLeaf(Token data)
{
    // Allocate memory for new node
    Node *node = calloc(1, sizeof(*node));

    // Assign data to this node
    node->data = data;

    // Initialize left and right children as NULL
    // node->leftNode = NULL;
    // node->rightNode = NULL;
    return node;
}

Node *newNodeRight(Token data, Node *rightNode)
{
    // Allocate memory for new node
    Node *node = calloc(1, sizeof(*node));

    // Assign data to this node
    node->data = data;

    // Initialize left and right children as NULL
    // node->leftNode = NULL;
    node->rightNode = rightNode;
    return node;
}

Node *newNodeBoth(Token data, Node *leftNode, Node *rightNode)
{
    // Allocate memory for new node
    Node *node = calloc(1, sizeof(*node));

    // Assign data to this node
    node->data = data;

    // Initialize left and right children as NULL
    node->leftNode = leftNode;
    node->rightNode = rightNode;

    return node;
}
