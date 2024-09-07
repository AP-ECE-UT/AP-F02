// This verifier is written because of a mistake made in test cases
// This will accept both answers

#include <fstream>

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
    for (int i = 1; i <= n; i++) {
        term *= (2 * x) / i;
        sum += term;
    }
    return sum;
}

float cut(float x, int precision) {
    int multiplier = power(10, precision);
    return static_cast<float>(static_cast<int>(x * multiplier)) / multiplier;
}

int main(int argc, const char* argv[]) {
    std::ifstream testIn(argv[1]);
    std::ifstream testOut(argv[2]);
    std::ifstream userOut(argv[3]);

    float testRes, userRes;
    testOut >> testRes;
    userOut >> userRes;
    if (testRes == userRes) {
        // Correct answer
        return 0;
    }

    int n;
    float x;
    testIn >> x >> n;
    float wrongRes = cut(e2x(x, n), FLOAT_PRECISION);
    if (wrongRes == userRes) {
        // First answer written in Description
        return 0;
    }
    return 1;
}
