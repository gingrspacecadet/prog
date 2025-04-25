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

void advance(const char* src, int* idx, int* line, int* col) {
    current_token = lookahead_token;
    lookahead_token = next_token(src, idx, line, col);
}

void consume(const char* src, int* idx, int* line, int* col) {
    advance(src, idx, line, col);
}

ASTNode* parse_expression() {
    ASTNode* node = NULL;

    if (current_token.type == TOKEN_IDENTIFIER) {
        node = (ASTNode*)malloc(sizeof(ASTNode));
        node->type = "IDENTIFIER";
        node->value = current_token.value;
        consume(NULL, NULL, NULL, NULL);
    } else if (current_token.type == TOKEN_NUMBER) {
        node = (ASTNode*)malloc(sizeof(ASTNode));
        node->type = "NUMBER";
        node->value = current_token.value;
        consume(NULL, NULL, NULL, NULL);
    } else if (current_token.type == TOKEN_OPERATOR) {
        char* op = current_token.value;
        consume(NULL, NULL, NULL, NULL);
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
        printf("Expected a type for declaration!\n");
        return NULL;
    }
    char* type = current_token.value;
    advance(NULL, NULL, NULL, NULL); // Consume type

    if (current_token.type != TOKEN_IDENTIFIER) {
        printf("Expected an identifier after type!\n");
        return NULL;
    }
    char* name = current_token.value;
    advance(NULL, NULL, NULL, NULL); // Consume identifier

    if (current_token.type == TOKEN_OPERATOR && strcmp(current_token.value, "=") == 0) {
        advance(NULL, NULL, NULL, NULL); // Consume '='
        ASTNode* value = parse_expression();
        if (!value) {
            printf("Expected an expression after '='!\n");
            return NULL;
        }

        if (current_token.type != TOKEN_SYMBOL || strcmp(current_token.value, ";") != 0) {
            printf("Expected ';' at the end of declaration!\n");
            return NULL;
        }
        advance(NULL, NULL, NULL, NULL); // Consume ';'

        ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
        node->type = "DECLARATION";
        node->value = name;
        node->left = value;
        node->right = NULL;
        return node;
    }

    printf("Expected '=' in declaration!\n");
    return NULL;
}

ASTNode* parse_assignment() {
    if (current_token.type != TOKEN_IDENTIFIER) {
        printf("Expected an identifier for assignment!\n");
        return NULL;
    }
    char* name = current_token.value;
    advance(NULL, NULL, NULL, NULL); // Consume identifier

    if (current_token.type != TOKEN_OPERATOR || strcmp(current_token.value, "=") != 0) {
        printf("Expected '=' in assignment!\n");
        return NULL;
    }
    advance(NULL, NULL, NULL, NULL); // Consume '='

    ASTNode* value = parse_expression();
    if (!value) {
        printf("Expected an expression after '='!\n");
        return NULL;
    }

    if (current_token.type != TOKEN_SYMBOL || strcmp(current_token.value, ";") != 0) {
        printf("Expected ';' at the end of assignment!\n");
        return NULL;
    }
    advance(NULL, NULL, NULL, NULL); // Consume ';'

    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = "ASSIGNMENT";
    node->value = name;
    node->left = value;
    node->right = NULL;
    return node;
}

ASTNode* parse_function_declaration(const char* src, int* idx, int* line, int* col) {
    if (current_token.type != TOKEN_TYPE) return NULL;
    char* return_type = current_token.value;
    advance(src, idx, line, col); // eat return type

    if (current_token.type != TOKEN_IDENTIFIER) {
        printf("Expected function name!\n");
        return NULL;
    }
    char* func_name = current_token.value;
    advance(src, idx, line, col); // eat function name

    if (current_token.type != TOKEN_SYMBOL || strcmp(current_token.value, "(") != 0) {
        printf("Expected '(' after function name!\n");
        return NULL;
    }
    advance(src, idx, line, col); // eat '('

    // Parse parameters
    while (current_token.type != TOKEN_SYMBOL || strcmp(current_token.value, ")") != 0) {
        if (current_token.type != TOKEN_TYPE) {
            printf("Expected type in parameter list!\n");
            return NULL;
        }
        char* param_type = current_token.value;
        advance(src, idx, line, col);

        if (current_token.type != TOKEN_IDENTIFIER) {
            printf("Expected name in parameter list!\n");
            return NULL;
        }
        char* param_name = current_token.value;
        advance(src, idx, line, col);

        printf("Parsed parameter: %s %s\n", param_type, param_name);

        if (current_token.type == TOKEN_SYMBOL && strcmp(current_token.value, ",") == 0) {
            advance(src, idx, line, col); // eat ','
        } else if (current_token.type == TOKEN_SYMBOL && strcmp(current_token.value, ")") == 0) {
            break; // done
        } else {
            printf("Unexpected token in parameter list: %s\n", current_token.value);
            return NULL;
        }
    }

    advance(src, idx, line, col); // eat ')'

    if (current_token.type != TOKEN_SYMBOL || strcmp(current_token.value, "{") != 0) {
        printf("Expected '{' to start function body!\n");
        return NULL;
    }
    advance(src, idx, line, col); // eat '{'

    // Skip everything until we hit the closing '}'
    while (current_token.type != TOKEN_SYMBOL || strcmp(current_token.value, "}") != 0) {
        advance(src, idx, line, col);
        if (current_token.type == TOKEN_EOF) {
            printf("Unexpected end of input in function body!\n");
            return NULL;
        }
    }

    advance(src, idx, line, col); // eat '}'

    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = "FUNCTION";
    node->value = func_name;
    node->left = NULL;
    node->right = NULL;
    return node;
}

ASTNode* parse_statement(const char* src, int* idx, int* line, int* col) {
    // Handle Const/Var declarations
    if (current_token.type == TOKEN_KEYWORD &&
        (strcmp(current_token.value, "Const") == 0 ||
         strcmp(current_token.value, "Var") == 0)) {
        advance(src, idx, line, col);
        return parse_declaration();
    }

    // Handle type-based declarations (could be variables or functions)
    if (current_token.type == TOKEN_TYPE && lookahead_token.type == TOKEN_IDENTIFIER) {
        // Lookahead beyond IDENTIFIER
        Token saved_current = current_token;
        Token saved_lookahead = lookahead_token;

        advance(src, idx, line, col); // TYPE → IDENTIFIER
        Token after_identifier = lookahead_token;

        // Rewind
        current_token = saved_current;
        lookahead_token = saved_lookahead;

        if (after_identifier.type == TOKEN_SYMBOL && strcmp(after_identifier.value, "(") == 0) {
            return parse_function_declaration(src, idx, line, col);
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

void parse_program(const char* src, int* idx, int* line, int* col) {
    while (current_token.type != TOKEN_EOF) {
        ASTNode* stmt = parse_statement(src, idx, line, col);
        if (stmt) {
            printf("Parsed a %s statement (value: %s)\n",
                   stmt->type,
                   stmt->value ? stmt->value : "NULL");
            // TODO: free stmt and its children
        } else {
            printf("Skipping unrecognized token: %s\n",
                   current_token.value ? current_token.value : "NULL");
        }
        advance(src, idx, line, col); // Make forward progress
    }
}