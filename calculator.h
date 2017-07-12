#ifndef __CALCULATOR_H_INCLUDED__
#define __CALCULATOR_H_INCLUDED__

#include <string>
#include <vector>
#include <stack>
#include <stdexcept>


// -------------------------------------------------------------------------------------------------
// Use
// static float Calculator::calculate(std::string &expression)
// to calculate the value of arithmetic expression.
// -------------------------------------------------------------------------------------------------
class Calculator {
private:
    
    // ---------------------------------------------------------------------------------------------
    // Hiding the arithmeric expression parser implementation.
    // Modificated Shunting-yard algorithm is used to parse and calculate the value of expression
    // in one pass.
    // ---------------------------------------------------------------------------------------------
    class Parser {
    private:
        const std::string &expression;
        size_t current_idx = 0;
        char current_symbol;
        bool operation_added_last = false;
        std::vector<float> output_queue;
        std::stack<char> operator_stack;
        
        bool read_token();
        bool read_number();
        bool read_operator();
        bool read_brackets();
        bool read_other();
        void perform(const char &operation);
        
        bool is_digit(const char &symbol);
        bool is_operator(const char &symbol);
        bool is_bracket(const char &symbol);
        bool is_whitespace(const char &symbol);
        int get_precedence(const char &operation);
        
        bool next();
        void verify_last_operation();
        
    public:
        Parser(const std::string &exp) :
            expression(exp),
            current_symbol(exp[0]),
            output_queue(),
            operator_stack() { }
        
        float get_result();
    };
    
public:
    static float calculate(const std::string &expression);
};

#endif
