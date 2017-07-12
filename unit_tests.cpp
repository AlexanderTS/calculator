#include <iostream>
#include <string>
#include <stdexcept>
#include <cassert>
#include <cmath>

#include "calculator.h"

bool are_equal(const float &a, const float &b) {
    return std::abs(a - b) < 1e-10;
}

void test_1() {
    std::string expression = "-1 + 5 - 3";
    float expected_result = 1;
    
    assert(are_equal(Calculator::calculate(expression), expected_result));
    std::cout << "Test 1: OK" << std::endl;
}

void test_2() {
    std::string expression = "-10 + (8 * 2.5) - (3 / 1,5)";
    float expected_result = 8;
    
    assert(are_equal(Calculator::calculate(expression), expected_result));
    std::cout << "Test 2: OK" << std::endl;
}

void test_3() {
    std::string expression = "1 + (2 * (2.5 + 2.5 + (3 - 2))) - (3 / 1.5)";
    float expected_result = 11;
    
    assert(are_equal(Calculator::calculate(expression), expected_result));
    std::cout << "Test 3: OK" << std::endl;
}


void test_4() {
    std::string expression = "-10,0 / 3";
    float expected_result = -3.33;
    
    assert(are_equal(Calculator::calculate(expression), expected_result));
    std::cout << "Test 4: OK" << std::endl;
}

void test_5() {
    std::string expression = "1.1 + 2.1 + abc";
    std::string expected_error = "Некорректный ввод, строка содержит недопустимое выражение abc";
    std::string error = "";
    
    try {
        Calculator::calculate(expression);
    } catch (std::runtime_error e) {
        error = e.what();
    }
    
    assert(error == expected_error);
    std::cout << "Test 5: OK" << std::endl;
}

void test_6() {
    std::string expression = "  (3+4) -(5 ";
    std::string expected_error = "Пропущена закрывающая скобка )";
    std::string error = "";
    
    try {
        Calculator::calculate(expression);
    } catch (std::runtime_error e) {
        error = e.what();
    }
    
    assert(error == expected_error);
    std::cout << "Test 6: OK" << std::endl;
}

void test_7() {
    std::string expression = "-1,3 +1) * 0.32";
    std::string expected_error = "Пропущена открывающая скобка (";
    std::string error = "";
    
    try {
        Calculator::calculate(expression);
    } catch (std::runtime_error e) {
        error = e.what();
    }
    
    assert(error == expected_error);
    std::cout << "Test 7: OK" << std::endl;
}

void test_8() {
    std::string expression = " ";
    std::string expected_error = "Некорректный ввод";
    std::string error = "";
    
    try {
        Calculator::calculate(expression);
    } catch (std::runtime_error e) {
        error = e.what();
    }
    
    assert(error == expected_error);
    std::cout << "Test 8: OK" << std::endl;
}

void test_9() {
    std::string expression = "0,1 * (5 + ) - 2";
    std::string expected_error = "Некорректный ввод, ожидались два операнда при +";
    std::string error = "";
    
    try {
        Calculator::calculate(expression);
    } catch (std::runtime_error e) {
        error = e.what();
    }
    
    assert(error == expected_error);
    std::cout << "Test 9: OK" << std::endl;
}

void test_10() {
    std::string expression = "*";
    std::string expected_error = "Некорректный ввод, ожидались два операнда при *";
    std::string error = "";
    
    try {
        Calculator::calculate(expression);
    } catch (std::runtime_error e) {
        error = e.what();
    }
    
    assert(error == expected_error);
    std::cout << "Test 10: OK" << std::endl;
}


int main() {
    test_1();
    test_2();
    test_3();
    test_4();
    test_5();
    test_6();
    test_7();
    test_8();
    test_9();
    test_10();
    
    return 0;
}
