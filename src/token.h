#ifndef TOKEN_H
#define TOKEN_H

#include <stddef.h>

/*

"="           { return ASSIGN; }                                                
"=="          { return EQ; }                                                    
">"           { return GT; }                                                    
"<"           { return LT; }                                                    
">="          { return GE; }                                                    
"<="          { return LE; }                                                    
"!="          { return NE; }                                                    
"->"          { return ARROW; }                                             
"if"          { return IF; }                                                    
"then"        { return THEN; }                                                  
"else"        { return ELSE; }                                                  
"("           { return LPAREN; }                                                
")"           { return RPAREN; }                                                
"+"           { return PLUS; }                                                  
"-"           { return MINUS; }                                                 
"*"           { return TIMES; }                                                 
"/"           { return OVER; }                                                  
"%"           { return PERCENT; }                                               
"^"           { return CARROT; }                                                
"{"           { return LBRACE; }                                                
"}"           { return RBRACE; }                                                
"["           { return LBRACK; }                                                
"]"           { return RBRACK; }                                                
";"           { return SEMICOLON; }                                             
":"           { return COLON; }                                                 
","           { return COMMA; }                                                 
"\""          { return QUOTE; } 

*/

typedef enum TokenKind {
    TOK_EOF,
    TOK_ID,
    TOK_INTEGER,
    TOK_FLOAT,
    TOK_ASSIGN,
    TOK_EQ,
    TOK_GT,
    TOK_LT,
    TOK_GE,
    TOK_LE,
    TOK_NE,
    TOK_IF,
    TOK_THEN,
    TOK_ELSE,
    TOK_LPAREN,
    TOK_RPAREN,
    TOK_PLUS,
    TOK_MINUS,
    TOK_STAR,
    TOK_OVER,
    TOK_PERCENT,
    TOK_CARROT,
    TOK_LBRACE,
    TOK_RBRACE,
    TOK_LBRACK,
    TOK_RBRACK,
    TOK_SEMICOLON,
    TOK_COLON,
    TOK_COMMA,
    TOK_QUOTE,
    TOK_MUL_ASSIGN,
    TOK_DIV_ASSIGN,
    TOK_SLASH,
    TOK_NOT_EQ,
    TOK_NOT,
    TOK_PLUS_ASSIGN,
    TOK_INC,
    TOK_MINUS_ASSIGN,
    TOK_DEC,
    TOK_HAT,
    TOK_RIGHT_SHIFT,
    TOK_LEFT_SHIFT,
    TOK_DOT,
    TOK_STRING,
    TOK_BREAK,
    TOK_CHAR,
    TOK_CONST,
    TOK_CONTINUE,
    TOK_DEFAULT,
    TOK_DO,
    TOK_DOUBLE,
    TOK_ENUM,
    TOK_EXTERN,
    TOK_AUTO,
    TOK_CASE,
    TOK_REGISTER,
    TOK_RETURN,
    TOK_SIZEOF,
    TOK_SIGNED,
    TOK_GOTO,
    TOK_SHORT,
    TOK_FOR,
    TOK_LONG,
    TOK_STATIC,
    TOK_STRUCT,
    TOK_SWITCH,
    TOK_TYPEDEF,
    TOK_UNSIGNED,
    TOK_VOID,
    TOK_VOLATILE,
    TOK_WHILE,
    TOK_AMPERSAND
} TokenKind;

typedef struct Token {
    size_t lineno;
    size_t col;
    char *repr;
    TokenKind kind;
} Token;

typedef struct TokList {
    Token tok;
    struct TokList *next;
} TokList;

TokList *toklist_new(Token tok);

TokList *toklist_insert(TokList *before, TokList *new);

TokList *toklist_push(TokList *before, TokList *new);

void toklist_print(TokList *head);

void toklist_free(TokList *head);

void tok_print(struct Token *tok);

void toklist_print(TokList *head);

void tok_init(
    Token *tok,
    size_t lineno,
    size_t col,
    const char *repr,
    TokenKind kind
);

void tok_free(Token *tok);

TokenKind token_to_kind(const char *tok, size_t lineno, size_t col);

const char *kind_repr(TokenKind kind);

#endif /* TOKEN_H */
