#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"   // bring in Token, TokenType

// The parser will use these globals:
extern Token current_token;
extern Token lookahead_token;

// ASTNode structure definition
typedef struct Param {
    char* type;
    char* name;
    struct Param* next;
} Param;

typedef struct ASTNode {
    char* type;
    char* value;
    struct ASTNode* left;
    struct ASTNode* right;

    // Optional:
    Param* params;  // used if this is a function node
} ASTNode;

// Called by main to advance tokens:
void advance(const char* src, int* idx, int* line, int* col);

// Parse the entire program:
void parse_program(const char* src, int* idx, int* line, int* col);

#endif // PARSER_H