#ifndef PARSE_H
#define PARSE_H

#include "token.h"
#include "ast.h"

AST *parse(TokList *tokens);

#endif /* PARSE_H */
