#ifndef AST_H
#define AST_H

typedef struct AST {
    struct AST *left;
    struct AST *right;
} AST;

void ast_free(AST *ast);

#endif /* AST_H */
