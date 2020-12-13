#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lex.h"
#include "util.h"

#define TOK_SIZE 1024

static void push_token(
    TokList **head,
    TokList **curr,
    const char *current_tok,
    size_t lineno,
    size_t col
) {
    printf("tok: %s\n", current_tok);
    Token tok;
    tok_init(&tok, lineno, col, current_tok, TOK_ID);
    if (*curr) {
        TokList *next = toklist_new(tok);
        *curr = toklist_insert(*curr, next);
    } else {
        *curr = toklist_new(tok);
        *head = *curr;
    }
}

static char is_token(const char *tok) {
    UNUSED(tok);
    return 1;
}

static TokList *parse_line(const char *line, const size_t lineno) {
    TokList *curr = NULL;
    TokList *head = NULL;
    size_t col;
    char current_tok[TOK_SIZE] = {0};
    size_t idx = 0;

    for (col = 1; (line[col] != '\n') && (line[col] != '\0'); col++) {
        char lookahead = line[col];
        current_tok[idx++] = line[col - 1];
        current_tok[idx++] = lookahead;

        /* try token with lookahead */
        if (is_token(current_tok)) {
            /* push token */
            push_token(
                &head,
                &curr,
                current_tok,
                lineno,
                col
            );
            idx = 0;
            col++;
            memset(current_tok, 0, TOK_SIZE);
        } else {
            /* unget char in token */
            current_tok[idx--] = '\0';
            if (is_token(current_tok)) {
                push_token(
                    &head,
                    &curr,
                    current_tok,
                    lineno,
                    col
                );
            }
            idx = 0;
            memset(current_tok, 0, TOK_SIZE);
        }
    }
    return head;
}

TokList *lex(
    const char *infile,
    const char *outfile,
    char assembly_output,
    char dont_link
) {
    char *line = NULL;
    size_t n = 0;
    size_t lineno = 1;

    FILE *fp = fopen(infile, "r");
    if (!fp) {
        return NULL;
    }

    while ((getline(&line, &n, fp)) > 0) {
        TokList *tokens = parse_line(line, lineno);
        lineno++;

        toklist_print(tokens);

        UNUSED(tokens);
    }

    UNUSED(infile);
    UNUSED(outfile);
    UNUSED(assembly_output);
    UNUSED(dont_link);

    return NULL;
}

#undef TOK_SIZE
