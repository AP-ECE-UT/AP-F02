#include <iostream>

constexpr int FLOAT_PRECISION = 2;

int power(int base, int exp) {
    int result = 1;
    for (int i = 0; i < exp; i++) {
        result *= base;
    }
    return result;
}

float e2x(float x, int n) {
    float term = 1.0f;
    float sum = term;
    for (int i = 1; i < n; i++) {
        term *= (2 * x) / i;
        sum += term;
    }
    return sum;
}

float cut(float x, int precision) {
    int multiplier = power(10, precision);
    return static_cast<float>(static_cast<int>(x * multiplier)) / multiplier;
}

int main() {
    float x;
    int n;
    std::cin >> x >> n;
    std::cout << cut(e2x(x, n), FLOAT_PRECISION) << std::endl;
}
