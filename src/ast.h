#ifndef AST_H
#define AST_H

typedef enum OpKind {
    OP_NOP,
    OP_ADD,
    OP_MINUS,
    OP_MUL,
    OP_DIV
} OpKind;

typedef struct AST {
    struct AST *left;
    struct AST *right;
    char *repr;
    OpKind op;
} AST;

AST *ast_new(void);

void ast_free(AST *ast);

AST *ast_op(OpKind op);

AST *ast_int(char *i);

#endif /* AST_H */
