#ifndef _AST_
#define _AST_

#include "lexer.h"

typedef struct nodes Node;

struct nodes
{
    Token data;
    Node *leftNode;
    Node *rightNode;
};

void printPostOrder(Node *node);
void freeTree(Node *node);

Node *newNodeLeaf(Token data);
Node *newNodeRight(Token data, Node *rightNode);
Node *newNodeBoth(Token data, Node *leftNode, Node *rightNode);

#endif
