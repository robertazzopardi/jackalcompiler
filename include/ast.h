/**
 * @file ast.h
 * @author robertazzopardi (robertazzopardi@icloud.com)
 * @brief Interface to the abstract syntax tree declaration
 * @version 0.1
 * @date 2021-01-15
 *
 * @copyright Copyright (c) 2021
 *
 */

#ifndef __AST__
#define __AST__

/**
 * @brief Type declaration of a node
 *
 */
typedef struct Node Node;

typedef enum Type Type;

typedef struct Token Token;

/**
 * @brief Node Struct declaration
 *
 */
struct Node {
    Type *type;
    Token *data;
    Node *leftNode;
    Node *rightNode;
    Node *parent;
};

/**
 * @brief In order tree traversal
 *
 * @param node
 */
void printInOrder(Node *node);

/**
 * @brief Pre order tree traversal
 *
 * @param node
 */
void printPreOrder(Node *node);

/**
 * @brief Post order tree traversal
 *
 * @param node
 */
void printPostOrder(Node *node);

/**
 * @brief Frees allocated memory
 *
 * @param node
 */
void freeTree(Node *node);

/**
 * @brief Format print ast
 *
 * @param root
 * @param space
 */
void print2d(Node *root, int space);

/**
 * @brief Find the right most node in the tree
 *
 * @param root
 * @return Node*
 */
Node *findRightmostElement(Node **root);

/**
 * @brief Create a new leaf node
 *
 * @param data
 * @return Node*
 */
Node *newNodeLeaf(Token *data);

/**
 * @brief Create new node with a node to the left
 *
 * @param data
 * @param node
 * @return Node*
 */
Node *newNodeLeft(Token *data, Node *node);

/**
 * @brief Create new node with a left and right node
 *
 * @param data
 * @param leftNode
 * @param rightNode
 * @return Node*
 */
Node *newNodeBoth(Token *data, Node *leftNode, Node *rightNode);

#endif
