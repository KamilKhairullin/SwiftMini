#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "Parse/Token.h"
#include "Parse/Lexer.h"

int main() {
    std::ifstream file("/Users/kamil/Developer/SwiftMini/test.swiftMini");
    if (!file.is_open()) {
        std::cerr << "Can not open file" << std::endl;
        return 1;
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();
    file.close();

    Lexer lexer(content);
    while(true) {
        Token result = lexer.lex();
        std::cout << result.getTokenName() << result.getText() << std::endl;
        if (result.isEOF()) {
            break;
        }
    }
    return 0;
}

