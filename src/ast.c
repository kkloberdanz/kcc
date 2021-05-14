#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"
#include "util.h"

static void *ast_malloc(size_t sz) {
    AST *ast = malloc(sz);
    if (!ast) {
        fprintf(stderr, "failed to allocate memory");
        exit(EXIT_FAILURE);
    }
    ast->left = NULL;
    ast->right = NULL;
    ast->repr = NULL;
    ast->type = TYPE_NONE;
    ast->op = OP_NOP;
    ast->kind = AST_UNASSIGNED;
    return ast;
}

AST *ast_new(void) {
    AST *ast = ast_malloc(sizeof(*ast));
    if (ast) {
        ast->type = TYPE_NONE;
    }
    return ast;
}

AST *ast_op(AST *left, OpKind op, AST *right) {
    AST *ast = ast_malloc(sizeof(*ast));
    if (ast) {
        ast->op = op;
        ast->left = left;
        ast->right = right;
    }
    return ast;
}

AST *ast_int(const char *i) {
    AST *ast = ast_malloc(sizeof(*ast));
    if (ast) {
        ast->repr = strdup(i);
        ast->type = TYPE_INT;
    }
    return ast;
}

AST *ast_float(const char *i) {
    AST *ast = ast_malloc(sizeof(*ast));
    if (ast) {
        ast->repr = strdup(i);
        ast->type = TYPE_FLOAT;
    }
    return ast;
}

AST *ast_char(const char *i) {
    AST *ast = ast_malloc(sizeof(*ast));
    if (ast) {
        ast->repr = strdup(i);
        ast->type = TYPE_CHAR;
    }
    return ast;
}

AST *ast_string(const char *i) {
    AST *ast = ast_malloc(sizeof(*ast));
    if (ast) {
        ast->repr = strdup(i);
        ast->type = TYPE_CHAR_P;
    }
    return ast;
}

AST *ast_type_specifier(Type type) {
    AST *ast = ast_malloc(sizeof(*ast));
    if (ast) {
        ast->type = type;
    }
    return ast;
}

AST *ast_identifier_decl(char *id) {
    AST *ast = ast_malloc(sizeof(*ast));
    if (ast) {
        ast->repr = id;
        ast->kind = AST_ID;
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
