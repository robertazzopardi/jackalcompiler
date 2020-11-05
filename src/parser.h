#ifndef _PARSER_
#define _PARSER_

#include "lexer.h"
#include "ast.h"
#include "stack.h"

void parse(Sequence seq);
Node *shuntingYardPostFix(const Sequence seq);
void addRemainingOperators(StackNode **operator_stack, StackNode **output_stack, size_t *output_size);

// void evaluateExpression(StackNode **operator_stack, StackNode **output_stack, size_t *output_size);
void evaluateExpression(Node **operator, Node * e1, Node * e2);

int getPrecedence(Token token);
Associates getAssociates(Token token);

#endif
