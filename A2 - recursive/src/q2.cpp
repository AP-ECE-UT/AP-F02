#include <iostream>
#include <string>
#include <vector>

const std::string NO_SOLUTION = "No Solution!";
const char ADD_OPERATOR = '+';
const char MULTIPLY_OPERATOR = '*';
const char SUBTRACT_OPERATOR = '-';
const char EQUAL_OPERATOR = '=';

std::vector<int> getNumbers(int n) {
    std::vector<int> input(n);
    for (int i = 0; i < n; i++) {
        int number;
        std::cin >> number;
        input[i] = number;
    }
    return input;
}

void printResult(const std::vector<int>& numbers, std::string operators, int result) {
    operators += EQUAL_OPERATOR;
    for (int i = 0; i < numbers.size(); i++) {
        std::cout << numbers[i] << operators[i];
    }
    std::cout << result << std::endl;
}

bool solve(const std::vector<int>& numbers, int result, int currResult = 0, std::string operators = "", int index = 1) {
    if (index == 1)
        currResult = numbers[0];

    if (index == numbers.size()) {
        if (currResult == result) {
            printResult(numbers, operators, result);
            return true;
        }
        else
            return false;
    }

    if (solve(numbers, result, currResult + numbers[index], operators + ADD_OPERATOR, index + 1))
        return true;

    if (solve(numbers, result, currResult - numbers[index], operators + SUBTRACT_OPERATOR, index + 1))
        return true;

    if (solve(numbers, result, currResult * numbers[index], operators + MULTIPLY_OPERATOR, index + 1))
        return true;

    return false;
}

int main() {
    int n, result;
    std::vector<int> operands;

    std::cin >> n;
    operands = getNumbers(n - 1);
    std::cin >> result;

    if (!solve(operands, result))
        std::cout << NO_SOLUTION << std::endl;
}