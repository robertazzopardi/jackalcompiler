#ifndef _AST_
#define _AST_

#include "lexer.h"

#define SPACES 10

typedef struct nodes Node;

struct nodes
{
    Token data;
    Node *leftNode;
    Node *rightNode;
    Node *condition;

    Node *parent;
};

// struct nodes
// {
//     Token data;
//     Node *leftNode;
//     Node *rightNode;
// };

void printInOrder(Node *node);

void printPreOrder(Node *node);
void printPostOrder(Node *node);
void freeTree(Node *node);

void print2d(Node *root, int space);

Node *findLastElement(Node **root);

Node *newNodeLeaf(Token data);
Node *newNodeLeft(Token data, Node *node);

Node *newNodeBoth(Token data, Node *leftNode, Node *rightNode);

#endif
