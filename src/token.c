#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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

const char *kind_repr(TokenKind kind) {
    /*
     * Generate this switch with Python
     * In [6]: for i in l:
   ...:     msg = (
   ...:         f"case {i}:\n"
   ...:         f'    return "{i}";'
   ...:     )
   ...:     print(msg)
     */
    switch (kind) {
        case TOK_STRING:
            return "TOK_STRING";
        case TOK_DOT:
            return "TOK_DOT";
        case TOK_ID:
            return "TOK_ID";
        case TOK_INTEGER:
            return "TOK_INTEGER";
        case TOK_FLOAT:
            return "TOK_FLOAT";
        case TOK_ASSIGN:
            return "TOK_ASSIGN";
        case TOK_EQ:
            return "TOK_EQ";
        case TOK_GT:
            return "TOK_GT";
        case TOK_LT:
            return "TOK_LT";
        case TOK_GE:
            return "TOK_GE";
        case TOK_LE:
            return "TOK_LE";
        case TOK_NE:
            return "TOK_NE";
        case TOK_IF:
            return "TOK_IF";
        case TOK_THEN:
            return "TOK_THEN";
        case TOK_ELSE:
            return "TOK_ELSE";
        case TOK_LPAREN:
            return "TOK_LPAREN";
        case TOK_RPAREN:
            return "TOK_RPAREN";
        case TOK_PLUS:
            return "TOK_PLUS";
        case TOK_MINUS:
            return "TOK_MINUS";
        case TOK_STAR:
            return "TOK_STAR";
        case TOK_OVER:
            return "TOK_OVER";
        case TOK_PERCENT:
            return "TOK_PERCENT";
        case TOK_CARROT:
            return "TOK_CARROT";
        case TOK_LBRACE:
            return "TOK_LBRACE";
        case TOK_RBRACE:
            return "TOK_RBRACE";
        case TOK_LBRACK:
            return "TOK_LBRACK";
        case TOK_RBRACK:
            return "TOK_RBRACK";
        case TOK_SEMICOLON:
            return "TOK_SEMICOLON";
        case TOK_HASH:
            return "TOK_HASH";
        case TOK_COLON:
            return "TOK_COLON";
        case TOK_COMMA:
            return "TOK_COMMA";
        case TOK_QUOTE:
            return "TOK_QUOTE";
        case TOK_MUL_ASSIGN:
            return "TOK_MUL_ASSIGN";
        case TOK_DIV_ASSIGN:
            return "TOK_DIV_ASSIGN";
        case TOK_SLASH:
            return "TOK_SLASH";
        case TOK_NOT_EQ:
            return "TOK_NOT_EQ";
        case TOK_NOT:
            return "TOK_NOT";
        case TOK_PLUS_ASSIGN:
            return "TOK_PLUS_ASSIGN";
        case TOK_INC:
            return "TOK_INC";
        case TOK_MINUS_ASSIGN:
            return "TOK_MINUS_ASSIGN";
        case TOK_DEC:
            return "TOK_DEC";
        case TOK_HAT:
            return "TOK_HAT";
        case TOK_RIGHT_SHIFT:
            return "TOK_RIGHT_SHIFT";
        case TOK_LEFT_SHIFT:
            return "TOK_LEFT_SHIFT";
        case TOK_AMPERSAND:
            return "TOK_AMPERSAND";
    }
    return "(invalid tok)";
}

void tok_print(struct Token *tok) {
    printf(
        "{Tok: '%s': kind: %s, line: %zu, col: %zu}",
        tok->repr,
        kind_repr(tok->kind),
        tok->lineno,
        tok->col
    );
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

TokenKind token_to_kind(const char *tok) {
    /* TODO: pass in line and col for better error messages */
    switch (*tok) {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9': {
            size_t i;
            size_t n_dots = 0;
            TokenKind kind = TOK_INTEGER;
            for (i = 0; tok[i] != '\0'; i++) {
                char c = tok[i];
                if (c == '.') {
                    kind = TOK_FLOAT;
                    n_dots++;
                } else {
                    if (!isdigit(c)) {
                        fprintf(
                            stderr,
                            "syntax error: line: y, col: x invalid identifier: '%s'",
                            tok
                        );
                        exit(1);
                    }
                }
                if (n_dots > 1) {
                    fprintf(
                        stderr,
                        "syntax error: line: y, col: x '%s'",
                        tok
                    );
                    exit(1);
                }
            }
            return kind;
        }

        case '=':
            if (strcmp(tok, "==") == 0) {
                return TOK_EQ;
            }
            return TOK_ASSIGN;

        case '*':
            if (strcmp(tok, "*=") == 0) {
                return TOK_MUL_ASSIGN;
            }
            return TOK_STAR;

        case '/':
            if (strcmp(tok, "/=") == 0) {
                return TOK_DIV_ASSIGN;
            }
            return TOK_SLASH;

        case '!':
            if (strcmp(tok, "!=") == 0) {
                return TOK_NOT_EQ;
            }
            return TOK_NOT;

        case '>':
            if (strcmp(tok, ">>") == 0) {
                return TOK_RIGHT_SHIFT;
            }
            if (strcmp(tok, ">=") == 0) {
                return TOK_GE;
            }
            return TOK_GT;

        case '"':
            return TOK_STRING;

        case '<':
            if (strcmp(tok, "<<") == 0) {
                return TOK_LEFT_SHIFT;
            }
            if (strcmp(tok, "<=") == 0) {
                return TOK_LE;
            }
            return TOK_LT;

        case '+':
            if (strcmp(tok, "+=") == 0) {
                return TOK_PLUS_ASSIGN;
            }
            if (strcmp(tok, "++") == 0) {
                return TOK_INC;
            }
            return TOK_PLUS;

        case '-':
            if (strcmp(tok, "-=") == 0) {
                return TOK_MINUS_ASSIGN;
            }
            if (strcmp(tok, "--") == 0) {
                return TOK_DEC;
            }
            return TOK_MINUS;

        case '&':
            return TOK_AMPERSAND;

        case ';':
            return TOK_SEMICOLON;

        case ':':
            return TOK_COLON;

        case '^':
            return TOK_HAT;

        case '#':
            return TOK_HASH;

        case '.':
            return TOK_DOT;

        case '{':
            return TOK_LBRACE;

        case '}':
            return TOK_RBRACE;

        case '[':
            return TOK_LBRACK;

        case ']':
            return TOK_RBRACK;

        case '(':
            return TOK_LPAREN;

        case ')':
            return TOK_RPAREN;

        default:
            return TOK_ID;
    }
}
