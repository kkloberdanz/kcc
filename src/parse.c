#include <stdio.h>
#include <stdlib.h>

#include "token.h"
#include "parse.h"
#include "util.h"

static TokList *tokens;

static void match(TokenKind kind) {
    if (kind != tokens->tok.kind) {
        char buf[2048];
        sprintf(
            buf,
            "expecting token: '%s', got: '%s'",
            kind_repr(kind),
            kind_repr(tokens->tok.kind)
        );
        syntax_error(buf, tokens->tok.lineno, tokens->tok.col);
        exit(1);
    } else {
        tokens = tokens->next;
    }
}

static OpKind binop(void) {
    /* TODO: operator precedence is not yet implemented
     * order of operations is not yet being respected
     */
    Token t;
    if (!tokens) {
        syntax_error("out of tokens", 0, 0);
        exit(1);
    }
    t = tokens->tok;
    switch (t.kind) {
        case TOK_PLUS:
            match(TOK_PLUS);
            return OP_ADD;
        case TOK_MINUS:
            match(TOK_MINUS);
            return OP_MINUS;
        case TOK_STAR:
            match(TOK_STAR);
            return OP_MUL;
        case TOK_SLASH:
            match(TOK_SLASH);
            return OP_DIV;
        default:
            syntax_error("expecting an op", t.lineno, t.col);
            exit(1);
    }
}

static AST *expr(void) {
    AST *ast = NULL;
    AST *left = NULL;
    OpKind op = OP_NOP;
    Token t = tokens->tok;
    switch (t.kind) {
        case TOK_INTEGER:
            left = ast_int(tokens->tok.repr);
            match(TOK_INTEGER);
            if (tokens->tok.kind != TOK_SEMICOLON) {
                op = binop();
                ast = ast_new();
                ast->left = left;
                ast->op = op;
                ast->right = expr();
            } else {
                return left;
            }
            break;

        default:
            syntax_error(
                "expecting an integer\n",
                tokens->tok.lineno,
                tokens->tok.col
            );
            exit(1);
    }
    return ast;
}

static AST *statement(void) {
    AST *ast = NULL;
    ast = expr();
    match(TOK_SEMICOLON);
    return ast;
}

AST *parse(TokList *tks) {
    AST *ast = NULL;

    tokens = tks;
    ast = statement();
    if (tokens) {
        syntax_error("unconsumed tokens", tokens->tok.lineno, tokens->tok.col);
    }
    return ast;
}
