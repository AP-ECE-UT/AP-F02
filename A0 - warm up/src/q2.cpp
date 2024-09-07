#include <iostream>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

struct Triplet {
    int f;
    int s;
    int t;

    bool operator==(const Triplet& other) const {
        std::unordered_set<int> firstSet({f, s, t});
        std::unordered_set<int> secondSet({other.f, other.s, other.t});
        return firstSet == secondSet;
    }
};

const int CARDS_CNT = 48;

const std::vector<std::pair<Triplet, std::string>> SPECIALS = {
    {{1, 2, 3}, "Khoshbakht"},
    {{6, 3, 12}, "Badbakht"},
    {{22, 8, 6}, "Pooldar"},
    {{48, 22, 10}, "Faghir"},
    {{15, 14, 13}, "OmreToolani"},
    {{18, 15, 33}, "JavoonMarg"},
};

bool isSpecial(const Triplet& triplet) {
    for (const auto& special : SPECIALS)
        if (triplet == special.first)
            return true;
    return false;
}

std::string getKeyword(const Triplet& triplet) {
    for (const auto& special : SPECIALS)
        if (triplet == special.first)
            return special.second;
    return nullptr;
}

void findFortune(const std::vector<int>& cards) {
    bool isSpecialSeen = false;
    for (int i = 0; i < CARDS_CNT; i += 3) {
        Triplet currentTriplet = {
            cards[i], cards[i + 1], cards[i + 2]};
        if (isSpecial(currentTriplet)) {
            isSpecialSeen = true;
            std::cout << getKeyword(currentTriplet) << std::endl;
        }
    }
    if (!isSpecialSeen)
        std::cout << "Bakhtet Pake!" << std::endl;
}

std::vector<int> getInput() {
    std::vector<int> cards;
    cards.reserve(CARDS_CNT);
    for (int i = 0; i < CARDS_CNT; ++i) {
        int temp;
        std::cin >> temp;
        cards.push_back(temp);
    }
    return cards;
}

int main() {
    std::vector<int> inputs = getInput();
    findFortune(inputs);
}
