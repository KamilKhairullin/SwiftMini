#ifndef Lexer_h
#define Lexer_h

#include "Token.h"

class Lexer {
    
    Token NextToken;
    
    // Pointer to the first character of the buffer
    const char *BufferStart;
    
    // Pointer to one past the end character of the buffer.
    // Because the buffer is always NUL-terminated,
    // this points to the NUL terminator.
    const char *BufferEnd;
    
    // Pointer to the next not consumed character.
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
    
    // Nul character meaning kind.
    enum class NulCharacterKind {
    // char buffer[] = "let x = 5";  // Automatically ends with \0
    BufferEnd,
    // "let x = 5\0let y = 10". Is an error
    Embedded,
    // Code completion marker.
    CodeCompletion
    };
    
    void initialize(std::string_view input);

    void lexImpl();
    
    void lexTrivia();
    
    void skipSlashSlashComment();
    
    void skipToEndOfLine();
    
    void skipSlashStarComment();
    
    void skipHashbang();

    NulCharacterKind getNulCharacterKind(const char *Ptr) const;
};

#endif
