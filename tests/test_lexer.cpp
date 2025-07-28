#include <gtest/gtest.h>
#include "Parse/Lexer.h"

class LexerTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(LexerTest, SimpleTokens) {
    std::string input = "let x = 42";
    Lexer lexer(input);
    
    Token tok1 = lexer.lex();
    EXPECT_EQ(tok1.getKind(), tok::START_OF_FILE);
    
    Token tok2 = lexer.lex();
    EXPECT_EQ(tok2.getKind(), tok::kw_let);
    
    Token tok3 = lexer.lex();
    EXPECT_EQ(tok3.getKind(), tok::identifier);
    EXPECT_EQ(std::string(tok3.getText()), "x");
    
    Token tok4 = lexer.lex();
    EXPECT_EQ(tok4.getKind(), tok::equal);
    
    Token tok5 = lexer.lex();
    EXPECT_EQ(tok5.getKind(), tok::integer_literal);
    EXPECT_EQ(std::string(tok5.getText()), "42");
    
    Token tok6 = lexer.lex();
    EXPECT_EQ(tok6.getKind(), tok::eof);
}

TEST_F(LexerTest, LexTriviaNewLine) {
    std::string input = "\nlet \n\n\nx = 42\n";
    Lexer lexer(input);
    
    Token tok1 = lexer.lex();
    EXPECT_EQ(tok1.getKind(), tok::START_OF_FILE);
    
    Token tok2 = lexer.lex();
    EXPECT_EQ(tok2.getKind(), tok::kw_let);
    
    Token tok3 = lexer.lex();
    EXPECT_EQ(tok3.getKind(), tok::identifier);
    EXPECT_EQ(std::string(tok3.getText()), "x");
    
    Token tok4 = lexer.lex();
    EXPECT_EQ(tok4.getKind(), tok::equal);
    
    Token tok5 = lexer.lex();
    EXPECT_EQ(tok5.getKind(), tok::integer_literal);
    EXPECT_EQ(std::string(tok5.getText()), "42");
    
    Token tok6 = lexer.lex();
    EXPECT_EQ(tok6.getKind(), tok::eof);
}

TEST_F(LexerTest, LexTriviaNewLineCarriage) {
    std::string input = "\rlet \r\nx = 42\r";
    Lexer lexer(input);
    
    Token tok1 = lexer.lex();
    EXPECT_EQ(tok1.getKind(), tok::START_OF_FILE);
    
    Token tok2 = lexer.lex();
    EXPECT_EQ(tok2.getKind(), tok::kw_let);
    
    Token tok3 = lexer.lex();
    EXPECT_EQ(tok3.getKind(), tok::identifier);
    EXPECT_EQ(std::string(tok3.getText()), "x");
    
    Token tok4 = lexer.lex();
    EXPECT_EQ(tok4.getKind(), tok::equal);
    
    Token tok5 = lexer.lex();
    EXPECT_EQ(tok5.getKind(), tok::integer_literal);
    EXPECT_EQ(std::string(tok5.getText()), "42");
    
    Token tok6 = lexer.lex();
    EXPECT_EQ(tok6.getKind(), tok::eof);
}

TEST_F(LexerTest, LexTriviaNewLineSpace) {
    std::string input = "let    x =      42  ";
    Lexer lexer(input);
    
    Token tok1 = lexer.lex();
    EXPECT_EQ(tok1.getKind(), tok::START_OF_FILE);
    
    Token tok2 = lexer.lex();
    EXPECT_EQ(tok2.getKind(), tok::kw_let);
    
    Token tok3 = lexer.lex();
    EXPECT_EQ(tok3.getKind(), tok::identifier);
    EXPECT_EQ(std::string(tok3.getText()), "x");
    
    Token tok4 = lexer.lex();
    EXPECT_EQ(tok4.getKind(), tok::equal);
    
    Token tok5 = lexer.lex();
    EXPECT_EQ(tok5.getKind(), tok::integer_literal);
    EXPECT_EQ(std::string(tok5.getText()), "42");
    
    Token tok6 = lexer.lex();
    EXPECT_EQ(tok6.getKind(), tok::eof);
}

TEST_F(LexerTest, LexTriviaTab) {
    std::string input = "let\tx\t=\t42";
    Lexer lexer(input);
    
    Token tok1 = lexer.lex();
    EXPECT_EQ(tok1.getKind(), tok::START_OF_FILE);
    
    Token tok2 = lexer.lex();
    EXPECT_EQ(tok2.getKind(), tok::kw_let);
    
    Token tok3 = lexer.lex();
    EXPECT_EQ(tok3.getKind(), tok::identifier);
    EXPECT_EQ(std::string(tok3.getText()), "x");
    
    Token tok4 = lexer.lex();
    EXPECT_EQ(tok4.getKind(), tok::equal);
    
    Token tok5 = lexer.lex();
    EXPECT_EQ(tok5.getKind(), tok::integer_literal);
    EXPECT_EQ(std::string(tok5.getText()), "42");
    
    Token tok6 = lexer.lex();
    EXPECT_EQ(tok6.getKind(), tok::eof);
}

TEST_F(LexerTest, LexTriviaVerticalTab) {
    std::string input = "let\vx\v=\v42";
    Lexer lexer(input);
    
    Token tok1 = lexer.lex();
    EXPECT_EQ(tok1.getKind(), tok::START_OF_FILE);
    
    Token tok2 = lexer.lex();
    EXPECT_EQ(tok2.getKind(), tok::kw_let);
    
    Token tok3 = lexer.lex();
    EXPECT_EQ(tok3.getKind(), tok::identifier);
    EXPECT_EQ(std::string(tok3.getText()), "x");
    
    Token tok4 = lexer.lex();
    EXPECT_EQ(tok4.getKind(), tok::equal);
    
    Token tok5 = lexer.lex();
    EXPECT_EQ(tok5.getKind(), tok::integer_literal);
    EXPECT_EQ(std::string(tok5.getText()), "42");
    
    Token tok6 = lexer.lex();
    EXPECT_EQ(tok6.getKind(), tok::eof);
}

TEST_F(LexerTest, LexTriviaFormFeed) {
    std::string input = "let\fx\f=\f42";
    Lexer lexer(input);
    
    Token tok1 = lexer.lex();
    EXPECT_EQ(tok1.getKind(), tok::START_OF_FILE);
    
    Token tok2 = lexer.lex();
    EXPECT_EQ(tok2.getKind(), tok::kw_let);
    
    Token tok3 = lexer.lex();
    EXPECT_EQ(tok3.getKind(), tok::identifier);
    EXPECT_EQ(std::string(tok3.getText()), "x");
    
    Token tok4 = lexer.lex();
    EXPECT_EQ(tok4.getKind(), tok::equal);
    
    Token tok5 = lexer.lex();
    EXPECT_EQ(tok5.getKind(), tok::integer_literal);
    EXPECT_EQ(std::string(tok5.getText()), "42");
    
    Token tok6 = lexer.lex();
    EXPECT_EQ(tok6.getKind(), tok::eof);
}

TEST_F(LexerTest, LexSlashSlashComment) {
    std::string input = "let // comment\nx = 42";
    Lexer lexer(input);
    
    Token tok1 = lexer.lex();
    EXPECT_EQ(tok1.getKind(), tok::START_OF_FILE);
    
    Token tok2 = lexer.lex();
    EXPECT_EQ(tok2.getKind(), tok::kw_let);
    
    Token tok3 = lexer.lex();
    EXPECT_EQ(tok3.getKind(), tok::identifier);
    EXPECT_EQ(std::string(tok3.getText()), "x");
    
    Token tok4 = lexer.lex();
    EXPECT_EQ(tok4.getKind(), tok::equal);
    
    Token tok5 = lexer.lex();
    EXPECT_EQ(tok5.getKind(), tok::integer_literal);
    EXPECT_EQ(std::string(tok5.getText()), "42");
    
    Token tok6 = lexer.lex();
    EXPECT_EQ(tok6.getKind(), tok::eof);
}

TEST_F(LexerTest, LexSlashSlashCommentSingleLine) {
    std::string input = "let // comment x = 42";
    Lexer lexer(input);
    
    Token tok1 = lexer.lex();
    EXPECT_EQ(tok1.getKind(), tok::START_OF_FILE);
    
    Token tok2 = lexer.lex();
    EXPECT_EQ(tok2.getKind(), tok::kw_let);
    
    Token tok3 = lexer.lex();
    EXPECT_EQ(tok3.getKind(), tok::eof);
}

TEST_F(LexerTest, LexSlashStarComment) {
    std::string input = "let /* multi\nline comment */ x = 42";
    Lexer lexer(input);
    
    Token tok1 = lexer.lex();
    EXPECT_EQ(tok1.getKind(), tok::START_OF_FILE);
    
    Token tok2 = lexer.lex();
    EXPECT_EQ(tok2.getKind(), tok::kw_let);
    
    Token tok3 = lexer.lex();
    EXPECT_EQ(tok3.getKind(), tok::identifier);
    EXPECT_EQ(std::string(tok3.getText()), "x");
    
    Token tok4 = lexer.lex();
    EXPECT_EQ(tok4.getKind(), tok::equal);
    
    Token tok5 = lexer.lex();
    EXPECT_EQ(tok5.getKind(), tok::integer_literal);
    EXPECT_EQ(std::string(tok5.getText()), "42");
    
    Token tok6 = lexer.lex();
    EXPECT_EQ(tok6.getKind(), tok::eof);
}

TEST_F(LexerTest, LexHashbang) {
    std::string input = "#!/usr/bin/swift\nlet x = 42";
    Lexer lexer(input);
    
    Token tok1 = lexer.lex();
    EXPECT_EQ(tok1.getKind(), tok::START_OF_FILE);
    
    Token tok2 = lexer.lex();
    EXPECT_EQ(tok2.getKind(), tok::kw_let);
    
    Token tok3 = lexer.lex();
    EXPECT_EQ(tok3.getKind(), tok::identifier);
    EXPECT_EQ(std::string(tok3.getText()), "x");
    
    Token tok4 =lexer.lex();
    EXPECT_EQ(tok4.getKind(), tok::equal);
    
    Token tok5 = lexer.lex();
    EXPECT_EQ(tok5.getKind(), tok::integer_literal);
    EXPECT_EQ(std::string(tok5.getText()), "42");
    
    Token tok6 = lexer.lex();
    EXPECT_EQ(tok6.getKind(), tok::eof);
}

TEST_F(LexerTest, LexMixedWhitespace) {
    std::string input = " \t\n\r\v\f let \t\n x \r = \v\f 42 ";
    Lexer lexer(input);
    
    Token tok1 = lexer.lex();
    EXPECT_EQ(tok1.getKind(), tok::START_OF_FILE);
    
    Token tok2 = lexer.lex();
    EXPECT_EQ(tok2.getKind(), tok::kw_let);
    
    Token tok3 = lexer.lex();
    EXPECT_EQ(tok3.getKind(), tok::identifier);
    EXPECT_EQ(std::string(tok3.getText()), "x");
    
    Token tok4 = lexer.lex();
    EXPECT_EQ(tok4.getKind(), tok::equal);
    
    Token tok5 = lexer.lex();
    EXPECT_EQ(tok5.getKind(), tok::integer_literal);
    EXPECT_EQ(std::string(tok5.getText()), "42");
    
    Token tok6 = lexer.lex();
    EXPECT_EQ(tok6.getKind(), tok::eof);
}

TEST_F(LexerTest, LexSpecialCharacters) {
    std::string input = "@ { [ ( } ] ) , ; : \\ $";
    Lexer lexer(input);
    
    Token tok1 = lexer.lex();
    EXPECT_EQ(tok1.getKind(), tok::START_OF_FILE);
    
    Token tok2 = lexer.lex();
    EXPECT_EQ(tok2.getKind(), tok::at_sign);
    
    Token tok3 = lexer.lex();
    EXPECT_EQ(tok3.getKind(), tok::l_brace);
    
    Token tok4 = lexer.lex();
    EXPECT_EQ(tok4.getKind(), tok::l_square);
    
    Token tok5 = lexer.lex();
    EXPECT_EQ(tok5.getKind(), tok::l_paren);
    
    Token tok6 = lexer.lex();
    EXPECT_EQ(tok6.getKind(), tok::r_brace);
    
    Token tok7 = lexer.lex();
    EXPECT_EQ(tok7.getKind(), tok::r_square);
    
    Token tok8 = lexer.lex();
    EXPECT_EQ(tok8.getKind(), tok::r_paren);
    
    Token tok9 = lexer.lex();
    EXPECT_EQ(tok9.getKind(), tok::comma);
    
    Token tok10 = lexer.lex();
    EXPECT_EQ(tok10.getKind(), tok::semi);
    
    Token tok11 = lexer.lex();
    EXPECT_EQ(tok11.getKind(), tok::colon);
    
    Token tok12 = lexer.lex();
    EXPECT_EQ(tok12.getKind(), tok::colon);
    
    Token tok13 = lexer.lex();
    EXPECT_EQ(tok13.getKind(), tok::dollarident);
    
    Token tok14 = lexer.lex();
    EXPECT_EQ(tok14.getKind(), tok::eof);
}

TEST_F(LexerTest, LexUnknownCharacters) {
    std::string input = "let ® x © = 42 ™";  // Неизвестные символы будут пропущены
    Lexer lexer(input);
    
    Token tok1 = lexer.lex();
    EXPECT_EQ(tok1.getKind(), tok::START_OF_FILE);
    
    // ® пропускается как unknown character
    Token tok2 = lexer.lex(); 
    EXPECT_EQ(tok2.getKind(), tok::kw_let);
    
    // © пропускается как unknown character
    Token tok3 = lexer.lex();
    EXPECT_EQ(tok3.getKind(), tok::identifier);
    EXPECT_EQ(std::string(tok3.getText()), "x");
    
    Token tok4 = lexer.lex();
    EXPECT_EQ(tok4.getKind(), tok::equal);
    
    Token tok5 = lexer.lex();
    EXPECT_EQ(tok5.getKind(), tok::integer_literal);
    EXPECT_EQ(std::string(tok5.getText()), "42");
    
    // ™ пропускается как unknown character
    Token tok6 = lexer.lex();
    EXPECT_EQ(tok6.getKind(), tok::eof);
}

TEST_F(LexerTest, LexSpecialCharactersAtEnd) {
    std::string input = "let x = 42";  // Обычные символы НЕ пропускаются
    Lexer lexer(input);
    
    Token tok1 = lexer.lex();
    EXPECT_EQ(tok1.getKind(), tok::START_OF_FILE);
    
    Token tok2 = lexer.lex();
    EXPECT_EQ(tok2.getKind(), tok::kw_let);
    
    Token tok3 = lexer.lex();
    EXPECT_EQ(tok3.getKind(), tok::identifier);
    EXPECT_EQ(std::string(tok3.getText()), "x");
    
    Token tok4 = lexer.lex();
    EXPECT_EQ(tok4.getKind(), tok::equal);
    
    Token tok5 = lexer.lex();
    EXPECT_EQ(tok5.getKind(), tok::integer_literal);
    EXPECT_EQ(std::string(tok5.getText()), "42");
    
    Token tok6 = lexer.lex();
    EXPECT_EQ(tok6.getKind(), tok::eof);
}

TEST_F(LexerTest, LexBufferEndHandling) {
    std::string input = "x"; // Короткий ввод для тестирования case 0
    Lexer lexer(input);
    
    Token tok1 = lexer.lex();
    EXPECT_EQ(tok1.getKind(), tok::START_OF_FILE);
    
    Token tok2 = lexer.lex();
    EXPECT_EQ(tok2.getKind(), tok::identifier);
    EXPECT_EQ(std::string(tok2.getText()), "x");
    
    // case 0: if (CurPtr == BufferEnd) goto Restart;
    Token tok3 = lexer.lex();
    EXPECT_EQ(tok3.getKind(), tok::eof);
}