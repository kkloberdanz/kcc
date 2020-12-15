#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lex.h"
#include "util.h"

#define TOK_SIZE 1024

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
            Token tok;
            tok_init(&tok, lineno, col, current_tok, TOK_ID);
            curr = toklist_push(curr, toklist_new(tok));
            if (!head) {
                head = curr;
            }

            idx = 0;
            col++;
            memset(current_tok, 0, TOK_SIZE);
        } else {
            /* unget char in token */
            current_tok[idx--] = '\0';
            if (is_token(current_tok)) {
                Token tok;
                tok_init(&tok, lineno, col, current_tok, TOK_ID);
                curr = toklist_push(curr, toklist_new(tok));
                if (!head) {
                    head = curr;
                }
            }
            idx = 0;
            memset(current_tok, 0, TOK_SIZE);
        }
    }

    return head;
}

TokList *lex(const char *infile) {
    char *line = NULL;
    size_t n = 0;
    size_t lineno = 1;

    FILE *fp = fopen(infile, "r");
    if (!fp) {
        return NULL;
    }

    TokList *curr = NULL;
    TokList *head = NULL;
    while ((getline(&line, &n, fp)) > 0) {
        TokList *ll = parse_line(line, lineno);

        if (ll) {
            curr = toklist_push(curr, ll);
            if (!head) {
                head = curr;
            }
        }
        lineno++;
    }

    fclose(fp);


    return head;
}

#undef TOK_SIZE
