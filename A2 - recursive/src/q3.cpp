#include <iostream>
#include <vector>

const std::string NOT_SOL = "No Solution!";
const char DELIMITER = ' ';

enum class Species {
    X = 'X',
    Z = 'Z',
    EMPTY = '-'
};

enum class Direction {
    LEFT = 'L',
    RIGHT = 'R',
    BOTTOM = 'B',
    TOP = 'T'
};

bool solveQuarantinePuzzle(std::vector<std::vector<Species>>& quarantineSolution, int row, int col, const std::vector<int>& colXcount,
                           const std::vector<int>& rowXcount, const std::vector<int>& colZcount, const std::vector<int>& rowZcount,
                           const std::vector<std::vector<Direction>>& rules, const int n, const int m);

std::vector<int> getNumberOfXandZ(const int count) {
    std::vector<int> horizontalVerticalXZcount(count);
    for (int i = 0; i < count; i++) {
        std::cin >> horizontalVerticalXZcount[i];
    }
    return horizontalVerticalXZcount;
}

std::vector<std::vector<Direction>> getRules(const int m, const int n) {
    std::vector<std::vector<Direction>> rules(m, std::vector<Direction>(n));
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            char rule;
            std::cin >> rule;
            rules[i][j] = static_cast<Direction>(rule);
        }
    }
    return rules;
}

int countInColsOrRows(const std::vector<std::vector<Species>>& quarantineSolution, Species chosenSpecies, const int rowOrCol, bool isRow, const int max) {
    int count = 0;
    for (int i = 0; i < max; i++) {
        if (!isRow && quarantineSolution[i][rowOrCol] == chosenSpecies) {
            count++;
        }
        else if (isRow && quarantineSolution[rowOrCol][i] == chosenSpecies) {
            count++;
        }
    }
    return count;
}

bool isAdjacentSame(const int row, const int col, const std::vector<std::vector<Species>>& quarantineSolution, Species chosenSpecies,
                    const int m, const int n) {
    return ((row - 1 >= 0 && quarantineSolution[row - 1][col] == chosenSpecies) ||
            (col - 1 >= 0 && quarantineSolution[row][col - 1] == chosenSpecies));
}

bool checkCountCorrectness(const int countInRowsRule, const int countInColsRule, const int rowCount, const int colCount) {
    if (countInColsRule != -1 && colCount >= countInColsRule) {
        return false;
    }
    if (countInRowsRule != -1 && rowCount >= countInRowsRule) {
        return false;
    }
    return true;
}

bool isSolSafe(const std::vector<std::vector<Species>>& quarantineSolution, const int row, const int col, Species chosenSpecies, const std::vector<int>& colXcount,
               const std::vector<int>& rowXcount, const std::vector<int>& colZcount, const std::vector<int>& rowZcount, const int m, const int n) {
    if (isAdjacentSame(row, col, quarantineSolution, chosenSpecies, m, n))
        return false;

    int rowCount = countInColsOrRows(quarantineSolution, chosenSpecies, row, true, n);
    int colCount = countInColsOrRows(quarantineSolution, chosenSpecies, col, false, m);

    if (chosenSpecies == Species::X) {
        if (!checkCountCorrectness(rowXcount[row], colXcount[col], rowCount, colCount))
            return false;
    }

    else if (chosenSpecies == Species::Z) {
        if (!checkCountCorrectness(rowZcount[row], colZcount[col], rowCount, colCount))
            return false;
    }
    return true;
}

bool validateAdjacentConfig(const std::vector<std::vector<Species>>& quarantineSolution, const int maxRowOrCol, const int maxRowOrCol2, Species species, const bool isRow,
                            const std::vector<int>& colOrRowCountRule) {
    for (int i = 0; i < maxRowOrCol; i++) {
        if (colOrRowCountRule[i] != -1 && countInColsOrRows(quarantineSolution, species, i, isRow, maxRowOrCol2) != colOrRowCountRule[i]) {
            return false;
        }
    }
    return true;
}

bool validateConfig(std::vector<std::vector<Species>>& quarantineSolution, const std::vector<int>& colXcount, const std::vector<int>& rowXcount,
                    const std::vector<int>& colZcount, const std::vector<int>& rowZcount, const int m, const int n) {
    if (!validateAdjacentConfig(quarantineSolution, n, m, Species::X, false, colXcount) ||
        !validateAdjacentConfig(quarantineSolution, m, n, Species::X, true, rowXcount) ||
        !validateAdjacentConfig(quarantineSolution, n, m, Species::Z, false, colZcount) ||
        !validateAdjacentConfig(quarantineSolution, m, n, Species::Z, true, rowZcount))
        return false;
    return true;
}

bool putXZorZXpair(const int row, const int col, const int adjRow, const int adjCol, std::vector<std::vector<Species>>& quarantineSolution, const std::vector<int>& colXcount,
                   const std::vector<int>& rowXcount, const std::vector<int>& colZcount, const std::vector<int>& rowZcount,
                   const int n, const int m, Species species1, Species species2, const std::vector<std::vector<Direction>>& rules) {
    if (isSolSafe(quarantineSolution, row, col, species1, colXcount, rowXcount, colZcount, rowZcount, m, n) &&
        isSolSafe(quarantineSolution, adjRow, adjCol, species2, colXcount, rowXcount, colZcount, rowZcount, m, n)) {
        quarantineSolution[row][col] = species1;
        quarantineSolution[adjRow][adjCol] = species2;

        if (solveQuarantinePuzzle(quarantineSolution, row, adjCol + 1, colXcount, rowXcount, colZcount, rowZcount, rules, n, m)) {
            return true;
        }

        quarantineSolution[row][col] = Species::EMPTY;
        quarantineSolution[adjRow][adjCol] = Species::EMPTY;
    }
    return false;
}

bool isPuttingXZorZXPossible(const int row, const int col, const int adjRow, const int adjCol, std::vector<std::vector<Species>>& quarantineSolution, const std::vector<int>& colXcount,
                             const std::vector<int>& rowXcount, const std::vector<int>& colZcount, const std::vector<int>& rowZcount,
                             const std::vector<std::vector<Direction>>& rules, const int n, const int m) {
    if (putXZorZXpair(row, col, adjRow, adjCol, quarantineSolution, colXcount, rowXcount, colZcount, rowZcount, n, m, Species::X, Species::Z, rules))
        return true;
    if (putXZorZXpair(row, col, adjRow, adjCol, quarantineSolution, colXcount, rowXcount, colZcount, rowZcount, n, m, Species::Z, Species::X, rules))
        return true;
    return false;
}

bool isRowCountOk(const std::vector<std::vector<Species>>& quarantineSolution, const int row, const std::vector<int>& rowXcount, const std::vector<int>& rowZcount,
                  const std::vector<int>& colXcount, const std::vector<int>& colZcount, const int n) {
    if ((rowXcount[row] != -1 && rowXcount[row] != countInColsOrRows(quarantineSolution, Species::X, row, true, n)) ||
        (rowZcount[row] != -1 && rowZcount[row] != countInColsOrRows(quarantineSolution, Species::Z, row, true, n))) {
        return false;
    }
    return true;
}

bool solveQuarantinePuzzle(std::vector<std::vector<Species>>& quarantineSolution, int row, int col, const std::vector<int>& colXcount,
                           const std::vector<int>& rowXcount, const std::vector<int>& colZcount, const std::vector<int>& rowZcount,
                           const std::vector<std::vector<Direction>>& rules, const int n, const int m) {
    if (row >= m - 1 && col >= n - 1)
        return (validateConfig(quarantineSolution, colXcount, rowXcount, colZcount, rowZcount, m, n));

    if (col >= n) {
        if (!isRowCountOk(quarantineSolution, row, rowXcount, rowZcount, colXcount, colZcount, n))
            return false;
        col = 0;
        row = row + 1;
    }

    if (rules[row][col] == Direction::RIGHT || rules[row][col] == Direction::BOTTOM) {
        if (solveQuarantinePuzzle(quarantineSolution, row, col + 1, colXcount, rowXcount, colZcount, rowZcount, rules, n, m)) {
            return true;
        }
    }

    if (rules[row][col] == Direction::LEFT && rules[row][col + 1] == Direction::RIGHT) {
        if (isPuttingXZorZXPossible(row, col, row, col + 1, quarantineSolution, colXcount, rowXcount, colZcount, rowZcount, rules, n, m))
            return true;
    }

    if (rules[row][col] == Direction::TOP && rules[row + 1][col] == Direction::BOTTOM) {
        if (isPuttingXZorZXPossible(row, col, row + 1, col, quarantineSolution, colXcount, rowXcount, colZcount, rowZcount, rules, n, m))
            return true;
    }
    return (solveQuarantinePuzzle(quarantineSolution, row, col + 1, colXcount, rowXcount, colZcount, rowZcount, rules, n, m));
}

void printSolution(const bool doesSolutionExist, const std::vector<std::vector<Species>>& quarantineSolution) {
    if (!doesSolutionExist) {
        std::cout << NOT_SOL << std::endl;
        return;
    }
    for (int i = 0; i < quarantineSolution.size(); i++) {
        for (int j = 0; j < quarantineSolution[i].size(); j++) {
            std::cout << static_cast<char>(quarantineSolution[i][j]) << DELIMITER;
        }
        std::cout << std::endl;
    }
}

void quarantineAliens(const int m, const int n) {
    std::vector<std::vector<Direction>> rules = getRules(m, n);
    std::vector<int> colXcount = getNumberOfXandZ(n);
    std::vector<int> colZcount = getNumberOfXandZ(n);
    std::vector<int> rowXcount = getNumberOfXandZ(m);
    std::vector<int> rowZcount = getNumberOfXandZ(m);

    std::vector<std::vector<Species>> quarantineSolution(m, std::vector<Species>(n));

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            quarantineSolution[i][j] = Species::EMPTY;
        }
    }
    bool doesSolutionExist = solveQuarantinePuzzle(quarantineSolution, 0, 0, colXcount, rowXcount, colZcount, rowZcount, rules, n, m);
    printSolution(doesSolutionExist, quarantineSolution);
}

int main() {
    int m, n;
    std::cin >> m >> n;
    quarantineAliens(m, n);
    return 0;
}
