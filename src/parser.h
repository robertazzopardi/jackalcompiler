#ifndef _PARSER_
#define _PARSER_

#include "lexer.h"
#include "ast.h"
#include "stack.h"

void parse(Sequence seq);
Node *shuntingYardPostFix(Sequence seq);
void addRemainingOperators(StackNode *operator_stack, StackNode *output_stack);

int getPrecedence(Node *node);
Associates getAssociates(Node *node);

#endif
