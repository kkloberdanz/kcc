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

void toklist_insert(TokList *before, TokList *new) {
    TokList *next = before->next;
    before->next = new;
    new->next = next;
}

void toklist_free(TokList *head) {
    while (head) {
        TokList *next = head->next;
        free(head);
        head = next;
    }
}

void tok_print(struct Token *tok) {
    printf("{Tok: %s: line: %zu, col: %zu}", tok->repr, tok->lineno, tok->col);
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

Token *tok_new(
    size_t lineno,
    size_t col,
    char *repr,
    TokenKind kind
) {
    Token *tok = malloc(sizeof(*tok));
    if (tok) {
        tok->lineno = lineno;
        tok->col = col;
        tok->repr = strdup(repr);
        tok->kind = kind;
    }
    return tok;
}

void tok_free(Token *tok) {
    if (tok) {
        free(tok->repr);
        free(tok);
    }
}
