#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lex.h"
#include "util.h"

#define TOK_SIZE 1024

/* TODO: instead of just TOK_ID, create function to determine the kind of
 * token before pushing it
 */
#define PUSH_TOKEN() \
    do { \
        if (*current_tok) { \
            tok_init(&tok, lineno, (1 + col) - strlen(current_tok), current_tok, TOK_ID); \
            tok_print(&tok); \
            putchar('\n'); \
            curr = toklist_push(curr, toklist_new(tok)); \
            idx = 0; \
            memset(current_tok, 0, TOK_SIZE); \
\
            if (!head && curr) { \
                head = curr; \
            } \
        } \
    } while (0)

#define UNGET_TOKEN() current_tok[--idx] = '\0'

#define PUSH_CHAR(C) current_tok[idx++] = (C)

#define NEXT_CHAR() current_tok[idx + 1]

static TokList *parse_line(const char *line, const size_t lineno) {
    TokList *curr = NULL;
    TokList *head = NULL;
    size_t col;
    char current_tok[TOK_SIZE] = {0};
    size_t idx = 0;
    Token tok;
    char in_string = 0;

    for (col = 0; line[col] != '\0'; col++) {
        char ch = line[col];
        char next;

        if (in_string) {
            PUSH_CHAR(ch);
            if (ch != '"') {
                continue;
            }
        }

        switch (ch) {
            case '"':
                PUSH_CHAR(ch);
                in_string = !in_string;
                break;

            case ' ':
            case '\r':
            case '\t':
                PUSH_TOKEN();
                break;

            case '\n':
                PUSH_TOKEN();
                goto exit;

            case '=':
                PUSH_TOKEN();
                next = NEXT_CHAR();
                if (next == '=') {
                    PUSH_CHAR(next);
                }
                PUSH_TOKEN();
                break;

            case '+':
                PUSH_TOKEN();
                next = NEXT_CHAR();
                switch (next) {
                    case '+':
                    case '=':
                        PUSH_CHAR(next);
                }
                PUSH_TOKEN();
                break;

            case '*':
                PUSH_TOKEN();
                next = NEXT_CHAR();
                switch (next) {
                    case '=':
                        PUSH_CHAR(next);
                }
                PUSH_TOKEN();
                break;

            case '/':
                PUSH_TOKEN();
                next = NEXT_CHAR();
                switch (next) {
                    case '=':
                        PUSH_CHAR(next);
                }
                PUSH_TOKEN();
                break;

            case '-':
                PUSH_TOKEN();
                next = NEXT_CHAR();
                switch (next) {
                    case '-':
                    case '=':
                    case '>':
                        PUSH_CHAR(next);
                }
                PUSH_TOKEN();
                break;

            case '>':
                PUSH_TOKEN();
                next = NEXT_CHAR();
                switch (next) {
                    case '>':
                        PUSH_CHAR(next);
                }
                PUSH_TOKEN();
                break;

            case '<':
                PUSH_TOKEN();
                next = NEXT_CHAR();
                switch (next) {
                    case '<':
                        PUSH_CHAR(next);
                }
                PUSH_TOKEN();
                break;

            case '(':
            case ')':
            case ';':
            case '\'':
            case '%':
            case '!':
            case '.':
            case '#':
            case '^':
            case '~':
            case ',':
            case ':':
                PUSH_TOKEN();
                PUSH_CHAR(ch);
                PUSH_TOKEN();
                break;

            default:
                PUSH_CHAR(ch);
                break;
        }
    }

exit:
    return head;
}

#undef PUSH_TOKEN
#undef UNGET_TOKEN
#undef PUSH_CHAR
#undef NEXT_CHAR

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
