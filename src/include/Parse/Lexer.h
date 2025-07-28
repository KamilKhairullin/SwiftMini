#ifndef Lexer_h
#define Lexer_h

#include "Token.h"

class Lexer {
    
    Token NextToken;
    
    // Указатель на первый символ в буффере
    // Буфер — это участок памяти, содержащий текст исходного кода одного файла
    // или фрагмента для лексического анализа.
    const char *BufferStart;
    
    // Указатель на последний символ в буффере
    // Буффер всегда NUL-terminated, поэтому указатель всегда указывает на NUL-terminator
    const char *BufferEnd;
    
    // Указатель на следующий не обработанный символ.
    const char *CurPtr;

public:
    explicit Lexer(std::string_view input);

    Token lex() {
        Token result = NextToken;
        if (result.isNot(tok::eof))
            lexImpl();

        return result;
    }

private:
    
    void initialize(std::string_view input);

    void lexImpl();
    
    void lexTrivia();
    
    void skipSlashSlashComment();
    
    void skipToEndOfLine();
    
    void skipSlashStarComment();
    
    void skipHashbang();

    void formToken(tok Kind, const char *TokStart);

    void lexIdentifier();

    void lexNumber();

    void lexStringLiteral();
};

#endif
