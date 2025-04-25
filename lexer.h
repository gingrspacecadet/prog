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

// Export the lexer functions
Token next_token(const char* src, int* idx, int* line, int* col);
void  tokenize(const char* src);

// For debug printing in lexer.c
extern const char* token_type_names[];

#endif // LEXER_H