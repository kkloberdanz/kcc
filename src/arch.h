#ifndef ARCH_H
#define ARCH_H

#include <stddef.h>

extern FILE *cg_out;
extern const size_t nregs_64;
extern const size_t nregs_32;
extern const size_t nregs_8;

void cg_push_64_literal(const char *literal);

void cg_pop_64(int r);

void cg_push_64(int r);

void cg_mov_64(int rdst, int rsrc);

void cg_add_64(int rdst, int r1, int r2);

void cg_sub_64(int rdst, int r1, int r2);

void cg_mul_64(int rdst, int r1, int r2);

void cg_div_64(int rdst, int r1, int r2);

void cg_func_begin(const char *name);

void cg_func_end(void);

void cg_begin(FILE *out);

void cg_end(void);

#endif /* ARCH_H */
