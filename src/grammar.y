/*
 * ANSI C Yacc grammar
 * In 1985, Jeff Lee published his Yacc grammar (which is accompanied by a
 * matching Lex specification) for the April 30, 1985 draft version of the
 * ANSI C standard.  Tom Stockfisch reposted it to net.sources in 1987; that
 * original, as mentioned in the answer to question 17.25 of the comp.lang.c
 * FAQ, can be ftp'ed from
 * ftp.uu.net, file usenet/net.sources/ansi.c.grammar.Z.
 *
 * Jutta Degener, 1995
 */

%{
#define YYPARSER

#include <stdio.h>
#include <stdlib.h>
#include <search.h>
#include <string.h>

#include "src/lex.h"
#include "src/ast.h"

#define YYERROR_VERBOSE

static int yylex(void);
static void yyerror(const char *msg);

static TokList *tokens = NULL;
static TokList *current_token = NULL;
static AST *tree = NULL;
AST *parse(FILE *infile);
char *strdup(const char *s);
static void add_type(void);

%}

%define parse.error verbose
%define parse.lac full

%token IDENTIFIER CONSTANT STRING_LITERAL SIZEOF
%token PTR_OP INC_OP DEC_OP LEFT_OP RIGHT_OP LE_OP GE_OP EQ_OP NE_OP
%token AND_OP OR_OP MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN ADD_ASSIGN
%token SUB_ASSIGN LEFT_ASSIGN RIGHT_ASSIGN AND_ASSIGN
%token XOR_ASSIGN OR_ASSIGN TYPE_NAME

%token TYPEDEF EXTERN STATIC AUTO REGISTER
%token CHAR SHORT INT LONG SIGNED UNSIGNED FLOAT DOUBLE CONST VOLATILE VOID
%token STRUCT UNION ENUM ELLIPSIS

%token CASE DEFAULT IF ELSE SWITCH WHILE DO FOR GOTO CONTINUE BREAK RETURN

%token BUILTIN_VA_LIST

%start translation_unit
%%

primary_expression
    : IDENTIFIER
    | CONSTANT
    | STRING_LITERAL
    | '(' expression ')'
    ;

postfix_expression
    : primary_expression
    | postfix_expression '[' expression ']'
    | postfix_expression '(' ')'
    | postfix_expression '(' argument_expression_list ')'
    | postfix_expression '.' IDENTIFIER
    | postfix_expression PTR_OP IDENTIFIER
    | postfix_expression INC_OP
    | postfix_expression DEC_OP
    ;

argument_expression_list
    : assignment_expression
    | argument_expression_list ',' assignment_expression
    ;

unary_expression
    : postfix_expression
    | INC_OP unary_expression
    | DEC_OP unary_expression
    | unary_operator cast_expression
    | SIZEOF unary_expression
    | SIZEOF '(' type_name ')'
    ;

unary_operator
    : '&'
    | '*'
    | '+'
    | '-'
    | '~'
    | '!'
    ;

cast_expression
    : unary_expression
    | '(' type_name ')' cast_expression
    ;

multiplicative_expression
    : cast_expression
    | multiplicative_expression '*' cast_expression
    | multiplicative_expression '/' cast_expression
    | multiplicative_expression '%' cast_expression
    ;

additive_expression
    : multiplicative_expression
    | additive_expression '+' multiplicative_expression
    | additive_expression '-' multiplicative_expression
    ;

shift_expression
    : additive_expression
    | shift_expression LEFT_OP additive_expression
    | shift_expression RIGHT_OP additive_expression
    ;

relational_expression
    : shift_expression
    | relational_expression '<' shift_expression
    | relational_expression '>' shift_expression
    | relational_expression LE_OP shift_expression
    | relational_expression GE_OP shift_expression
    ;

equality_expression
    : relational_expression
    | equality_expression EQ_OP relational_expression
    | equality_expression NE_OP relational_expression
    ;

and_expression
    : equality_expression
    | and_expression '&' equality_expression
    ;

exclusive_or_expression
    : and_expression
    | exclusive_or_expression '^' and_expression
    ;

inclusive_or_expression
    : exclusive_or_expression
    | inclusive_or_expression '|' exclusive_or_expression
    ;

logical_and_expression
    : inclusive_or_expression
    | logical_and_expression AND_OP inclusive_or_expression
    ;

logical_or_expression
    : logical_and_expression
    | logical_or_expression OR_OP logical_and_expression
    ;

conditional_expression
    : logical_or_expression
    | logical_or_expression '?' expression ':' conditional_expression
    ;

assignment_expression
    : conditional_expression
    | unary_expression assignment_operator assignment_expression
    ;

assignment_operator
    : '='
    | MUL_ASSIGN
    | DIV_ASSIGN
    | MOD_ASSIGN
    | ADD_ASSIGN
    | SUB_ASSIGN
    | LEFT_ASSIGN
    | RIGHT_ASSIGN
    | AND_ASSIGN
    | XOR_ASSIGN
    | OR_ASSIGN
    ;

expression
    : assignment_expression
    | expression ',' assignment_expression
    ;

constant_expression
    : conditional_expression
    ;

declaration
    : declaration_specifiers ';'
    | declaration_specifiers init_declarator_list ';'
    ;

/* TODO: add the type specifier to the types table */
declaration_specifiers
    : storage_class_specifier
        {
            fprintf(stderr, "storage: %s\n", current_token->tok.repr);
            add_type();
        }

    | storage_class_specifier declaration_specifiers
        {
            fprintf(stderr, "storage decl: %s\n", current_token->tok.repr);
            add_type();
        }

    | type_specifier
        {
            fprintf(stderr, "type spec: %s\n", current_token->tok.repr);
        }

    | type_specifier declaration_specifiers
        {
            fprintf(stderr, "type spec delc: %s\n", current_token->tok.repr);
            add_type();
        }

    | type_qualifier
        {
            fprintf(stderr, "type qual: %s\n", current_token->tok.repr);
            add_type();
        }

    | type_qualifier declaration_specifiers
        {
            fprintf(stderr, "type qual decl: %s\n", current_token->tok.repr);
            add_type();
        }
    ;

init_declarator_list
    : init_declarator
    | init_declarator_list ',' init_declarator
    ;

init_declarator
    : declarator
    | declarator '=' initializer
    ;

storage_class_specifier
    : TYPEDEF
    | EXTERN
    | STATIC
    | AUTO
    | REGISTER
    ;

type_specifier
    : VOID
    | CHAR
    | SHORT
    | INT
    | LONG
    | FLOAT
    | DOUBLE
    | BUILTIN_VA_LIST
    | SIGNED
    | UNSIGNED
    | struct_or_union_specifier
    | enum_specifier
    | TYPE_NAME
    ;

struct_or_union_specifier
    : struct_or_union IDENTIFIER '{' struct_declaration_list '}'
    | struct_or_union '{' struct_declaration_list '}'
    | struct_or_union IDENTIFIER
    ;

struct_or_union
    : STRUCT
    | UNION
    ;

struct_declaration_list
    : struct_declaration
    | struct_declaration_list struct_declaration
    ;

struct_declaration
    : specifier_qualifier_list struct_declarator_list ';'
    ;

specifier_qualifier_list
    : type_specifier specifier_qualifier_list
    | type_specifier
    | type_qualifier specifier_qualifier_list
    | type_qualifier
    ;

struct_declarator_list
    : struct_declarator
    | struct_declarator_list ',' struct_declarator
    ;

struct_declarator
    : declarator
    | ':' constant_expression
    | declarator ':' constant_expression
    ;

enum_specifier
    : ENUM '{' enumerator_list '}'
    | ENUM IDENTIFIER '{' enumerator_list '}'
    | ENUM IDENTIFIER
    ;

enumerator_list
    : enumerator
    | enumerator_list ',' enumerator
    ;

enumerator
    : IDENTIFIER
    | IDENTIFIER '=' constant_expression
    ;

type_qualifier
    : CONST
    | VOLATILE
    ;

declarator
    : pointer direct_declarator
    | direct_declarator
    ;

direct_declarator
    : IDENTIFIER
    | '(' declarator ')'
    | direct_declarator '[' constant_expression ']'
    | direct_declarator '[' ']'
    | direct_declarator '(' parameter_type_list ')'
    | direct_declarator '(' identifier_list ')'
    | direct_declarator '(' ')'
    ;

pointer
    : '*'
    | '*' type_qualifier_list
    | '*' pointer
    | '*' type_qualifier_list pointer
    ;

type_qualifier_list
    : type_qualifier
    | type_qualifier_list type_qualifier
    ;


parameter_type_list
    : parameter_list
    | parameter_list ',' ELLIPSIS
    ;

parameter_list
    : parameter_declaration
    | parameter_list ',' parameter_declaration
    ;

parameter_declaration
    : declaration_specifiers declarator
    | declaration_specifiers abstract_declarator
    | declaration_specifiers
    ;

identifier_list
    : IDENTIFIER
    | identifier_list ',' IDENTIFIER
    ;

type_name
    : specifier_qualifier_list
    | specifier_qualifier_list abstract_declarator
    ;

abstract_declarator
    : pointer
    | direct_abstract_declarator
    | pointer direct_abstract_declarator
    ;

direct_abstract_declarator
    : '(' abstract_declarator ')'
    | '[' ']'
    | '[' constant_expression ']'
    | direct_abstract_declarator '[' ']'
    | direct_abstract_declarator '[' constant_expression ']'
    | '(' ')'
    | '(' parameter_type_list ')'
    | direct_abstract_declarator '(' ')'
    | direct_abstract_declarator '(' parameter_type_list ')'
    ;

initializer
    : assignment_expression
    | '{' initializer_list '}'
    | '{' initializer_list ',' '}'
    ;

initializer_list
    : initializer
    | initializer_list ',' initializer
    ;

statement
    : labeled_statement
    | compound_statement
    | expression_statement
    | selection_statement
    | iteration_statement
    | jump_statement
    ;

labeled_statement
    : IDENTIFIER ':' statement
    | CASE constant_expression ':' statement
    | DEFAULT ':' statement
    ;

compound_statement
    : '{' '}'
    | '{' statement_list '}'
    | '{' declaration_list '}'
    | '{' declaration_list statement_list '}'
    ;

declaration_list
    : declaration
    | declaration_list declaration
    ;

statement_list
    : statement
    | statement_list statement
    ;

expression_statement
    : ';'
    | expression ';'
    ;

selection_statement
    : IF '(' expression ')' statement
    | IF '(' expression ')' statement ELSE statement
    | SWITCH '(' expression ')' statement
    ;

iteration_statement
    : WHILE '(' expression ')' statement
    | DO statement WHILE '(' expression ')' ';'
    | FOR '(' expression_statement expression_statement ')' statement
    | FOR '(' expression_statement expression_statement expression ')' statement
    ;

jump_statement
    : GOTO IDENTIFIER ';'
    | CONTINUE ';'
    | BREAK ';'
    | RETURN ';'
    | RETURN expression ';'
    ;

translation_unit
    : external_declaration
    | translation_unit external_declaration
    ;

external_declaration
    : function_definition
    | declaration
    ;

function_definition
    : declaration_specifiers declarator declaration_list compound_statement
    | declaration_specifiers declarator compound_statement
    | declarator declaration_list compound_statement
    | declarator compound_statement
    ;

%%

static void add_type(void) {
    ENTRY e;
    char type_alias[2048] = {0};
    char buf[2048] = {0};
    TokList *curr = NULL;
    char start_recording = 0;

    e.key = current_token->tok.repr;
    if (!hsearch(e, FIND)) {
        for (curr = tokens; curr != NULL; curr = curr->next) {
            if (curr->tok.kind == TOK_TYPEDEF) {
                start_recording = 1;
                continue;
            } else if (curr->tok.kind == TOK_ID) {
                break;
            }

            if (start_recording) {
                sprintf(type_alias, "%s %s", buf, curr->tok.repr);
                strcpy(buf, type_alias);
            }
        }
        e.key = strdup(current_token->tok.repr);
        e.data = strdup(type_alias);
        hsearch(e, ENTER);
        fprintf(
            stderr,
            "%sadding type: %s -> %s\n",
            lex_current_line(),
            e.key,
            (char *)e.data
        );
    }
}

static int translate_tok(TokenKind t) {
    ENTRY *search = NULL;
    ENTRY entry;

    entry.key = strdup(current_token->tok.repr);
    search = hsearch(entry, FIND);
    if (search) {
        fprintf(stderr, "found: '%s'\n", search->key);
        return TYPE_NAME;
    }

    switch (t) {
        case TOK_EOF:
            return 0;

        case TOK_ARROW:
            return PTR_OP;

        case TOK_ID:
            return IDENTIFIER;

        case TOK_INT:
            return INT;

        case TOK_FLOAT:
            return FLOAT;

        case TOK_DOUBLE:
            return DOUBLE;

        case TOK_INT_LIT:
        case TOK_FLOAT_LIT:
        case TOK_STRING:
        case TOK_CHAR_LIT:
            return CONSTANT;

        case TOK_QUESTION:
            return '?';

        case TOK_ASSIGN:
            return '=';

        case TOK_EQ:
            return EQ_OP;

        case TOK_GT:
            return '>';

        case TOK_LT:
            return '<';

        case TOK_GE:
            return GE_OP;

        case TOK_LE:
            return LE_OP;

        case TOK_NE:
            return '!';

        case TOK_IF:
            return IF;

        case TOK_THEN:
            exit(43);

        case TOK_ELSE:
            return ELSE;

        case TOK_LPAREN:
            return '(';

        case TOK_RPAREN:
            return ')';

        case TOK_PLUS:
            return '+';

        case TOK_MINUS:
            return '-';

        case TOK_STAR:
            return '*';

        case TOK_OVER:
            return '/';

        case TOK_PERCENT:
            return '%';

        case TOK_LBRACE:
            return '{';

        case TOK_RBRACE:
            return '}';

        case TOK_LBRACK:
            return '[';

        case TOK_RBRACK:
            return ']';

        case TOK_SEMICOLON:
            return ';';

        case TOK_COLON:
            return ':';

        case TOK_COMMA:
            return ',';

        case TOK_QUOTE:
            return '"';

        case TOK_MUL_ASSIGN:
            return MUL_ASSIGN;

        case TOK_DIV_ASSIGN:
            return DIV_ASSIGN;

        case TOK_SLASH:
            return '/';

        case TOK_NOT_EQ:
            return NE_OP;

        case TOK_NOT:
            return '!';

        case TOK_PLUS_ASSIGN:
            return ADD_ASSIGN;

        case TOK_INC:
            return INC_OP;

        case TOK_MINUS_ASSIGN:
            return SUB_ASSIGN;

        case TOK_DEC:
            return DEC_OP;

        case TOK_HAT:
            return '^';

        case TOK_RIGHT_SHIFT:
            return RIGHT_OP;

        case TOK_LEFT_SHIFT:
            return LEFT_OP;

        case TOK_DOT:
            return '.';

        case TOK_BREAK:
            return BREAK;

        case TOK_CHAR:
            return CHAR;

        case TOK_CONST:
            return CONST;

        case TOK_CONTINUE:
            return CONTINUE;

        case TOK_DEFAULT:
            return DEFAULT;

        case TOK_DO:
            return DO;

        case TOK_ENUM:
            return ENUM;

        case TOK_EXTERN:
            return EXTERN;

        case TOK_AUTO:
            return AUTO;

        case TOK_CASE:
            return CASE;

        case TOK_REGISTER:
            return REGISTER;

        case TOK_RETURN:
            return RETURN;

        case TOK_SIZEOF:
            return SIZEOF;

        case TOK_SIGNED:
            return SIGNED;

        case TOK_GOTO:
            return GOTO;

        case TOK_SHORT:
            return SHORT;

        case TOK_FOR:
            return FOR;

        case TOK_LONG:
            return LONG;

        case TOK_STATIC:
            return STATIC;

        case TOK_STRUCT:
            return STRUCT;

        case TOK_UNION:
            return UNION;

        case TOK_SWITCH:
            return SWITCH;

        case TOK_TYPEDEF:
            return TYPEDEF;

        case TOK_UNSIGNED:
            return UNSIGNED;

        case TOK_BUILTIN_VA_LIST:
            return BUILTIN_VA_LIST;

        case TOK_VOID:
            return VOID;

        case TOK_VOLATILE:
            return VOLATILE;

        case TOK_WHILE:
            return WHILE;

        case TOK_AMPERSAND:
            return '&';
    }
    return -1;
}

static int yylex(void) {
    int tok;

    if (!tokens) {
        tokens = lex();
        current_token = tokens;
    } else if (current_token->next) {
        current_token = current_token->next;
    } else {
        toklist_free(tokens);
        tokens = lex();
        current_token = tokens;
    }

    tok = translate_tok(current_token->tok.kind);
    if (tok < 0) {
        fprintf(
            stderr,
            "(%4lu, %3lu) invalid token: '%s'\n",
            current_token->tok.lineno,
            current_token->tok.col,
            current_token->tok.repr
        );
        exit(2);
    }
    return tok;
}

static void yyerror(const char *msg) {
    size_t i = 0;

    fflush(stdout);
    fprintf(
        stderr,
        "\n\033[35m%s\033[0m: "
        "(line: \033[92m%lu\033[0m col: \033[92m%lu\033[0m)\n%s: '%s'\n"
        "> %s",
        lex_current_filename(),
        current_token->tok.lineno,
        current_token->tok.col,
        msg,
        current_token->tok.repr,
        lex_current_line()
    );

    for (i = 0; i < current_token->tok.col + 1; i++) {
        fprintf(stderr, " ");
    }

    fprintf(stderr, "\033[91m");
    for (i = 0; current_token->tok.repr[i]; i++) {
        fprintf(stderr, "~");
    }
    fprintf(stderr, "\033[0m\n");
}

AST *parse(FILE *infile) {
    lex_set_file(infile);
    if (!hcreate(10000)) {
        fprintf(stderr, "failed to initialize htable\n");
        exit(4);
    }
    yyparse();
    hdestroy();
    return tree;
}
