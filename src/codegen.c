#include <stdio.h>

#include "arch.h"
#include "ast.h"
#include "codegen.h"
#include "util.h"

static void binop(AST *ast) {
    cg_pop_64(1);
    cg_pop_64(0);
    switch (ast->op) {
        case OP_ADD:
            cg_add_64(2, 1, 0);
            cg_push_64(2);
            break;

        case OP_MINUS:
            cg_sub_64(2, 1, 0);
            cg_push_64(2);
            break;

        case OP_DIV:
            /* TODO */
            break;

        case OP_MUL:
            cg_mul_64(2, 1, 0);
            cg_push_64(2);
            break;

        case OP_NOP:
            /* TODO */
            break;
    }
}

static void cg_traverse(AST *ast) {
    if (ast) {
        cg_traverse(ast->left);
        cg_traverse(ast->right);
        if (ast->repr) {
            cg_push_64_literal(ast->repr);
        } else {
            binop(ast);
        }
    }
}

void codegen(AST *ast, FILE *output) {
    cg_begin(output);
    cg_func_begin("main");
    cg_traverse(ast);

    cg_func_end();

    cg_end();
}
