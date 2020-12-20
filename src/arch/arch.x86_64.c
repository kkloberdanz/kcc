#include <stdio.h>

#include "arch.h"
#include "util.h"

/* private */
static const char * const regs_i64[] = {
    "%rax", "%rdi", "%rsi", "%rdx", "%rcx", "%r8", "%r9",
    "%rbx", "%r10", "%r11", "%r12", "%r13", "%r14", "%r15"
};

static const char * const regs_i32[] = {
    "%eax", "%edi", "%esi", "%edx", "%ecx", "%r8d", "%r9d",
    "%ebx", "%r10d", "%r11d", "%r12d", "%r13d", "%r14d", "%r15d"
};

static const char * const regs_i16[] = {
    "%ax", "%di", "%si", "%dx", "%cx", "%r8w", "%r9w",
    "%bx", "%r10w", "%r11w", "%r12w", "%r13w", "%r14w", "%r15w"
};

static const char * const regs_i8[] = {
    "%al", "%dil", "%sil", "%dl", "%cl", "%r8b", "%r9b",
    "%bl", "%r10b", "%r11b", "%r12b", "%r13b", "%r14b", "%r15b"
};
/* ------- */

/* public */
const size_t nregs_i64 = sizeof(regs_i64) / sizeof(*regs_i64);
const size_t nregs_i32 = sizeof(regs_i32) / sizeof(*regs_i32);
const size_t nregs_i16 = sizeof(regs_i16) / sizeof(*regs_i16);
const size_t nregs_i8 = sizeof(regs_i8) / sizeof(*regs_i8);

const size_t cg_end_args_regs_64 = 6;
const size_t cg_end_args_regs_32 = 6;
const size_t cg_end_args_regs_16 = 6;
const size_t cg_end_args_regs_8 = 6;

FILE *cg_out = NULL;

void cg_push_i64_literal(const char *literal) {
    fprintf(cg_out, "\tpushq\t$%s\n", literal);
}

void cg_pop_i64(int r) {
    fprintf(cg_out, "\tpopq\t%s\n", regs_i64[r]);
}

void cg_push_i64(int r) {
    fprintf(cg_out, "\tpushq\t%s\n", regs_i64[r]);
}

void cg_mov_i64(int rdst, int rsrc) {
    if (rdst != rsrc) {
        fprintf(cg_out, "\tmovq\t%s, %s\n", regs_i64[rsrc], regs_i64[rdst]);
    }
}

void cg_add_i64(int rdst, int r1, int r2) {
    cg_mov_i64(rdst, r2);
    fprintf(cg_out, "\taddq\t%s, %s\n", regs_i64[r1], regs_i64[rdst]);
}

void cg_sub_i64(int rdst, int r1, int r2) {
    cg_mov_i64(rdst, r2);
    fprintf(cg_out, "\tsubq\t%s, %s\n", regs_i64[r1], regs_i64[rdst]);
}

void cg_mul_i64(int rdst, int r1, int r2) {
    if (rdst != 0) {
        cg_push_i64(0); /* xi86_i64 stores output of mul into rax */
    }
    cg_mov_i64(0, r2);
    fprintf(cg_out, "\tmulq\t%s\n", regs_i64[r1]);
    if (rdst != 0) {
        cg_mov_i64(rdst, 0);
    }
    if (rdst != 0) {
        cg_pop_i64(0);
    }
}

void cg_div_i64(int rdst, int r1, int r2) {
    UNUSED(rdst);
    UNUSED(r1);
    UNUSED(r2);
}

void cg_func_begin(const char *name) {
    const char * const fmt = \
        "\t.globl\t%s\n"
        "\t.type\t%s, @function\n"
        "%s:\n"
        "\t.cfi_startproc\n"
        "\tpushq\t%%rbp\n"
        "\tmovq\t%%rsp, %%rbp\n"
        ;
    fprintf(cg_out, fmt, name, name, name);
}

void cg_func_end(void) {
    const char * const inst = \
        "\tpopq\t%rax\n"
        "\tleave\n"
        "\tret\n"
        "\t.cfi_endproc\n"
        ;
    fprintf(cg_out, "%s", inst);
}

void cg_begin(FILE *out) {
    const char *preamble = \
        "\t.text\n"
        "\t.globl\t_start\n"
        "\t.globl\t_end\n"
        ;
    cg_out = out;
    fprintf(cg_out, "%s", preamble);
    UNUSED(regs_i16);
    UNUSED(regs_i32);
    UNUSED(regs_i8);
}

void cg_end(void) {
    const char *preamble = \
        "_start:\n"
        "\tcall\tmain\n"
        "_end:\n"
        "\tmovq\t%rax, %rdi\n" /* rdi, status to pass to exit_group */
        "\tcall\texit\n"
        ;
    fprintf(cg_out, "%s\n", preamble);
}
/* ------ */
