// lexer.h
#ifndef LEXER_H
#define LEXER_H

typedef enum {
    TOKEN_KEYWORD,
    TOKEN_TYPE,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_NUMBER,
    TOKEN_STRING,
    TOKEN_SYMBOL,
    TOKEN_EOF,
    TOKEN_INVALID
} TokenType;

typedef struct {
    TokenType type;
    char*     value;
    int       line;
    int       col;
} Token;

// The lexer functions:
Token next_token(const char* src, int* idx, int* line, int* col);
void  tokenize(const char* src);

#endif // LEXER_H