#ifndef _TOKEN_H
#define _TOKEN_H

#include <string>

enum TokenType {
    TOK_NONE,
    TOK_EOF,

    // common
    TOK_IDENTIFIER,
    TOK_NUMBER,
    TOK_ASCII,
};

struct Token {
public:
    Token(TokenType type = TOK_NONE,
            std::string identifier_str = std::string{});

    double GetDouble() const;

    char GetAscii() const;

    bool IsIdentifierOf(std::string identifier) const;

    bool IsAsciiOf(char ascii) const;

    TokenType type;
    std::string identifier_str{};
};

class TokenFactory {
public:
    static Token MakeEOF();

    static Token MakeNumber(std::string number);
    static Token MakeIdentifier(std::string identifier);
    static Token MakeAscii(char ascii_char);
};

#endif // _TOKEN_H
