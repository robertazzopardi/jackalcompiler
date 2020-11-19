#ifndef _PARSER_
#define _PARSER_

#include "lexer.h"
#include "ast.h"
#include "stack.h"

Node *parse(Sequence seq);
Node *shuntingYardPostFix(const Sequence seq);
void addRemainingOperators(StackNode **operator_stack, StackNode **output_stack, size_t *output_size);
Node *evaluateExpression(Node *operator, Node * e1, Node * e2);

#endif
