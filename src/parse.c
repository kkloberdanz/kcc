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

static AST *expr(void) {
    AST *ast = NULL;
    return ast;
}

static AST *statement_sequence(void) {
    AST *ast = NULL;
    return ast;
}

static AST *term(void) {
    AST *ast = NULL;
    Token t = tokens->tok;
    while ((t.kind == TOK_STAR) || (t.kind == TOK_SLASH)) {
        match(t.kind);
    }
    return ast;
}

static AST *factor(void) {
    char buf[2048];
    AST *ast = NULL;
    switch (tokens->tok.kind) {
        case TOK_FLOAT:
        case TOK_INTEGER:
            break;

        case TOK_ID:
            break;

        case TOK_LPAREN:
            match(TOK_LPAREN);
            ast = expr();
            match(TOK_RPAREN);
            break;

        default:
            sprintf(buf, "unexpected token: '%s'", tokens->tok.repr);
            syntax_error(buf, tokens->tok.lineno, tokens->tok.col);
            exit(1);
    }
    return ast;
}

AST *parse(TokList *tks) {
    AST *ast = NULL;
    return ast;

    tokens = tks;
    ast = statement_sequence();
    if (tokens) {
        syntax_error("unconsumed tokens", tokens->tok.lineno, tokens->tok.col);
    }
    UNUSED(factor);
    UNUSED(term);
    return ast;
}
