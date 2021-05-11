#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "token.h"
#include "util.h"

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
        tok_free(&head->tok);
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
        case TOK_ARROW:
            return "TOK_ARROW";
        case TOK_STRING:
            return "TOK_STRING";
        case TOK_CHAR_LIT:
            return "TOK_CHAR_LIT";
        case TOK_DOT:
            return "TOK_DOT";
        case TOK_EOF:
            return "TOK_EOF";
        case TOK_ID:
            return "TOK_ID";
        case TOK_INT:
            return "TOK_INT";
        case TOK_INT_LIT:
            return "TOK_INT_LIT";
        case TOK_FLOAT_LIT:
            return "TOK_FLOAT_LIT";
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
        case TOK_QUESTION:
            return "TOK_QUESTION";
        case TOK_BREAK:
            return "TOK_BREAK";
        case TOK_CHAR:
            return "TOK_CHAR";
        case TOK_CONST:
            return "TOK_CONST";
        case TOK_CONTINUE:
            return "TOK_CONTINUE";
        case TOK_DEFAULT:
            return "TOK_DEFAULT";
        case TOK_DO:
            return "TOK_DO";
        case TOK_DOUBLE:
            return "TOK_DOUBLE";
        case TOK_BUILTIN_VA_LIST:
            return "TOK_BUILTIN_VA_LIST";
        case TOK_ENUM:
            return "TOK_ENUM";
        case TOK_EXTERN:
            return "TOK_EXTERN";
        case TOK_AUTO:
            return "TOK_AUTO";
        case TOK_CASE:
            return "TOK_CASE";
        case TOK_RETURN:
            return "TOK_RETURN";
        case TOK_SIZEOF:
            return "TOK_SIZEOF";
        case TOK_SIGNED:
            return "TOK_SIGNED";
        case TOK_GOTO:
            return "TOK_GOTO";
        case TOK_SHORT:
            return "TOK_SHORT";
        case TOK_FOR:
            return "TOK_FOR";
        case TOK_LONG:
            return "TOK_LONG";
        case TOK_REGISTER:
            return "TOK_REGISTER";
        case TOK_STRUCT:
            return "TOK_STRUCT";
        case TOK_SWITCH:
            return "TOK_SWITCH";
        case TOK_TYPEDEF:
            return "TOK_TYPEDEF";
        case TOK_UNSIGNED:
            return "TOK_UNSIGNED";
        case TOK_VOID:
            return "TOK_VOID";
        case TOK_VOLATILE:
            return "TOK_VOLATILE";
        case TOK_WHILE:
            return "TOK_WHILE";
        case TOK_STATIC:
            return "TOK_STATIC";
    }
    return "(invalid tok)";
}

void tok_print(struct Token *tok) {
    printf(
        "{tok: '%s', kind: %s, line: %zu, col: %zu}",
        tok->repr,
        kind_repr(tok->kind),
        tok->lineno,
        tok->col
    );
}

void toklist_print(TokList *head) {
    printf("[\n");
    if (head) {
        while (head->next) {
            printf("\t");
            tok_print(&head->tok);
            printf(",\n");
            head = head->next;
        }
        printf("\t");
        tok_print(&head->tok);
    }
    puts("\n]");
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

TokenKind token_to_kind(const char *tok, size_t lineno, size_t col) {
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
            TokenKind kind = TOK_INT_LIT;
            for (i = 0; tok[i] != '\0'; i++) {
                char c = tok[i];
                if (c == '.') {
                    kind = TOK_FLOAT_LIT;
                    n_dots++;
                } else {
                    if (!isdigit(c)) {
                        syntax_error(tok, lineno, col);
                        exit(1);
                    }
                }
                if (n_dots > 1) {
                    syntax_error(tok, lineno, col);
                    exit(1);
                }
            }
            return kind;
        }

        case '=':
            switch (tok[1]) {
                case '=':
                    return TOK_EQ;
                case '\0':
                    return TOK_ASSIGN;
                default:
                    syntax_error(tok, lineno, col);
                    exit(1);
            }

        case '*':
            switch (tok[1]) {
                case '=':
                    return TOK_MUL_ASSIGN;
                case '\0':
                    return TOK_STAR;
                default:
                    syntax_error(tok, lineno, col);
                    exit(1);
            }

        case '/':
            switch (tok[1]) {
                case '=':
                    return TOK_DIV_ASSIGN;
                case '\0':
                    return TOK_SLASH;
                default:
                    syntax_error(tok, lineno, col);
                    exit(1);
            }

        case '!':
            switch (tok[1]) {
                case '=':
                    return TOK_NOT_EQ;
                case '\0':
                    return TOK_NOT;
                default:
                    syntax_error(tok, lineno, col);
                    exit(1);
            }

        case '>':
            switch (tok[1]) {
                case '>':
                    return TOK_RIGHT_SHIFT;
                case '=':
                    return TOK_GE;
                case '\0':
                    return TOK_GT;
                default:
                    syntax_error(tok, lineno, col);
                    exit(1);
            }

        case '"':
            return TOK_STRING;

        case '\'':
            return TOK_CHAR_LIT;

        case '<':
            switch (tok[1]) {
                case '<':
                    return TOK_LEFT_SHIFT;
                case '=':
                    return TOK_LE;
                case '\0':
                    return TOK_LT;
                default:
                    syntax_error(tok, lineno, col);
                    exit(1);
            }

        case '+':
            switch (tok[1]) {
                case '=':
                    return TOK_PLUS_ASSIGN;
                case '+':
                    return TOK_INC;
                case '\0':
                    return TOK_PLUS;
                default:
                    syntax_error(tok, lineno, col);
                    exit(1);
            }

        case '-':
            switch (tok[1]) {
                case '=':
                    return TOK_MINUS_ASSIGN;
                case '-':
                    return TOK_DEC;
                case '>':
                    return TOK_ARROW;
                case '\0':
                    return TOK_MINUS;
                default:
                    syntax_error(tok, lineno, col);
                    exit(1);
            }

        case '&':
            return TOK_AMPERSAND;

        case '?':
            return TOK_QUESTION;

        case ';':
            return TOK_SEMICOLON;

        case ':':
            return TOK_COLON;

        case '^':
            return TOK_HAT;

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

        case ',':
            return TOK_COMMA;

        default:
            if (strcmp(tok, "auto") == 0) {
                return TOK_AUTO;
            }
            if (strcmp(tok, "break") == 0) {
                return TOK_BREAK;
            }
            if (strcmp(tok, "case") == 0) {
                return TOK_CASE;
            }
            if (strcmp(tok, "char") == 0) {
                return TOK_CHAR;
            }
            if (strcmp(tok, "const") == 0) {
                return TOK_CONST;
            }
            if (strcmp(tok, "continue") == 0) {
                return TOK_CONTINUE;
            }
            if (strcmp(tok, "default") == 0) {
                return TOK_DEFAULT;
            }
            if (strcmp(tok, "do") == 0) {
                return TOK_DO;
            }
            if (strcmp(tok, "double") == 0) {
                return TOK_DOUBLE;
            }
            if (strcmp(tok, "__builtin_va_list") == 0) {
                return TOK_BUILTIN_VA_LIST;
            }
            if (strcmp(tok, "else") == 0) {
                return TOK_ELSE;
            }
            if (strcmp(tok, "enum") == 0) {
                return TOK_ENUM;
            }
            if (strcmp(tok, "extern") == 0) {
                return TOK_EXTERN;
            }
            if (strcmp(tok, "float") == 0) {
                return TOK_FLOAT;
            }
            if (strcmp(tok, "for") == 0) {
                return TOK_FOR;
            }
            if (strcmp(tok, "goto") == 0) {
                return TOK_GOTO;
            }
            if (strcmp(tok, "if") == 0) {
                return TOK_IF;
            }
            if (strcmp(tok, "long") == 0) {
                return TOK_LONG;
            }
            if (strcmp(tok, "register") == 0) {
                return TOK_REGISTER;
            }
            if (strcmp(tok, "float") == 0) {
                return TOK_FLOAT;
            }
            if (strcmp(tok, "int") == 0) {
                return TOK_INT;
            }
            if (strcmp(tok, "return") == 0) {
                return TOK_RETURN;
            }
            if (strcmp(tok, "short") == 0) {
                return TOK_SHORT;
            }
            if (strcmp(tok, "signed") == 0) {
                return TOK_SIGNED;
            }
            if (strcmp(tok, "sizeof") == 0) {
                return TOK_SIZEOF;
            }
            if (strcmp(tok, "static") == 0) {
                return TOK_STATIC;
            }
            if (strcmp(tok, "struct") == 0) {
                return TOK_STRUCT;
            }
            if (strcmp(tok, "switch") == 0) {
                return TOK_SWITCH;
            }
            if (strcmp(tok, "typedef") == 0) {
                return TOK_TYPEDEF;
            }
            if (strcmp(tok, "union") == 0) {
                return TOK_TYPEDEF;
            }
            if (strcmp(tok, "unsigned") == 0) {
                return TOK_UNSIGNED;
            }
            if (strcmp(tok, "void") == 0) {
                return TOK_VOID;
            }
            if (strcmp(tok, "volatile") == 0) {
                return TOK_VOLATILE;
            }
            if (strcmp(tok, "while") == 0) {
                return TOK_WHILE;
            }
            return TOK_ID;
    }
}
