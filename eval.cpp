#include <string>
#include <deque>
#include <iostream>
#include <unordered_map>

enum SymbolType {
    unknown,
    numeric_literal,
    operation,
    bracket_close
};

struct Symbol {
    std::string str_value;
    SymbolType type;
};

static Symbol MakeOperation(char op) {
    return {
        {op}, operation
    };
}

static Symbol MakeNumericLiteral(char literal) {
    return {
        {literal}, numeric_literal 
    };
}



Symbol ParseSymbol(char character) {
    std::string symbol{character};


    std::cerr << "[Error] unrecognized symbol\n";
    return Symbol{};
}

void EvalMathExpression(const std::string& raw_expr,
        std::deque<Symbol> *output_stack) {
    std::deque<char> operation_stack;
    std::unordered_map<char, int> operation_predecence{
        {'*', 2}, {'/', 2}, {'+', 1}, {'-', 1}, {'(', -1}
    };

    // lexer
    for(char character : raw_expr) {
        if(isspace(character))
            continue;

        if(isdigit(character)) {
            output_stack->push_back(
                MakeNumericLiteral(character)
            );
        }

        if(character == '+' || character == '-' ||
                character == '*' || character == '/') {
            int predecence = operation_predecence[character];

            while(!operation_stack.empty()) {
                char operation = operation_stack.front();
                int front_predecence = operation_predecence[operation];
    
                if(front_predecence >= predecence) {
                    operation_stack.pop_front();
                    output_stack->push_back(
                        MakeOperation(operation)
                    );
                }
                else
                    break;
            }
            // can safely assume can push

            operation_stack.push_front(character);
        }
    }

    // flush all operations into output stack
    while(!operation_stack.empty()) {
        char op = operation_stack[0];
        operation_stack.pop_front();

        output_stack->push_back(
            MakeOperation(op)
        );

    }
}

int main() {
    std::string expression;

    std::cout << "Expression: ";
    std::getline(std::cin, expression);

    std::deque<Symbol> output_stack;

    EvalMathExpression(expression, &output_stack);

    std::cout << "RLH: ";

    for(Symbol& symbol : output_stack) {
        std::cout << symbol.str_value << " ";
    }

    std::cout << std::endl;

    std::deque<double> result_stack;

    while(!output_stack.empty()) {
        Symbol symbol = output_stack.front();
        output_stack.pop_front();
        
        if(symbol.type == numeric_literal) {
            double value = std::strtod(symbol.str_value.c_str(), nullptr);
            result_stack.push_front(value);
        }
        else if(symbol.type == operation) {
            double &a = result_stack[1];
            double &b = result_stack[0];

            if(symbol.str_value == "+") result_stack[1] = a + b;
            if(symbol.str_value == "-") result_stack[1] = a - b;
            if(symbol.str_value == "*") result_stack[1] = a * b;
            if(symbol.str_value == "/") result_stack[1] = a / b;

            // after operation, we should consume the used numeric literal
            result_stack.pop_front();
        }
    }


    std::cout << "Result: " << result_stack.front() << std::endl;


    return 0;
}
