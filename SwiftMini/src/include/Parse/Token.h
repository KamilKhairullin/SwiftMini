#ifndef Token_h
#define Token_h

#include <string_view>

enum class tok: uint8_t {

    // Keywords

    kw_let,
    kw_var,
    kw_func,
    kw_if,
    kw_else,
    kw_return,

    // Punctuation
    l_paren,        // (
    r_paren,        // )
    l_brace,        // {
    r_brace,        // }
    colon,          // :
    comma,          // ,
    semicolon,      // ;
    arrow,          // ->
    
    // Operators
    equal,          // =
    plus,           // +
    minus,          // -
    star,           // *
    slash,          // /
    equal_equal,    // ==
    exclaim_equal,  // !=
    less,           // <
    greater,        // >

    // Identifier
    identifier,     // variable names
    
    // Literals
    integer_literal,// Integer numbers
    string_literal, // strings

    // Special
    newline,
    eof,
    unknown,

    // Invalid
    NUM_TOKENS
};

class Token {
private:
    tok Kind;
    std::string_view Text; // Swift uses StringRef class from LLVM
    
public:
    Token() : Token(tok::NUM_TOKENS, {}) {}
    Token(tok kind, std::string_view text)
        : Kind(kind), Text(text) {}

    tok getKind() const { return Kind; }
    std::string_view getText() const { return Text; }
    
    // is/isNot - Predicates to check if this token is a specific kind, as in
    // "if (Tok.is(tok::l_brace)) {...}".
    bool is(tok K) const { return Kind == K; }
    bool isNot(tok K) const { return Kind != K; }
    
    bool isKeyword() const {
        switch (Kind) {
        case tok::kw_let:
        case tok::kw_var:
        case tok::kw_func:
        case tok::kw_if:
        case tok::kw_else:
        case tok::kw_return:
            return true;
        case tok::l_paren:
        case tok::r_paren:
        case tok::l_brace:
        case tok::r_brace:
        case tok::colon:
        case tok::comma:
        case tok::semicolon:
        case tok::arrow:
        case tok::equal:
        case tok::plus:
        case tok::minus:
        case tok::star:
        case tok::slash:
        case tok::equal_equal:
        case tok::exclaim_equal:
        case tok::less:
        case tok::greater:
        case tok::identifier:
        case tok::integer_literal:
        case tok::string_literal:
        case tok::newline:
        case tok::eof:
        case tok::unknown:
        case tok::NUM_TOKENS:
            return false;
        }
    }
    
    bool isPunctuation() const {
        switch (Kind) {
        case tok::l_paren:
        case tok::r_paren:
        case tok::l_brace:
        case tok::r_brace:
        case tok::colon:
        case tok::comma:
        case tok::semicolon:
        case tok::arrow:
            return true;
        case tok::kw_let:
        case tok::kw_var:
        case tok::kw_func:
        case tok::kw_if:
        case tok::kw_else:
        case tok::kw_return:
        case tok::equal:
        case tok::plus:
        case tok::minus:
        case tok::star:
        case tok::slash:
        case tok::equal_equal:
        case tok::exclaim_equal:
        case tok::less:
        case tok::greater:
        case tok::identifier:
        case tok::integer_literal:
        case tok::string_literal:
        case tok::newline:
        case tok::eof:
        case tok::unknown:
        case tok::NUM_TOKENS:
            return false;
        }
    }
    
    bool isOperator() const {
        switch (Kind) {
        case tok::equal:
        case tok::plus:
        case tok::minus:
        case tok::star:
        case tok::slash:
        case tok::equal_equal:
        case tok::exclaim_equal:
        case tok::less:
        case tok::greater:
            return true;
        case tok::kw_let:
        case tok::kw_var:
        case tok::kw_func:
        case tok::kw_if:
        case tok::kw_else:
        case tok::kw_return:
        case tok::l_paren:
        case tok::r_paren:
        case tok::l_brace:
        case tok::r_brace:
        case tok::colon:
        case tok::comma:
        case tok::semicolon:
        case tok::arrow:
        case tok::identifier:
        case tok::integer_literal:
        case tok::string_literal:
        case tok::newline:
        case tok::eof:
        case tok::unknown:
        case tok::NUM_TOKENS:
            return false;
        }
    }
    
    bool isLiteral() const {
        switch (Kind) {
        case tok::integer_literal:
        case tok::string_literal:
            return true;
        case tok::kw_let:
        case tok::kw_var:
        case tok::kw_func:
        case tok::kw_if:
        case tok::kw_else:
        case tok::kw_return:
        case tok::l_paren:
        case tok::r_paren:
        case tok::l_brace:
        case tok::r_brace:
        case tok::colon:
        case tok::comma:
        case tok::semicolon:
        case tok::arrow:
        case tok::equal:
        case tok::plus:
        case tok::minus:
        case tok::star:
        case tok::slash:
        case tok::equal_equal:
        case tok::exclaim_equal:
        case tok::less:
        case tok::greater:
        case tok::identifier:
        case tok::newline:
        case tok::eof:
        case tok::unknown:
        case tok::NUM_TOKENS:
            return false;
        }
    }

    bool isIdentifier() const {
        return Kind == tok::identifier;
    }

    bool isIdentifierOrLiteral() const {
        return isIdentifier() || isLiteral();
    }
    
    bool isEOF() const {
        return Kind == tok::eof;
    }

    std::string_view toString() const {
        switch (Kind) {
        // Keywords
        case tok::kw_let: return "let";
        case tok::kw_var: return "var";
        case tok::kw_func: return "func";
        case tok::kw_if: return "if";
        case tok::kw_else: return "else";
        case tok::kw_return: return "return";
        
        // Punctuation
        case tok::l_paren: return "(";
        case tok::r_paren: return ")";
        case tok::l_brace: return "{";
        case tok::r_brace: return "}";
        case tok::colon: return ":";
        case tok::comma: return ",";
        case tok::semicolon: return ";";
        case tok::arrow: return "->";
        
        // Operators
        case tok::equal: return "=";
        case tok::plus: return "+";
        case tok::minus: return "-";
        case tok::star: return "*";
        case tok::slash: return "/";
        case tok::equal_equal: return "==";
        case tok::exclaim_equal: return "!=";
        case tok::less: return "<";
        case tok::greater: return ">";
        
        // Identifiers
        case tok::identifier: return Text;
        
        // Literals
        case tok::integer_literal: return Text;
        case tok::string_literal: return Text;
        
        // Special
        case tok::newline: return "\\n";
        case tok::eof: return "<EOF>";
        case tok::unknown: return "<unknown>";
        case tok::NUM_TOKENS: return "<invalid>";
        }
    };
};

#endif

