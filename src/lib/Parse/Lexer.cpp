#include <cassert>
#include <stdio.h>
#include "Parse/Lexer.h"

Lexer::Lexer(std::string_view input) {
    initialize(input);
};

void Lexer::initialize(std::string_view input) {
    BufferStart = input.data();
    BufferEnd = input.data() + input.size();
    CurPtr = BufferStart;
};

void Lexer::lexImpl() {
    assert(CurPtr >= BufferStart &&
           CurPtr <= BufferEnd && "Current pointer out of range!");
    lexTrivia();
    
    // Remember the start of the token so we can form the text range.
    const char *TokStart = CurPtr;
    
    if (CurPtr >= BufferEnd) {
        return formToken(tok::eof, TokStart);
    }

    switch (*CurPtr++) {
        case '\n':
        case '\r':
            assert(false && "Newlines should be handled by lexTrivia");
            break;
        case ' ':
        case '\t':
        case '\f':
        case '\v':
            assert(false && "Whitespace should be handled by lexTrivia");
            break;
        case 0:
            --CurPtr;
            return formToken(tok::eof, TokStart);
        case '{': return formToken(tok::l_brace, TokStart);
        case '[': return formToken(tok::l_square, TokStart);
        case '(': return formToken(tok::l_paren, TokStart);
        case '}': return formToken(tok::r_brace, TokStart);
        case ']': return formToken(tok::r_square, TokStart);
        case ')': return formToken(tok::r_paren, TokStart);

        case ',': return formToken(tok::comma, TokStart);
        case ';': return formToken(tok::semi, TokStart);
        case ':': return formToken(tok::colon, TokStart);
        case '=': return formToken(tok::equal, TokStart);
        case '@': return formToken(tok::at_sign, TokStart);
        case '$': return formToken(tok::dollarident, TokStart);
        case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G':
        case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N':
        case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U':
        case 'V': case 'W': case 'X': case 'Y': case 'Z':
        case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g':
        case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n':
        case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u':
        case 'v': case 'w': case 'x': case 'y': case 'z':
        case '_':
            return lexIdentifier();
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
          return lexNumber();
        case '\'':
        case '"':
          return lexStringLiteral();
    }
    
};

void Lexer::lexTrivia() {
Restart:
    const char *TriviaStart = CurPtr;
    
    switch(*CurPtr++) {
        case '\n':
            goto Restart;
        case '\r':
            // - Встретили \r (carriage return)
            // - Проверяем, не следует ли за ним \n (line feed) - это Windows-стиль \r\n
            // - Если да, то пропускаем и \n тоже
            if (CurPtr[0] == '\n') {
                ++CurPtr;
            }
            goto Restart;
        case ' ':
        case '\t':
        case '\v':
        case '\f':
            goto Restart;
        case '/':
            if (*CurPtr == '/') {
                // '// ...' comment.
                skipSlashSlashComment();
                goto Restart;
            } else if (*CurPtr == '*') {
                // '/* ... */' comment.
                skipSlashStarComment();
                goto Restart;
            }
            break;
        case '#':
            if (TriviaStart == BufferStart && *CurPtr == '!') {
                --CurPtr;  // Возвращаемся на '#'
                skipHashbang();
                goto Restart;
            }
            break;
        case '<':
        case '>':
            break;
        case 0:
            if (CurPtr == BufferEnd) {
                goto Restart;
            }
            break;
        // Start character of tokens.
        case (char)-1: case (char)-2:
        case '@': case '{': case '[': case '(': case '}': case ']': case ')':
        case ',': case ';': case ':': case '\\': case '$':
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
        case '"': case '\'': case '`':
        // Start of identifiers.
        case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G':
        case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N':
        case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U':
        case 'V': case 'W': case 'X': case 'Y': case 'Z':
        case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g':
        case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n':
        case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u':
        case 'v': case 'w': case 'x': case 'y': case 'z':
        case '_':
        // Start of operators.
        case '%': case '!': case '?': case '=':
        case '-': case '+': case '*':
        case '&': case '|': case '^': case '~': case '.':
            break;
        default:
          // Unknown symbol - mark as trivia
          goto Restart;
    }
    --CurPtr;
}

static bool advanceToEndOfLine(const char *&CurPtr, const char *BufferEnd) {
  while (CurPtr < BufferEnd) {
    if (*CurPtr == '\n' || *CurPtr == '\r') {
      return true; // нашли конец строки
    }
    if (*CurPtr == '\0') {
      return false; // конец файла
    }
    ++CurPtr;
  }
  return false; // дошли до конца буфера
}

static bool skipToEndOfSlashStarComment(const char *&CurPtr, const char *BufferEnd) {
  assert(CurPtr[-1] == '/' && CurPtr[0] == '*' && "Not a /* comment");
  
  // Пропускаем * чтобы не обработать /*/ как начало и конец
  ++CurPtr;
  
  unsigned Depth = 1;  // Счетчик вложенности
  
  while (CurPtr < BufferEnd && Depth > 0) {
    char c = *CurPtr++;
    
    switch (c) {
    case '*':
      if (CurPtr < BufferEnd && *CurPtr == '/') {
        ++CurPtr;
        --Depth;  // Нашли */
      }
      break;
      
    case '/':
      if (CurPtr < BufferEnd && *CurPtr == '*') {
        ++CurPtr;
        ++Depth;  // Нашли /* (вложенный)
      }
      break;
      
    case '\0':
      // Незавершенный комментарий - просто выходим
      return false;
    }
  }
  
  return true;
}

void Lexer::skipSlashStarComment() {
  skipToEndOfSlashStarComment(CurPtr, BufferEnd);
}


void Lexer::skipSlashSlashComment() {
  assert(CurPtr[-1] == '/' && CurPtr[0] == '/' && "Not a // comment");
  skipToEndOfLine();
}

void Lexer::skipToEndOfLine() {
  advanceToEndOfLine(CurPtr, BufferEnd);
}

void Lexer::skipHashbang() {
  assert(CurPtr == BufferStart && CurPtr[0] == '#' && CurPtr[1] == '!' &&
         "Not a hashbang");
  skipToEndOfLine();
}


void Lexer::formToken(tok Kind, const char *TokStart) {
    assert(CurPtr >= BufferStart &&
           CurPtr <= BufferEnd && "Current pointer out of range!");
    
    std::string_view TokenText { TokStart, static_cast<size_t>(CurPtr - TokStart) };

    NextToken.setToken(Kind, TokenText);
}

void Lexer::lexIdentifier() {
  const char *TokStart = CurPtr - 1;
    
  while (CurPtr < BufferEnd) {
    char c = *CurPtr;
    if (isalnum(c) || c == '_') {
      ++CurPtr;
    } else {
      break;
    }
  }
  
  tok Kind = Token::kindOfIdentifier(TokStart, CurPtr);
  return formToken(Kind, TokStart);
}

void Lexer::lexNumber() {
    const char *TokStart = CurPtr - 1;
    // Hex numbers: 0xFF
    if (*TokStart == '0' && *CurPtr == 'x') {
      ++CurPtr; // skip 'x'
      while (CurPtr < BufferEnd && (isdigit(*CurPtr) ||
             (*CurPtr >= 'a' && *CurPtr <= 'f') ||
             (*CurPtr >= 'A' && *CurPtr <= 'F') ||
             *CurPtr == '_')) {
        ++CurPtr;
      }
      return formToken(tok::integer_literal, TokStart);
    }

    // Binary: 0b1010
    if (*TokStart == '0' && *CurPtr == 'b') {
      ++CurPtr; // skip 'b'
      while (CurPtr < BufferEnd && (*CurPtr == '0' || *CurPtr == '1' || *CurPtr == '_')) {
        ++CurPtr;
      }
      return formToken(tok::integer_literal, TokStart);
    }

    // Decimal numbers: 123 or 123.45
    while (CurPtr < BufferEnd && (isdigit(*CurPtr) || *CurPtr == '_')) {
      ++CurPtr;
    }
    // Check for float: 123.45
    if (CurPtr < BufferEnd && *CurPtr == '.' &&
        CurPtr + 1 < BufferEnd && isdigit(*(CurPtr + 1))) {
        ++CurPtr; // skip '.'
        while (CurPtr < BufferEnd && (isdigit(*CurPtr) || *CurPtr == '_')) {
            ++CurPtr;
        }
        return formToken(tok::floating_literal, TokStart);
    }
      
    return formToken(tok::integer_literal, TokStart);

}

void Lexer::lexStringLiteral() {
  const char *TokStart = CurPtr - 1;
  const char QuoteChar = *(CurPtr - 1); // '"' or '\''
  
  while (CurPtr < BufferEnd) {
    char c = *CurPtr;
    
    // Конец строки
    if (c == QuoteChar) {
      ++CurPtr;
      return formToken(tok::string_literal, TokStart);
    }
    
    if (c == '\n' || c == '\r') {
      return formToken(tok::unknown, TokStart);
    }
    
    if (c == '\\') {
      ++CurPtr;
      if (CurPtr < BufferEnd) {
        ++CurPtr;
      }
    } else {
      ++CurPtr;
    }
  }
  return formToken(tok::unknown, TokStart);
}
