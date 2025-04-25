#include "lexer.h"
#include "parser.h"

const char* source_code = "Const Int4 x = 42;\nvoid main() { print(\"Hi\"); }";
int idx = 0, line = 1, col = 1;
extern Token current_token;
extern Token lookahead_token;

void advance(void) {
    current_token   = lookahead_token;
    lookahead_token = next_token(source_code, &idx, &line, &col);
}

int main() {
    lookahead_token = next_token(source_code, &idx, &line, &col);
    advance();            // set current_token = first token
    parse_program();      // now drives the parser
    return 0;
}