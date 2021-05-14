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
#define YYSTYPE AST *

static int yylex(void);
static void yyerror(const char *msg);
static void display_error(const char *msg);
static void debug_display(const char *msg);
static void fatal_error(const char *msg);

static TokList *tokens = NULL;
static TokList *current_token = NULL;
static AST *tree = NULL;
AST *parse(FILE *infile);
char *strdup(const char *s);

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
        {
            debug_display("IDENTIFIER");
        }
    | CONSTANT
        {
            debug_display("CONSTANT");
            const char *repr = strdup(current_token->tok.repr);
            switch (current_token->tok.kind) {
                case TOK_INT_LIT:
                    $$ = ast_int(repr);
                    break;
                case TOK_FLOAT_LIT:
                    $$ = ast_float(repr);
                    break;
                case TOK_CHAR_LIT:
                    $$ = ast_char(repr);
                    break;
                default:
                    fatal_error("invalid token for constant");
            }
        }
    | STRING_LITERAL
        {
            const char *repr = strdup(current_token->tok.repr);
            $$ = ast_string(repr);
        }
    | '(' expression ')'
        {
            debug_display("'(' expression ')'");
        }
    ;

postfix_expression
    : primary_expression
        {
            $$ = $1;
        }
    | postfix_expression '[' expression ']'
        {
            debug_display("postfix_expression '[' expression ']'");
        }
    | postfix_expression '(' ')'
        {
            debug_display("postfix_expression '(' ')'");
        }
    | postfix_expression '(' argument_expression_list ')'
        {
            debug_display("postfix_expression '(' argument_expression_list ')'");
        }
    | postfix_expression '.' IDENTIFIER
        {
            debug_display("postfix_expression '.' IDENTIFIER");
        }
    | postfix_expression PTR_OP IDENTIFIER
        {
            debug_display("postfix_expression PTR_OP IDENTIFIER");
        }
    | postfix_expression INC_OP
        {
            debug_display("postfix_expression INC_OP");
        }
    | postfix_expression DEC_OP
        {
            debug_display("postfix_expression DEC_OP");
        }
    ;

argument_expression_list
    : assignment_expression
        {
            debug_display("assignment_expression");
        }
    | argument_expression_list ',' assignment_expression
        {
            debug_display("argument_expression_list ',' assignment_expression");
        }
    ;

unary_expression
    : postfix_expression
        {
            $$ = $1;
        }
    | INC_OP unary_expression
        {
            debug_display("INC_OP unary_expression");
        }
    | DEC_OP unary_expression
        {
            debug_display("DEC_OP unary_expression");
        }
    | unary_operator cast_expression
        {
            debug_display("unary_operator cast_expression");
        }
    | SIZEOF unary_expression
        {
            debug_display("SIZEOF unary_expression");
        }
    | SIZEOF '(' type_name ')'
        {
            debug_display("SIZEOF '(' type_name ')'");
        }
    ;

unary_operator
    : '&'
        {
            debug_display("unary_operator: '&'");
        }
    | '*'
        {
            debug_display("unary_operator: '*'");
        }
    | '+'
        {
            debug_display("unary_operator: '+'");
        }
    | '-'
        {
            debug_display("unary_operator: '-'");
        }
    | '~'
        {
            debug_display("unary_operator: '~'");
        }
    | '!'
        {
            debug_display("unary_operator: '!'");
        }
    ;

cast_expression
    : unary_expression
        {
            $$ = $1;
        }
    | '(' type_name ')' cast_expression
        {
            debug_display("'(' type_name ')' cast_expression");
        }
    ;

multiplicative_expression
    : cast_expression
    | multiplicative_expression '*' cast_expression
        {
            $$ = ast_op($1, OP_MUL, $3);
        }
    | multiplicative_expression '/' cast_expression
        {
            $$ = ast_op($1, OP_DIV, $3);
        }
    | multiplicative_expression '%' cast_expression
        {
            $$ = ast_op($1, OP_MOD, $3);
        }
    ;

additive_expression
    : multiplicative_expression
        {
            $$ = $1;
        }
    | additive_expression '+' multiplicative_expression
        {
            $$ = ast_op($1, OP_ADD, $3);
        }
    | additive_expression '-' multiplicative_expression
        {
            $$ = ast_op($1, OP_SUB, $3);
        }
    ;

shift_expression
    : additive_expression
        {
            $$ = $1;
        }
    | shift_expression LEFT_OP additive_expression
        {
            debug_display("shift_expression LEFT_OP additive_expression");
        }
    | shift_expression RIGHT_OP additive_expression
        {
            debug_display("shift_expression RIGHT_OP additive_expression");
        }
    ;

relational_expression
    : shift_expression
        {
            debug_display("shift_expression");
        }
    | relational_expression '<' shift_expression
        {
            debug_display("relational_expression '<' shift_expression");
        }
    | relational_expression '>' shift_expression
        {
            debug_display("relational_expression '>' shift_expression");
        }
    | relational_expression LE_OP shift_expression
        {
            debug_display("relational_expression LE_OP shift_expression");
        }
    | relational_expression GE_OP shift_expression
        {
            debug_display("relational_expression GE_OP shift_expression");
        }
    ;

equality_expression
    : relational_expression
        {
            debug_display("relational_expression");
        }
    | equality_expression EQ_OP relational_expression
        {
            debug_display("equality_expression EQ_OP relational_expression");
        }
    | equality_expression NE_OP relational_expression
        {
            debug_display("equality_expression NE_OP relational_expression");
        }
    ;

and_expression
    : equality_expression
        {
            debug_display("equality_expression");
        }
    | and_expression '&' equality_expression
        {
            debug_display("and_expression '&' equality_expression");
        }
    ;

exclusive_or_expression
    : and_expression
        {
            debug_display("and_expression");
        }
    | exclusive_or_expression '^' and_expression
        {
            debug_display("exclusive_or_expression '^' and_expression");
        }
    ;

inclusive_or_expression
    : exclusive_or_expression
        {
            debug_display("exclusive_or_expression");
        }
    | inclusive_or_expression '|' exclusive_or_expression
        {
            debug_display("inclusive_or_expression '|' exclusive_or_expression");
        }
    ;

logical_and_expression
    : inclusive_or_expression
        {
            debug_display("inclusive_or_expression");
        }
    | logical_and_expression AND_OP inclusive_or_expression
        {
            debug_display("logical_and_expression AND_OP inclusive_or_expression");
        }
    ;

logical_or_expression
    : logical_and_expression
        {
            debug_display("logical_and_expression");
        }
    | logical_or_expression OR_OP logical_and_expression
        {
            debug_display("logical_or_expression OR_OP logical_and_expression");
        }
    ;

conditional_expression
    : logical_or_expression
        {
            debug_display("logical_or_expression");
        }
    | logical_or_expression '?' expression ':' conditional_expression
        {
            debug_display("logical_or_expression '?' expression ':' conditional_expression");
        }
    ;

assignment_expression
    : conditional_expression
        {
            debug_display("conditional_expression");
        }
    | unary_expression assignment_operator assignment_expression
        {
            debug_display("unary_expression assignment_operator assignment_expression");
        }
    ;

assignment_operator
    : '='
        {
            debug_display("'='");
        }
    | MUL_ASSIGN
        {
            debug_display("MUL_ASSIGN");
        }
    | DIV_ASSIGN
        {
            debug_display("DIV_ASSIGN");
        }
    | MOD_ASSIGN
        {
            debug_display("MOD_ASSIGN");
        }
    | ADD_ASSIGN
        {
            debug_display("ADD_ASSIGN");
        }
    | SUB_ASSIGN
        {
            debug_display("SUB_ASSIGN");
        }
    | LEFT_ASSIGN
        {
            debug_display("LEFT_ASSIGN");
        }
    | RIGHT_ASSIGN
        {
            debug_display("RIGHT_ASSIGN");
        }
    | AND_ASSIGN
        {
            debug_display("AND_ASSIGN");
        }
    | XOR_ASSIGN
        {
            debug_display("XOR_ASSIGN");
        }
    | OR_ASSIGN
        {
            debug_display("OR_ASSIGN");
        }
    ;

expression
    : assignment_expression
        {
            debug_display("assignment_expression");
        }
    | expression ',' assignment_expression
        {
            debug_display("expression ',' assignment_expression");
        }
    ;

constant_expression
    : conditional_expression
        {
            debug_display("conditional_expression");
        }
    ;

declaration
    : declaration_specifiers ';'
        {
            debug_display("declaration_specifiers ';'");
        }
    | declaration_specifiers init_declarator_list ';'
        {
            debug_display("declaration_specifiers init_declarator_list ';'");
        }
    ;

/* TODO: add the type specifier to the types table */
declaration_specifiers
    : storage_class_specifier
        {
            debug_display("storage_class_specifier");
        }

    | storage_class_specifier declaration_specifiers
        {
            debug_display("storage_class_specifier declaration_specifiers");
        }

    | type_specifier
        {
            $$ = $1;
            debug_display("declaration_specifiers::type_specifier");
        }

    | type_specifier declaration_specifiers
        {
            debug_display("type_specifier declaration_specifiers");
        }

    | type_qualifier
        {
            debug_display("type_qualifier");
        }

    | type_qualifier declaration_specifiers
        {
            debug_display("type_qualifier declaration_specifiers");
        }
    ;

init_declarator_list
    : init_declarator
        {
            debug_display("init_declarator");
        }
    | init_declarator_list ',' init_declarator
        {
            debug_display("init_declarator_list ',' init_declarator");
        }
    ;

init_declarator
    : declarator
        {
            $$ = $1;
        }
    | declarator '=' initializer
        {
            debug_display("declarator '=' initializer");
        }
    ;

storage_class_specifier
    : TYPEDEF
        {
            debug_display("TYPEDEF");
        }
    | EXTERN
        {
            debug_display("EXTERN");
        }
    | STATIC
        {
            debug_display("STATIC");
        }
    | AUTO
        {
            debug_display("AUTO");
        }
    | REGISTER
        {
            debug_display("REGISTER");
        }
    ;

type_specifier
    : VOID
        {
            $$ = ast_type_specifier(TYPE_VOID);
        }
    | CHAR
        {
            $$ = ast_type_specifier(TYPE_CHAR);
        }
    | SHORT
        {
            $$ = ast_type_specifier(TYPE_SHORT);
        }
    | INT
        {
            $$ = ast_type_specifier(TYPE_INT);
        }
    | LONG
        {
            $$ = ast_type_specifier(TYPE_LONG);
        }
    | FLOAT
        {
            $$ = ast_type_specifier(TYPE_FLOAT);
        }
    | DOUBLE
        {
            $$ = ast_type_specifier(TYPE_DOUBLE);
        }
    | BUILTIN_VA_LIST
        {
            $$ = ast_type_specifier(TYPE_BUILTIN_VA_LIST);
        }
    | SIGNED
        {
            $$ = ast_type_specifier(TYPE_SIGNED);
        }
    | UNSIGNED
        {
            $$ = ast_type_specifier(TYPE_UNSIGNED);
        }
    | struct_or_union_specifier
        {
            debug_display("struct_or_union_specifier");
        }
    | enum_specifier
        {
            debug_display("enum_specifier");
        }
    | TYPE_NAME
        {
            debug_display("TYPE_NAME");
        }
    ;

struct_or_union_specifier
    : struct_or_union IDENTIFIER '{' struct_declaration_list '}'
        {
            debug_display("struct_or_union IDENTIFIER '{' struct_declaration_list '}'");
        }
    | struct_or_union '{' struct_declaration_list '}'
        {
            debug_display("struct_or_union '{' struct_declaration_list '}'");
        }
    | struct_or_union IDENTIFIER
        {
            debug_display("struct_or_union IDENTIFIER");
        }
    ;

struct_or_union
    : STRUCT
        {
            debug_display("STRUCT");
        }
    | UNION
        {
            debug_display("UNION");
        }
    ;

struct_declaration_list
    : struct_declaration
        {
            debug_display("struct_declaration");
        }
    | struct_declaration_list struct_declaration
        {
            debug_display("struct_declaration_list struct_declaration");
        }
    ;

struct_declaration
    : specifier_qualifier_list struct_declarator_list ';'
        {
            debug_display("specifier_qualifier_list struct_declarator_list ';'");
        }
    ;

specifier_qualifier_list
    : type_specifier specifier_qualifier_list
        {
            debug_display("type_specifier specifier_qualifier_list");
        }
    | type_specifier
        {
            debug_display("specifier_qualifier_list::type_specifier");
        }
    | type_qualifier specifier_qualifier_list
        {
            debug_display("type_qualifier specifier_qualifier_list");
        }
    | type_qualifier
        {
            debug_display("type_qualifier");
        }
    ;

struct_declarator_list
    : struct_declarator
        {
            debug_display("");
        }
    | struct_declarator_list ',' struct_declarator
        {
            debug_display("");
        }
    ;

struct_declarator
    : declarator
        {
            debug_display("declarator");
        }
    | ':' constant_expression
        {
            debug_display("':' constant_expression");
        }
    | declarator ':' constant_expression
        {
            debug_display("declarator ':' constant_expression");
        }
    ;

enum_specifier
    : ENUM '{' enumerator_list '}'
        {
            debug_display("ENUM '{' enumerator_list '}'");
        }
    | ENUM IDENTIFIER '{' enumerator_list '}'
        {
            debug_display("ENUM IDENTIFIER '{' enumerator_list '}'");
        }
    | ENUM IDENTIFIER
        {
            debug_display("ENUM IDENTIFIER");
        }
    ;

enumerator_list
    : enumerator
        {
            debug_display("enumerator");
        }
    | enumerator_list ',' enumerator
        {
            debug_display("enumerator_list ',' enumerator");
        }
    ;

enumerator
    : IDENTIFIER
        {
            debug_display("IDENTIFIER");
        }
    | IDENTIFIER '=' constant_expression
        {
            debug_display("IDENTIFIER '=' constant_expression");
        }
    ;

type_qualifier
    : CONST
        {
            debug_display("CONST");
        }
    | VOLATILE
        {
            debug_display("VOLATILE");
        }
    ;

declarator
    : pointer direct_declarator
        {
            debug_display("pointer direct_declarator");
        }
    | direct_declarator
        {
            debug_display("direct_declarator");
        }
    ;

direct_declarator
    : IDENTIFIER
        {
            debug_display("IDENTIFIER");
        }
    | '(' declarator ')'
        {
            debug_display("'(' declarator ')'");
        }
    | direct_declarator '[' constant_expression ']'
        {
            debug_display("direct_declarator '[' constant_expression ']'");
        }
    | direct_declarator '[' ']'
        {
            debug_display("direct_declarator '[' ']'");
        }
    | direct_declarator '(' parameter_type_list ')'
        {
            debug_display("direct_declarator '(' parameter_type_list ')'");
        }
    | direct_declarator '(' identifier_list ')'
        {
            debug_display("direct_declarator '(' identifier_list ')'");
        }
    | direct_declarator '(' ')'
        {
            debug_display("direct_declarator '(' ')'");
        }
    ;

pointer
    : '*'
        {
            debug_display("'*'");
        }
    | '*' type_qualifier_list
        {
            debug_display("'*' type_qualifier_list");
        }
    | '*' pointer
        {
            debug_display("'*' pointer");
        }
    | '*' type_qualifier_list pointer
        {
            debug_display("'*' type_qualifier_list pointer");
        }
    ;

type_qualifier_list
    : type_qualifier
        {
            debug_display("type_qualifier");
        }
    | type_qualifier_list type_qualifier
        {
            debug_display("type_qualifier_list type_qualifier");
        }
    ;


parameter_type_list
    : parameter_list
        {
            debug_display("parameter_list");
        }
    | parameter_list ',' ELLIPSIS
        {
            debug_display("parameter_list ',' ELLIPSIS");
        }
    ;

parameter_list
    : parameter_declaration
        {
            debug_display("parameter_declaration");
        }
    | parameter_list ',' parameter_declaration
        {
            debug_display("parameter_list ',' parameter_declaration");
        }
    ;

parameter_declaration
    : declaration_specifiers declarator
        {
            debug_display("declaration_specifiers declarator");
        }
    | declaration_specifiers abstract_declarator
        {
            debug_display("declaration_specifiers abstract_declarator");
        }
    | declaration_specifiers
        {
            debug_display("declaration_specifiers");
        }
    ;

identifier_list
    : IDENTIFIER
        {
            debug_display("IDENTIFIER");
        }
    | identifier_list ',' IDENTIFIER
        {
            debug_display("identifier_list ',' IDENTIFIER");
        }
    ;

type_name
    : specifier_qualifier_list
        {
            debug_display("specifier_qualifier_list");
        }
    | specifier_qualifier_list abstract_declarator
        {
            debug_display("specifier_qualifier_list abstract_declarator");
        }
    ;

abstract_declarator
    : pointer
        {
            debug_display("pointer");
        }
    | direct_abstract_declarator
        {
            debug_display("direct_abstract_declarator");
        }
    | pointer direct_abstract_declarator
        {
            debug_display("pointer direct_abstract_declarator");
        }
    ;

direct_abstract_declarator
    : '(' abstract_declarator ')'
        {
            debug_display("'(' abstract_declarator ')'");
        }
    | '[' ']'
        {
            debug_display("'[' ']'");
        }
    | '[' constant_expression ']'
        {
            debug_display("'[' constant_expression ']'");
        }
    | direct_abstract_declarator '[' ']'
        {
            debug_display("direct_abstract_declarator '[' ']'");
        }
    | direct_abstract_declarator '[' constant_expression ']'
        {
            debug_display("direct_abstract_declarator '[' constant_expression ']'");
        }
    | '(' ')'
        {
            debug_display("'(' ')'");
        }
    | '(' parameter_type_list ')'
        {
            debug_display("'(' parameter_type_list ')'");
        }
    | direct_abstract_declarator '(' ')'
        {
            debug_display("direct_abstract_declarator '(' ')'");
        }
    | direct_abstract_declarator '(' parameter_type_list ')'
        {
            debug_display("direct_abstract_declarator '(' parameter_type_list ')'");
        }
    ;

initializer
    : assignment_expression
        {
            debug_display("assignment_expression");
        }
    | '{' initializer_list '}'
        {
            debug_display("'{' initializer_list '}'");
        }
    | '{' initializer_list ',' '}'
        {
            debug_display("'{' initializer_list ',' '}'");
        }
    ;

initializer_list
    : initializer
        {
            debug_display("initializer");
        }
    | initializer_list ',' initializer
        {
            debug_display("initializer_list ',' initializer");
        }
    ;

statement
    : labeled_statement
        {
            debug_display("labeled_statement");
        }
    | compound_statement
        {
            debug_display("compound_statement");
        }
    | expression_statement
        {
            debug_display("expression_statement");
        }
    | selection_statement
        {
            debug_display("selection_statement");
        }
    | iteration_statement
        {
            debug_display("iteration_statement");
        }
    | jump_statement
        {
            debug_display("jump_statement");
        }
    ;

labeled_statement
    : IDENTIFIER ':' statement
        {
            debug_display("IDENTIFIER ':' statement");
        }
    | CASE constant_expression ':' statement
        {
            debug_display("CASE constant_expression ':' statement");
        }
    | DEFAULT ':' statement
        {
            debug_display("DEFAULT ':' statement");
        }
    ;

compound_statement
    : '{' '}'
        {
            debug_display("'{' '}'");
        }
    | '{' statement_list '}'
        {
            debug_display("'{' statement_list '}'");
        }
    | '{' declaration_list '}'
        {
            debug_display("'{' declaration_list '}'");
        }
    | '{' declaration_list statement_list '}'
        {
            debug_display("'{' declaration_list statement_list '}'");
        }
    ;

declaration_list
    : declaration
        {
            debug_display("declaration_list::declaration");
        }
    | declaration_list declaration
        {
            debug_display("declaration_list declaration");
        }
    ;

statement_list
    : statement
        {
            debug_display("statement");
        }
    | statement_list statement
        {
            debug_display("statement_list statement");
        }
    ;

expression_statement
    : ';'
        {
            debug_display("expression_statement::';'");
        }
    | expression ';'
        {
            debug_display("expression ';'");
        }
    ;

selection_statement
    : IF '(' expression ')' statement
        {
            debug_display("IF '(' expression ')' statement");
        }
    | IF '(' expression ')' statement ELSE statement
        {
            debug_display("IF '(' expression ')' statement ELSE statement");
        }
    | SWITCH '(' expression ')' statement
        {
            debug_display("SWITCH '(' expression ')' statement");
        }
    ;

iteration_statement
    : WHILE '(' expression ')' statement
        {
            debug_display("WHILE '(' expression ')' statement");
        }
    | DO statement WHILE '(' expression ')' ';'
        {
            debug_display("DO statement WHILE '(' expression ')' ';'");
        }
    | FOR '(' expression_statement expression_statement ')' statement
        {
            debug_display("FOR '(' expression_statement expression_statement ')' statement");
        }
    | FOR '(' expression_statement expression_statement expression ')' statement
        {
            debug_display("FOR '(' expression_statement expression_statement expression ')' statement");
        }
    ;

jump_statement
    : GOTO IDENTIFIER ';'
        {
            debug_display("GOTO IDENTIFIER ';'");
        }
    | CONTINUE ';'
        {
            debug_display("CONTINUE ';'");
        }
    | BREAK ';'
        {
            debug_display("BREAK ';'");
        }
    | RETURN ';'
        {
            debug_display("RETURN ';'");
        }
    | RETURN expression ';'
        {
            debug_display("RETURN expression ';'");
        }
    ;

translation_unit
    : external_declaration
        {
            debug_display("external_declaration");
        }
    | translation_unit external_declaration
        {
            tree = $1;
            debug_display("translation_unit external_declaration");
        }
    ;

external_declaration
    : function_definition
        {
            debug_display("function_definition");
        }
    | declaration
        {
            debug_display("external_declaration::declaration");
        }
    ;

function_definition
    : declaration_specifiers declarator declaration_list compound_statement
        {
            debug_display("declaration_specifiers declarator declaration_list compound_statement");
        }
    | declaration_specifiers declarator compound_statement
        {
            debug_display("declaration_specifiers declarator compound_statement");
        }
    | declarator declaration_list compound_statement
        {
            debug_display("declarator declaration_list compound_statement");
        }
    | declarator compound_statement
        {
            debug_display("declarator compound_statement");
        }
    ;

%%

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

static void debug_display(const char *msg) {
#ifdef DEBUG
    display_error(msg);
#else
    (void)(msg);
    puts("HERE");
#endif
}

static void fatal_error(const char *msg) {
    display_error(msg);
    exit(EXIT_FAILURE);
}

static void display_error(const char *msg) {
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
        fprintf(stderr, "^");
    }
    fprintf(stderr, "\033[0m\n");
}

static void yyerror(const char *msg) {
    fatal_error(msg);
}

AST *parse(FILE *infile) {
    tree = ast_new();
    lex_set_file(infile);
    if (!hcreate(10000)) {
        fprintf(stderr, "failed to initialize htable\n");
        exit(4);
    }
    yyparse();
    hdestroy();
    return tree;
}
