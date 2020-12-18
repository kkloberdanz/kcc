#include <stdlib.h>

#include "ast.h"
#include "util.h"

void ast_free(AST *ast) {
    UNUSED(ast);
}

AST *ast_new(void) {
    AST *ast = malloc(sizeof(*ast));
    if (ast) {
        ast->left = NULL;
        ast->right = NULL;
        ast->repr = NULL;
        ast->op = OP_NOP;
    }
    return ast;
}

AST *ast_op(OpKind op) {
    AST *ast = malloc(sizeof(*ast));
    if (ast) {
        ast->op = op;
        ast->left = NULL;
        ast->right = NULL;
    }
    return ast;
}

AST *ast_int(char *i) {
    AST *ast = malloc(sizeof(*ast));
    if (ast) {
        ast->left = NULL;
        ast->right = NULL;
        ast->op = OP_NOP;
        ast->repr = i;
    }
    return ast;
}
