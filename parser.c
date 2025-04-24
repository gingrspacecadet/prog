#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    TOKEN_KEYWORD,
    TOKEN_TYPE,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_NUMBER,
    TOKEN_SYMBOL,
    TOKEN_STRING,
    TOKEN_EOF
} TokenType;

typedef struct Token {
    TokenType type;
    char* value;
} Token;

typedef struct ASTNode {
    char* type;
    char* value;
    struct ASTNode* left;
    struct ASTNode* right;
} ASTNode;

Token current_token;
Token next_token;

void consume() {
    current_token = next_token;
    // Advance the lexer here and set the next token
}

ASTNode* parse_expression();
ASTNode* parse_declaration();
ASTNode* parse_assignment();
ASTNode* parse_statement();

ASTNode* parse_expression() {
    ASTNode* node = NULL;

    if (current_token.type == TOKEN_IDENTIFIER) {
        node = (ASTNode*)malloc(sizeof(ASTNode));
        node->type = "IDENTIFIER";
        node->value = current_token.value;
        consume();
    } else if (current_token.type == TOKEN_NUMBER) {
        node = (ASTNode*)malloc(sizeof(ASTNode));
        node->type = "NUMBER";
        node->value = current_token.value;
        consume();
    } else if (current_token.type == TOKEN_OPERATOR) {
        // Handle binary operator expressions
        char* op = current_token.value;
        consume();
        ASTNode* left = parse_expression();
        ASTNode* right = parse_expression();
        node = (ASTNode*)malloc(sizeof(ASTNode));
        node->type = "BINARY_OP";
        node->value = op;
        node->left = left;
        node->right = right;
    }

    return node;
}

ASTNode* parse_declaration() {
    if (current_token.type != TOKEN_TYPE) {
        printf("Expected type for declaration!\n");
        return NULL;
    }
    char* type = current_token.value;
    consume();

    if (current_token.type != TOKEN_IDENTIFIER) {
        printf("Expected identifier for declaration!\n");
        return NULL;
    }
    char* identifier = current_token.value;
    consume();

    if (current_token.type != TOKEN_OPERATOR || strcmp(current_token.value, "=") != 0) {
        printf("Expected '=' for assignment!\n");
        return NULL;
    }
    consume();

    ASTNode* expr = parse_expression();
    if (current_token.type != TOKEN_SYMBOL || strcmp(current_token.value, ";") != 0) {
        printf("Expected semicolon at the end of declaration!\n");
        return NULL;
    }
    consume();

    // Create AST for the declaration
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = "DECLARATION";
    node->value = identifier;
    node->left = expr;
    return node;
}

ASTNode* parse_assignment() {
    if (current_token.type != TOKEN_IDENTIFIER) {
        printf("Expected identifier for assignment!\n");
        return NULL;
    }
    char* identifier = current_token.value;
    consume();

    if (current_token.type != TOKEN_OPERATOR || strcmp(current_token.value, "=") != 0) {
        printf("Expected '=' for assignment!\n");
        return NULL;
    }
    consume();

    ASTNode* expr = parse_expression();
    if (current_token.type != TOKEN_SYMBOL || strcmp(current_token.value, ";") != 0) {
        printf("Expected semicolon at the end of assignment!\n");
        return NULL;
    }
    consume();

    // Create AST for the assignment
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = "ASSIGNMENT";
    node->value = identifier;
    node->left = expr;
    return node;
}

ASTNode* parse_statement() {
    if (current_token.type == TOKEN_KEYWORD) {
        if (strcmp(current_token.value, "int") == 0 || strcmp(current_token.value, "float") == 0) {
            return parse_declaration();
        } else if (strcmp(current_token.value, "if") == 0) {
            // Handle "if" statement (not implemented here)
        }
    } else if (current_token.type == TOKEN_IDENTIFIER) {
        return parse_assignment();
    }

    return NULL;
}

void parse_program() {
    while (current_token.type != TOKEN_EOF) {
        ASTNode* stmt = parse_statement();
        // Process the statement or add it to the AST
    }
}

int main() {
    // Initialize the lexer and parser
    // Parse the program and output the AST
    return 0;
}