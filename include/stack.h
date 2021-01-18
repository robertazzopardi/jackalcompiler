/**
 * @file stack.h
 * @author robertazzopardi (robertazzopardi@icloud.com)
 * @brief Definition for a stack data structure
 * @version 0.1
 * @date 2021-01-17
 *
 * @copyright Copyright (c) 2021
 *
 */
#ifndef _STACK_
#define _STACK_

#include "ast.h"

/**
 * @brief Stack Node Declaration
 *
 */
typedef struct stack StackNode;

/**
 * @brief stack Node Struct
 *
 */
typedef struct stack
{
    Node *data;
    StackNode *next;
} StackNode;

/**
 * @brief Add a new node to the stack
 *
 * @param root
 * @param data
 */
void push(StackNode **root, Node *data);

/**
 * @brief Remove node from the stack
 *
 * @param root
 * @return Node*
 */
Node *pop(StackNode **root);

/**
 * @brief Get the top element in the stack
 *
 * @param root
 * @return Node*
 */
Node *top(StackNode *root);

// void printStack(StackNode *root);

#endif
