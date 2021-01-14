#include <stdio.h>
#include <stdlib.h>

#include "../include/ast.h"

Node *findLastElement(Node **root)
{
    if (root == NULL)
        return NULL;

    while ((*root)->rightNode != NULL)
        root = &(*root)->rightNode;

    return *root;
}

void printInOrder(Node *node)
{
    if (!node)
        return;

    printPreOrder(node->leftNode);

    printf("%s ", node->data.value);

    printPreOrder(node->rightNode);
}

void printPreOrder(Node *node)
{
    if (!node)
        return;

    printf("%s ", node->data.value);

    printPreOrder(node->leftNode);
    printPreOrder(node->rightNode);
}

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
    // Node *node = malloc(sizeof(*node));

    // Assign data to this node
    node->data = data;

    // Initialize left and right children as NULL
    // node->leftNode = NULL;
    // node->rightNode = NULL;
    return node;
}

Node *newNodeLeft(Token data, Node *node)
{
    // Allocate memory for new node
    Node *root = calloc(1, sizeof(*root));

    // Assign data to this node
    root->data = data;

    // Initialize left and right children as NULL
    // node->leftNode = NULL;
    root->leftNode = node;
    return root;
}

Node *newNodeBoth(Token data, Node *leftNode, Node *rightNode)
{
    // Allocate memory for new node
    Node *node = calloc(1, sizeof(*node));
    // Node *node = malloc(sizeof(*node));

    // Assign data to this node
    node->data = data;

    // Initialize left and right children as NULL
    node->leftNode = leftNode;
    node->rightNode = rightNode;

    return node;
}

void print2d(Node *root, int space)
{
    // Base case
    if (root == NULL)
        return;

    // Increase distance between levels
    space += SPACES; // COUNT;

    // Process right child first
    print2d(root->rightNode, space);

    // Print current node after space count
    printf("\n");
    for (int i = SPACES; i < space; i++)
        printf(" ");
    printf("%s\n", root->data.value);

    // Process left child
    print2d(root->leftNode, space);
}
