#include <stdio.h>

#include "arch.h"
#include "util.h"

/* private */
static const char * const regs_64[] = {
    "%rax", "%rbx", "%rcx", "%rdx", "%rdi", "%rsi"
};
static const char * const regs_32[] = {
    "%eax", "%ebx", "%ecx", "%edx", "%edi", "%esi"
};
static const char * const regs_16[] = {
    "%ax", "%bx", "%cx", "%dx", "%di", "%si"
};
static const char * const regs_8[] = {
    "%al", "%bl", "%cl", "%dl", "%di", "%si"
};
/* ------- */

/* public */
const size_t nregs_64 = sizeof(regs_64) / sizeof(*regs_64);
const size_t nregs_32 = sizeof(regs_32) / sizeof(*regs_32);
const size_t nregs_8 = sizeof(regs_8) / sizeof(*regs_8);

FILE *cg_out = NULL;

void cg_push_64_literal(const char *literal) {
    fprintf(cg_out, "\tpush\t$%s\n", literal);
}

void cg_pop_64(int r) {
    fprintf(cg_out, "\tpop \t%s\n", regs_64[r]);
}

void cg_push_64(int r) {
    fprintf(cg_out, "\tpush\t%s\n", regs_64[r]);
}

void cg_mov_64(int rdst, int rsrc) {
    if (rdst != rsrc) {
        fprintf(cg_out, "\tmov \t%s, %s\n", regs_64[rsrc], regs_64[rdst]);
    }
}

void cg_add_64(int rdst, int r1, int r2) {
    cg_mov_64(r2, rdst);
    fprintf(cg_out, "\taddq\t%s, %s\n", regs_64[r1], regs_64[rdst]);
}

void cg_sub_64(int rdst, int r1, int r2) {
    cg_mov_64(rdst, r2);
    fprintf(cg_out, "\tsubq\t%s, %s\n", regs_64[r1], regs_64[rdst]);
}

void cg_mul_64(int rdst, int r1, int r2) {
    if (rdst != 0) {
        cg_push_64(0); /* x86_64 stores output of mul into rax */
    }
    cg_mov_64(0, r2);
    fprintf(cg_out, "\tmulq\t%s\n", regs_64[r1]);
    if (rdst != 0) {
        cg_mov_64(rdst, 0);
    }
    if (rdst != 0) {
        cg_pop_64(0);
    }
}

void cg_div_64(int rdst, int r1, int r2) {
    UNUSED(rdst);
    UNUSED(r1);
    UNUSED(r2);
}

void cg_func_begin(const char *name) {
    UNUSED(name);
}

void cg_func_end(void) {
}

void cg_begin(FILE *out) {
    const char *preamble = \
        "\t.text\n"
        "\t.globl	main\n"
        "\t.globl	_start\n"
        "\t.globl	_end\n"
        "_start:\n"
        "main:\n"
        ".LFB0:\n";
    cg_out = out;
    fprintf(cg_out, "%s", preamble);
    UNUSED(regs_16);
    UNUSED(regs_32);
    UNUSED(regs_8);
}

void cg_end(void) {
    const char *preamble = \
        "_end:\n"
        "\tmovq	%rax, %rdi\n" /* rdi, status to pass to exit_group */
        "\tmovq	$0xe7, %rax\n" /* exit_group syscall */
        "\tsyscall\n";
    fprintf(cg_out, "%s\n", preamble);
}
/* ------ */
