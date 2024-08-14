#include "../include/token.h"

Token::Token(TokenType type, std::string identifier_str) :
        type(type), identifier_str(identifier_str) { }

double Token::GetDouble() const {
    return std::strtod(identifier_str.c_str(), nullptr);
}

char Token::GetAscii() const {
    return identifier_str[0];
}

bool Token::IsIdentifierOf(std::string identifier) const {
    return type == TOK_IDENTIFIER && identifier_str == identifier;
}

bool Token::IsAsciiOf(char ascii) const {
    return type == TOK_ASCII && identifier_str.size() == 1 && GetAscii() == ascii;
}

Token TokenFactory::MakeEOF() {
    return Token{
        TOK_EOF
    };
}

Token TokenFactory::MakeNumber(std::string number) {
    return Token{
        TOK_NUMBER, number
    };
}

Token TokenFactory::MakeIdentifier(std::string identifier) {
    return Token{
        TOK_IDENTIFIER, identifier
    };
}

Token TokenFactory::MakeAscii(char ascii_char) {
    return Token{
        TOK_ASCII, std::string{ascii_char}
    };
}
