#ifndef AST_H
#define AST_H

#include "type.h"

typedef enum OpKind {
    OP_NOP,
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_MOD
} OpKind;

typedef enum Kind {
    AST_UNASSIGNED,
    AST_ID,
    AST_OP,
    AST_CONSTANT
} Kind;

typedef struct AST {
    struct AST *left;
    struct AST *right;
    char *repr;
    OpKind op;
    Type type;
    Kind kind;
} AST;

AST *ast_new(void);

void ast_free(AST *ast);

AST *ast_op(AST *left, OpKind op, AST *right);

AST *ast_int(const char *i);

AST *ast_float(const char *);

AST *ast_char(const char *);

AST *ast_string(const char *);

AST *ast_type_specifier(Type type);

AST *ast_identifier_decl(char *id);

#endif /* AST_H */
