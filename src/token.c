#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "token.h"

TokList *toklist_new(Token tok) {
    TokList *ll = malloc(sizeof(*ll));
    if (ll) {
        ll->next = NULL;
        ll->tok = tok;
    }
    return ll;
}

TokList *toklist_insert(TokList *before, TokList *new) {
    if (before) {
        TokList *next = before->next;
        before->next = new;
        new->next = next;
    }
    return new;
}

TokList *toklist_push(TokList *before, TokList *new) {
    if (before) {
        while (before->next) {
            before = before->next;
        }
        before->next = new;
    }
    return new;
}

void toklist_free(TokList *head) {
    while (head) {
        TokList *next = head->next;
        free(head);
        head = next;
    }
}

void tok_print(struct Token *tok) {
    printf("{Tok: '%s': line: %zu, col: %zu}", tok->repr, tok->lineno, tok->col);
}

void toklist_print(TokList *head) {
    printf("[");
    if (head) {
        while (head->next) {
            tok_print(&head->tok);
            printf(", ");
            head = head->next;
        }
        tok_print(&head->tok);
    }
    puts("]");
}

void tok_init(
    Token *out,
    size_t lineno,
    size_t col,
    const char *repr,
    TokenKind kind
) {
    out->lineno = lineno;
    out->col = col;
    out->repr = strdup(repr);
    out->kind = kind;
}

void tok_free(Token *tok) {
    if (tok) {
        free(tok->repr);
    }
}
