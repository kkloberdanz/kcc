#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lex.h"
#include "util.h"

#define TOK_SIZE 1024

#define PUSH_TOKEN() \
    do { \
        if (*current_tok) { \
            TokenKind kind = token_to_kind(current_tok, lineno, col); \
            tok_init(&tok, lineno, (1 + col) - strlen(current_tok), current_tok, kind); \
            curr = toklist_push(curr, toklist_new(tok)); \
            idx = 0; \
            memset(current_tok, 0, TOK_SIZE); \
\
            if (!head && curr) { \
                head = curr; \
            } \
        } \
    } while (0)

#define PUSH_CHAR(C) \
    do { \
        if (idx > 1024) { \
            syntax_error("token too long", lineno, col); \
            exit(1); \
        } \
        current_tok[idx++] = (C); \
    } while (0)


#define NEXT_CHAR() line[col] ? line[++col] : '\0'

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

            case '\'':
                PUSH_TOKEN();
                PUSH_CHAR(ch);
                next = NEXT_CHAR();
                PUSH_CHAR(next);
                if (next == '\\') {
                    next = NEXT_CHAR();
                    PUSH_CHAR(next);
                }
                next = NEXT_CHAR();
                if (next != '\'') {
                    syntax_error(
                        "char literal must only be a single char",
                        lineno,
                        col
                    );
                }
                PUSH_CHAR(next);
                PUSH_TOKEN();
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
                PUSH_CHAR(ch);
                next = NEXT_CHAR();
                if (next == '=') {
                    PUSH_CHAR(next);
                }
                PUSH_TOKEN();
                break;

            case '+':
                PUSH_TOKEN();
                PUSH_CHAR(ch);
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
                PUSH_CHAR(ch);
                next = NEXT_CHAR();
                switch (next) {
                    case '=':
                        PUSH_CHAR(next);
                }
                PUSH_TOKEN();
                break;

            case '/':
                PUSH_TOKEN();
                PUSH_CHAR(ch);
                next = NEXT_CHAR();
                switch (next) {
                    case '=':
                        PUSH_CHAR(next);
                }
                PUSH_TOKEN();
                break;

            case '-':
                PUSH_TOKEN();
                PUSH_CHAR(ch);
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
                PUSH_CHAR(ch);
                next = NEXT_CHAR();
                switch (next) {
                    case '>':
                        PUSH_CHAR(next);
                }
                PUSH_TOKEN();
                break;

            case '<':
                PUSH_TOKEN();
                PUSH_CHAR(ch);
                next = NEXT_CHAR();
                switch (next) {
                    case '<':
                        PUSH_CHAR(next);
                }
                PUSH_TOKEN();
                break;

            case '!':
                PUSH_TOKEN();
                PUSH_CHAR(ch);
                next = NEXT_CHAR();
                switch (next) {
                    case '=':
                        PUSH_CHAR(next);
                }
                PUSH_TOKEN();
                break;

            case '{':
            case '}':
            case '[':
            case ']':
            case '(':
            case ')':
            case ';':
            case '%':
            case '.':
            case '^':
            case '~':
            case ',':
            case ':':
            case '&':
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
#undef PUSH_CHAR
#undef NEXT_CHAR

static FILE *infile = NULL;
static char *line = NULL;
static char src_file[2048] = {0};
size_t lineno = 1;

void lex_set_file(FILE *fp) {
    infile = fp;
}

char *lex_current_line(void) {
    return line;
}

char *lex_current_filename(void) {
    return src_file;
}

TokList *lex(void) {
    size_t n = 0;
    Token tok;
    TokList *token_list = NULL;
    int a, b, c; /* throw away the 3 trailing ints from the cpp output */

    if (!infile) {
        fprintf(
            stderr,
            "%s\n",
            "must first set file using lex_set_file before calling lex"
        );
        exit(3);
    }

    free(line);
    line = NULL;

    while ((getline(&line, &n, infile)) > 0) {
        if (*line == '#') {
            sscanf(line, "# %zu \"%s %d %d %d", &lineno, src_file, &a, &b, &c);
            src_file[strlen(src_file) - 1] = 0;
            continue;
        }
        token_list = parse_line(line, lineno);
        lineno++;
        if (token_list) {
            return token_list;
        }
    }
    tok_init(&tok, lineno, 0, "EOF", TOK_EOF);
    return toklist_new(tok);
}

#undef TOK_SIZE
