#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Keywords and types
const char* KEYWORDS[] = {"Const", "Var", "if", "else", "for", "while", "return", "void", "true", "false"};
const char* TYPES[] = {"Float", "Bool"};
const char* OPERATORS[] = {"==", "!=", "<=", ">=", "&&", "||", "+", "-", "*", "/", "=", "<", ">", "!"};
const char* SYMBOLS = "(){},;";

// Helper function to check if a string is a keyword
int is_keyword(const char* str) {
    for (int i = 0; i < sizeof(KEYWORDS) / sizeof(KEYWORDS[0]); i++) {
        if (strcmp(str, KEYWORDS[i]) == 0) return 1;
    }
    return 0;
}

// Helper function to check if a string is a type
int is_type(const char* str) {
    for (int i = 0; i < sizeof(TYPES) / sizeof(TYPES[0]); i++) {
        if (strcmp(str, TYPES[i]) == 0) return 1;
    }
    if (strncmp(str, "Int", 3) == 0 || strncmp(str, "Vec", 3) == 0) return 1;
    return 0;
}

// Substring helper function
char* substr(const char* src, int start, int end) {
    int len = end - start;
    char* out = malloc(len + 1);
    strncpy(out, src + start, len);
    out[len] = '\0';
    return out;
}

// Skip whitespace characters and update column positions
void skip_whitespace(const char* src, int* i, int* line, int* col) {
    while (isspace(src[*i])) {
        if (src[*i] == '\n') {
            (*line)++;
            *col = 1; // Reset column at the start of a new line
        } else {
            (*col)++; // Increment column for each space or tab
        }
        (*i)++;
    }
}

// Handle comments (skip single-line comments)
void skip_comments(const char* src, int* i, int* line, int* col) {
    if (src[*i] == '/' && src[*i + 1] == '/') {
        (*i) += 2;  // Skip "//"
        while (src[*i] != '\n' && src[*i] != '\0') {
            (*i)++;
        }
        (*line)++;
        *col = 1;
    }
}

// Main function to return the next token and update column
Token next_token(const char* src, int* i, int* line, int* col) {
    // skip spaces/newlines up front (updates *line and *col)
    skip_whitespace(src, i, line, col);
    skip_comments(src, i, line, col);
    skip_whitespace(src, i, line, col);

    int start   = *i;
    int startCol = *col;
    char c      = src[*i];

    Token tok;
    tok.line = *line;
    tok.col  = startCol;
    tok.value = NULL;

    if (c == '\0') {
        tok.type = TOKEN_EOF;
        return tok;
    }

    // IDENT / KEYWORD / TYPE
    if (isalpha(c) || c == '_') {
        while (isalnum(src[*i]) || src[*i] == '_') (*i)++;
        char* txt = substr(src, start, *i);
        tok.value = txt;
        tok.type = is_keyword(txt) ? TOKEN_KEYWORD
                 : is_type(txt)    ? TOKEN_TYPE
                                   : TOKEN_IDENTIFIER;
    }
    // NUMBER
    else if (isdigit(c)) {
        while (isdigit(src[*i]) || src[*i] == '.') (*i)++;
        tok.value = substr(src, start, *i);
        tok.type  = TOKEN_NUMBER;
    }
    // STRING
    else if (c == '"') {
        (*i)++;  // skip opening quote
        int strStart = *i;
        while (src[*i] && src[*i] != '"') (*i)++;
        tok.value = substr(src, strStart, *i);
        tok.type  = TOKEN_STRING;
        if (src[*i] == '"') (*i)++;  // skip closing quote
    }
    // OPERATOR (longest first)
    else {
        int matched = 0;
        for (int j = 0; j < (int)(sizeof(OPERATORS)/sizeof(*OPERATORS)); j++) {
            int L = strlen(OPERATORS[j]);
            if (strncmp(src + *i, OPERATORS[j], L) == 0) {
                tok.value = strdup(OPERATORS[j]);
                tok.type  = TOKEN_OPERATOR;
                *i += L;
                matched = 1;
                break;
            }
        }
        if (!matched) {
            // SYMBOL
            if (strchr(SYMBOLS, c)) {
                tok.value = malloc(2);
                tok.value[0] = c;
                tok.value[1] = '\0';
                tok.type = TOKEN_SYMBOL;
                (*i)++;
            } else {
                // INVALID
                tok.value = strdup("?");
                tok.type  = TOKEN_INVALID;
                (*i)++;
            }
        }
    }

    // finally bump the column by exactly how many chars we consumed
    int consumed = *i - start;
    *col = startCol + consumed;
    return tok;
}

void tokenize(const char* src) {
    int i = 0, line = 1, col = 1;
    while (1) {
        Token tok = next_token(src, &i, &line, &col);
        if (tok.type == TOKEN_EOF) break;

        printf("Line %d, Col %2d: %-15s | Value: %s\n",
               tok.line, tok.col,
               token_type_names[tok.type],
               tok.value);

        free(tok.value);
    }
}

int main() {
    const char* code = "Const Int4 x = 42;\nvoid main() { print(\"Hi\"); }";
    tokenize(code);
    return 0;
}