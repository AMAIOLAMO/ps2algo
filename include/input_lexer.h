#ifndef INPUT_LEXER_H
#define INPUT_LEXER_H

#include <fstream>
#include <iostream>

#include "token.h"

class InputLexer {
public:
    InputLexer(std::ifstream *stream_ptr);

    Token AdvanceToken();

private:
    int GetNextChar();

    std::ifstream *stream_ptr;
    char current_char = ' ';
};

#endif // INPUT_LEXER_H
