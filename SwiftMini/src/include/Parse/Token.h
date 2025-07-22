#ifndef Token_h
#define Token_h

#include <string_view>

enum class tok {
  unknown = 0,
  eof,
  identifier,
  oper_binary,
  oper_postfix,
  oper_prefix,
  dollarident,
  integer_literal,
  floating_literal,
  string_literal,
  character_literal,
  comment,
  
  #define KEYWORD(X) kw_ ## X,
  #define PUNCTUATOR(X, Y) X,
  #include "Tokens.def"
  
  NUM_TOKENS
};

class Token {
private:
    tok Kind;
    // Text - The actual string covered by the token in the source buffer.
    std::string_view Text;
    
public:
    Token() : Token(tok::NUM_TOKENS, {}) {}
    Token(tok kind, std::string_view text)
        : Kind(kind), Text(text) {}

    
    tok getKind() const { return Kind; }
    std::string_view getText() const { return Text; }
    
    void setToken(tok K, std::string_view T) {
        Kind = K;
        Text = T;
    }
    
    // is/isNot - Predicates to check if this token is a specific kind, as in
    // "if (Tok.is(tok::l_brace)) {...}".
    bool is(tok K) const { return Kind == K; }

    bool isNot(tok K) const { return Kind != K; }
    
    bool isAnyOperator() const {
      return Kind == tok::oper_binary || Kind == tok::oper_postfix ||
             Kind == tok::oper_prefix;
    }

    bool isKeyword() const {
      switch (Kind) {
        #define KEYWORD(X) case tok::kw_##X: return true;
        #include "Tokens.def"
      default: 
        return false;
      }
    }
    
    bool isPunctuation() const {
        switch (Kind) {
          #define PUNCTUATOR(X, Y) case tok::X: return true;
          #include "Tokens.def"
        default:
          return false;
        }
    }

    bool isLiteral() const {
        switch (Kind) {
            case tok::integer_literal:
            case tok::floating_literal:
            case tok::string_literal:
            case tok::character_literal:
                return true;
            default:
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
    
    std::string_view getTokenName() {
        switch (Kind) {
            case tok::unknown: return "<unknown>";
            case tok::eof: return "<EOF>";
            case tok::identifier: return "<identifier>";
            case tok::oper_binary: return "<oper_binary>";
            case tok::oper_postfix: return "<oper_postfix>";
            case tok::oper_prefix: return "<oper_prefix>";
            case tok::dollarident: return "<dollarident>";
            case tok::integer_literal: return "<integer_literal>";
            case tok::floating_literal: return "<floating_literal>";
            case tok::string_literal: return "<string_literal>";
            case tok::character_literal: return "<character_literal>";
            case tok::comment: return "<comment>";
            
            #define KEYWORD(X) case tok::kw_ ## X: return "<kw_" #X ">";
            #define PUNCTUATOR(X, Y) case tok::X: return "<" #X ">";
            #include "Tokens.def"
            
            default: return "<INVALID_TOKEN>";
        }
    }
};

#endif

