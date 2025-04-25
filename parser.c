#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define global variables
Token current_token;
Token lookahead_token;

ASTNode* parse_expression();
ASTNode* parse_declaration();
ASTNode* parse_assignment();
ASTNode* parse_statement();

void consume() {
    advance();
}

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

ASTNode* parse_function_declaration() {
    if (current_token.type != TOKEN_TYPE) return NULL;
    char* return_type = current_token.value;
    advance(); // eat return type

    if (current_token.type != TOKEN_IDENTIFIER) {
        printf("Expected function name!\n");
        return NULL;
    }
    char* func_name = current_token.value;
    advance(); // eat function name

    if (current_token.type != TOKEN_SYMBOL || strcmp(current_token.value, "(") != 0) {
        printf("Expected '(' after function name!\n");
        return NULL;
    }
    advance(); // eat '('

    // TODO: parse parameters here (for now, just skip until ')')
    if (current_token.type != TOKEN_SYMBOL || strcmp(current_token.value, ")") != 0) {
        printf("Expected ')' after function parameters!\n");
        return NULL;
    }
    advance(); // eat ')'

    if (current_token.type != TOKEN_SYMBOL || strcmp(current_token.value, "{") != 0) {
        printf("Expected '{' to start function body!\n");
        return NULL;
    }
    advance(); // eat '{'

    // Skip everything until we hit the closing '}'
    while (current_token.type != TOKEN_SYMBOL || strcmp(current_token.value, "}") != 0) {
        advance();
        if (current_token.type == TOKEN_EOF) {
            printf("Unexpected end of input in function body!\n");
            return NULL;
        }
    }

    advance(); // eat '}'

    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = "FUNCTION";
    node->value = func_name;
    node->left = NULL;
    node->right = NULL;
    return node;
}

ASTNode* parse_statement() {
    // Handle Const/Var declarations
    if (current_token.type == TOKEN_KEYWORD &&
        (strcmp(current_token.value, "Const") == 0 ||
         strcmp(current_token.value, "Var") == 0)) {
        advance();
        return parse_declaration();
    }

    // Handle type-based declarations (could be variables or functions)
    if (current_token.type == TOKEN_TYPE && lookahead_token.type == TOKEN_IDENTIFIER) {
        // Lookahead beyond IDENTIFIER
        Token saved_current = current_token;
        Token saved_lookahead = lookahead_token;
    
        advance(); // TYPE → IDENTIFIER
        Token after_identifier = lookahead_token;
    
        // Rewind
        current_token = saved_current;
        lookahead_token = saved_lookahead;
    
        if (after_identifier.type == TOKEN_SYMBOL && strcmp(after_identifier.value, "(") == 0) {
            return parse_function_declaration();
        } else {
            return parse_declaration();
        }
    }    

    // Assignments
    if (current_token.type == TOKEN_IDENTIFIER) {
        return parse_assignment();
    }

    return NULL;
}

ASTNode* parse_declaration() {
    // current_token.type == TOKEN_TYPE
    char* typ = current_token.value;
    advance();  // eat the type

    if (current_token.type != TOKEN_IDENTIFIER) {
        printf("Expected identifier after type!\n");
        return NULL;
    }
    char* name = current_token.value;
    advance(); // eat the identifier

    if (current_token.type != TOKEN_OPERATOR ||
        strcmp(current_token.value, "=") != 0) {
        printf("Expected '=' after identifier!\n");
        return NULL;
    }
    advance(); // eat '='

    ASTNode* expr = parse_expression();
    if (!expr) {
        printf("Expected expression after '='!\n");
        return NULL;
    }

    if (current_token.type != TOKEN_SYMBOL ||
        strcmp(current_token.value, ";") != 0) {
        printf("Expected ';' at end of declaration!\n");
        return NULL;
    }
    // leave current_token on ';', advance happens in parse_program

    ASTNode* node = malloc(sizeof(ASTNode));
    node->type  = "DECLARATION";
    node->value = name;
    node->left  = expr;
    node->right = NULL;
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

void parse_program() {
    while (current_token.type != TOKEN_EOF) {
        ASTNode* stmt = parse_statement();
        if (stmt) {
            printf("Parsed a %s statement (value: %s)\n",
                   stmt->type,
                   stmt->value ? stmt->value : "NULL");
            // TODO: free stmt and its children
        } else {
            printf("Skipping unrecognized token: %s\n",
                   current_token.value ? current_token.value : "NULL");
        }
        // In either case, advance to the next token so we make forward progress:
        advance();
    }
}