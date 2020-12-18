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

void ast_free(AST *ast);

AST *ast_op(OpKind op);

AST *ast_int(char *i);

AST *ast_new(void);

#endif /* AST_H */
