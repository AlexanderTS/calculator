#include <cmath>
#include "calculator.h"

float Calculator::calculate(const std::string &expression) {
    Parser parser(expression);
    float exact_result = parser.get_result();
    return round(exact_result * 100) / 100;
}

float Calculator::Parser::get_result() {
    while (read_token());
    
    verify_last_operation();
    
    while (!operator_stack.empty()) {
        
        if (operator_stack.top() == '(') {
            throw std::runtime_error("Пропущена закрывающая скобка )");
        }
        
        perform(operator_stack.top());
        operator_stack.pop();
    }
    
    if (output_queue.empty()) {
        throw std::runtime_error("Некорректный ввод");
    }
    
    return output_queue.front();
}

bool Calculator::Parser::read_token() {
    return read_operator() || read_number() || read_brackets() || read_other();
}

bool Calculator::Parser::read_number() {
    if (is_digit(current_symbol)) {
        
        size_t start_pos = current_idx;
        float number = current_symbol - '0';
        
        while (next() && is_digit(current_symbol)) {
            number = number * 10 + current_symbol - '0';
        }
        
        if (current_symbol == '.' || current_symbol == ',') {
            
            if (next() && is_digit(current_symbol)) {
                float k = 0.1;
                do {
                    
                    number = number + (current_symbol - '0') * k;
                    k *= 0.1;
                } while (next() && is_digit(current_symbol));
            } else {
                
                throw std::runtime_error(
                    std::string("Некорректный ввод, строка содержит недопустимое выражение ") +
                    expression.substr(start_pos, current_idx - start_pos + 1));
            }
        }
        
        operation_added_last = false;
        output_queue.push_back(number);
        return true;
    }
    
    return false;
}

bool Calculator::Parser::read_operator() {
    if (is_operator(current_symbol)) {
        char operation = current_symbol;
        
        if (get_precedence(current_symbol) == 0) {
            if (operation_added_last) {
                output_queue.push_back(0.0);
            }
            
            int k = current_symbol == '+' ? 1 : -1;
            
            while (next() && get_precedence(current_symbol) == 0) {
                k *= current_symbol == '+' ? 1 : -1;
            }
            operation = k == 1 ? '+' : '-';
        } else {
            next();
        }
        
        while (!operator_stack.empty() &&
            get_precedence(operator_stack.top()) >= get_precedence(operation)) {
                perform(operator_stack.top());
                operator_stack.pop();
        }
        
        operator_stack.push(operation);
        operation_added_last = true;
        return true;
    }
    
    return false;
}

bool Calculator::Parser::read_brackets() {
    
    if (is_bracket(current_symbol)) {
        if (current_symbol == '(') {
            operator_stack.push(current_symbol);
        } else if (current_symbol == ')') {
            
            verify_last_operation();
            
            while (!operator_stack.empty() && operator_stack.top() != '(') {
                perform(operator_stack.top());
                operator_stack.pop();
            }
            
            if (!operator_stack.empty() && operator_stack.top() == '(') {
                operator_stack.pop();
            } else {
                throw std::runtime_error("Пропущена открывающая скобка (");
            }
        }
        
        next();
        return true;
    }
    
    return false;
}

bool Calculator::Parser::read_other() {
    if (current_symbol == 0) return false;
    
    if (current_symbol == ' ') return next();
    
    size_t start_pos = current_idx;
    
    while (next()
        && !is_digit(current_symbol)
        && !is_operator(current_symbol)
        && !is_bracket(current_symbol)
        && !is_whitespace(current_symbol));
    
    throw std::runtime_error(
        std::string("Некорректный ввод, строка содержит недопустимое выражение ")
        + expression.substr(start_pos, current_idx - start_pos));
    return false;
}

void Calculator::Parser::perform(const char &operation) {
    
    if (output_queue.size() == 1) {
        if (get_precedence(operation) == 0) {
            output_queue.insert(output_queue.end() - 1, 0.0);
        } else {
            throw std::runtime_error(
                std::string("Некорректный ввод, ожидались два операнда при ") + operation);
        }
    }
    
    float rvalue = output_queue.back();
    output_queue.pop_back();
    
    float lvalue = output_queue.back();
    output_queue.pop_back();
    
    switch (operation) {
        case '+':
            output_queue.push_back(lvalue + rvalue);
            break;
            
        case '-':
            output_queue.push_back(lvalue - rvalue);
            break;
            
        case '*':
            output_queue.push_back(lvalue * rvalue);
            break;
            
        case '/':
            if (std::abs(rvalue) <= 1e-10) {
                throw std::runtime_error("Ошибка при вычислении, деление на ноль");
            }
            output_queue.push_back(lvalue / rvalue);
            break;
            
        default:
            throw std::runtime_error("Внутренняя ошибка программы");
    }
}

bool Calculator::Parser::next() {
    if (++current_idx < expression.size()) {
        current_symbol = expression[current_idx];
        return true;
    }
    
    current_symbol = 0;
    return false;
}

int Calculator::Parser::get_precedence(const char &operation) {
    switch (operation) {
        case '+':
        case '-':
            return 0;
        
        case '*':
        case '/':
            return 1;
            
        default:
            return -1;
    }
}

void Calculator::Parser::verify_last_operation() {
    if (operation_added_last) {
        throw std::runtime_error(
            std::string("Некорректный ввод, ожидались два операнда при ") + operator_stack.top());
    }
}

bool Calculator::Parser::is_digit(const char &symbol) {
    return symbol >= '0' && symbol <= '9';
}

bool Calculator::Parser::is_operator(const char &symbol) {
    return std::string("+-*/").find(symbol) != std::string::npos;
}

bool Calculator::Parser::is_bracket(const char &symbol) {
    return symbol == '(' || symbol == ')';
}

bool Calculator::Parser::is_whitespace(const char &symbol) {
    return symbol == ' ';
}
