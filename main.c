#include "lexer.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    const char* src = "Const Int4 x = 42;\nvoid main() { print(\"Hi\"); }";
    // 1) Dump tokens to see how 'void' and 'main' are classified:
    printf("---- LEXER DUMP ----\n");
    int  idx = 0, line = 1, col = 1;
    Token t;
    while ((t = next_token(src, &idx, &line, &col)).type != TOKEN_EOF) {
        printf("  %s : %s\n",
               token_type_names[t.type],
               t.value);
        free(t.value);
    }
    printf("--------------------\n\n");

    // 2) Now run parser
    idx = 0; line = 1; col = 1;
    lookahead_token = next_token(src, &idx, &line, &col);
    advance(src, &idx, &line, &col);  
    parse_program(src, &idx, &line, &col);
    return 0;
}