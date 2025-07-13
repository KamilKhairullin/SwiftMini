#include <iostream>
#include "Token.h"
#include "Lexer.h"

int main(int argc, const char * argv[]) {
    std::cout << "Hello, World!\n";
    Lexer lexer("let x = 42");
    lexer.lex();
    return 0;
}
