#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <deque>
#include <unordered_map>
#include <assert.h>

// TODO: replace this with a search, instead of a path search
#include "../include/token.h"
#include "../include/input_lexer.h"

size_t line_number = 1;

size_t GetNextLineNumber() {
    return line_number++;
}

// TODO: Messy code, requires clean up
void EvalExpr2RevPolarNotation(
        const std::vector<Token> &token_expr,
        std::deque<Token> *output_stack_ptr) {
    std::deque<Token> op_stack;

    static std::unordered_map<char, int> op_priority{
        {'*', 2}, {'/', 2}, {'+', 1}, {'-', 1}
    };

    // lexer
    for(Token token : token_expr) {
        if(token.type == TOK_NUMBER || token.type == TOK_IDENTIFIER) {
            output_stack_ptr->push_back(token);
            continue;
        }

        if(token.IsAsciiOf('+') || token.IsAsciiOf('-') ||
                token.IsAsciiOf('*') || token.IsAsciiOf('/')) {

            int priority = op_priority[token.GetAscii()];

            while(!op_stack.empty()) {
                Token op = op_stack.back();
                int front_priority = op_priority[op.GetAscii()];
    
                if(front_priority >= priority) {
                    op_stack.pop_back();
                    output_stack_ptr->push_back(op);
                    continue;
                }
                // else
                
                break;
            }
            // can safely assume can push

            op_stack.push_back(token);
            continue;
        }

        if(token.IsAsciiOf('(')) {
            op_stack.push_back(token);
            continue;
        }

        if(token.IsAsciiOf(')')) {
            // flush all tokens until the current token matches the closest '('
            // also flush the '('
            Token token;

            do {
                token = op_stack.back();
                op_stack.pop_back();
    
                if(token.IsAsciiOf('(') == false)
                    output_stack_ptr->push_back(token);

            } while(token.IsAsciiOf('(') == false);

            continue;
        }
    }

    // flush all operations into output stack
    while(!op_stack.empty()) {
        Token op = op_stack.back();
        op_stack.pop_back();

        output_stack_ptr->push_back(op);
    }
}

bool TryParseOut(const std::vector<Token> &token_line) {
    if(token_line.size() != 2)
        return false;
    
    auto &out = token_line[0];
    auto &variable_name = token_line[1];

    if(out.IsIdentifierOf("out") == false)
        return false;

    if(variable_name.type != TOK_IDENTIFIER)
        return false;

    std::cout << GetNextLineNumber() << ". " << "Return " << variable_name.identifier_str << std::endl;

    return true;
}

bool MatchOperationExpr(const std::vector<Token> &token_expr) {
    std::deque<Token> output_stack;
    EvalExpr2RevPolarNotation(token_expr, &output_stack);

    std::deque<Token> result_stack;

    bool is_first_operator = true;

    for(Token token : output_stack) {
        if(token.type == TOK_IDENTIFIER || token.type == TOK_NUMBER) {
            result_stack.push_front(token);
            continue;
        }
        // else

        // everything after must be a token, assume each time
        assert(token.type == TOK_ASCII);

        if(is_first_operator) {
            // the result stack must have at least two numbers for us to operate in
            assert(result_stack.size() >= 2);

            // print the action first
            if(token.identifier_str == "+")
                std::cout << "Add ";
            else if(token.identifier_str == "-")
                std::cout << "Subtract ";
            else if(token.identifier_str == "*")
                std::cout << "Multiply ";
            else if(token.identifier_str == "/")
                std::cout << "Divide ";
            else
                std::cout << "UNKNOWN OPERATION ";

            Token rhs_token = result_stack[0];
            Token lhs_token = result_stack[1];

            std::cout << lhs_token.identifier_str << " with "
                << rhs_token.identifier_str << " ";

            // pop twice, so it removes both rhs and lhs
            result_stack.pop_front();
            result_stack.pop_front();


            is_first_operator = false;
            continue;
        }
        // else, is the second or third operator
        std::cout << "and ";

        if(token.identifier_str == "+")
            std::cout << "add ";
        else if(token.identifier_str == "-")
            std::cout << "subtract ";
        else if(token.identifier_str == "*")
            std::cout << "multiply ";
        else if(token.identifier_str == "/")
            std::cout << "divide ";
        else
            std::cout << "UNKNOWN OPERATION ";

        Token rhs_token = result_stack[0];

        std::cout << "with " << rhs_token.identifier_str << " ";
    
        result_stack.pop_front();
    }
    
    return true;
}

bool TryAssignment(const std::vector<Token> &token_line) {
    // at least 3 tokens (such as x = 1)
    if(token_line.size() <= 2)
        return false;

    auto &variable = token_line[0];
    auto &equal = token_line[1];

    if(variable.type != TOK_IDENTIFIER)
        return false;

    if(equal.IsAsciiOf('=') == false)
        return false;
    // else
    
    std::vector<Token> token_expressions;

    for(size_t i = 2; i < token_line.size(); i++) {
        Token token = token_line[i];
        token_expressions.emplace_back(token);
    }

    std::cout << GetNextLineNumber() << ". ";

    MatchOperationExpr(token_expressions);


    std::cout  << "then assign into the variable " <<
        variable.identifier_str << std::endl;


    return true;
}

// TODO: Convert the line number with a packaged output logger

bool TryParseInput(const std::vector<Token> &token_line) {
    if(token_line.size() != 4)
        return false;

    auto &input                = token_line[0];
    auto &variable_name        = token_line[1];
    auto &colon                = token_line[2];
    auto &original_requirement = token_line[3];

    // in x:num1
    if(input.IsIdentifierOf("in") == false)
        return false;

    if(variable_name.type != TOK_IDENTIFIER)
        return false;

    if(colon.IsAsciiOf(':') == false)
        return false;

    if(original_requirement.type != TOK_IDENTIFIER)
        return false;

    std::cout << GetNextLineNumber() << ". " << "Input " << original_requirement.identifier_str
        << " as variable " << variable_name.identifier_str << std::endl;

    return true;
}

typedef char* cstr;

cstr PopArgs(int *argc_ptr, cstr **argv_ptr) {
    cstr arg_str = (*argv_ptr)[0];
    *argc_ptr -= 1;
    *argv_ptr += 1;

    return arg_str;
}

int main(int argc, char *argv[]) {
    cstr program_name = PopArgs(&argc, &argv);

    if(argc != 1) {
        std::cerr << "[ERROR] did not specify file path:" << std::endl;
        std::cerr << "\t<usage>: " << program_name << " <pseudo_code_file_path>" << std::endl;
        return -1;
    }

    cstr file_path = PopArgs(&argc, &argv);
    std::ifstream file{file_path};

    if(file.good() == false) {
        std::cerr << "[ERROR] the given file path does not exist" << std::endl;
        return -1;
    }

    InputLexer lexer{&file};
    Token token;

    std::vector<Token> token_list{};


    std::cout << GetNextLineNumber() << ". " << "START" << std::endl;

    while((token = lexer.AdvanceToken()).type != TOK_EOF) {

        // TODO: check for multiple line statements here for now
        if(false) {

        }


        if(token.IsAsciiOf(';') == false) {
            token_list.emplace_back(token);
            continue;
        }
        // else
        

        else if(TryParseInput(token_list)) {}
        else if(TryAssignment(token_list)) {}
        else if(TryParseOut(token_list)) {}

        token_list.clear();
    }

    std::cout << GetNextLineNumber() << ". " << "END" << std::endl;

    return 0;
}
