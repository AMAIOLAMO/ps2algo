#include "../include/input_lexer.h"

InputLexer::InputLexer(std::ifstream *stream_ptr) :
        stream_ptr(stream_ptr) {}

Token InputLexer::AdvanceToken() {
    // consumes all white spaces
    while(isspace(current_char))
        current_char = GetNextChar();

    // checks for an identifier: [a-zA-Z_][a-zA-Z0-9_]*
    if(isalpha(current_char) || current_char == '_') {
        std::string identifier_str{current_char};

        // [a-zA-Z0-9]* match
        while(isalnum(current_char = GetNextChar()) || current_char == '_')
            identifier_str += current_char;

        return TokenFactory::MakeIdentifier(identifier_str);
    }

    // search number: [0-9.]+
    if(isdigit(current_char) || current_char == '.') {
        std::string number_str{current_char};

        while(isdigit(current_char = GetNextChar()) || current_char == '.')
            number_str += current_char;

        return TokenFactory::MakeNumber(number_str);
    }

    // comment
    if(current_char == '#') {
        do
            current_char = GetNextChar();
        while(current_char != EOF && current_char != '\n' && current_char != '\r');

        if(current_char != EOF)
            return AdvanceToken();
    }

    if(current_char == EOF) 
        return TokenFactory::MakeEOF();
    // else

    char previous_character = current_char;
    current_char = GetNextChar();

    return TokenFactory::MakeAscii(previous_character);
}

int InputLexer::GetNextChar() {
    return stream_ptr->get();
}

