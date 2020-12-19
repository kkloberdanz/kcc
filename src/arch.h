#ifndef ARCH_H
#define ARCH_H

#include <stddef.h>

extern FILE *cg_out;

extern const size_t nregs_i64;
extern const size_t nregs_i32;
extern const size_t nregs_i16;
extern const size_t nregs_i8;

extern const size_t cg_end_args_regs_64;
extern const size_t cg_end_args_regs_32;
extern const size_t cg_end_args_regs_16;
extern const size_t cg_end_args_regs_8;

void cg_push_i64_literal(const char *literal);

void cg_pop_i64(int r);

void cg_push_i64(int r);

void cg_mov_i64(int rdst, int rsrc);

void cg_add_i64(int rdst, int r1, int r2);

void cg_sub_i64(int rdst, int r1, int r2);

void cg_mul_i64(int rdst, int r1, int r2);

void cg_div_i64(int rdst, int r1, int r2);

void cg_func_begin(const char *name);

void cg_func_end(void);

void cg_begin(FILE *out);

void cg_end(void);

#endif /* ARCH_H */
