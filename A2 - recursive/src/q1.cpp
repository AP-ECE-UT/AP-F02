#include <iostream>
#include <string>

bool isPalindromeImpl(const std::string& text, int start, int end) {
    if (start >= end) {
        return true;
    }

    if (!isalpha(text[start])) {
        return isPalindromeImpl(text, start + 1, end);
    }

    if (!isalpha(text[end])) {
        return isPalindromeImpl(text, start, end - 1);
    }

    if (tolower(text[start]) != tolower(text[end])) {
        return false;
    }

    return isPalindromeImpl(text, start + 1, end - 1);
}

bool isPalindrome(const std::string& text) {
    return isPalindromeImpl(text, 0, text.length() - 1);
}

int main() {
    std::string inp;

    while (std::getline(std::cin, inp)) {
        bool result = isPalindrome(inp);
        std::cout << std::boolalpha << result << std::endl;
    }

    return 0;
}
