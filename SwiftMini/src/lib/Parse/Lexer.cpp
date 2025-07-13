#include <cassert>
#include <stdio.h>
#include "Lexer.h"

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
};

void Lexer::lexTrivia() {
Restart:
    const char *TriviaStart = CurPtr;
    
    switch(*CurPtr++) {
        case '\n':
            goto Restart;
            // - Встретили \r (carriage return)
            // - Проверяем, не следует ли за ним \n (line feed) - это Windows-стиль \r\n
            // - Если да, то пропускаем и \n тоже
        case '\r':
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
            switch (getNulCharacterKind(CurPtr - 1)) {
                case NulCharacterKind::Embedded: {
                    goto Restart;
                }
                case NulCharacterKind::CodeCompletion:
                case NulCharacterKind::BufferEnd:
                    break;
            };
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

static bool isValidIdentifierStartChar(char c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

static bool isValidOperatorStartChar(char c) {
  switch (c) {
  case '+': case '-': case '*': case '/': case '%':
  case '=': case '!': case '<': case '>': case '&':
  case '|': case '^': case '~': case '?': case ':':
  case '.':
    return true;
  default:
    return false;
  }
}

static bool advanceIfValidStartOfIdentifier(const char *&ptr, const char *end) {
  if (ptr >= end) return false;
  
  char c = *ptr;
  if (isValidIdentifierStartChar(c)) {
    ++ptr;
    return true;
  }
  return false;
}

static bool advanceIfValidStartOfOperator(const char *&ptr, const char *end) {
  if (ptr >= end) return false;
  
  char c = *ptr;
  if (isValidOperatorStartChar(c)) {
    ++ptr;
    return true;
  }
  return false;
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

Lexer::NulCharacterKind Lexer::getNulCharacterKind(const char *Ptr) const {
  if (Ptr == BufferEnd) {
    return NulCharacterKind::BufferEnd;
  }
  return NulCharacterKind::Embedded;
}
