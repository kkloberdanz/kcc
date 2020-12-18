#include <stdio.h>

#include "ast.h"
#include "util.h"

static FILE *out = NULL;

void cg_traverse(AST *ast) {
    if (ast) {
        cg_traverse(ast->left);
        cg_traverse(ast->right);
        if (ast->repr) {
            fprintf(out, "\tpush\t$%s\n", ast->repr);
        } else {
            fprintf(out, "\tpop \t%%rax\n");
            fprintf(out, "\tpop \t%%rcx\n");
            switch (ast->op) {
                case OP_ADD:
                    fprintf(out, "\taddq\t%%rcx, %%rax\n");
                    fprintf(out, "\tpush\t%%rax\n");
                    break;
                case OP_MINUS:
                    fprintf(out, "\tsubq\t%%rcx, %%rax\n");
                    fprintf(out, "\tpush\t%%rax\n");
                    break;
                case OP_DIV:
                    fprintf(out, "\tidivq\t%%rcx, %%rax\n");
                    fprintf(out, "\tpush\t%%rax\n");
                    break;
                case OP_MUL:
                    fprintf(out, "\tmulq\t%%rcx\n");
                    fprintf(out, "\tpush\t%%rax\n");
                    break;
                case OP_NOP:
                    fprintf(out, "%s", "NOP");
                    break;
            }
        }
    }
}

void cg_begin() {
    const char *preamble = \
        "\t.text\n"
        "\t.globl	main\n"
        "\t.globl	inc\n"
        "\t.globl	_start\n"
        "\t.globl	_end\n"
        "_start:\n"
        "main:\n"
        ".LFB0:\n";
    fprintf(out, "%s", preamble);
}

void cg_end() {
    const char *preamble = \
        "_end:\n"
        "\tmovq	%rax, %rbx\n"
        "\tmovq	$0x1, %rax\n"
        "\tint		$0x80\n";

    fprintf(out, "%s\n", preamble);
}

void codegen(AST *ast, FILE *output) {
    out = output;
    cg_begin();
    cg_traverse(ast);
    cg_end();
}
