#include "lexer.h"
#include "parser.h"

int main() {
    const char* source_code = "Const Int4 x = 42;\nvoid main() { print(\"Hi\"); }";  // Example input
    // Initialize lexer state here (if needed)

    parse_program();  // Start parsing program
    return 0;
}