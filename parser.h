#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"   // bring in Token, TokenType

// The parser will use these globals:
extern Token current_token;
extern Token lookahead_token;

// ASTNode structure definition
typedef struct ASTNode {
    char* type;
    char* value;
    struct ASTNode* left;
    struct ASTNode* right;
} ASTNode;

// Called by main to advance tokens:
void advance(void);

// Parse the entire program:
void parse_program(void);

#endif // PARSER_H