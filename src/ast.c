#include <stdlib.h>
#include <string.h>

#include "ast.h"
#include "util.h"

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
        ast->repr = strdup(i);
    }
    return ast;
}

void ast_free(AST *ast) {
    if (ast) {
        ast_free(ast->left);
        ast_free(ast->right);
        free(ast->repr);
        free(ast);
    }
}
