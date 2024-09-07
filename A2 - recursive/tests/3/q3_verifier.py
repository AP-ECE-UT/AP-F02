import sys
SPECIES_X = 'X'
SPECIES_Z = 'Z'
TOP = 'T'
BOTTOM = 'B'
LEFT = 'L'
RIGHT = 'R'
EMPTY = '-'
VERIFIED = '*'

inputFilePath = sys.argv[1]
outputFilePath = sys.argv[2]

def checkAdjacent(out, i, j, m, n):
    for dx in range (-1,2):
        for dy in range (-1,2):
            if (i + dx >= 0 and i + dx < m and j + dy >= 0 and j + dy < n) and abs(dx) != abs(dy):
                if out[i + dx][j + dy] == out[i][j] and out[i][j] != EMPTY:
                    return -1    
    return 0

def getInput():
    with open(inputFilePath, "r") as inputFile:
        lines = inputFile.readlines() 
    rules = []
    rowXcountRule = []
    colXcountRule = []
    rowZcountRule = []
    colZcountRule = []
    counter = 0
    for line in lines:
        if (counter == 0):
            rowCol = line.strip().split()
            m , n = int(rowCol[0]), int(rowCol[1])
        elif (counter <= m):
            values = line.strip().split()
            row = [val for val in values]
            rules.append(row)
        elif (counter == m + 1):
            colXcountRule = line.strip().split()
            colXcountRule = [int(val) for val in colXcountRule]
        elif (counter == m + 2):
            colZcountRule = line.strip().split()
            colZcountRule = [int(val) for val in colZcountRule]
        elif (counter == m + 3):
            rowXcountRule = line.strip().split()
            rowXcountRule = [int(val) for val in rowXcountRule]
        elif (counter == m + 4):
            rowZcountRule = line.strip().split()
            rowZcountRule = [int(val) for val in rowZcountRule]
        counter += 1
    return rules, rowXcountRule, colXcountRule, rowZcountRule, colZcountRule, m, n

def getOutput():
    out = []
    with open(outputFilePath, "r") as outputFile:
        lines = outputFile.readlines()
    for line in lines:
        values = line.strip().split()
        row = [val for val in values]
        out.append(row)
    return out

def checkAdjacent(out, i, j, m, n):
    for dx in range (-1,2):
        for dy in range (-1,2):
            if (i + dx >= 0 and i + dx < m and j + dy >= 0 and j + dy < n) and abs(dx) != abs(dy):
                if out[i + dx][j + dy] == out[i][j] and out[i][j] != EMPTY:
                    return -1    
    return 0

def countSpecies(m, n, out):
    rowXcount = [0 for val in range(0, m)]
    rowZcount = [0 for val in range(0, m)]
    colXcount = [0 for val in range(0, n)]
    colZcount = [0 for val in range(0, n)]
    for i in range(m):
        for j in range(n):
            if checkAdjacent(out, i, j, m, n) == -1:
                sys.exit(-1)
            if out[i][j] == SPECIES_X:
                rowXcount[i] += 1
                colXcount[j] += 1
            elif out[i][j] == SPECIES_Z:
                rowZcount[i] += 1
                colZcount[j] += 1
    return rowXcount, rowZcount, colXcount, colZcount

def checkXandZCount(rowXcountRule, rowXcount, rowZcountRule, rowZcount, colXcountRule, colXcount, colZcountRule, colZcount, m, n):
    for i in range(0, m):
        if (rowXcountRule[i] != -1 and rowXcount[i] != rowXcountRule[i]) or (rowZcountRule[i] != -1 and rowZcount[i] != rowZcountRule[i]):
            sys.exit(-1)

    for i in range(0, n):
        if (colXcountRule[i] != -1 and colXcount[i] != colXcountRule[i]) or (colZcountRule[i] != -1 and colZcount[i] != colZcountRule[i]):
            sys.exit(-1)

def verifyVerticalHorizontal(rules, out, m, n):
    for i in range(0,m):
        for j in range(0,n):
            if rules[i][j] == LEFT and ((out[i][j] == SPECIES_X and out[i][j + 1] == SPECIES_Z) or (out[i][j] == SPECIES_Z and out[i][j + 1] == SPECIES_X) or (out[i][j] == EMPTY and out[i][j + 1] == EMPTY)):
                rules[i][j] = VERIFIED
                rules[i][j + 1] = VERIFIED
            if rules[i][j] == TOP and ((out[i][j] == SPECIES_X and out[i + 1][j] == SPECIES_Z) or (out[i][j] == SPECIES_Z and out[i + 1][j] == SPECIES_X) or (out[i][j] == EMPTY and out[i + 1][j] == EMPTY)):
                rules[i][j] = VERIFIED
                rules[i + 1][j] = VERIFIED
            
    for i in range(0,m):
        for j in range(0,n):
            if rules[i][j] != VERIFIED:
                sys.exit(-1)

def main():
    rules, rowXcountRule, colXcountRule, rowZcountRule, colZcountRule, m, n = getInput()
    out = getOutput()            
    rowXcount, rowZcount, colXcount, colZcount = countSpecies(m, n, out)
    checkXandZCount(rowXcountRule, rowXcount, rowZcountRule, rowZcount, colXcountRule, colXcount, colZcountRule, colZcount, m, n)
    verifyVerticalHorizontal(rules, out, m, n)
    sys.exit(0)

if __name__ == "__main__":
    main()
